#include "ShadowMap.h"
#include "ShadowMapShader.h"
#include "OrthographicFrustum.h"
#include "PerspectiveFrustum.h"
#include "Device.h"
#include "UniformArray.h"
#include "UniformSampler.h"

namespace argeo
{

	const std::vector<vec4d> ShadowMap::g_frustum_corners_ndc =
	{
		vec4d(-1.0, -1.0, -1.0, 1.0),
		vec4d(1.0, -1.0, -1.0, 1.0),
		vec4d(1.0, 1.0, -1.0, 1.0),
		vec4d(-1.0, 1.0, -1.0, 1.0),
		vec4d(-1.0, -1.0, 1.0, 1.0),
		vec4d(1.0, -1.0, 1.0, 1.0),
		vec4d(1.0, 1.0, 1.0, 1.0),
		vec4d(-1.0, 1.0, 1.0, 1.0)
	};

	const std::vector<vec3d> ShadowMap::g_ups =
	{
		vec3d(0.0, -1.0, 0.0),
		vec3d(0.0, 0.0, -1.0),
		vec3d(0.0, -1.0, 0.0),
		vec3d(0.0, -1.0, 0.0),
		vec3d(0.0, 0.0, 1.0),
		vec3d(0.0, -1.0, 0.0)
	};

	const std::vector<vec3d> ShadowMap::g_rights =
	{
		vec3d(0.0, 0.0, 1.0),
		vec3d(1.0, 0.0, 0.0),
		vec3d(-1.0, 0.0, 0.0),
		vec3d(0.0, 0.0, -1.0),
		vec3d(1.0, 0.0, 0.0),
		vec3d(1.0, 0.0, 0.0)
	};

	const std::vector<vec3d> ShadowMap::g_directions =
	{
		vec3d(-1.0, 0.0, 0.0),
		vec3d(0.0, -1.0, 0.0),
		vec3d(0.0, 0.0, -1.0),
		vec3d(1.0, 0.0, 0.0),
		vec3d(0.0, 1.0, 0.0),
		vec3d(0.0, 0.0, 1.0)
	};

	ShadowMap::ShadowMap(
		Context* context,
		Camera* light_camera,
		const bool& enabled,
		const bool& from_light_source,
		const bool& is_point_light,
		const double& point_light_radius,
		const bool& cascades_enabled,
		const unsigned int& number_of_cascades,
		const double& maximum_distance,
		const std::size_t& size,
		const bool& soft_shadows,
		const double& darkness)
		: m_enabled(enabled)
		, m_soft_shadows(soft_shadows)
		, m_dirty(true)
		, m_from_light_source(from_light_source)
		, m_is_point_light(is_point_light)
		, m_point_light_radius(point_light_radius)
		, m_darkness(darkness)
		, m_maximum_distance(maximum_distance)
		, m_out_of_view(false)
		, m_out_of_view_previous(false)
		, m_needs_update(true)
		, m_depth_texture_attachment(nullptr)
		, m_depth_render_buffer_attachment(nullptr)
		, m_color_attachment(nullptr)
		, m_light_camera(light_camera)
		, m_clear_pass_state(context)
		, m_size(size)
		, m_texture_size(0)
		, m_shadow_map_texture(nullptr)
		, m_scene_camera(nullptr)
		, m_distance(0.0)
		, m_bounding_sphere(new BoundingSphere())
		, m_fit_near_far(true)
	{
		if (((number_of_cascades !=1) && (number_of_cascades != 4)))
		{
			throw std::invalid_argument("Only one or four cascades are supported.");
		}

		// In IE11 and Edge polygon offset is not functional.
		// TODO : Also disabled for instances of Firefox and Chrome running ANGLE that do not support depth textures.
		// Re-enable once https://github.com/AnalyticalGraphicsInc/cesium/issues/4560 is resolved.
		bool polygon_offset_supported = true;
		//if (FeatureDetection.isInternetExplorer() || FeatureDetection.isEdge() || ((FeatureDetection.isChrome() || FeatureDetection.isFirefox()) && FeatureDetection.isWindows() && !context.depthTexture)) 
		//{
		//	polygonOffsetSupported = false;
		//}
		//this._polygonOffsetSupported = polygonOffsetSupported;
		
		//
		//// Uniforms
		//this._shadowMapCullingVolume = undefined;
		
		m_cascades_enabled   = m_is_point_light ? false : cascades_enabled;
		m_number_of_cascades = !m_cascades_enabled ? 0 : number_of_cascades;
		m_maximum_cascade_distance = { 25.0, 150.0, 700.0, std::numeric_limits<double>::max()};
		

		m_is_spot_light = false;

		if (m_cascades_enabled) 
		{
			// Cascaded shadows are always orthographic. The frustum dimensions are calculated on the fly.
			m_shadow_map_camera.frustum.reset(new OrthographicFrustum());
		}
		else if (dynamic_cast<PerspectiveFrustum*>(light_camera->get_frustum()) != nullptr)
		{
			// If the light camera uses a perspective frustum, then the light source is a spot light
			m_is_spot_light = true;
		}
		
		m_cascade_splits.resize(2);
		m_cascade_matrices.resize(4);
		
		unsigned int number_of_passes;
		if (m_is_point_light) 
		{
			number_of_passes = 6; // One shadow map for each direction
		}
		else if (!m_cascades_enabled) 
		{
			number_of_passes = 1;
		}
		else 
		{
			number_of_passes = m_number_of_cascades;
		}
		
		for (unsigned int i = 0; i < number_of_passes; ++i) 
		{
			m_passes.push_back(ShadowPass(context));
		}

		//
		//this._usesDepthTexture = context.depthTexture;
		
		if (m_is_point_light) 
		{
			m_uses_depth_texture = false;
		}

		m_terrain_bias = Bias(
			polygon_offset_supported,
            1.1,
            4.0,
            true,
            0.5,
            true,
            0.3,
            0.0001);

        m_primitive_bias = Bias(
            polygon_offset_supported,
            1.1,
            4.0,
            true,
            0.1,
            true,
            0.05,
            0.00002);

        m_point_bias = Bias(
            false,
            1.1,
            4.0,
            false,
            0.0,
            true,
            0.1,
            0.0005);

		create_render_states();
	}


