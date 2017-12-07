#include "Scene.h"

#include "PreRenderListener.h"
#include "PostRenderListener.h"
#include "UniformSampler.h"
#include "SceneTransforms.h"
#include "GeologicalImageSource.h"
#include "SrtmTerrainSource.h"
#include "ClipmapTerrain.h"
#include "Device.h"
#include "GeographicProjection.h"
#include "BitmapAlgorithms.h"
#include "Transforms.h"

#include <async.h>

namespace argeo
{
    double Scene::OPAQUE_FRUSTUM_NEAR_OFFSET = 0.99;

    Scene::Scene(Context* context)
		: m_context(context)
		, m_frame_state(context)
		, m_pass_state(context)
		, m_camera_preview(nullptr)
		, m_shader_frame_count(0)
		, m_terrain_opacity(0.5)
		, m_projection(new GeographicProjection())
		, m_camera(new Camera(this))
		, m_last_camera(new Camera(this))
		, m_sun_camera(new Camera(this))
        , m_downloader(new PboDownloader())
		, m_render_to_viewport_command(nullptr)
		, m_picking_rectangle(Rectangle(0, 0, 3, 3))
		, m_picking_mode(PickingMode::Single)
		, m_far_to_near_ratio(1000.0)
        , m_near_to_far_distance_2D(1.75e6)
        , m_mode(SceneMode::Scene3D)
        , m_map_mode(MapMode2D::InfiniteScroll)
        , m_tween_collection(new TweenCollection())
        , m_transitioner(new SceneTransitioner(this))
        , m_use_vr(false)
        , m_copy_terrain_depth(false)
        , m_pick_position_cache_dirty(true)
    {
		RenderStateParameters parameters;
		parameters.depth_test.enabled = true;
		parameters.blending.enabled = true;

		m_clear_color.buffers = ClearBuffers::ColorAndStencilBuffer;
		m_clear_color.color = Color(0, 0, 0, 0);
		m_clear_color.stencil = 0;

		m_clear_depth.buffers = ClearBuffers::DepthBuffer;
		m_clear_depth.depth = 1.0;

		////////////////////////////////////////////////////////////////
		//						RENDER TO TEXTURE
		////////////////////////////////////////////////////////////////

		std::string fs =
			R"(
				
				uniform sampler2D u_texture;
				uniform float u_opacity;

				in  vec2 fsTextureCoordinates;
				out vec4 fragColor;

				void main()
				{
					fragColor    = texture(u_texture, fsTextureCoordinates);
					fragColor.a *= u_opacity;
				})";

		m_render_to_viewport_command = std::unique_ptr<DrawCommand>(
			m_context->create_viewport_quad_command(fs));

		m_terrain_to_ar_buffer_uniform_map = UniformMap
		{
			{
				"u_texture",
				[&](IUniform* uniform) { static_cast<UniformSampler*>(uniform)->set_texture(m_terrain_depth->get_color_texture()); }
			},
			{
				"u_opacity",
				[&](IUniform* uniform) { static_cast<Uniform<float>*>(uniform)->set_value(m_terrain_opacity); }
			}
		};

		m_mixed_reality_to_display_uniform_map = UniformMap
		{
			{
				"u_texture",
				[&](IUniform* uniform) { static_cast<UniformSampler*>(uniform)->set_texture(const_cast<Texture*>(m_ar_buffer->get_mixed_reality_frame_buffer()->get_color_attachments()->get_attachment(0))); }
			},
			{
				"u_opacity",
				[&](IUniform* uniform) { static_cast<Uniform<float>*>(uniform)->set_value(1.0); }
			}
		};

		Ellipsoid ellipsoid = Ellipsoid::wgs84();
		m_terrain = std::unique_ptr<ITerrain>(new ClipmapTerrain(
			this,
			ellipsoid,
			511));

        m_default_raster_source = std::unique_ptr<RasterSource>(new SrtmTerrainSource(m_terrain.get()));
        m_default_imagery = std::unique_ptr<RasterSource>(new GeologicalImageSource(m_terrain.get()));
        m_raster_source   = m_default_raster_source.get();

		m_terrain->set_imagery_source(m_default_imagery.get());
        m_terrain->set_raster_source(m_raster_source);

		UniformState* uniform_state = m_context->get_uniform_state();
		uniform_state->set_diffuse_intensity(0.90f);
		uniform_state->set_specular_intensity(0.05f);
		uniform_state->set_ambient_intensity(0.05f);


		////////////////////////////////////////////////////////////////
		//						SHADOWS
		////////////////////////////////////////////////////////////////

		m_shadow_map = std::unique_ptr<ShadowMap>(new ShadowMap(
			context,
			m_sun_camera.get(),
			true));

		////////////////////////////////////////////////////////////////
		//						DEPTH
		////////////////////////////////////////////////////////////////
		
		m_use_depth_picking = true;

		m_ar_buffer     = std::unique_ptr<ARBuffer>(new ARBuffer());
        m_ttar_buffer   = std::unique_ptr<ARBuffer>(new ARBuffer());
		m_depth_plane   = std::unique_ptr<DepthPlane>(new DepthPlane());
		m_terrain_depth = std::unique_ptr<TerrainDepth>(new TerrainDepth());

		// initial guess at frustums.
		const double near = m_camera->get_frustum()->get_near();
		const double far = m_camera->get_frustum()->get_far();
		const unsigned int num_frustums = ::ceil(::log(far / near) / ::log(m_far_to_near_ratio));