	ShadowMap::~ShadowMap()
	{
	}

	void ShadowMap::create_render_states()
	{
		// Enable the color mask if the shadow map is backed by a color texture, e.g. when depth textures aren't supported
		bool color_mask = !m_uses_depth_texture;
		m_primitive_render_state = create_render_state(color_mask, m_primitive_bias);
		m_terrain_render_state   = create_render_state(color_mask, m_terrain_bias);
		m_point_render_state     = create_render_state(color_mask, m_point_bias);
	}

	RenderState* ShadowMap::create_render_state(
		const bool& color_mask,
		const Bias& bias)
	{
		RenderStateParameters params;
		params.facet_culling = FacetCulling(
			true,
			CullFace::Back);

		params.depth_test = DepthTest(true);
		params.color_mask = ColorMask(color_mask);
		params.depth_mask = true;
		params.polygon_offset = PolygonOffset(
			bias.polygon_offset,
			bias.polygon_offset_factor,
			bias.polygon_offset_units);

		return RenderState::from_cache(params);
	}

	void ShadowMap::destroy_frame_buffer()
	{
		for (ShadowPass& pass : m_passes) 
		{
			if (pass.frame_buffer != nullptr) 
			{
				delete pass.frame_buffer;
			}
			pass.frame_buffer = nullptr;
		}

		// Destroy the framebuffer attachments
		if (m_depth_render_buffer_attachment != nullptr)
		{
			delete m_depth_render_buffer_attachment;
			m_depth_render_buffer_attachment = nullptr;
		}
		
		if (m_depth_texture_attachment != nullptr)
		{
			delete m_depth_texture_attachment;
			m_depth_texture_attachment = nullptr;
		}

		if (m_color_attachment != nullptr)
		{
			delete m_color_attachment;
			m_color_attachment = nullptr;
		}
	}

	TextureSampler* ShadowMap::create_sampler()
	{
		return Device::get_texture_samplers()->get_nearest_clamp();
	}

	void ShadowMap::create_frame_buffer(Context* context)
	{
		if (m_is_point_light)
		{
			create_frame_buffer_cube(context);
		}
		else if (m_uses_depth_texture) 
		{
			create_frame_buffer_depth(context);
		}
		else 
		{
			create_frame_buffer_color(context);
		}
	}

	void ShadowMap::create_frame_buffer_color(Context* context)
	{
		m_depth_render_buffer_attachment = Device::create_render_buffer(RenderBufferDescription(
			m_texture_size,
			m_texture_size,
			RenderBufferFormat::DepthComponent16)).release();

		m_color_attachment = Device::create_texture2D(TextureDescription(
			m_texture_size,
			m_texture_size,
			TextureFormat::RedGreenBlueAlpha8,
			false),
			create_sampler()).release();

		m_shadow_map_texture = m_color_attachment;

		FrameBuffer* frame_buffer = context->create_frame_buffer().release();
		frame_buffer->set_depth_attachment_render_buffer(m_depth_render_buffer_attachment);
		frame_buffer->get_color_attachments()->set_attachment(0, m_color_attachment);

		for (ShadowPass& pass : m_passes)
		{
			pass.frame_buffer = frame_buffer;
			pass.pass_state.set_framebuffer(frame_buffer);
		}
	}

	void ShadowMap::create_frame_buffer_depth(Context* context)
	{
		m_depth_texture_attachment = Device::create_texture2D(TextureDescription(
			m_texture_size,
			m_texture_size,
			TextureFormat::Depth24Stencil8,
			false),
			create_sampler()).release();

		m_shadow_map_texture = m_depth_texture_attachment;

		FrameBuffer* frame_buffer = context->create_frame_buffer().release();
		frame_buffer->set_depth_stencil_attachment_texture(m_depth_texture_attachment);
		frame_buffer->get_color_attachments()->set_attachment(0, m_color_attachment);

		for (ShadowPass& pass : m_passes)
		{
			pass.frame_buffer = frame_buffer;
			pass.pass_state.set_framebuffer(frame_buffer);
		}
	}

	void ShadowMap::create_frame_buffer_cube(Context* context)
	{
		m_depth_render_buffer_attachment = Device::create_render_buffer(RenderBufferDescription(
			m_texture_size,
			m_texture_size,
			RenderBufferFormat::DepthComponent16)).release();
		
		CubeMap* cube_map = Device::create_cube_map(
			TextureDescription(
				m_texture_size,
				m_texture_size,
				TextureFormat::RedGreenBlueAlpha8),
			false,
			create_sampler()).release();
		
        std::vector<CubeMapFace*> faces = 
		{
			cube_map->get_negative_x(),
			cube_map->get_negative_y(),
			cube_map->get_negative_z(),
			cube_map->get_positive_x(),
			cube_map->get_positive_y(),
			cube_map->get_positive_z(),
		};
		
		for (unsigned int i = 0; i < 6; i++)
		{
			FrameBuffer* frame_buffer = context->create_frame_buffer().release();
			frame_buffer->set_depth_attachment_render_buffer(m_depth_render_buffer_attachment);
			frame_buffer->get_color_attachments()->set_attachment(i, faces[i]);
		
			ShadowPass& pass = m_passes[i];
            pass.frame_buffer = frame_buffer;
            pass.pass_state.set_framebuffer(frame_buffer);
        }

		m_shadow_map_texture = cube_map;
		m_color_attachment   = cube_map;
	}

	void ShadowMap::check_frame_buffer(Context* context)
	{
		// Attempt to make an FBO with only a depth texture. If it fails, fallback to a color texture.
		if (m_uses_depth_texture && m_passes[0].frame_buffer->get_status() != FrameBufferStatus::Complete)
		{
			m_uses_depth_texture = false;
			create_render_states();
			destroy_frame_buffer();
			create_frame_buffer(context);
		}
	}

	bool ShadowMap::get_is_point_light() const
	{
		return m_is_point_light;
	}

	bool ShadowMap::get_is_spot_light() const
	{
		return m_is_spot_light;
	}

	unsigned int ShadowMap::get_number_of_cascades() const
	{
		return m_number_of_cascades;
	}

	bool ShadowMap::get_soft_shadows() const
	{
		return m_soft_shadows;
	}

	bool ShadowMap::get_uses_depth_texture() const
	{
		return m_uses_depth_texture;
	}

	bool ShadowMap::get_polygon_offset_supported() const
	{
		return false;
	}

	Bias ShadowMap::get_terrain_bias() const
	{
		return m_terrain_bias;
	}

	Bias ShadowMap::get_primitive_bias() const
	{
		return m_primitive_bias;
	}

	Bias ShadowMap::get_point_bias() const
	{
		return m_point_bias;
	}

	void ShadowMap::update_frame_buffer(Context* context)
	{
		if (m_passes[0].frame_buffer == nullptr || m_shadow_map_texture->get_description().get_width() != m_texture_size)
		{
			destroy_frame_buffer();
			create_frame_buffer(context);
			check_frame_buffer(context);
			clear_frame_buffer(context);
		}
	}

	void ShadowMap::clear_frame_buffer(
		Context* context,
		const unsigned int& shadow_pass)
	{
		if (m_is_point_light || shadow_pass == 0)
		{
			m_clear_command.frame_buffer = m_passes[shadow_pass].frame_buffer;
			m_clear_command.execute(context, &m_clear_pass_state);
		}
	}

	void ShadowMap::resize(const std::size_t& size)
	{
		//unsigned int number_of_passes = m_passes.size();
		//
		//if (m_is_point_light) 
		//{
		//	std::size_t size = (Device::get_maximum_cube_map_size() >= size) ? size : Device::get_maximum_cube_map_size();
		//	m_texture_size = size;
		//	m_texture_size = size;
		//
		//	Rectangle face_viewport(0, 0, size, size);
		//	m_passes[0].pass_state.set_viewport(face_viewport);
		//	m_passes[1].pass_state.viewport = face_viewport;
		//	m_passes[2].pass_state.viewport = face_viewport;
		//	m_passes[3].pass_state.viewport = face_viewport;
		//	m_passes[4].pass_state.viewport = face_viewport;
		//	m_passes[5].pass_state.viewport = face_viewport;
		//}
		//else if (number_of_passes == 1)
		//{
		//	// +----+
		//	// |  1 |
		//	// +----+
		//	std::size_t size = (Device::get_maximum_texture_size() >= size) ? size : Device::get_maximum_texture_size();
		//	m_texture_size = size;
		//	m_passes[0].pass_state.set_viewport(face_viewport);
		//}
		//else if (number_of_passes == 4)
		//{
		//	// +----+----+
		//	// |  3 |  4 |
		//	// +----+----+
		//	// |  1 |  2 |
		//	// +----+----+
		//	std::size_t size = (Device::get_maximum_texture_size() >= size * 2) ? size : Device::get_maximum_texture_size() / 2;
		//	m_texture_size = size * 2;
		//
		//	passes[0].passState.viewport = new BoundingRectangle(0, 0, size, size);
		//	passes[1].passState.viewport = new BoundingRectangle(size, 0, size, size);
		//	passes[2].passState.viewport = new BoundingRectangle(0, size, size, size);
		//	passes[3].passState.viewport = new BoundingRectangle(size, size, size, size);
		//}
		//
		//// Update clear pass state
		//shadowMap._clearPassState.viewport = new BoundingRectangle(0, 0, textureSize.x, textureSize.y);
		//
		//// Transforms shadow coordinates [0, 1] into the pass's region of the texture
		//for (unsigned int i = 0; i < number_of_passes; ++i) 
		//{
		//	ShadowPass& pass    = m_passes[i];
		//	Rectangle* viewport = pass.pass_state.get_viewport();
		//
		//	double biasX  = viewport->get_left()  / m_texture_size;
		//	double biasY  = viewport->get_right() / m_texture_size;
		//	double scaleX = viewport->get_width   / m_texture_size;
		//	double scaleY = viewport->get_height  / m_texture_size;
		//
		//	pass.texture_offsets = mat4(
		//		scaleX, 0.0, 0.0, biasX, 
		//		0.0,    scaleY, 0.0, biasY, 
		//		0.0, 0.0,    1.0,    0.0,
		//		0.0, 0.0,    0.0,    1.0);
		//}
	}