		update_frustums(
			near,
			far,
			num_frustums);
	}


	Scene::~Scene()
	{ }

    TweenCollection* Scene::get_tween_collection()
    {
        return m_tween_collection.get();
    }

    void Scene::set_map_mode_2D(const MapMode2D& value)
    {
        m_map_mode = value;
    }

    MapMode2D Scene::get_map_mode_2D() const
    {
        return m_map_mode;
    }

    SceneMode Scene::get_mode() const
    {
        return m_mode;
    }

    void Scene::set_mode(const SceneMode &value)
    {
        if (m_mode != value)
        {
            if (value == SceneMode::Scene2D)
            {
                morph_to_2D(0);
            }
            else if (value == SceneMode::Scene3D)
            {
                morph_to_3D(0);
            }
            else if(value == SceneMode::SceneCV)
            {
                morph_to_CV(0);
            }
            else
            {
                throw std::invalid_argument("value must be a valid SceneMode enumeration.");
            }

            m_mode = value;
        }
    }

    void Scene::morph_to_2D(const double& duration)
    {
        m_transitioner->morph_to_2D(
                duration,
                m_terrain->get_ellipsoid());
    }

    void Scene::morph_to_3D(const double& duration)
    {
        m_transitioner->morph_to_3D(
                duration,
                m_terrain->get_ellipsoid());
    }

    void Scene::morph_to_CV(const double& duration)
    {
        m_transitioner->morph_to_CV(
                duration,
                m_terrain->get_ellipsoid());
    }


	void Scene::set_terrain_opacity(const float& value)
	{
		if (value < 0.0 || value > 1.0)
		{
			throw std::invalid_argument("The opacity must be between 0.0 and 1.0.");
		}

		m_terrain_opacity = value;
	}

	float Scene::get_terrain_opacity()
	{
		return m_terrain_opacity;
	}

	unsigned int Scene::get_drawing_surface_height()
    {
        return m_context->get_drawing_surface_height();
    }

    unsigned int Scene::get_drawing_surface_width()
    {
        return m_context->get_drawing_surface_width();
    }


    void Scene::set_camera_preview(IVideoCapturePreview* value)
	{
		m_camera_preview = value;
	}

	void Scene::remove_primitive(IPrimitive* primitive)
	{
		if (m_primitives.find(primitive) == m_primitives.end())
		{
			throw std::invalid_argument(
				"The primitive does not exist in the collection");
		}

		m_primitives.erase(primitive);
		primitive->remove_pick_listener(this);
	}

	IPrimitive* Scene::add_primitive(IPrimitive* primitive)
	{
		if (m_primitives.find(primitive) != m_primitives.end())
		{
			throw std::invalid_argument(
				"The renderable already exist in the collection");
		}

		m_primitives.insert(primitive);
		primitive->add_pick_listener(this);

		return primitive;
	}

    bool Scene::get_use_vr() const
    {
        return m_use_vr;
    }

    void Scene::set_use_vr(const bool& value)
    {
        if (m_use_vr && dynamic_cast<OrthographicFrustum*>(m_camera->get_frustum()) != nullptr)
        {
            throw std::logic_error("VR is not supported with an orthographic projection.");
        }

        if (value != m_use_vr)
        {
            if (value)
            {
                m_saved_aspect_ratio = m_camera->get_frustum()->get_aspect_ratio();
            }
            else
            {
                PerspectiveFrustum* frustum = dynamic_cast<PerspectiveFrustum*>(m_camera->get_frustum());
                frustum->set_aspect_ratio(m_saved_aspect_ratio);
                frustum->set_x_offset(0.0);
            }

            m_use_vr = value;
        }
    }

	bool Scene::add_post_render_listener(PostRenderListener* listener)
	{
		return m_post_render_listeners.insert(listener).second;
	}

	bool Scene::add_pre_render_listener(PreRenderListener* listener)
	{
		return m_pre_render_listeners.insert(listener).second;
	}

	bool Scene::remove_post_render_listener(PostRenderListener* listener)
	{
		return m_post_render_listeners.erase(listener);
	}

	bool Scene::remove_pre_render_listener(PreRenderListener* listener)
	{
		return m_pre_render_listeners.erase(listener);
	}

	PassState* Scene::get_pass_state()
	{
		return &m_pass_state;
	}


	Camera* Scene::get_camera()
	{
		return m_camera.get();
	}

	Context* Scene::get_context()
	{
		return m_context;
	}

	ITerrain* Scene::get_terrain()
	{
		return m_terrain.get();
	}

	RasterSource* Scene::get_raster_source()
	{
		return m_raster_source;
	}

	MapProjection* Scene::get_projection()
	{
		return m_projection.get();
	}

	void Scene::set_raster_source(RasterSource* value)
	{
		if (m_raster_source != value)
		{
			m_raster_source = value;
			on_raster_source_changed(value);
		}
	}

	bool Scene::add_raster_source_changed_listener(RasterSourceChangedListener* listener)
	{
		return m_raster_source_changed_listeners.insert(listener).second;
	}

	bool Scene::remove_raster_source_changed_listeners(RasterSourceChangedListener* listener)
	{
		return m_raster_source_changed_listeners.erase(listener);
	}

	void Scene::on_raster_source_changed(RasterSource* value)
	{
		for (RasterSourceChangedListener* listener : m_raster_source_changed_listeners)
		{
			listener->on_raster_source_changed(value);
		}
	}

	void Scene::update_environment_state()
	{
		m_environment_state.clear_terrain_depth =
                (m_terrain != nullptr) &&
                (!m_terrain->get_depth_test_against() || m_mode == SceneMode::Scene2D);

		m_environment_state.use_depth_plane =
                m_environment_state.clear_terrain_depth && m_mode == SceneMode::Scene3D;

		if (m_environment_state.use_depth_plane) 
		{
			// Update the depth plane that is rendered in 3D when the primitives are
			// not depth tested against terrain so primitives on the backface
			// of the globe are not picked.
			m_depth_plane->update(&m_frame_state);
		}
	}

	void Scene::update_frame_state(
		unsigned int frame_number,
		EngineClockTick time)
	{
		// Clear the commands queue
		m_frame_state.get_command_queue().clear();
		m_frame_state.get_shadow_maps().clear();

        m_frame_state.set_mode(m_mode);
        m_frame_state.set_morph_time(m_morph_time);
		m_frame_state.set_map_projection(m_projection.get());
		m_frame_state.set_time(time);
		m_frame_state.set_frame_number(frame_number);
		m_frame_state.set_camera(m_camera.get());

		m_culling_volume = m_camera->get_frustum()->get_culling_volume(
			m_camera->get_position_wc(),
			m_camera->get_direction_wc(),
			m_camera->get_up_wc());

		m_frame_state.set_culling_volume(m_culling_volume.get());
	}

	bool Scene::is_visible(
		DrawCommand* command,
		CullingVolume* culling_volume)
	{
		return !command->get_cull() ||
			command->get_bounding_volume() == nullptr ||
			culling_volume->visibility(command->get_bounding_volume()) != Intersect::Outside;
	}

	void Scene::insert_into_bin(
		DrawCommand* command,
		const Interval& distances)
	{
		if (m_frame_state.get_pass() != Pass::Pick)
		{
			update_derived_commands(command);
		}

		for (FrustumCommands& frustum_commands : m_frustum_commands_queue)
		{
			const double cur_near = frustum_commands.near;
			const double cur_far  = frustum_commands.far;

			if (distances.start > cur_far)
			{
				continue;
			}

			if (distances.stop < cur_near)
			{
				break;
			}

			frustum_commands.commands[command->get_pass()].push_back(command);

			if (command->get_execute_on_closest_frustum())
			{
				break;
			}
		}
	}


	void Scene::create_potentially_visible_set()
	{
		m_overlay_commands_queue.clear();
		m_camera_preview_commands_queue.clear();

		for (FrustumCommands& frustum_commands : m_frustum_commands_queue)
		{
			for (unsigned int p = 0; p < static_cast<unsigned int>(CommandPass::NumberOfPasses); ++p)
			{
				frustum_commands.commands[static_cast<CommandPass>(p)].clear();
			}
		}

		double near =  std::numeric_limits<double>::max();
		double far  = -std::numeric_limits<double>::max();

		const bool shadows_enabled = m_frame_state.get_shadow_hints().shadows_enabled;
		double shadow_closest_object_size = m_frame_state.get_shadow_hints().closest_object_size;
		double shadow_near =  std::numeric_limits<double>::max();
		double shadow_far  = -std::numeric_limits<double>::max();

		Interval distances;
        bool undefined_bounding_volume = false;

		for (DrawCommand* command : m_frame_state.get_command_queue())
		{
			CommandPass pass = command->get_pass();

			if (pass == CommandPass::OverlayCommand)
			{
				m_overlay_commands_queue.push_back(command);
			}
			else if (pass == CommandPass::CameraPreview)
			{
				m_camera_preview_commands_queue.push_back(command);
			}
			else
			{
				BoundingVolume* bounding_volume = command->get_bounding_volume();

				if (bounding_volume != nullptr)
				{
					if (!is_visible(command, m_culling_volume.get()))
					{
						continue;
					}

					distances = bounding_volume->compute_plane_distances(
						m_camera->get_position_wc(),
						m_camera->get_direction_wc());

					// Compute a tight near and far plane for commands that receive shadows. This helps compute
					// good splits for cascaded shadow maps. Ignore commands that exceed the maximum distance.
					// When moving the camera low LOD globe tiles begin to load, whose bounding volumes
					// throw off the near/far fitting for the shadow map. Only update for globe tiles that the
					// camera isn't inside.
					// TODO: ShadowMap::MaximumDistance
					near = ::fmin(near, distances.start);
					far  = ::fmax(far, distances.stop);
					if (shadows_enabled && command->get_receive_shadows() && (distances.start < 20000.0) &&
						!((pass == CommandPass::Globe) && (distances.start < -100.0) && (distances.stop > 100.0)))
					{
						// Get the smallest bounding volume the camera is near. This is used to place more shadow detail near the object.
						double size = distances.stop - distances.start;
						if ((pass != CommandPass::Globe) && (distances.start < 100.0))
						{
							shadow_closest_object_size = ::fmin(shadow_closest_object_size, size);
						}

						shadow_near = ::fmin(shadow_near, distances.start);
						shadow_far = ::fmax(shadow_far, distances.stop);
					}
				}
				else
				{
					// Clear commands don't need a bounding volume - just add the clear to all frustums.
					// If another command has no bounding volume, though, we need to use the camera's
					// worst-case near and far planes to avoid clipping something important.
					distances.start = m_camera->get_frustum()->get_near();
					distances.stop  = m_camera->get_frustum()->get_far();
                    undefined_bounding_volume = true;
				}

				insert_into_bin(command, distances);
			}
		}

		if (undefined_bounding_volume)
		{
			near = m_camera->get_frustum()->get_near();
			far  = m_camera->get_frustum()->get_far();

		}
		else
		{
			// The computed near plane must be between the user defined near and far planes.
			// The computed far plane must between the user defined far and computed near.
			// This will handle the case where the computed near plane is further than the user defined far plane.
			near = ::fmin(::fmax(near, m_camera->get_frustum()->get_near()), m_camera->get_frustum()->get_far());
			far  = ::fmax(::fmin(far, m_camera->get_frustum()->get_far()), near);

			if (shadows_enabled)
			{
				shadow_near = ::fmin(::fmax(shadow_near, m_camera->get_frustum()->get_near()), m_camera->get_frustum()->get_far());
				shadow_far = ::fmax(::fmin(shadow_far, m_camera->get_frustum()->get_far()), shadow_near);
			}
		}

		// Use the computed near and far for shadows
		if (shadows_enabled)
		{
			ShadowHints& shadow_hints = m_frame_state.get_shadow_hints();
			shadow_hints.near_plane = shadow_near;
			shadow_hints.far_plane = shadow_far;
			shadow_hints.closest_object_size = shadow_closest_object_size;
		}

		// Exploit temporal coherence. If the frustums haven't changed much, use the frustums computed
        // last frame, else compute the new frustums and sort them by frustum again.

        const bool is_2D = m_mode == SceneMode::Scene2D;


		// last frame, else compute the new frustums and sort them by frustum again.
		// The multifrustum for 3D is non-uniformly distributed.
		unsigned int num_frustums = 0;

        if(!is_2D)
        {
            num_frustums = ::ceil(::log(far / near) / ::log(m_far_to_near_ratio));
        }
        else
        {
            // The multifrustum for 2D is uniformly distributed. To avoid z-fighting in 2D,
            // the camera is moved to just before the frustum and the frustum depth is scaled
            // to be in [1.0, nearToFarDistance2D].
            far  = ::min(far, m_camera->get_position().z + m_near_to_far_distance_2D);
            near = ::min(near, far);

            num_frustums = ::ceil(::max(1.0, far - near) / m_near_to_far_distance_2D);
        }


		unsigned int current_num_frustums = m_frustum_commands_queue.size();

		if ((near != std::numeric_limits<double>::max()) && 
			(num_frustums != current_num_frustums ||
			(current_num_frustums != 0 && (near < m_frustum_commands_queue[0].near || far > m_frustum_commands_queue[current_num_frustums - 1].far))))
		{
			update_frustums(
				near,
				far,
				num_frustums,
                is_2D);

			create_potentially_visible_set();
		}
	}

	void Scene::update_frustums(
		const double& near,
		const double& far,
        const unsigned int& num_frustums,
        const bool& is_2D)
	{
		m_frustum_commands_queue.resize(num_frustums);
		for (unsigned int m = 0; m < num_frustums; m++)
		{
			FrustumCommands& frustum_commands = m_frustum_commands_queue[m];

			double cur_near;
			double cur_far;

            if (!is_2D)
            {
                cur_near = ::fmax(near, ::pow(m_far_to_near_ratio, m) * near);
                cur_far  = ::fmin(far, m_far_to_near_ratio * cur_near);
            }
            else
            {
                cur_near = ::fmin(far - m_near_to_far_distance_2D, near + m * m_near_to_far_distance_2D);
                cur_far  = ::fmin(far, cur_near + m_near_to_far_distance_2D);
            }

			frustum_commands.near = cur_near;
			frustum_commands.far = cur_far;
		}
	}

    void Scene::execute_translucent_commands_sorted(
            PassState* pass_state,
            std::vector<DrawCommand*>& commands)
    {
        std::sort(
                commands.begin(),
                commands.end(),
                [&](DrawCommand* a, DrawCommand* b)
                {
                    const double a_dist = static_cast<BoundingSphere*>(a->get_bounding_volume())->distance_squared_to(m_camera->get_position_wc());
                    const double b_dist = static_cast<BoundingSphere*>(b->get_bounding_volume())->distance_squared_to(m_camera->get_position_wc());
                    return b_dist < a_dist;
                });

        for (DrawCommand* command : commands)
        {
            execute_command(command, pass_state);
        }
    }

	void Scene::execute_overlay_commands(PassState* pass_state)
	{
		for (DrawCommand* command : m_overlay_commands_queue)
		{
			execute_command(
				command,
				pass_state);
		}
	}

	void Scene::execute_reality_commands(PassState* pass_state)
	{
		FrameBuffer* frame_buffer = pass_state->get_framebuffer();
		pass_state->set_framebuffer(m_ar_buffer->get_reality_frame_buffer());

		// Camera preview pass
		for (DrawCommand* command : m_camera_preview_commands_queue)
		{
			execute_command(
				command,
				pass_state);
		}

		pass_state->set_framebuffer(frame_buffer);
	}

	void Scene::execute_mix_reality_and_overlay_commands(PassState* pass_state)
	{
        // TODO: Terrain merge
		// Render terrain texture to virtual texture.
		pass_state->set_framebuffer(m_ar_buffer->get_virtuality_frame_buffer());

		m_render_to_viewport_command->set_render_state(m_viewport_render_state);
		m_render_to_viewport_command->set_uniform_map(&m_terrain_to_ar_buffer_uniform_map);
		m_render_to_viewport_command->execute(m_context, pass_state);

		// Execute the ovelay commands in the virtuality buffer
		execute_overlay_commands(pass_state);

		// Mix reality and virtuality to get the mixed reality.
		m_ar_buffer->execute_mix_reality(
			m_context,
			pass_state);
	}

	void Scene::execute_commands(
		PassState* pass_state,
		const bool& picking)
	{
		UniformState* uniform_state = m_context->get_uniform_state();

		uniform_state->update_camera(m_camera.get());

        const double height_2D = m_camera->get_position().z;

		// Create a working frustum from the original camera frustum.
		std::unique_ptr<Frustum> frustum = std::unique_ptr<Frustum>(
			m_camera->get_frustum()->clone());

        /*
		if (!picking)
		{
			FrameBuffer* frame_buffer = nullptr;
			frame_buffer =  m_ar_buffer->get_reality_frame_buffer();
			pass_state->set_framebuffer(frame_buffer);
			// Ideally, we would render the sky box and atmosphere last for
			// early-z, but we would have to draw it in each frustum
			frustum->set_near(m_camera->get_frustum()->get_near());
			frustum->set_far(m_camera->get_frustum()->get_far());
			// Ideally, we would render the sky box and atmosphere last for
			// early-z, but we would have to draw it in each frustum
			uniform_state->update_frustum(m_camera->get_frustum());
			if (m_environment_state.use_terrain_depth_frame_buffer)
			{
				frame_buffer = m_terrain_depth->get_frame_buffer();
			}
			else
			{
				frame_buffer = m_environment_state.original_frame_buffer;
			}
			pass_state->set_framebuffer(frame_buffer);
		}
        */

		pass_state->set_framebuffer(m_ar_buffer->get_virtuality_frame_buffer());
		unsigned int num_frustums = m_frustum_commands_queue.size();


        // Globe passes
        FrameBuffer* frame_buffer = nullptr;
        if (m_terrain_depth != nullptr && m_environment_state.use_terrain_depth_frame_buffer)
        {
            frame_buffer = pass_state->get_framebuffer();
            pass_state->set_framebuffer(m_terrain_depth->get_frame_buffer());
        }

        // Execute commands in each frustum in back to front order
		for (unsigned int i = 0; i < num_frustums; i++)
        {
			unsigned int index = num_frustums - i - 1;
			FrustumCommands &frustum_commands = m_frustum_commands_queue[index];

			if (m_mode == SceneMode::Scene2D)
            {
				// To avoid z-fighting in 2D, move the camera to just before the frustum
				// and scale the frustum depth to be in [1.0, nearToFarDistance2D].
				m_camera->set_position(vec3d(
						m_camera->get_position().x,
						m_camera->get_position().y,
						height_2D - frustum_commands.near + 1.0));

				frustum->set_near(1.0);
				frustum->set_far(::max(1.0, frustum_commands.far - frustum_commands.near));

				uniform_state->update(&m_frame_state);
				uniform_state->update_frustum(frustum.get());
			}
            else
            {
				// Avoid tearing artifacts between adjacent frustums in the passes
				frustum->set_near(index != 0 ? frustum_commands.near * OPAQUE_FRUSTUM_NEAR_OFFSET
											 : frustum_commands.near);
				frustum->set_far(frustum_commands.far);
				uniform_state->update_frustum(frustum.get());
			}

            m_clear_depth.execute(m_context, pass_state);
            for (DrawCommand* command : frustum_commands.commands[CommandPass::Globe])
            {
                execute_command(
                        command,
                        pass_state);
            }

			if (m_copy_terrain_depth && m_environment_state.use_terrain_depth_frame_buffer && m_copy_terrain_depth)
			{
				m_terrain_depth->update(m_context, m_render_target_viewport);
				m_terrain_depth->execute_copy_depth(m_context, pass_state);
			}

            /*
            if (m_environment_state.clear_terrain_depth)
            {
                m_clear_depth.execute(m_context, pass_state);
                if (m_environment_state.use_depth_plane)
                {
                    m_depth_plane->execute(m_context, pass_state);
                }
            }
            */

            bool depth_only = false;
            if ((m_terrain_depth != nullptr) &&
                (m_environment_state.use_terrain_depth_frame_buffer || depth_only) &&
                (m_use_depth_picking))
            {
                // PERFORMANCE_IDEA: Use MRT to avoid the extra copy.
                PickDepth* pick_depth = get_pick_depth(index);
                pick_depth->update(
                        m_context,
                        //static_cast<Texture2D*>(pass_state->get_framebuffer()->get_depth_attachment_texture()));
                        m_terrain_depth->get_depth_stencil_texture());
                pick_depth->execute(m_context, pass_state);
            }
		}


		// Execute commands in each frustum in back to front order
        if (m_terrain_depth != nullptr && m_environment_state.use_terrain_depth_frame_buffer)
        {
            pass_state->set_framebuffer(frame_buffer);
        }
		for (unsigned int i = 0; i < num_frustums; i++)
		{
			unsigned int index = num_frustums - i - 1;
			FrustumCommands& frustum_commands = m_frustum_commands_queue[index];

            if (m_mode == SceneMode::Scene2D)
            {
                // To avoid z-fighting in 2D, move the camera to just before the frustum
                // and scale the frustum depth to be in [1.0, nearToFarDistance2D].
                m_camera->set_position(vec3d(
                        m_camera->get_position().x,
                        m_camera->get_position().y,
                        height_2D - frustum_commands.near + 1.0));

                frustum->set_near(1.0);
                frustum->set_far(::max(1.0, frustum_commands.far - frustum_commands.near));

                uniform_state->update(&m_frame_state);
                uniform_state->update_frustum(frustum.get());
            }
            else
            {
                // Avoid tearing artifacts between adjacent frustums in the passes
                frustum->set_near(index != 0 ? frustum_commands.near * OPAQUE_FRUSTUM_NEAR_OFFSET : frustum_commands.near);
                frustum->set_far(frustum_commands.far);
                uniform_state->update_frustum(frustum.get());
            }

            m_clear_depth.execute(m_context, pass_state);

			// Opaque passes
			for (DrawCommand* command : frustum_commands.commands[CommandPass::Opaque])
			{
				execute_command(
					command,
					pass_state);
			}

            if (index != 0 && m_mode != SceneMode::Scene2D)
            {
                // Do not overlap frustums in the translucent pass to avoid blending artifacts
                frustum->set_near(frustum_commands.near);
                uniform_state->update_frustum(frustum.get());
            }

            execute_translucent_commands_sorted(
                    pass_state,
                    frustum_commands.commands[CommandPass::Translucent]);
		}
	}

	void Scene::execute_command(
		DrawCommand* command,
		PassState* pass_state)
	{
		const bool shadows_enabled = m_frame_state.get_shadow_hints().shadows_enabled;
		const bool light_shadows_enabled = shadows_enabled && (m_frame_state.get_shadow_hints().light_shadow_maps.size() > 0);

		if (light_shadows_enabled &&
			command->get_receive_shadows() &&
			command->get_derived_commands()->shadows.receive_command != nullptr)
		{
			// If the command receives shadows, execute the derived shadows command.
			// Some commands, such as OIT derived commands, do not have derived shadow commands themselves
			// and instead shadowing is built-in. In this case execute the command regularly below.
			command->get_derived_commands()->shadows.receive_command->execute(
				m_context,
				pass_state);
		}
		else
		{
			command->execute(
				m_context,
				pass_state);
		}
	}

	void Scene::on_pre_render(EngineClockTick time)
	{
		for (PreRenderListener* listener : m_pre_render_listeners)
		{
			listener->pre_render(this, time);
		}
	}

	void Scene::on_post_render(EngineClockTick time)
	{
		for (PostRenderListener* listener : m_post_render_listeners)
		{
			listener->post_render(this, time);
		}
	}

	void Scene::update_derived_commands(DrawCommand* draw_command)
	{
		// Update derived commands when any shadow maps become dirty
		bool shadows_dirty = false;
		std::time_t last_time_dirty = m_frame_state.get_shadow_hints().last_time_dirty;
		if (draw_command->get_last_time_dirty() != last_time_dirty)
		{
			draw_command->set_last_time_dirty(last_time_dirty);
			draw_command->set_dirty(true);
			shadows_dirty = true;
		}

		if (draw_command->get_dirty())
		{
			draw_command->set_dirty(false);
			if (m_frame_state.get_shadow_hints().shadows_enabled && (draw_command->get_cast_shadows() || draw_command->get_receive_shadows()))
			{
				ShadowMap::create_derived_commands(
					m_frame_state.get_shadow_hints().shadow_maps,
					m_frame_state.get_shadow_hints().light_shadow_maps,
					draw_command,
					shadows_dirty,
					m_context,
					draw_command->get_derived_commands()->shadows);
			}
		}
	}

	void Scene::update_primitives(PassState* pass_state)
	{
		if (m_camera_preview != nullptr)
		{
			m_camera_preview->update(&m_frame_state);
		}

		m_terrain->update(&m_frame_state);

		for (auto primitive : m_primitives)
		{
			primitive->update(&m_frame_state);
		}
	}

	void Scene::update_and_clear_framebuffers(
		PassState* pass_state,
		Color clear_color,
		const bool& picking,
        const bool& reality_commands)
	{
		// Preserve the reference to the original framebuffer.
		m_environment_state.original_frame_buffer = pass_state->get_framebuffer();

		// Clear the pass state framebuffer
		m_clear_color.color = clear_color;
		m_clear_color.execute(m_context, pass_state);
		
		// Update globe depth rendering based on the current context and clear the globe depth framebuffer.
		m_environment_state.use_terrain_depth_frame_buffer = !picking && m_terrain_depth != nullptr;
		if (m_environment_state.use_terrain_depth_frame_buffer)
		{
			m_terrain_depth->update(m_context, m_render_target_viewport);
			m_terrain_depth->clear(m_context, pass_state, clear_color);
		}

		m_ar_buffer->clear(
                m_context,
                pass_state,
                clear_color,
                reality_commands);

		if (m_environment_state.use_terrain_depth_frame_buffer)
		{
			pass_state->set_framebuffer(m_terrain_depth->get_frame_buffer());
		}

		if (pass_state->get_framebuffer() != nullptr)
		{
			m_clear_color.execute(m_context, pass_state);
		}
	}

	void Scene::update_and_execute_commands(
		PassState* pass_state,
		Color clear_color,
		const bool& picking,
        const bool& reality_commands,
        const bool& render_to_display)
	{
        if (m_frame_state.get_pass() == Pass::Render)
        {
            if (m_camera_preview != nullptr)
            {
                PerspectiveFrustum* frustum = dynamic_cast<PerspectiveFrustum*>(
                        m_camera->get_frustum());

                if (frustum != nullptr)
                {
                    //frustum->set_near(46.0 * 0.001);
                    frustum->set_field_of_view_y(
                            m_camera_preview->get_current_vertical_view_angle());
                    frustum->set_aspect_ratio(
                            m_camera_preview->get_current_aspect_ratio());
                }
            }
        }


        if (m_use_vr && m_mode != SceneMode::Scene2D)
        {
            PerspectiveFrustum* camera_frustum = static_cast<PerspectiveFrustum*>(
                    m_camera->get_frustum());

            update_and_clear_framebuffers(
                    pass_state,
                    clear_color,
                    picking,
                    reality_commands);

            update_primitives();
            create_potentially_visible_set();
            execute_shadow_map_cast_commands();

            // Based on Calculating Stereo pairs by Paul Bourke
            // http://paulbourke.net/stereographics/stereorender/
            m_display_viewport.set_left(0);
            m_display_viewport.set_bottom(0);
            m_display_viewport.set_width(m_display_viewport.get_width() * 0.5);
            m_display_viewport.set_height(m_display_viewport.get_height());

            Camera* saved_camera = m_camera->clone();

            const double near = camera_frustum->get_near();
            const double fo = near * 5.0;
            const double eye_separation = fo / 30.0;
            const vec3d eye_translation = saved_camera->get_right() * eye_separation * 0.5;

            camera_frustum->set_aspect_ratio(
                    (double) m_display_viewport.get_width() /
                    (double) m_display_viewport.get_height());

            const double offset = 0.5 * eye_separation * near / fo;

            // Only one reality snapshot for the two viewport.
            if (reality_commands)
            {
                execute_reality_commands(pass_state);
            }

            // First eye
            m_camera->set_position(saved_camera->get_position() + eye_translation);
            camera_frustum->set_x_offset(offset);
            execute_commands(pass_state);
            execute_mix_reality_and_overlay_commands(pass_state);

            // Render the mixed reality texture to display framebuffer.
            if (render_to_display)
            {
                pass_state->set_viewport(&m_display_viewport);
                pass_state->set_framebuffer(m_environment_state.original_frame_buffer);
                m_render_to_viewport_command->set_render_state(m_display_viewport_render_state);
                m_render_to_viewport_command->set_uniform_map(
                        &m_mixed_reality_to_display_uniform_map);
                m_render_to_viewport_command->execute(m_context, pass_state);
            }

            // Second eye
            pass_state->set_framebuffer(nullptr);
            pass_state->set_viewport(&m_render_target_viewport);
            m_camera->set_position(saved_camera->get_position() - eye_translation);
            camera_frustum->set_x_offset(-offset);
            execute_commands(pass_state);
            execute_mix_reality_and_overlay_commands(pass_state);

            // Render the mixed reality texture to display framebuffer.
            if (render_to_display)
            {
                m_display_viewport.set_left(m_display_viewport.get_width());
                pass_state->set_viewport(&m_display_viewport);
                pass_state->set_framebuffer(m_environment_state.original_frame_buffer);
                m_render_to_viewport_command->set_render_state(m_display_viewport_render_state);
                m_render_to_viewport_command->set_uniform_map(&m_mixed_reality_to_display_uniform_map);
                m_render_to_viewport_command->execute(m_context, pass_state);
            }

            saved_camera->clone(m_camera.get());
            delete saved_camera;
        }
        else
        {
            update_and_clear_framebuffers(
                    pass_state,
                    clear_color,
                    picking,
                    reality_commands);

            if (m_mode != SceneMode::Scene2D || m_map_mode == MapMode2D::Rotate)
            {
                // Only one reality snapshot for the two viewport.
                if (reality_commands)
                {
                    execute_reality_commands(pass_state);
                }

                execute_commands_in_viewport(
                        true,
                        pass_state,
                        picking);
            }
            else
            {
                execute_2D_commands_in_viewport(
                        true,
                        pass_state,
                        picking);
            }

            execute_mix_reality_and_overlay_commands(pass_state);

            // Render the mixed reality texture to display framebuffer.
            if (render_to_display)
            {
                m_pass_state.set_viewport(&m_display_viewport);
                m_pass_state.set_framebuffer(m_environment_state.original_frame_buffer);
                m_render_to_viewport_command->set_render_state(m_display_viewport_render_state);
                m_render_to_viewport_command->set_uniform_map(&m_mixed_reality_to_display_uniform_map);
                m_render_to_viewport_command->execute(m_context, &m_pass_state);
            }
        }
	}

	void Scene::execute_commands_in_viewport(
		const bool& is_first_viewport,
		PassState* pass_state,
		const bool& picking)
	{
		if (!is_first_viewport)
		{
			m_frame_state.get_command_queue().clear();
		}

		update_primitives();
		create_potentially_visible_set();

		if (is_first_viewport)
		{
			execute_shadow_map_cast_commands();
		}

		execute_commands(pass_state, picking);
	}

    void Scene::execute_2D_commands_in_viewport(
            const bool& is_first_viewport,
            PassState* pass_state,
            const bool& picking)
    {
		//TODO: CHECK THIS!!
        Rectangle* original_viewport = pass_state->get_viewport();
        Rectangle* cloned_viewport   = new Rectangle(*original_viewport);
        pass_state->set_viewport(cloned_viewport);

        OrthographicOffCenterFrustum* cloned_frustum =
                static_cast<OrthographicOffCenterFrustum*>(m_camera->get_frustum()->clone());

        OrthographicOffCenterFrustum* camera_frustum =
                static_cast<OrthographicOffCenterFrustum*>(m_camera->get_frustum());

        Geodetic3D max_cartographic(PI, PI_OVER_TWO);
        vec3d max_coord  = m_projection->project(max_cartographic).to_vec3d();
        vec3d position   = m_camera->get_position();
        mat4d transform  = m_camera->get_transform();

        m_camera->set_transform(mat4d::identity());

        const mat4d viewport_transformation = compute_viewport_transformation_matrix(
                *cloned_viewport,
                0.0,
                1.0);

        const mat4d projection_matrix = camera_frustum->get_projection();

        double x = m_camera->get_position_wc().y;
        vec3d eye_point(
                ArgeoMath::sign(x) * max_coord.x - x,
                0.0,
                -m_camera->get_position_wc().x);

        vec2d window_coordinates = Transforms::point_to_gl_window_coordinates(
                projection_matrix,
                viewport_transformation,
                eye_point);

        window_coordinates.x = std::floor(window_coordinates.x);

        const unsigned int viewport_x     = cloned_viewport->get_left();
        const unsigned int viewport_width = cloned_viewport->get_width();

        if (x == 0.0 ||
            window_coordinates.x <= viewport_x ||
            window_coordinates.x >= viewport_x + viewport_width)
        {
            execute_commands_in_viewport(
                    true,
                    pass_state);
        }
        else if (std::abs(viewport_x + viewport_width * 0.5 - window_coordinates.x) < 1.0)
        {
            cloned_viewport->set_width(window_coordinates.x - cloned_viewport->get_left());
            m_camera->set_position(vec3d(
                    std::abs(m_camera->get_position().x),
                    m_camera->get_position().y,
                    m_camera->get_position().z));
            camera_frustum->set_right(0.0);

            m_culling_volume = camera_frustum->get_culling_volume(
                    m_camera->get_position_wc(),
                    m_camera->get_direction_wc(),
                    m_camera->get_up_wc());
            m_frame_state.set_culling_volume(m_culling_volume.get());
            m_context->get_uniform_state()->update(&m_frame_state);
            execute_commands_in_viewport(
                    true,
                    pass_state);


            cloned_viewport->set_left(static_cast<unsigned int>(window_coordinates.x));
            m_camera->set_position(vec3d(
                    -m_camera->get_position().x,
                     m_camera->get_position().y,
                     m_camera->get_position().z));

            camera_frustum->set_right(-camera_frustum->get_left());
            camera_frustum->set_left(0.0);

            m_culling_volume = camera_frustum->get_culling_volume(
                    m_camera->get_position_wc(),
                    m_camera->get_direction_wc(),
                    m_camera->get_up_wc());
            m_frame_state.set_culling_volume(m_culling_volume.get());
            m_context->get_uniform_state()->update(&m_frame_state);

            execute_commands_in_viewport(
                    false,
                    pass_state);
        }
        else if (window_coordinates.x > viewport_x + viewport_width * 0.5)
        {
            cloned_viewport->set_width(window_coordinates.x - viewport_x);
            const double right = camera_frustum->get_right();
            camera_frustum->set_right(max_coord.x - x);

            m_culling_volume = camera_frustum->get_culling_volume(
                    m_camera->get_position_wc(),
                    m_camera->get_direction_wc(),
                    m_camera->get_up_wc());
            m_frame_state.set_culling_volume(m_culling_volume.get());
            m_context->get_uniform_state()->update(&m_frame_state);
            execute_commands_in_viewport(
                    true,
                    pass_state);

            cloned_viewport->set_left(window_coordinates.x);
            cloned_viewport->set_width(viewport_x + viewport_width - window_coordinates.x);
            m_camera->set_position(vec3d(
                    -m_camera->get_position().x,
                     m_camera->get_position().y,
                     m_camera->get_position().z));
            camera_frustum->set_left(-camera_frustum->get_right());
            camera_frustum->set_right(right - camera_frustum->get_right() * 2.0);

            m_culling_volume = camera_frustum->get_culling_volume(
                    m_camera->get_position_wc(),
                    m_camera->get_direction_wc(),
                    m_camera->get_up_wc());
            m_frame_state.set_culling_volume(m_culling_volume.get());
            m_context->get_uniform_state()->update(&m_frame_state);

            execute_commands_in_viewport(
                    false,
                    pass_state);
        }
        else
        {
            cloned_viewport->set_left(window_coordinates.x);
            cloned_viewport->set_width(viewport_x + viewport_width - window_coordinates.x);
            const double left = camera_frustum->get_left();
            camera_frustum->set_left(-max_coord.x - x);

            m_culling_volume = camera_frustum->get_culling_volume(
                    m_camera->get_position_wc(),
                    m_camera->get_direction_wc(),
                    m_camera->get_up_wc());
            m_frame_state.set_culling_volume(m_culling_volume.get());
            m_context->get_uniform_state()->update(&m_frame_state);

            execute_commands_in_viewport(
                    true,
                    pass_state);

            cloned_viewport->set_left(viewport_x);
            cloned_viewport->set_width(window_coordinates.x - viewport_x);

            m_camera->set_position(vec3d(
                    -m_camera->get_position().x,
                     m_camera->get_position().y,
                     m_camera->get_position().z));

            camera_frustum->set_right(-camera_frustum->get_left());
            camera_frustum->set_left(left - camera_frustum->get_left() * 2.0);

            m_culling_volume = camera_frustum->get_culling_volume(
                    m_camera->get_position_wc(),
                    m_camera->get_direction_wc(),
                    m_camera->get_up_wc());
            m_frame_state.set_culling_volume(m_culling_volume.get());
            m_context->get_uniform_state()->update(&m_frame_state);

            execute_commands_in_viewport(
                    false,
                    pass_state);
        }

        m_camera->set_transform(transform);
        m_camera->set_position(position);
        m_camera->set_frustum(cloned_frustum);

        pass_state->set_viewport(original_viewport);

        delete camera_frustum;
        delete cloned_viewport;
    }

	void Scene::execute_shadow_map_cast_commands()
	{
		if (!m_frame_state.get_shadow_hints().shadows_enabled)
		{
			return;
		}

		UniformState* uniform_state = m_context->get_uniform_state();

		for (ShadowMap* shadow_map : m_frame_state.get_shadow_maps())
		{
			if (shadow_map->get_is_out_of_view())
			{
				continue;
			}

			// Reset the command lists
			for (ShadowPass& shadow_pass : shadow_map->get_passes())
			{
				shadow_pass.command_queue.clear();
			}

			// Insert the primitive/model commands into the command lists
			insert_shadow_cast_commands(
				m_frame_state.get_command_queue(),
				shadow_map);

			std::vector<ShadowPass>& shadow_passes = shadow_map->get_passes();
			for (unsigned int i = 0; i < shadow_passes.size(); i++)
			{
				ShadowPass& shadow_pass = shadow_map->get_passes()[i];

				uniform_state->update_camera(shadow_pass.camera);

				shadow_map->update_pass(
					m_context,
					i);

				for (DrawCommand* command : shadow_pass.command_queue)
				{
					// Set the correct pass before rendering into the shadow map because some shaders
					// conditionally render based on whether the pass is translucent or opaque.
					//uniform_state.updatePass(command.pass);
					execute_command(
						command->get_derived_commands()->shadows.cast_commands[i],
						&shadow_pass.pass_state);
				}
			}
		}
	}

	void Scene::insert_shadow_cast_commands(
		CommandQueue commands,
		ShadowMap* shadow_map)
	{
		std::vector<ShadowPass>& passes = shadow_map->get_passes();
		unsigned int number_of_passes = passes.size();

		for (DrawCommand* command : commands)
		{
			update_derived_commands(command);
			if (command->get_cast_shadows() && (command->get_pass() == CommandPass::Globe || command->get_pass() == CommandPass::Opaque || command->get_pass() == CommandPass::Translucent))
			{
				if (is_visible(command, shadow_map->get_culling_volume()))
				{
					if (shadow_map->get_is_point_light())
					{
						for (unsigned int k = 0; k < number_of_passes; ++k)
						{
							passes[k].command_queue.push_back(command);
						}
					}
					else if (number_of_passes == 1)
					{
						passes[0].command_queue.push_back(command);
					}
					else
					{
						bool was_visible = false;
						// Loop over cascades from largest to smallest
						for (int j = number_of_passes - 1; j >= 0; --j)
						{
							CullingVolume* cascade_volume = passes[j].culling_volume.get();

							if (is_visible(command, cascade_volume))
							{
								passes[j].command_queue.push_back(command);
								was_visible = true;
							}
							else if (was_visible)
							{
								// If it was visible in the previous cascade but now isn't
								// then there is no need to check any more cascades
								break;
							}
						}
					}
				}
			}
		}
	}

	void Scene::update_viewports(
            Rectangle& render_target_viewport)
	{
		Rectangle display_viewport(
			0,
			0,
			m_context->get_drawing_surface_width(),
			m_context->get_drawing_surface_height());

		if (render_target_viewport.get_width() == 0 ||
            render_target_viewport.get_height() == 0)
		{
            render_target_viewport = display_viewport;
		}

		if (m_display_viewport != display_viewport)
		{
			m_display_viewport = display_viewport;

			RenderStateParameters params;
			params.viewport  = display_viewport;
			m_display_viewport_render_state = RenderState::from_cache(params);
		}

		if (m_render_target_viewport != render_target_viewport)
		{
			m_render_target_viewport = render_target_viewport;
			RenderStateParameters params;
			params.viewport = render_target_viewport;
			m_viewport_render_state = RenderState::from_cache(params);
		}
	}

	bool Scene::camera_equals(
		Camera* camera0,
		Camera* camera1,
		const double& epsilon)
	{
		double max = ::fmax(
			vec_max_component(camera0->get_position()),
			vec_max_component(camera1->get_position()));

		double scalar = 1.0 / ::fmax(1, max);

		vec3d position0 = camera0->get_position() * scalar;
		vec3d position1 = camera1->get_position() * scalar;

		return
			vec_equals_epsilon(position0, position1, epsilon) &&
			vec_equals_epsilon(camera0->get_direction(), camera1->get_direction(), epsilon) &&
			vec_equals_epsilon(camera0->get_up(), camera1->get_up(), epsilon) &&
			vec_equals_epsilon(camera0->get_right(), camera1->get_right(), epsilon) &&
			mat4d_equals_epsilon(camera0->get_transform(), camera1->get_transform(), epsilon);
	}

	void Scene::render(EngineClockTick time)
	{
        std::time_t current_time;
        std::time(&current_time);

        m_pick_position_cache_dirty = true;

		// Trigger camera move events.
		if (!camera_equals(
			m_camera.get(),
			m_last_camera.get(),
			EPSILON6))
		{
			if (!m_camera_start_fired)
			{
				m_camera->move_start();
				m_camera_start_fired = true;
			}

			std::time(&m_camera_moved_time);
			m_camera->clone(m_last_camera.get());
		}
		else
		{
			if (m_camera_start_fired && current_time - m_camera_moved_time > m_camera_event_wait_time)
			{
				m_camera->move_end();
				m_camera_start_fired = false;
			}
		}

		// Destroy released shaders once every 120 frames to avoid thrashing the cache.
		if (m_shader_frame_count++ == 120)
		{
			m_shader_frame_count = 0;
			m_context->get_shader_cache()->destroy_released_shader_programs();
		}

        m_tween_collection->update(current_time);

        m_camera->update(m_mode);
		m_camera->update_camera_changed();

		// Begin frame render
		m_context->begin_frame();

		// Pre render event.
		on_pre_render(time);

		// Update the current frame state.
		unsigned int frame_number = ArgeoMath::increment_wrap(
			m_frame_state.get_frame_number(),
			15000000,
			1);

		update_frame_state(
			frame_number,
			time);
		m_frame_state.set_pass(Pass::Render);

		// Update the uniform state based on the current frame state
		UniformState* uniform_state = m_context->get_uniform_state();
		uniform_state->update(&m_frame_state);

        Rectangle render_target_viewport = m_camera_preview != nullptr ?
                       m_camera_preview->get_current_preview_size() :
                       Rectangle(
                               0,
                               0,
                               m_context->get_drawing_surface_width(),
                               m_context->get_drawing_surface_height());

        update_viewports(render_target_viewport);

        m_ar_buffer->update(
                m_context,
                m_render_target_viewport);

		// Reset the pass state
		m_pass_state.set_framebuffer(nullptr);
		m_pass_state.set_blending(nullptr);
		m_pass_state.set_scissor_test(nullptr);		
		m_pass_state.set_viewport(&m_render_target_viewport);

		update_environment_state();
		update_and_execute_commands(&m_pass_state);

		// Post render event.
		on_post_render(time);

		// End the frame
		m_context->end_frame();
	}

	std::unique_ptr<CullingVolume> Scene::get_pick_perspective_culling_volume(
		const vec2d& surface_position,
		int width,
		int height) const
	{
        PerspectiveOffCenterFrustum* frustum;

        if (dynamic_cast<PerspectiveFrustum*>(m_camera->get_frustum()) != nullptr)
        {
            frustum = static_cast<PerspectiveFrustum*>(m_camera->get_frustum())->get_offcenter_frustum();
        }
        else
        {
            frustum = static_cast<PerspectiveOffCenterFrustum*>(m_camera->get_frustum());
        }

		double near = frustum->get_near();
		double tan_phi = tan(frustum->get_field_of_view_y() * 0.5);
		double tan_theta = frustum->get_aspect_ratio() * tan_phi;

		Rectangle* viewport = m_pass_state.get_viewport();
		int x = 2.0 * (surface_position.x - viewport->get_left()) / viewport->get_width() - 1.0;
		int y = 2.0 * (viewport->get_height() - surface_position.y - viewport->get_bottom()) / viewport->get_height() - 1.0;

		double x_dir = x * near * tan_theta;
		double y_dir = y * near * tan_phi;

		vec2d pixel_size = frustum->get_pixel_dimension(viewport->get_width(), viewport->get_height(), 1.0);
		double pick_width = pixel_size.x * width  * 0.5;
		double pick_height = pixel_size.y * height * 0.5;

		PerspectiveOffCenterFrustum off_center_frustum(
			x_dir - pick_width,
			x_dir + pick_width,
			y_dir - pick_height,
			y_dir + pick_height,
			near,
			frustum->get_far());

		return off_center_frustum.get_culling_volume(
			m_camera->get_position_wc(),
			m_camera->get_direction_wc(),
			m_camera->get_up_wc());
	}

    std::unique_ptr<CullingVolume> Scene::get_pick_orthographic_culling_volume(
            const vec2d& surface_position,
            int width,
            int height) const
    {
        OrthographicOffCenterFrustum* frustum;

        if (dynamic_cast<OrthographicFrustum*>(m_camera->get_frustum()) != nullptr)
        {
            frustum = static_cast<OrthographicFrustum*>(m_camera->get_frustum())->get_offcenter_frustum();
        }
        else
        {
            frustum = static_cast<OrthographicOffCenterFrustum*>(m_camera->get_frustum());
        }


        Rectangle viewport = *m_pass_state.get_viewport();

        double x = 2.0 * (surface_position.x - viewport.get_left()) / viewport.get_width() - 1.0;
        x *= (frustum->get_right() - frustum->get_left()) * 0.5;

        double y = 2.0 * (viewport.get_height() - surface_position.y - viewport.get_bottom()) / viewport.get_height() - 1.0;
        y *= (frustum->get_top() - frustum->get_bottom()) * 0.5;

        mat4d transform = m_camera->get_transform();
        m_camera->set_transform(mat4d::identity());

        vec3d origin  = m_camera->get_position();
        origin += m_camera->get_right() * x + m_camera->get_up() * y;

        m_camera->set_transform(transform);

        if (m_mode == SceneMode::Scene2D)
        {
             origin = vec3d(
                     origin.z,
                     origin.x,
                     origin.y);
        }

        const vec2d pixel_size = frustum->get_pixel_dimension(
                viewport.get_width(),
                viewport.get_height(),
                1.0);

        OrthographicOffCenterFrustum off_center_frustum;
        off_center_frustum.set_right(pixel_size.x * 0.5);
        off_center_frustum.set_left(-off_center_frustum.get_right());
        off_center_frustum.set_top(pixel_size.y * 0.5);
        off_center_frustum.set_bottom(-off_center_frustum.get_top());
        off_center_frustum.set_near(frustum->get_near());
        off_center_frustum.set_far(frustum->get_far());

        return off_center_frustum.get_culling_volume(
                origin,
                m_camera->get_direction_wc(),
                m_camera->get_up_wc());
    }

	PickDepth* Scene::get_pick_depth(const unsigned int& index)
	{
		if (m_pick_depths.find(index) == m_pick_depths.end())
		{
			m_pick_depths[index] = std::unique_ptr<PickDepth>(new PickDepth());
		}

		return m_pick_depths[index].get();
	}

	std::unique_ptr<CullingVolume> Scene::get_pick_culling_volume(
		const vec2d& surface_position,
		int width,
		int height) const
	{
        Frustum* frustum = m_camera->get_frustum();

        if(dynamic_cast<OrthographicFrustum*>(frustum) != nullptr ||
           dynamic_cast<OrthographicOffCenterFrustum*>(frustum) != nullptr)
        {
            return get_pick_orthographic_culling_volume(
                    surface_position,
                    width,
                    height);
        }

		return get_pick_perspective_culling_volume(
			surface_position,
			width,
			height);
	}

    void Scene::morph_complete(
            SceneTransitioner *transitioner,
            const SceneMode &previous_mode,
            const SceneMode &mode,
            const bool &was_morphing)
    { }

    void Scene::morph_start(
            SceneTransitioner *transitioner,
            const SceneMode &previous_mode,
            const SceneMode &mode,
            const bool &was_morphing)
    { }

	bool Scene::add_snapshot_listener(SnapshotListener* listener)
	{
		return m_snapshot_listeners.emplace(listener).second;
	}

	bool Scene::remove_snapshot_listener(SnapshotListener* listener)
	{
		return m_snapshot_listeners.erase(listener);
	}

	void Scene::take_ttar_snapshot(
		Bitmap* reality_snapshot,
        Bitmap* ttar_snapshot,
		Camera* camera,
        const unsigned int& width,
        const unsigned int& height)
	{
        const bool use_depth_picking = m_use_depth_picking;
        m_use_depth_picking = false;

		Camera* current_camera = nullptr;
		if (camera != nullptr)
		{
			current_camera = m_camera.release();
			m_camera.reset(camera);
		}

        m_ar_buffer.swap(m_ttar_buffer);

		// Update with previous frame's number and time, assuming that render is called before taking the snapshot.
		update_frame_state(
			m_frame_state.get_frame_number(),
			m_frame_state.get_time());
		m_frame_state.set_pass(Pass::Render);

        m_downloader->update(
                &m_frame_state,
                height,
                width,
                ImageFormat::BlueGreenRedAlpha,
                ImageDataType::UnsignedByte);

        // Update the uniform state based on the current frame state
        UniformState* uniform_state = m_context->get_uniform_state();
        uniform_state->update(&m_frame_state);

        Rectangle render_target_viewport(
                0,
                0,
                width,
                height);
        update_viewports(render_target_viewport);


        m_ar_buffer->update(
            m_context,
            m_render_target_viewport);


        // Reset the pass state
        // Set an arbitrary framebuffer to avoid clearing the display
        m_pass_state.set_framebuffer(m_ar_buffer->get_mixed_reality_frame_buffer());
        m_pass_state.set_blending(nullptr);
        m_pass_state.set_scissor_test(nullptr);
        m_pass_state.set_viewport(&m_render_target_viewport);

        update_environment_state();


		if (reality_snapshot != nullptr)
		{
			ColorAttachments* attachments = m_ar_buffer->get_reality_frame_buffer()->get_color_attachments();
			const Texture2D* attachment   = static_cast<const Texture2D*>(attachments->get_attachment(0));

			std::unique_ptr<WritePixelBuffer> write_buffer = Device::create_write_pixel_buffer(
					PixelBufferHint::Static,
					reality_snapshot->get_size_in_bytes());
			write_buffer->copy_from_bitmap(reality_snapshot, false);
			attachment->copy_from_buffer(
					write_buffer.get(),
					ImageFormat::RedGreenBlueAlpha,
					ImageDataType::UnsignedByte);
		}


        update_and_execute_commands(
            &m_pass_state,
            Color(0, 0, 0, 0),
            false,
            reality_snapshot == nullptr,
            false);


        unsigned int reality_downloader_index;
        unsigned int mixed_reality_downloader_index;

        if (reality_snapshot == nullptr)
        {
            reality_downloader_index = m_downloader->download(
                    m_ar_buffer->get_reality_frame_buffer(),
                0,
                0,
                width,
                height);
        }

        mixed_reality_downloader_index = m_downloader->download(
                m_ar_buffer->get_mixed_reality_frame_buffer(),
                0,
                0,
                width,
                height);

		if (camera != nullptr)
		{
			m_camera.release();
			m_camera.reset(current_camera);
		}
		else
		{
			camera = m_camera->clone();			
		}

        m_use_depth_picking = use_depth_picking;
        m_context->end_frame();

		std::unique_ptr<Fence> fence = Device::create_fence();
        fence->client_wait();

		std::thread thread(
			[&, width, height, reality_downloader_index, mixed_reality_downloader_index]
            (Bitmap* reality_snapshot, Bitmap* ttar_snapshot, Camera* camera)
		{
			std::unique_ptr<Surface> surface = Device::create_pixel_buffer_surface(1, 1);

			std::vector<vec4ub> rgba;

			if (reality_snapshot == nullptr)
			{
				reality_snapshot = new Bitmap(
					width,
					height,
					PixelFormat::Format32bppArgb);
				rgba = m_downloader->read_pixels<vec4ub>(reality_downloader_index);
				reality_snapshot->set_pixels(reinterpret_cast<unsigned int*>(rgba.data()));
			}

            if (ttar_snapshot == nullptr)
            {
                ttar_snapshot = new Bitmap(
                        width,
                        height,
                        PixelFormat::Format32bppArgb);
            }
			rgba = m_downloader->read_pixels<vec4ub>(mixed_reality_downloader_index);
			ttar_snapshot->set_pixels(reinterpret_cast<unsigned int*>(rgba.data()));

			auto cached_listeners = m_snapshot_listeners;
			for (SnapshotListener* listener : cached_listeners)
			{
                listener->on_ttar_snapshot_available(
					reality_snapshot,
					ttar_snapshot,
					camera);
			}
		},
        reality_snapshot,
        ttar_snapshot,
        camera);

        m_ar_buffer.swap(m_ttar_buffer);

		thread.detach();
	}

	void Scene::take_ar_buffer_snapshot()
	{
		unsigned int width  = m_context ->get_drawing_surface_width();
		unsigned int height = m_context->get_drawing_surface_height();

        m_downloader->update(
                &m_frame_state,
                height,
                width,
                ImageFormat::RedGreenBlueAlpha,
                ImageDataType::UnsignedByte);

		unsigned int reality_downloader_index = m_downloader->download(
			m_ar_buffer->get_reality_frame_buffer(),
			0,
			0,
			width,
			height);

        unsigned int virtuality_downloader_index = m_downloader->download(
			m_ar_buffer->get_virtuality_frame_buffer(),
			0,
			0,
			width,
			height);

        unsigned int mixed_reality_downloader_index = m_downloader->download(
			m_ar_buffer->get_mixed_reality_frame_buffer(),
			0,
			0,
			width,
			height);

		std::unique_ptr<Fence> fence = Device::create_fence();
		fence->client_wait();

		std::thread thread(
			[&, width, height, reality_downloader_index, mixed_reality_downloader_index, virtuality_downloader_index]
		{
			std::unique_ptr<Surface> surface = Device::create_pixel_buffer_surface(1, 1);

			std::vector<vec4ub> rgba;
			rgba = m_downloader->read_pixels<vec4ub>(reality_downloader_index);
			Bitmap* reality_snapshot = new Bitmap(
				width,
				height,
				PixelFormat::Format32bppArgb);
			reality_snapshot->set_pixels(reinterpret_cast<unsigned int*>(rgba.data()));


            rgba = m_downloader->read_pixels<vec4ub>(virtuality_downloader_index);
			Bitmap* virtuality_snapshot = new Bitmap(
				width,
				height,
				PixelFormat::Format32bppArgb);
			virtuality_snapshot->set_pixels(reinterpret_cast<unsigned int*>(rgba.data()));

            rgba = m_downloader->read_pixels<vec4ub>(mixed_reality_downloader_index);
			Bitmap* mixed_reality_snapshot = new Bitmap(
				width,
				height,
				PixelFormat::Format32bppArgb);
			mixed_reality_snapshot->set_pixels(reinterpret_cast<unsigned int*>(rgba.data()));


			auto cached_listeners = m_snapshot_listeners;
			for (SnapshotListener* listener : cached_listeners)
			{
				listener->on_ar_buffer_snapshot_available(
					reality_snapshot,
					virtuality_snapshot,
					mixed_reality_snapshot);
			}
		});

		thread.detach();
	}

	void Scene::take_snapshot()
	{
		unsigned int width  = m_context->get_drawing_surface_width();
		unsigned int height = m_context->get_drawing_surface_height();

        m_downloader->update(
                &m_frame_state,
                height,
                width,
                ImageFormat::RedGreenBlueAlpha,
                ImageDataType::UnsignedByte);

        unsigned int downloader_index = m_downloader->download(
			m_ar_buffer->get_mixed_reality_frame_buffer(),
			0,
			0,
			width,
			height);

		std::unique_ptr<Fence> fence = Device::create_fence();
		fence->client_wait();

		std::thread thread(
			[&, width, height, downloader_index]
		{
			std::unique_ptr<Surface> surface = Device::create_pixel_buffer_surface(1, 1);
			
			std::vector<vec4ub> rgba = m_downloader->read_pixels<vec4ub>(downloader_index);
			Bitmap* bitmap = new Bitmap(
				width,
				height,
				PixelFormat::Format32bppArgb);
			bitmap->set_pixels(reinterpret_cast<unsigned int*>(rgba.data()));

			auto cached_listeners = m_snapshot_listeners;
			for (SnapshotListener* listener : cached_listeners)
			{
				listener->on_snapshot_available(bitmap);
			}
		});

		thread.detach();
	}

	IPickable* Scene::pick_primitive(const vec2d& window_position)
	{
		// Update the uniforms state.
		UniformState* uniform_state = m_context->get_uniform_state();

		// Begin frame render
		m_context->begin_frame();

		vec2d surface_position = SceneTransforms::window_to_drawing_buffer(this, window_position);

		if (m_pick_framebuffer == nullptr)
		{
			m_pick_framebuffer = m_context->create_pick_frame_buffer();
		}

		// Update with previous frame's number and time, assuming that render is called before picking.
		update_frame_state(
			m_frame_state.get_frame_number(),
			m_frame_state.get_time());
		m_frame_state.set_pass(Pass::Pick);

		// Calculate the culling volume for the picking.
		std::unique_ptr<CullingVolume> pick_cull_volume = get_pick_culling_volume(
			surface_position,
			m_picking_rectangle.get_width(),
			m_picking_rectangle.get_height());
		m_frame_state.set_culling_volume(pick_cull_volume.get());

		uniform_state->update(&m_frame_state);

		Rectangle screen_space_rectangle(
			surface_position.x - ((m_picking_rectangle.get_width() - 1.0) * 0.5),
			(m_context->get_drawing_surface_height() - surface_position.y) - ((m_picking_rectangle.get_height() - 1.0) * 0.5),
			m_picking_rectangle.get_width(),
			m_picking_rectangle.get_height());

		PassState* pass_state = m_pick_framebuffer->begin(screen_space_rectangle);
		update_and_execute_commands(pass_state, Color(0, 0, 0, 0), true);
		IPickable* object = m_pick_framebuffer->end(screen_space_rectangle);

		if (object != nullptr)
		{
			object->on_picked();
		}

		// End frame render.
		m_context->end_frame();

		return object;
	}

	void Scene::on_picked(IPickable* object)
	{
		m_picked_list.push_back(object);

		if (m_picking_mode == PickingMode::Single)
		{
			for (IPickable* pickable : m_picked_list)
			{
				if (pickable != object)
				{
					pickable->set_is_picked(false);
				}
			}
		}
	}

    vec3d Scene::pick_position(const vec2d& window_position)
    {
        vec3d result = pick_position_world_coordinates(window_position);

        if (!vec_undefined(result) &&
            m_mode != SceneMode::Scene3D)
        {
            result = vec3d(
                    result.y,
                    result.z,
                    result.x);

            Ellipsoid ellipsoid = m_projection->get_ellipsoid();
            Geodetic3D geodetic = m_projection->unproject(result);
            result              = ellipsoid.to_geocentric3D(geodetic).to_vec3d();
        }

        return result;
    }

	vec3d Scene::pick_position_world_coordinates(const vec2d& window_position)
	{
		if (!m_use_depth_picking)
        {
            return vec3d::undefined();
        }

        std::string cache_key = vec_string(window_position);

        if (m_pick_position_cache_dirty)
        {
            m_pick_position_cache.clear();
            m_pick_position_cache_dirty = false;
        }
        else if (m_pick_position_cache.find(cache_key) != m_pick_position_cache.end())
        {
            return m_pick_position_cache[cache_key];
        }

        vec2d surface_position = SceneTransforms::window_to_drawing_buffer(this, window_position);
        surface_position.y = m_context->get_drawing_surface_height() - surface_position.y;

        // Create a working frustum from the original camera frustum.
        std::unique_ptr<Frustum> frustum = std::unique_ptr<Frustum>(
            m_camera->get_frustum()->clone());

        UniformState* uniform_state = m_context->get_uniform_state();
        unsigned int num_frustums   = m_frustum_commands_queue.size();

        for (unsigned int index = 0; index < num_frustums; index++)
        {
            PickDepth* pick_depth = get_pick_depth(index);

            float depth = ArgeoMath::float_from_vec4(m_context->read_pixels(
                surface_position.x,
                surface_position.y,
                1,
                1,
                pick_depth->get_frame_buffer()).at(0));

            if (depth > 0.0 && depth < 1.0)
            {
                const FrustumCommands rendered_frustum = m_frustum_commands_queue[index];
                double height_2D = 0.0;

                if(m_mode == SceneMode::Scene2D)
                {
                    height_2D = m_camera->get_position().z;

                    m_camera->set_position(vec3d(
                            m_camera->get_position().x,
                            m_camera->get_position().y,
                            height_2D - rendered_frustum.near + 1.0));

                    frustum->set_near(1.0);
                    frustum->set_far(::max(1.0, rendered_frustum.far - rendered_frustum.near));

                    uniform_state->update(&m_frame_state);
                    uniform_state->update_frustum(frustum.get());
                }
                else
                {
                    frustum->set_near(rendered_frustum.near * (index != 0 ? OPAQUE_FRUSTUM_NEAR_OFFSET : 1.0));
                    frustum->set_far(rendered_frustum.far);
                    uniform_state->update_frustum(frustum.get());
                }

                if (m_mode == SceneMode::Scene2D)
                {
                    m_camera->set_position(vec3d(
                            m_camera->get_position().x,
                            m_camera->get_position().y,
                            height_2D));
                    uniform_state->update(&m_frame_state);
                }

                vec3d result = SceneTransforms::drawing_buffer_to_cartesian_coordinates(
                        this,
                        surface_position,
                        depth);

                m_pick_position_cache[cache_key] = result;
                return result;
            }
        }

        m_pick_position_cache[cache_key] = vec3d::undefined();
        return vec3d::undefined();
	}
}