	void ShadowMap::compute_cascades(FrameState* frame_state)
	{
		double camera_near = m_scene_camera->get_frustum()->get_near();
		double camera_far  = m_scene_camera->get_frustum()->get_far();
		
		// Split cascades. Use a mix of linear and log splits.
		double range = camera_far - camera_near;
		double ratio = camera_far / camera_near;
		
		double lambda = 0.9;
		bool   clamp_cascade_distances = false;
		
		// When the camera is close to a relatively small model, provide more detail in the closer cascades.
		// If the camera is near or inside a large model, such as the root tile of a city, then use the default values.
		// To get the most accurate cascade splits we would need to find the min and max values from the depth texture.
		// TODO: Shadow Hints
		//if (frameState.shadowHints.closestObjectSize < 200.0) 
		//{
		//	clampCascadeDistances = true;
		//	lambda = 0.9;
		//}
		
		std::vector<double> cascade_distances(m_number_of_cascades);
		std::vector<double> splits(m_number_of_cascades);		
		splits[0] = camera_near;
		splits[m_number_of_cascades] = camera_far;
		
		// Find initial splits
		for (unsigned int i = 0; i < m_number_of_cascades; ++i) 
		{
			double p = static_cast<double>(i + 1) / m_number_of_cascades;
			double log_scale     = camera_near * ::pow(ratio, p);
			double uniform_scale = camera_near + range * p;
			double split = ArgeoMath::lerp(uniform_scale, log_scale, lambda);

			splits[i + 1] = split;
			cascade_distances[i] = split - splits[i];
		}
		
		if (clamp_cascade_distances)
		{
			// Clamp each cascade to its maximum distance
			for (unsigned int i = 0; i < m_number_of_cascades; ++i) 
			{
				cascade_distances[i] = ::fmin(
					cascade_distances[i],
					m_maximum_cascade_distance[i]);
			}
		
			// Recompute splits
			double distance = splits[0];
			for (unsigned int i = 0; i < m_number_of_cascades - 1; ++i) 
			{
				distance += cascade_distances[i];
				splits[i + 1] = distance;
			}
		}
		
		m_cascade_splits[0] = vec4d(
			splits[0],
			splits[1],
			splits[2],
			splits[3]);

		m_cascade_splits[1] = vec4d(
			splits[4],
			splits[5],
			splits[6],
			splits[7]);

		m_cascade_distances = vec4d(
			cascade_distances[0],
			cascade_distances[1],
			cascade_distances[2],
			cascade_distances[3]);

		OrthographicFrustum* shadow_frustum = static_cast<OrthographicFrustum*>(m_shadow_map_camera.frustum.get());
		double left   = shadow_frustum->get_left();
		double right  = shadow_frustum->get_right();
		double bottom = shadow_frustum->get_bottom();
		double top    = shadow_frustum->get_top();
		double near   = shadow_frustum->get_near();
		double far    = shadow_frustum->get_far();
		
		vec3d position  = m_shadow_map_camera.position_wc;
		vec3d direction = m_shadow_map_camera.direction_wc;
		vec3d up        = m_shadow_map_camera.up_wc;
		
		Frustum* cascade_sub_frustum = m_scene_camera->get_frustum()->clone();
		mat4d shadow_view_projection = m_shadow_map_camera.get_view_projection_matrix();
		
		for (unsigned int i = 0; i < m_number_of_cascades; ++i) 
		{
			// Find the bounding box of the camera sub-frustum in shadow map texture space
			cascade_sub_frustum->set_near(splits[i]);
			cascade_sub_frustum->set_far(splits[i + 1]);

			mat4d view_projection         = cascade_sub_frustum->get_projection() * m_scene_camera->get_view_matrix();
			mat4d inverse_view_projection = mat_inverse(view_projection);
			mat4d shadow_map_matrix       = shadow_view_projection * inverse_view_projection;
		
			// Project each corner from camera NDC space to shadow map texture space. Min and max will be from 0 to 1.
			vec3d min(std::numeric_limits<double>::max());
			vec3d max(-std::numeric_limits<double>::max());
		
			for (unsigned int k = 0; k < 8; ++k) 
			{
				vec4d corner = g_frustum_corners_ndc[k];

				corner *= shadow_map_matrix;

				// Handle the perspective divide
				corner /= corner.w;
				min = vec_min_by_component(to_vec3d(corner), min);
				max = vec_max_by_component(to_vec3d(corner), max);
			}
		
			// Limit light-space coordinates to the [0, 1] range
			min.x = ::fmax(min.x, 0.0);
			min.y = ::fmax(min.y, 0.0);
			min.z = 0.0; // Always start cascade frustum at the top of the light frustum to capture objects in the light's path
			max.x = ::fmin(max.x, 1.0);
			max.y = ::fmin(max.y, 1.0);
			max.z = ::fmin(max.z, 1.0);
		
			ShadowPass& pass = m_passes[i];
			ShadowMapCamera& cascade_camera = pass.camera;

			// PERFORMANCE_IDEA : could do a shallow clone for all properties except the frustum
			cascade_camera.position_wc = m_shadow_map_camera.position_wc;
			cascade_camera.position_geodetic = m_shadow_map_camera.position_geodetic;
			cascade_camera.direction_wc = m_shadow_map_camera.direction_wc;
			cascade_camera.up_wc = m_shadow_map_camera.up_wc;
			cascade_camera.right_wc = m_shadow_map_camera.right_wc;
			cascade_camera.inverse_view_matrix = m_shadow_map_camera.inverse_view_matrix;
			cascade_camera.view_matrix = m_shadow_map_camera.view_matrix;
			//cascade_camera.frustum.reset(m_shadow_map_camera.frustum->clone());
		
			OrthographicFrustum* frustum = static_cast<OrthographicFrustum*>(cascade_camera.frustum.get());
			frustum->set_left(left + min.x * (right - left));
			frustum->set_right(left + max.x * (right - left));
			frustum->set_bottom(bottom + min.y * (top - bottom));
			frustum->set_top(bottom + max.y * (top - bottom));
			frustum->set_near(near + min.z * (far - near));
			frustum->set_far(near + max.z * (far - near));
		
			pass.culling_volume = cascade_camera.frustum->get_culling_volume(
				position,
				direction,
				up);
		
			// Transforms from eye space to the cascade's texture space
			mat4d& cascade_matrix = m_cascade_matrices[i];
			cascade_matrix = cascade_camera.get_view_projection_matrix() * m_scene_camera->get_inverse_view_matrix();
			cascade_matrix = pass.texture_offsets * cascade_matrix;
		}
	}

	void ShadowMap::fit_shadow_map_to_scene(FrameState* frame_state)
	{
		// TODO: Fast inverse transformation.
		// 1. First find a tight bounding box in light space that contains the entire camera frustum.
		mat4d view_projection =
			m_scene_camera->get_frustum()->get_projection() * 
			m_scene_camera->get_view_matrix();
		mat4d inverse_view_projection = mat_inverse(view_projection);

		// Start to construct the light view matrix. Set translation later once the bounding box is found.
		vec3d light_dir   = m_shadow_map_camera.direction_wc;
		vec3d light_up    = m_scene_camera->get_direction_wc(); // Align shadows to the camera view.
		vec3d light_right = vec_cross(light_dir, light_up);
		light_up		  = vec_cross(light_right, light_dir); // Recalculate up now that right is derived
		light_up		  = vec_normalize(light_up);
		light_right		  = vec_normalize(light_right);

		vec3d light_position(0.0);
		mat4d light_view      = compute_view(light_position, light_dir, light_up, light_right);
		mat4d camera_to_light = light_view * inverse_view_projection;

		// Project each corner from NDC space to light view space, and calculate a min and max in light view space
		vec3d min(std::numeric_limits<double>::max());
		vec3d max(-std::numeric_limits<double>::max());
		for (unsigned int i = 0; i < 8; ++i) 
		{
			vec4d corner = g_frustum_corners_ndc[i];
			corner *= camera_to_light;

			// Handle the perspective divide
			corner /= corner.w;
			min = vec_min_by_component(to_vec3d(corner), min);
			max = vec_max_by_component(to_vec3d(corner), max);
		}

		// 2. Set bounding box back to include objects in the light's view
		max.z += 1000.0; // Note: in light space, a positive number is behind the camera
		min.z -= 10.0; // Extend the shadow volume forward slightly to avoid problems right at the edge

		// 3. Adjust light view matrix so that it is centered on the bounding volume
		vec3d translation(
			-(0.5 * (min.x + max.x)),
			-(0.5 * (min.y + max.y)),
			-max.z);

		mat4d translation_matrix = mat_translate(translation);
		light_view = translation_matrix * light_view;

		// 4. Create an orthographic frustum that covers the bounding box extents
		double half_width = 0.5 * (max.x - min.x);
		double half_height = 0.5 * (max.y - min.y);
		double depth = max.z - min.z;

		OrthographicFrustum* frustum = static_cast<OrthographicFrustum*>(m_shadow_map_camera.frustum.get());
		frustum->set_left(-half_width);
		frustum->set_right(half_width);
		frustum->set_bottom(-half_height);
		frustum->set_top(half_height);
		frustum->set_near(0.01);
		frustum->set_far(depth);

		// 5. Update the shadow map camera
		m_shadow_map_camera.view_matrix = light_view;
		m_shadow_map_camera.inverse_view_matrix = mat_inverse(light_view);
		m_shadow_map_camera.position_wc = mat_get_translation(m_shadow_map_camera.inverse_view_matrix);
		m_shadow_map_camera.position_geodetic = frame_state->get_map_projection()->get_ellipsoid().to_geodetic3D(m_shadow_map_camera.position_wc);
		m_shadow_map_camera.direction_wc = light_dir;
		m_shadow_map_camera.up_wc        = light_up;
		m_shadow_map_camera.right_wc     = light_right;
	}


	void ShadowMap::compute_omnidirectional(FrameState* frame_state)
	{
		// All sides share the same frustum
		PerspectiveFrustum* frustum = new PerspectiveFrustum();
		frustum->set_field_of_view_y(PI_OVER_TWO);
		frustum->set_near(1.0);
		frustum->set_far(m_point_light_radius);
		frustum->set_aspect_ratio(1.0);

		for (unsigned int i = 0; i < 6; ++i) 
		{
			ShadowMapCamera& camera = m_passes[i].camera;

			camera.position_wc = m_shadow_map_camera.position_wc;
			camera.position_geodetic = frame_state->get_map_projection()->get_ellipsoid().to_geodetic3D(camera.position_wc);
			camera.direction_wc      = g_directions[i];
			camera.up_wc		     = g_ups[i];
			camera.right_wc			 = g_rights[i];

			camera.view_matrix = compute_view(
				camera.position_wc,
				camera.direction_wc,
				camera.up_wc,
				camera.right_wc);
			camera.inverse_view_matrix = mat_inverse(camera.view_matrix);
			
			camera.frustum.reset(frustum);
		}

	}

	void ShadowMap::check_visibility(FrameState* frame_state)
	{
		// Check whether the shadow map is in view and needs to be updated
		if (m_cascades_enabled) 
		{
			// If the nearest shadow receiver is further than the shadow map's maximum distance then the shadow map is out of view.
			if (m_scene_camera->get_frustum()->get_near() >= m_maximum_distance) 
			{
				m_out_of_view = true;
				m_needs_update = false;
				return;
			}

			// If the light source is below the horizon then the shadow map is out of view
			vec3d surface_normal  = frame_state->get_map_projection()->get_ellipsoid().geodetic_surface_normal(m_scene_camera->get_position_wc()).to_vec3d();
			vec3d light_direction = -m_shadow_map_camera.direction_wc;
			double dot = vec_dot(surface_normal, light_direction);

			// Shadows start to fade out once the light gets closer to the horizon.
			// At this point the globe uses vertex lighting alone to darken the surface.
			double darkness_amount = ArgeoMath::clamp(dot / 0.1, 0.0, 1.0);
			m_darkness = ArgeoMath::lerp(1.0, m_darkness, darkness_amount);

			if (dot < 0.0) 
			{
				m_out_of_view = true;
				m_needs_update = false;
				return;
			}

			// By default cascaded shadows need to update and are always in view
			m_needs_update = true;
			m_out_of_view = false;
		}
		else if (m_is_point_light) 
		{
			// Sphere-frustum intersection test
			BoundingSphere* bounding_sphere = new BoundingSphere(
				m_shadow_map_camera.position_wc,
				m_point_light_radius);

			m_out_of_view  = frame_state->get_culling_volume()->visibility(bounding_sphere) == Intersect::Outside;
			m_needs_update = !m_out_of_view && !((*bounding_sphere).equals(*m_bounding_sphere));

			m_bounding_sphere.reset(bounding_sphere);
		}
		else 
		{
			// Simplify frustum-frustum intersection test as a sphere-frustum test
			double frustum_radius = m_shadow_map_camera.frustum->get_far() / 2.0;
			vec3d  frustum_center = m_shadow_map_camera.position_wc + (m_shadow_map_camera.direction_wc * frustum_radius);

			BoundingSphere* bounding_sphere = new BoundingSphere(
				frustum_center,
				frustum_radius);

			m_out_of_view  = frame_state->get_culling_volume()->visibility(bounding_sphere) == Intersect::Outside;
			m_needs_update = !m_out_of_view && !((*bounding_sphere).equals(*m_bounding_sphere));

			m_bounding_sphere.reset(bounding_sphere);
		}

	}

	void ShadowMap::update_cameras(FrameState* frame_state)
	{
		// The actual camera in the scene
		Camera* camera = frame_state->get_camera(); 

		// Clone light camera into the shadow map camera
		if (m_cascades_enabled) 
		{
			m_shadow_map_camera.direction_wc = m_light_camera->get_direction_wc();
		}
		else if (m_is_point_light) 
		{
			m_shadow_map_camera.position_wc = m_light_camera->get_position_wc();
		}
		else 
		{
			m_shadow_map_camera.direction_wc		= m_light_camera->get_direction_wc();
			m_shadow_map_camera.right_wc			= m_light_camera->get_right_wc();
			m_shadow_map_camera.up_wc				= m_light_camera->get_up_wc();
			m_shadow_map_camera.position_wc         = m_light_camera->get_position_wc();
			m_shadow_map_camera.position_geodetic   = m_light_camera->get_position_geodetic();
			m_shadow_map_camera.inverse_view_matrix = m_light_camera->get_inverse_view_matrix();
			m_shadow_map_camera.view_matrix         = m_light_camera->get_view_matrix();
			m_shadow_map_camera.frustum.reset(m_light_camera->get_frustum()->clone());
		}

		// Get the light direction in eye coordinates
		m_light_direction_ec =  mat_multiply_by_point_as_vector(camera->get_view_matrix(), m_shadow_map_camera.direction_wc);
		m_light_direction_ec = -vec_normalize(m_light_direction_ec);

		// Get the light position in eye coordinates
		m_light_position_ec = vec4d(
			mat_multiply_by_point(camera->get_view_matrix(), m_shadow_map_camera.position_wc),
			m_point_light_radius);

		// Get the near and far of the scene camera
		double near;
		double far;
		if (m_fit_near_far) 
		{
			// shadowFar can be very large, so limit to shadowMap.maximumDistance
			// Push the far plane slightly further than the near plane to avoid degenerate frustum
			near = ::fmin(frame_state->get_shadow_hints().near_plane, m_maximum_distance);
			far  = ::fmin(frame_state->get_shadow_hints().far_plane, m_maximum_distance + 1.0);
		}
		else 
		{
			near = camera->get_frustum()->get_near();
			far  = m_maximum_distance;
		}

		m_scene_camera = camera->clone();
		m_scene_camera->get_frustum()->set_near(near);
		m_scene_camera->get_frustum()->set_far(far);
		m_distance = far - near;

		check_visibility(frame_state);

		if (!m_out_of_view_previous && m_out_of_view) 
		{
			m_needs_update = true;
		}

		m_out_of_view_previous = m_out_of_view;
	}


	void ShadowMap::update(FrameState* frame_state)
	{
		update_cameras(frame_state);

		if (m_needs_update) 
		{
			update_frame_buffer(frame_state->get_context());

			if (m_is_point_light) 
			{
				compute_omnidirectional(frame_state);
			}

			if (m_cascades_enabled) 
			{
				fit_shadow_map_to_scene(frame_state);

				if (m_number_of_cascades > 1) 
				{
					compute_cascades(frame_state);
				}
			}

			if (!m_is_point_light) 
			{
				// Compute the culling volume
				m_shadow_map_culling_volume = m_shadow_map_camera.frustum->get_culling_volume(
					m_shadow_map_camera.position_wc, 
					m_shadow_map_camera.direction_wc,
					m_shadow_map_camera.up_wc);

				if (m_passes.size() == 1)
				{
					// Since there is only one pass, use the shadow map camera as the pass camera.
					m_passes[0].camera = m_shadow_map_camera;
				}
			}
			else 
			{
				m_shadow_map_culling_volume = CullingVolume::from_bounding_sphere(m_bounding_sphere.get());
			}
		}

		if (m_passes.size() == 1) 
		{
			// Transforms from eye space to shadow texture space.
			// Always requires an update since the scene camera constantly changes.
			m_shadow_map_matrix = 
				m_shadow_map_camera.get_view_projection_matrix() * 
				m_scene_camera->get_inverse_view_matrix();
		}
	}


	void ShadowMap::update_pass(
		Context* context,
		const unsigned int& shadow_pass)
	{
		clear_frame_buffer(
			context, 
			shadow_pass);
	}

	void ShadowMap::create_derived_commands(
		std::vector<ShadowMap*> shadow_maps,
		std::vector<ShadowMap*> light_shadow_maps,
		DrawCommand* command,
		const bool& shadows_dirty,
		Context* context,
		DerivedCommands::ShadowCommand& derived_commands)
	{
		ShaderProgram* shader_program = command->get_shader_program();
		std::string vertex_shader_source = shader_program->get_vertex_shader()->get_source();
		std::string fragment_shader_source = shader_program->get_fragment_shader()->get_source();
	
		bool light_shadow_maps_enabled = (light_shadow_maps.size() > 0);
		bool is_terrain = command->get_pass() == CommandPass::Globe;
		bool has_terrain_normal = false;
		if (is_terrain)
		{
			// TODO: This
			has_terrain_normal = true;//command.owner.data.pickTerrain.mesh.encoding.hasVertexNormals;
		}

		if (command->get_cast_shadows()) 
		{
			unsigned int old_shader_id = derived_commands.cast_shader_program_id;
			unsigned int shadow_map_length = shadow_maps.size();
		
			derived_commands.cast_commands.resize(shadow_map_length);
			for (unsigned int i = 0; i < shadow_map_length; ++i)
			{
				shadow_maps[i]->create_cast_derived_command(
					shadows_dirty,
					command, 
					context,
					old_shader_id, 
					derived_commands.cast_commands[i]);
			}
		
			derived_commands.cast_shader_program_id = command->get_shader_program()->get_id();
		}
		
		if (command->get_receive_shadows() && light_shadow_maps_enabled)
		{
			// Only generate a receiveCommand if there is a shadow map originating from a light source.
			ShaderProgram* receive_shader = nullptr;
			UniformMap*    receive_uniform_map = nullptr;
			
			if (derived_commands.receive_command != nullptr) 
			{
				receive_shader = derived_commands.receive_command->get_shader_program();
				receive_uniform_map = derived_commands.receive_command->get_uniform_map();
			}
		
			derived_commands.receive_command = command->shallow_clone(derived_commands.receive_command);
			derived_commands.cast_shadows    = false;
			derived_commands.receive_shadows = true;
		
			// If castShadows changed, recompile the receive shadows shader. The normal shading technique simulates
			// self-shadowing so it should be turned off if castShadows is false.
			bool cast_shadows_dirty = derived_commands.receive_shader_cast_shadows != command->get_cast_shadows();
			bool shader_dirty = derived_commands.receive_shader_program_id != command->get_shader_program()->get_id();
		
			if (receive_shader == nullptr || shader_dirty || shadows_dirty || cast_shadows_dirty)
			{
				if (receive_shader != nullptr)
				{
					delete receive_shader;
				}
		
				std::string receiveVS = ShadowMapShader::create_shadow_receive_vertex_shader(
					vertex_shader_source, 
					is_terrain, 
					has_terrain_normal);

				std::string receiveFS = ShadowMapShader::create_shadow_receive_fragment_shader(
					fragment_shader_source, 
					light_shadow_maps[0], 
					command->get_cast_shadows(),
					is_terrain,
					has_terrain_normal);
		
				receive_shader = context->get_shader_cache()->get_shader_program(
					receiveVS,
					receiveFS,
					shader_program->get_attribute_locations());

				receive_uniform_map = light_shadow_maps[0]->combine_uniforms(
					command->get_uniform_map(), 
					is_terrain);
			}
		
			derived_commands.receive_command->set_shader_program(receive_shader);
			derived_commands.receive_command->set_uniform_map(receive_uniform_map);
			derived_commands.receive_shader_program_id = command->get_shader_program()->get_id();
			derived_commands.receive_shader_cast_shadows = command->get_cast_shadows();
		}
	}


	void ShadowMap::create_cast_derived_command(
		const bool& shadows_dirty,
		DrawCommand* command,
		Context* context,
		const unsigned int& old_shader_id,
		DrawCommand* cast_command)
	{
		ShaderProgram* cast_shader = nullptr;
		RenderState* cast_render_state = nullptr;
		UniformMap* cast_uniform_map = nullptr;
		
		if (cast_command != nullptr)
		{
			cast_shader       = cast_command->get_shader_program();
			cast_render_state = cast_command->get_render_state();
			cast_uniform_map  = cast_command->get_uniform_map();
		}

		cast_command = command->shallow_clone(cast_command);
		cast_command->set_cast_shadows(true);
		cast_command->set_receive_shadows(false);

		if (cast_shader == nullptr || old_shader_id != command->get_shader_program()->get_id() || shadows_dirty)
		{
			if (cast_shader != nullptr) 
			{
				delete cast_shader;
			}

			ShaderProgram* shader_program = command->get_shader_program();
			std::string vertex_shader_source = shader_program->get_vertex_shader()->get_source();
			std::string fragment_shader_source = shader_program->get_fragment_shader()->get_source();

			bool is_terrain = command->get_pass() == CommandPass::Globe;
			bool is_opaque  = command->get_pass() != CommandPass::Translucent;
			bool is_point_light = m_is_point_light;
			bool uses_depth_texture = m_uses_depth_texture;

			std::string castVS = ShadowMapShader::create_shadow_cast_vertex_shader(
				vertex_shader_source, 
				is_point_light,
				is_terrain);

			std::string castFS = ShadowMapShader::create_shadow_cast_fragment_shader(
				fragment_shader_source, 
				is_point_light,
				uses_depth_texture,
				is_opaque);

			cast_shader = context->get_shader_cache()->get_shader_program(
				castVS,
				castFS,
				shader_program->get_attribute_locations());

			cast_render_state = m_primitive_render_state;
			if (is_point_light) 
			{
				cast_render_state = m_point_render_state;
			}
			else if (is_terrain) 
			{
				cast_render_state = m_terrain_render_state;
			}

			// Modify the render state for commands that do not use back-face culling, e.g. flat textured walls
			bool cull_enabled = command->get_render_state()->get_facet_culling().enabled;
			if (!cull_enabled) 
			{
				RenderStateParameters params = cast_render_state->get_params();
				params.facet_culling.enabled = false;
				cast_render_state = RenderState::from_cache(params);
			}

			cast_uniform_map = combine_uniforms(
				command->get_uniform_map(), 
				is_terrain);
		}

		cast_command->set_shader_program(cast_shader);
		cast_command->set_render_state(cast_render_state);
		cast_command->set_uniform_map(cast_uniform_map);
	}

	std::vector<ShadowPass>& ShadowMap::get_passes()
	{
		return m_passes;
	}

	CullingVolume* ShadowMap::get_culling_volume()
	{
		return m_shadow_map_culling_volume.get();
	}

	bool ShadowMap::get_is_out_of_view() const
	{
		return m_out_of_view;
	}

	UniformMap* ShadowMap::combine_uniforms(
		UniformMap* uniform_map,
		const bool& is_terrain)
	{
		Bias bias = m_is_point_light ?
			m_point_bias : (is_terrain ? m_terrain_bias : m_primitive_bias);

		UniformMap* uniforms = new UniformMap
		{
			{
				"shadowMap_texture",
				[&](IUniform* uniform)
				{
					static_cast<UniformSampler*>(uniform)->set_texture(m_shadow_map_texture);
				}
			},
			{
				"shadowMap_textureCube",
				[&](IUniform* uniform)
				{
					static_cast<UniformSampler*>(uniform)->set_texture(m_shadow_map_texture);
				}
			},
			{
				"shadowMap_matrix",
				[&](IUniform* uniform)
				{
					static_cast<Uniform<mat4d>*>(uniform)->set_value(m_shadow_map_matrix);
				}
			},
			{
				"shadowMap_cascadeSplits",
				[&](IUniform* uniform)
				{
					static_cast<UniformArray<vec4d>*>(uniform)->set_values(m_cascade_splits);
				}
			},
			{
				"shadowMap_cascadeMatrices",
				[&](IUniform* uniform)
				{
					static_cast<UniformArray<mat4d>*>(uniform)->set_values(m_cascade_matrices);
				}
			},
			{
				"shadowMap_cascadeDistances",
				[&](IUniform* uniform)
				{
					static_cast<Uniform<vec4d>*>(uniform)->set_value(m_cascade_distances);
				}
			},
			{
				"shadowMap_lightDirectionEC",
				[&](IUniform* uniform)
				{
					static_cast<Uniform<vec3d>*>(uniform)->set_value(m_light_direction_ec);
				}
			},
			{
				"shadowMap_lightPositionEC",
				[&](IUniform* uniform)
				{
					static_cast<Uniform<vec4d>*>(uniform)->set_value(m_light_position_ec);
				}
			},
			{
				"shadowMap_texelSizeDepthBiasAndNormalShadingSmooth",
				[&, bias](IUniform* uniform)
				{
					static_cast<Uniform<vec4>*>(uniform)->set_value(vec4(
						1.0 / m_texture_size,
						1.0 / m_texture_size,
						bias.depth_bias,
						bias.normal_shading_smooth));
				}
			},
			{
				"shadowMap_normalOffsetScaleDistanceMaxDistanceAndDarkness",
				[&, bias](IUniform* uniform)
				{
					static_cast<Uniform<vec4>*>(uniform)->set_value(vec4(
						bias.normal_offset_scale, 
						m_distance, 
						m_maximum_distance,
						m_darkness));
				}
			},
		};

		uniforms->insert(uniform_map->begin(), uniform_map->end());
		return uniforms;
	}

}