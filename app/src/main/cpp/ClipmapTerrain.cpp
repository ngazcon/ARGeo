#include <OrientedBoundingBox.h>
#include <Log.h>
#include <TextureGL.h>
#include <Texture2DGL.h>
#include "ClipmapTerrain.h"
#include "ClipmapShaders.h"
#include "Device.h"
#include "RectangleTessellator.h"
#include "TriangleIndicesUnsignedShort.h"
#include "IndicesUnsignedShort.h"
#include "VertexAttributeFloatVector2.h"
#include "Scene.h"
#include "IntersectionAlgorithms.h"

namespace argeo
{

    const ShaderVertexAttributeLocations ClipmapTerrain::attribute_locations =
    {
            { "position", 0 },
    };

	ClipmapTerrain::ClipmapTerrain(
		Scene* scene,
        const Ellipsoid& ellipsoid,
        const unsigned int& clipmap_posts)
		: m_wireframe(false)
		, m_show_imagery(false)
		, m_show(true)
        , m_depth_test_against(false)
		, m_height_exaggeration_dirty(false)
		, m_ellipsoid(ellipsoid)
		, m_clipmap_posts(clipmap_posts)
		, m_clipmap_segments(clipmap_posts - 1)
		, m_clipmap_center(0, 0)
		, m_height_exaggeration(1.0)
		, m_resources_dirty(true)
		, m_updater_dirty(true)
		, m_raster_source(nullptr)
        , m_imagery_source(nullptr)
		, m_scene(scene)
        , m_shader_set(new TerrainShaderSet(globe_clipmap_vs, globe_clipmap_fs))
        , m_mode(SceneMode::Scene3D)
        , m_use_double_precision(false)
	{
		MaterialTemplate diffuse_map_template(
			MaterialTemplate::Components
		{
			{ "diffuse" , "texture(diffuseTexture, materialInput.st).rgb" }
		},
			MaterialTemplate::Values
		{
			{ "diffuseTexture", new UniformValue<std::string>("imagery/ColorRamp.jpg", UniformType::Sampler2D) }
		});

		MaterialTemplate material_template(
			MaterialTemplate::Components
		{
			{ "diffuse" , "diffuseColor * diffuseMaterial.diffuse" }
		},
			MaterialTemplate::Values
		{
			{ "diffuseColor", new UniformValue<vec3>(vec3(1.0f), UniformType::FloatVector3) }
		},
			MaterialTemplate::Materials
		{
			{ "diffuseMaterial", diffuse_map_template }
		});

		m_material = std::unique_ptr<Material>(new Material(
			"ClipmapMaterial",
			material_template));

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// GEOCLIPMAPS PATCHS
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		m_fill_patch_posts    = (clipmap_posts + 1) / 4; // M
		m_fill_patch_segments = m_fill_patch_posts - 1;

        m_scene->add_raster_source_changed_listener(this);
	}


	ClipmapTerrain::~ClipmapTerrain()
	{
		m_scene->remove_raster_source_changed_listeners(this);
	}
	
	void ClipmapTerrain::on_raster_source_changed(RasterSource* value)
	{
		set_raster_source(value);
	}

	void ClipmapTerrain::notify_dirty(const RasterTile *tile)
	{
        unsigned int level_index = tile->get_identifier().level;
        set_level_bounding_volume_dirty(level_index);
	}

    void ClipmapTerrain::set_imagery_source(RasterSource *value)
    {
        m_imagery_source = value;
    }

    RasterSource* ClipmapTerrain::get_imagery_source()
    {
        return m_imagery_source;
    }

	void ClipmapTerrain::set_raster_source(RasterSource* value)
	{
		if (m_raster_source != value)
		{
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// CLIPMAPS LEVELS
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			int clipmap_levels = value->get_levels().size();

			m_clipmap_levels.resize(clipmap_levels);
			for (int i = 0; i < m_clipmap_levels.size(); ++i)
			{
				m_clipmap_levels[i] = std::unique_ptr<ClipmapLevel>(new ClipmapLevel());
			}

			for (int i = 0; i < m_clipmap_levels.size(); ++i)
			{
				// First create the terrain raster for this level.
				RasterLevel* terrain_level = value->get_levels()[i];

				m_clipmap_levels[i]->set_terrain(terrain_level);

				m_clipmap_levels[i]->set_height_texture(
					Device::create_texture2D(TextureDescription(
						m_clipmap_posts,
						m_clipmap_posts,
						TextureFormat::RedGreenBlueAlpha8,
						false),
						Device::get_texture_samplers()->get_linear_repeat()));

				m_clipmap_levels[i]->set_normal_texture(
					Device::create_texture2D(TextureDescription(
						m_clipmap_posts,
						m_clipmap_posts,
						TextureFormat::RedGreenBlue8,
						false),
						Device::get_texture_samplers()->get_linear_repeat()));


				// Set the coarser level for all levels but not to the zero level.
				m_clipmap_levels[i]->set_coarser_level(i == 0 ?
					nullptr :
					m_clipmap_levels[i - 1].get());

				// Set the finer level for all levels but not to the last level.
				m_clipmap_levels[i]->set_finer_level(i == m_clipmap_levels.size() - 1 ?
					nullptr :
					m_clipmap_levels[i + 1].get());

				// Second create the imagery raster for this level.

				// Aim for roughly one imagery texel per geometry texel.
				// Find the first imagery level that meets our resolution needs.
				double longitude_res_required = terrain_level->get_post_delta_longitude();
				double latitude_res_required = terrain_level->get_post_delta_latitude();

				RasterLevel* imageryLevel = nullptr;
				for (int j = 0; j < m_imagery_source->get_levels().size(); ++j)
				{
					imageryLevel = m_imagery_source->get_levels()[j];

					if (imageryLevel->get_post_delta_longitude() <= longitude_res_required &&
						imageryLevel->get_post_delta_latitude() <= latitude_res_required)
					{
						break;
					}
				}

				m_clipmap_levels[i]->set_imagery(imageryLevel);

				m_clipmap_levels[i]->set_imagery_width(
					(int)ceilf(m_clipmap_posts  * terrain_level->get_post_delta_longitude() / imageryLevel->get_post_delta_longitude()));

				m_clipmap_levels[i]->set_imagery_height(
					(int)ceilf(m_clipmap_posts * terrain_level->get_post_delta_latitude() / imageryLevel->get_post_delta_latitude()));

				m_clipmap_levels[i]->set_imagery_texture(
					Device::create_texture2D(TextureDescription(
						m_clipmap_levels[i]->get_imagery_width(),
						m_clipmap_levels[i]->get_imagery_height(),
						TextureFormat::RedGreenBlue8,
						false),
						Device::get_texture_samplers()->get_linear_repeat()));
			}

			m_raster_source   = value;
			m_updater_dirty   = true;
			m_resources_dirty = true;
		}
	}

	RasterSource* ClipmapTerrain::get_raster_source()
	{ 
		return m_scene->get_raster_source();
	}

	void ClipmapTerrain::update_clipmap_updater(Context* context)
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// CLIPMAP UPDATER
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		std::vector<ClipmapLevel*> levels;
		levels.resize(m_clipmap_levels.size());
		for (int i = 0; i < m_clipmap_levels.size(); ++i)
		{
			levels[i] = m_clipmap_levels[i].get();
		}

		m_updater = std::unique_ptr<ClipmapUpdater>(
			new ClipmapUpdater(context, levels));

		for (int i = 0; i < m_clipmap_levels.size(); ++i)
		{
			m_updater->set_height_exaggeration(
				m_height_exaggeration,
				levels[i]);
		}

		m_updater_dirty = false;
	}

	vec3d ClipmapTerrain::pick(
			const Ray& ray,
			Scene* scene)
	{
        std::vector<ClipmapLevel*> intersection_levels;

        unsigned int levels = m_clipmap_levels.size();
        for (int i = levels - 1; i >= 0; i--)
        {
            ClipmapLevel* level = m_clipmap_levels[i].get();
            std::unique_ptr<BoundingSphere> bounding_sphere;

            if (scene->get_mode() == SceneMode::Scene2D)
            {
                GeodeticExtent extent = level->get_geodetic_extent();
                bounding_sphere = BoundingSphere::from_rectangle_with_heights_2D(
                        extent,
                        m_scene->get_projection(),
                        level->get_minimum_height(),
                        level->get_maximum_height());
            }
            else
            {
                bounding_sphere = std::unique_ptr<BoundingSphere>(
                        static_cast<BoundingSphere*>(level->get_bounding_volume()->clone()));
            }


            Interval bounding_sphere_intersection = IntersectionAlgorithms::ray_sphere(
                    ray,
                    bounding_sphere.get());

            if(std::isnan(bounding_sphere_intersection.start) &&
               std::isnan(bounding_sphere_intersection.stop))
            {
                intersection_levels.push_back(level);
            }
        }

        std::sort(
                intersection_levels.begin(),
                intersection_levels.end(),
                [&, ray](ClipmapLevel* a, ClipmapLevel* b)
                {
                    const double a_dist = static_cast<BoundingSphere*>(a->get_bounding_volume())->distance_squared_to(ray.origin);
                    const double b_dist = static_cast<BoundingSphere*>(b->get_bounding_volume())->distance_squared_to(ray.origin);
                    return b_dist < a_dist;
                });

        vec3d intersection = vec3d::undefined();
        for (ClipmapLevel* level : intersection_levels)
        {
            intersection = level->pick(
                    ray,
                    m_scene->get_mode(),
                    m_scene->get_projection(),
                    true);

            if (!vec_undefined(intersection))
            {
                break;
            }
        }

        return intersection;
	}

    vec3d ClipmapTerrain::pick_from_tile(
            const RasterTile *tile,
            const Ray &ray,
            const SceneMode &mode,
            MapProjection *projection,
            const bool &cull_back_faces)
    {
        return m_clipmap_levels[tile->get_level()->get_level()]->pick(
                ray,
                mode,
                projection,
                cull_back_faces);
    }

	void ClipmapTerrain::create_vertex_arrays(Context* context)
	{
		// Create the MxM block used to fill the ring and the field.
		std::unique_ptr<Mesh> field_block_mesh = RectangleTessellator::compute(
			RectangleD(vec2d(0.0, 0.0), vec2d(m_fill_patch_segments, m_fill_patch_segments)),
			m_fill_patch_segments,
			m_fill_patch_segments);

		m_render_resources.fill_patch = context->create_vertex_array(
			field_block_mesh.get(),
            attribute_locations,
			BufferHint::StaticDraw);

		// Create the Mx3 block used to fill the space between the MxM blocks in the ring
		std::unique_ptr<Mesh> ring_fixup_horizontal_mesh = RectangleTessellator::compute(
			RectangleD(vec2d(0.0, 0.0), vec2d(m_fill_patch_segments, 2.0)),
			m_fill_patch_segments,
			2);
		m_render_resources.horizontal_fixup_patch = context->create_vertex_array(
			ring_fixup_horizontal_mesh.get(),
            attribute_locations,
			BufferHint::StaticDraw);

		// Create the 3xM block used to fill the space between the MxM blocks in the ring
		std::unique_ptr<Mesh> ring_fixup_vertical_mesh = RectangleTessellator::compute(
			RectangleD(vec2d(0.0, 0.0), vec2d(2.0, m_fill_patch_segments)),
			2,
			m_fill_patch_segments);
		m_render_resources.vertical_fixup_patch = context->create_vertex_array(
			ring_fixup_vertical_mesh.get(),
            attribute_locations,
			BufferHint::StaticDraw);

		std::unique_ptr<Mesh> offset_strip_horizontal_mesh = RectangleTessellator::compute(
			RectangleD(vec2d(0.0, 0.0), vec2d(2 * m_fill_patch_posts, 1.0)),
			2 * m_fill_patch_posts,
			1);
		m_render_resources.horizontal_offset_patch = context->create_vertex_array(
			offset_strip_horizontal_mesh.get(),
            attribute_locations,
			BufferHint::StaticDraw);

		std::unique_ptr<Mesh> offset_strip_vertical_mesh = RectangleTessellator::compute(
			RectangleD(vec2d(0.0, 0.0), vec2d(1.0, 2 * m_fill_patch_posts - 1)),
			1,
			2 * m_fill_patch_posts - 1);
		m_render_resources.vertical_offset_patch = context->create_vertex_array(
			offset_strip_vertical_mesh.get(),
            attribute_locations,
			BufferHint::StaticDraw);

		std::unique_ptr<Mesh> center_mesh = RectangleTessellator::compute(
			RectangleD(vec2d(0.0, 0.0), vec2d(2.0, 2.0)),
			2,
			2);
		m_render_resources.center_patch = context->create_vertex_array(
			center_mesh.get(),
            attribute_locations,
			BufferHint::StaticDraw);

		std::unique_ptr<Mesh> degenerate_triangle_mesh = create_degenerate_triangle_mesh();
		m_render_resources.degenerate_triangle_patch = context->create_vertex_array(
			degenerate_triangle_mesh.get(),
            attribute_locations,
			BufferHint::StaticDraw);

		m_render_resources.primitive_type = field_block_mesh->get_primitive_type();
	}

	void ClipmapTerrain::create_resources(Context* context)
	{
		RenderStateParameters parameters;
		parameters.facet_culling = FacetCulling(
			true,
			CullFace::Back,
			WindingOrder::CounterClockwise);
		parameters.depth_test.enabled = true;
		parameters.depth_test.function = DepthTestFunction::LessOrEqual;
		m_render_resources.render_state = RenderState::from_cache(parameters);

		create_vertex_arrays(context);
		create_block_commands();

		m_resources_dirty = false;
	}

	GeodeticExtent ClipmapTerrain::get_block_geodetic_extent(
		unsigned int level_index,
		unsigned int block_index)
	{
		std::stringstream id_stream;
		id_stream << level_index << "." << block_index;
		std::string id = id_stream.str();

		UniformValues* uniform_values = m_render_resources.uniform_values[id];

		vec2 patch_origin_in_clipped_level  = static_cast<UniformValue<vec2>*>((*uniform_values)["u_patchOriginInClippedLevel"])->get_data();
		vec2 level_offset_from_world_origin = static_cast<UniformValue<vec2>*>((*uniform_values)["u_levelOffsetFromWorldOrigin"])->get_data();
		vec2 level_scale_factor             = static_cast<UniformValue<vec2>*>((*uniform_values)["u_levelScaleFactor"])->get_data();
		vec2 level_zero_world_scale_factor  = static_cast<UniformValue<vec2>*>((*uniform_values)["u_levelZeroWorldScaleFactor"])->get_data();

		vec2 sw = patch_origin_in_clipped_level;
		vec2 ne = patch_origin_in_clipped_level;

        ne += get_block_size(block_index);

		ne += level_offset_from_world_origin;
        sw += level_offset_from_world_origin;

		ne *= level_scale_factor * level_zero_world_scale_factor;
		sw *= level_scale_factor * level_zero_world_scale_factor;

		return GeodeticExtent(
			ArgeoMath::to_radians(sw.x),
			ArgeoMath::to_radians(sw.y),
			ArgeoMath::to_radians(ne.x),
			ArgeoMath::to_radians(ne.y));
	}

    vec2 ClipmapTerrain::get_block_size(const unsigned int& block_index)
    {
        switch ((BlockIndex)block_index)
        {
            // M x M block.
            case BlockIndex::FillBlock0:
            case BlockIndex::FillBlock1:
            case BlockIndex::FillBlock2:
            case BlockIndex::FillBlock3:
            case BlockIndex::FillBlock4:
            case BlockIndex::FillBlock5:
            case BlockIndex::FillBlock6:
            case BlockIndex::FillBlock7:
            case BlockIndex::FillBlock8:
            case BlockIndex::FillBlock9:
            case BlockIndex::FillBlock10:
            case BlockIndex::FillBlock11:
            case BlockIndex::CenterFillBlock0:
            case BlockIndex::CenterFillBlock1:
            case BlockIndex::CenterFillBlock2:
            case BlockIndex::CenterFillBlock3:
                return vec2(m_fill_patch_segments);

                // Fix-up rings
            case BlockIndex::HorizontalFixup0:
            case BlockIndex::HorizontalFixup1:
            case BlockIndex::CenterHorizontalFixup0:
            case BlockIndex::CenterHorizontalFixup1:
                return vec2(m_fill_patch_segments, 3.0);


            case BlockIndex::VerticalFixup0:
            case BlockIndex::VerticalFixup1:
            case BlockIndex::CenterVerticalFixup0:
            case BlockIndex::CenterVerticalFixup1:
                return vec2(3.0, m_fill_patch_segments);

            case BlockIndex::HorizontalInteriorTrim:
                return vec2(2.0 * m_fill_patch_posts, 2.0);

            case BlockIndex::VerticalInteriorTrim:
                return vec2(2.0, 2.0 * m_fill_patch_posts - 1);

            case BlockIndex::DegenerateTrianglesOuterRing:
                return vec2(m_fill_patch_segments);

            case BlockIndex::CenterBlock:
                return vec2(3.0);
        }
    }


    VertexArray* ClipmapTerrain::get_block_vertex_array(unsigned int block_index)
	{
		switch ((BlockIndex)block_index)
		{
			// M x M block.
		case BlockIndex::FillBlock0:
		case BlockIndex::FillBlock1:
		case BlockIndex::FillBlock2:
		case BlockIndex::FillBlock3:
		case BlockIndex::FillBlock4:
		case BlockIndex::FillBlock5:
		case BlockIndex::FillBlock6:
		case BlockIndex::FillBlock7:
		case BlockIndex::FillBlock8:
		case BlockIndex::FillBlock9:
		case BlockIndex::FillBlock10:
		case BlockIndex::FillBlock11:
		case BlockIndex::CenterFillBlock0:
		case BlockIndex::CenterFillBlock1:
		case BlockIndex::CenterFillBlock2:
		case BlockIndex::CenterFillBlock3:
			return m_render_resources.fill_patch.get();

			// Fix-up rings
		case BlockIndex::HorizontalFixup0:
		case BlockIndex::HorizontalFixup1:
		case BlockIndex::CenterHorizontalFixup0:
		case BlockIndex::CenterHorizontalFixup1:
			return m_render_resources.horizontal_fixup_patch.get();

		case BlockIndex::VerticalFixup0:
		case BlockIndex::VerticalFixup1:
		case BlockIndex::CenterVerticalFixup0:
		case BlockIndex::CenterVerticalFixup1:
			return m_render_resources.vertical_fixup_patch.get();

			// Interior trim block.
		case BlockIndex::HorizontalInteriorTrim:
			return m_render_resources.horizontal_offset_patch.get();

		case BlockIndex::VerticalInteriorTrim:
			return m_render_resources.vertical_offset_patch.get();

			// Degenerate triangle.
		case BlockIndex::DegenerateTrianglesOuterRing:
			return m_render_resources.degenerate_triangle_patch.get();

			// Center patch.
		case BlockIndex::CenterBlock:
			return m_render_resources.center_patch.get();

		}
	}

	std::unique_ptr<Mesh> ClipmapTerrain::create_degenerate_triangle_mesh()
	{
		Mesh* mesh = new Mesh();

		mesh->set_primitive_type(PrimitiveType::Triangles);
		mesh->set_front_face_winding_order(WindingOrder::CounterClockwise);

		int number_of_positions = m_clipmap_segments * 4;
		VertexAttributeFloatVector2* positions_attribute = new VertexAttributeFloatVector2("position", number_of_positions);

		int number_of_indices = (number_of_positions - 2) * 3;
		IndicesUnsignedShort* indices = new IndicesUnsignedShort(number_of_indices);
		mesh->set_indices(indices);

		std::vector<vec2> positions;
		for (int i = 0; i < m_clipmap_posts; ++i)
		{
			positions.push_back(vec2(0.0f, i));
		}

		for (int i = 1; i < m_clipmap_posts; ++i)
		{
			positions.push_back(vec2(i, m_clipmap_segments));
		}

		for (int i = m_clipmap_segments - 1; i >= 0; --i)
		{
			positions.push_back(vec2(m_clipmap_segments, i));
		}

		for (int i = m_clipmap_segments - 1; i > 0; --i)
		{
			positions.push_back(vec2(i, 0.0f));
		}

		for (int i = 0; i < number_of_positions - 2; ++i)
		{
			indices->add_triangle(TriangleIndicesUnsignedShort(
				(unsigned short)i, 
				(unsigned short)(i + 1), 
				(unsigned short)(i + 2)));
		}

		positions_attribute->set_values(positions);
		mesh->add_attribute(positions_attribute);

		return std::unique_ptr<Mesh>(mesh);
	}

	void ClipmapTerrain::update_bounding_volume(
            const unsigned int& level_index,
            const unsigned int& block_index,
            FrameState* frame_state)
	{
		std::stringstream id_stream;
		id_stream << level_index << "." << block_index;
		std::string id = id_stream.str();

		DrawCommand* draw_command   = m_block_commands[id];		
		ClipmapLevel* level			= m_clipmap_levels[level_index].get();
		GeodeticExtent block_extent = get_block_geodetic_extent(
			level_index, 
			block_index);

		draw_command->set_cull(true);
		
		if (draw_command->get_bounding_volume() != nullptr)
		{
			delete draw_command->get_bounding_volume();
		}

        if (frame_state->get_mode() != SceneMode::Scene3D)
        {
            BoundingSphere* bounding_volume = BoundingSphere::from_rectangle_with_heights_2D(
                    block_extent,
                    frame_state->get_map_projection(),
                    level->get_minimum_height(),
                    level->get_maximum_height()).release();

            const vec3d center = bounding_volume->get_center();
            bounding_volume->set_center(vec3d(
                    center.z,
                    center.x,
                    center.y));

            draw_command->set_bounding_volume(bounding_volume);
        }
        else
        {
            draw_command->set_bounding_volume(OrientedBoundingBox::from_rectangle(
                    block_extent,
                    level->get_minimum_height(),
                    level->get_maximum_height(),
                    m_ellipsoid).release());
        }
	}

    void ClipmapTerrain::set_level_bounding_volume_dirty(const unsigned int &level_index)
    {
        for (unsigned int block_index = 0; block_index < BlockIndex::NumberOfBlocks; block_index++)
        {
            std::stringstream id_stream;
            id_stream << level_index << "." << block_index;
            std::string id = id_stream.str();
            m_block_bounding_volume_dirty[id] = true;
        }
    }

	void ClipmapTerrain::update_terrain(
		unsigned int& level_index,
		FrameState* frame_state)
	{
		ClipmapLevel* level	= m_clipmap_levels[level_index].get();

		// Calculate the delta displacement from the last renderer extent.
		int delta_x = level->get_next_extent().west  - level->get_current_extent().west;
		int delta_y = level->get_next_extent().south - level->get_current_extent().south;

		// If no displacement happen since last render then there is nothing to update.
		if (delta_x == 0 && delta_y == 0)
			return;

		int min_longitude = delta_x > 0 ? level->get_current_extent().east + 1 : level->get_next_extent().west;
		int max_longitude = delta_x > 0 ? level->get_next_extent().east : level->get_current_extent().west - 1;
		int min_latitude  = delta_y > 0 ? level->get_current_extent().north + 1 : level->get_next_extent().south;
		int max_latitude  = delta_y > 0 ? level->get_next_extent().north : level->get_current_extent().south - 1;

		int width  = max_longitude - min_longitude + 1;
		int height = max_latitude  - min_latitude  + 1;

		if (level->get_current_extent().west > level->get_current_extent().east || // initial update
			width >= m_clipmap_posts || height >= m_clipmap_posts)				   // complete update
		{
			// Initial or complete update.
			width   = m_clipmap_posts;
			height  = m_clipmap_posts;

			min_longitude = level->get_next_extent().west;
			max_longitude = level->get_next_extent().east;
			min_latitude  = level->get_next_extent().south;
			max_latitude  = level->get_next_extent().north;
		}

		// First horizontal update the terrain.
		if (height > 0)
		{
			ClipmapUpdate horizontal_update(
				level,
				level->get_next_extent().west,
				min_latitude,
				level->get_next_extent().east,
				max_latitude);

			m_updater->update_terrain(frame_state, horizontal_update);
		}

		// Second vertical update the terrain.
		if (width > 0)
		{
			ClipmapUpdate vertical_update(
				level,
				min_longitude,
				level->get_next_extent().south,
				max_longitude,
				level->get_next_extent().north);

			m_updater->update_terrain(frame_state, vertical_update);
		}

		// Set the next extent as the current extent
		level->set_current_extent(level->get_next_extent());
        set_level_bounding_volume_dirty(level_index);
	}

	void ClipmapTerrain::update_imagery(
		unsigned int& level_index,
		FrameState* frame_state)
	{
		ClipmapLevel* level	 = m_clipmap_levels[level_index].get();

		int delta_x = level->get_next_imagery_extent().west - level->get_current_imagery_extent().west;
		int delta_y = level->get_next_imagery_extent().south - level->get_current_imagery_extent().south;

		if (delta_x == 0 && delta_y == 0)
			return;

		int min_longitude = delta_x > 0 ? level->get_current_imagery_extent().east + 1 : level->get_next_imagery_extent().west;
		int max_longitude = delta_x > 0 ? level->get_next_imagery_extent().east : level->get_current_imagery_extent().west - 1;
		int min_latitude  = delta_y > 0 ? level->get_current_imagery_extent().north + 1 : level->get_next_imagery_extent().south;
		int max_latitude  = delta_y > 0 ? level->get_next_imagery_extent().north : level->get_current_imagery_extent().south - 1;

		int width  = max_longitude - min_longitude + 1;
		int height = max_latitude  - min_latitude + 1;

		if (level->get_current_imagery_extent().west > level->get_current_imagery_extent().east || // initial update
			width >= m_clipmap_posts || height >= m_clipmap_posts) // complete update
		{
			// Initial or complete update.
			width   = m_clipmap_posts;
			height  = m_clipmap_posts;

			min_longitude = level->get_next_imagery_extent().west;
			max_longitude = level->get_next_imagery_extent().east;
			min_latitude  = level->get_next_imagery_extent().south;
			max_latitude  = level->get_next_imagery_extent().north;
		}

		if (height > 0)
		{
			ClipmapUpdate horizontal_update(
				level,
				level->get_next_imagery_extent().west,
				min_latitude,
				level->get_next_imagery_extent().east,
				max_latitude);

			m_updater->update_imagery(frame_state, horizontal_update);
		}

		if (width > 0)
		{
			ClipmapUpdate vertical_update(
				level,
				min_longitude,
				level->get_next_imagery_extent().south,
				max_longitude,
				level->get_next_imagery_extent().north);

			m_updater->update_imagery(frame_state, vertical_update);
		}

		level->set_current_imagery_extent(level->get_next_imagery_extent());
	}

	void ClipmapTerrain::update_origin_in_textures(ClipmapLevel* level)
	{
		int delta_x = level->get_next_extent().west  - level->get_current_extent().west;
		int delta_y = level->get_next_extent().south - level->get_current_extent().south;

		if (delta_x == 0 && delta_y == 0)
			return;

		if (level->get_current_extent().west > level->get_current_extent().east ||  // initial update
			std::abs(delta_x) >= m_clipmap_posts || std::abs(delta_y) >= m_clipmap_posts)       // complete update
		{
			level->set_origin_in_textures(vec2i(0, 0));
		}
		else
		{
			int new_origin_x = (level->get_origin_in_textures().x + delta_x) % m_clipmap_posts;

			if (new_origin_x < 0)
				new_origin_x += m_clipmap_posts;

			int new_origin_y = (level->get_origin_in_textures().y + delta_y) % m_clipmap_posts;

			if (new_origin_y < 0)
				new_origin_y += m_clipmap_posts;

			level->set_origin_in_textures(vec2i(new_origin_x, new_origin_y));
		}
	}

	void ClipmapTerrain::update_imagery_origin_in_textures(ClipmapLevel* level)
	{
		int delta_x = level->get_next_imagery_extent().west  - level->get_current_imagery_extent().west;
		int delta_y = level->get_next_imagery_extent().south - level->get_current_imagery_extent().south;
		
		if (delta_x == 0 && delta_y == 0)
			return;

		if (level->get_current_imagery_extent().west > level->get_current_imagery_extent().east ||   // initial update
			std::abs(delta_x) >= level->get_imagery_width() || std::abs(delta_y) >= level->get_imagery_height()) // complete update
		{
			level->set_origin_in_imagery(vec2i(0, 0));
		}
		else
		{
			int new_origin_x = (level->get_origin_in_imagery().x + delta_x) % level->get_imagery_width();

			if (new_origin_x < 0)
				new_origin_x += level->get_imagery_width();

			int new_origin_y = (level->get_origin_in_imagery().y + delta_y) % level->get_imagery_height();

			if (new_origin_y < 0)
				new_origin_y += level->get_imagery_height();

			level->set_origin_in_imagery(vec2i(new_origin_x, new_origin_y));
		}
	}


	void ClipmapTerrain::update(FrameState* frame_state)
	{
		if (m_updater_dirty)
		{
			update_clipmap_updater(frame_state->get_context());
		}

		if (m_height_exaggeration_dirty)
		{
			for (unsigned int i = 0; i < m_clipmap_levels.size(); i++)
			{
				m_updater->set_height_exaggeration(
					m_height_exaggeration,
					m_clipmap_levels[i].get());
			}
			m_height_exaggeration_dirty = false;
		}

		if (m_resources_dirty)
		{
			create_resources(frame_state->get_context());
		}

        pre_render(frame_state);

		m_material->update(frame_state);
        m_raster_source->update(frame_state);

		if (m_show)
		{
			generate_command_queue(frame_state);
		}
	}

	void ClipmapTerrain::create_block_commands()
	{
		// For each level create a uniform map per patch.
		// There are 12 M x M blocks, 4 fix-up rings, 2 interior trims and a degenerate triangle patch.
		// The first level has a fill patch.
		for (unsigned int i = 0; i < m_clipmap_levels.size(); ++i)
		{
			unsigned int block_index = 0;

			// Create the M x M blocks.
			for (int j = 0; j < 12; j++)
			{
				create_block_command(
					i,
					block_index++);
			}

			// Create the 4 fix-up rings.
			for (int j = 0; j < 4; j++)
			{
				create_block_command(
					i,
					block_index++);
			}

			// Create the degenerate triangle patch.
			create_block_command(
				i,
				block_index++);

            // Create the 2 interior trims.
            for (int j = 0; j < 2; j++)
            {
                create_block_command(
                        i,
                        block_index++);
            }

			// Create the 4 fill patchs.
			for (int j = 0; j < 4; j++)
			{
				create_block_command(
					i,
					block_index++);
			}

			// Create the 4 fix-up rings.
			for (int j = 0; j < 4; j++)
			{
				create_block_command(
					i,
					block_index++);
			}

			// Create the center patch.
			create_block_command(
				i,
				block_index);
		}
	}

	void ClipmapTerrain::create_block_command(
		unsigned int level_index,
		unsigned int block_index)
	{
		std::stringstream id_stream;
		id_stream << level_index << "." << block_index;
		std::string id = id_stream.str();

		create_uniform_map(id);

		m_block_commands[id] = new DrawCommand(
			m_render_resources.primitive_type,
			get_block_vertex_array(block_index),
            nullptr,
			m_render_resources.render_state,
			nullptr,
			m_render_resources.uniform_maps[id]);

		m_block_commands[id]->set_pass(CommandPass::Globe);

        m_block_bounding_volume_dirty[id] = true;
	}

	void ClipmapTerrain::create_uniform_map(std::string id)
	{
		UniformMap* uniform_map = new UniformMap();

        (*uniform_map)["u_tileRectangle"] = [&, id](IUniform* value)
        {
            static_cast<Uniform<vec4>*>(value)->set_value(static_cast<UniformValue<vec4>*>(
            (*m_render_resources.uniform_values[id])["u_tileRectangle"])->get_data());
        };

        (*uniform_map)["southAndNorthLatitude"] = [&, id](IUniform* value)
        {
            static_cast<Uniform<vec2>*>(value)->set_value(static_cast<UniformValue<vec2>*>(
            (*m_render_resources.uniform_values[id])["southAndNorthLatitude"])->get_data());
        };

        (*uniform_map)["southMercatorYAndOneOverHeight"] = [&, id](IUniform* value)
        {
            static_cast<Uniform<vec2>*>(value)->set_value(static_cast<UniformValue<vec2>*>(
            (*m_render_resources.uniform_values[id])["southMercatorYAndOneOverHeight"])->get_data());
        };

        (*uniform_map)["u_modifiedModelViewProjectionMatrix"] = [&, id](IUniform* value)
        {
            static_cast<Uniform<mat4>*>(value)->set_value(static_cast<UniformValue<mat4>*>(
             (*m_render_resources.uniform_values[id])["u_modifiedModelViewProjectionMatrix"])->get_data());
        };

        (*uniform_map)["u_modifiedModelViewMatrix"] = [&, id](IUniform* value)
        {
            static_cast<Uniform<mat4>*>(value)->set_value(static_cast<UniformValue<mat4>*>(
             (*m_render_resources.uniform_values[id])["u_modifiedModelViewMatrix"])->get_data());
        };

		// Set the level zero scale factor.
		(*uniform_map)["u_levelZeroWorldScaleFactor"] = [&, id](IUniform* value)
		{
			static_cast<Uniform<vec2>*>(value)->set_value(static_cast<UniformValue<vec2>*>(
				(*m_render_resources.uniform_values[id])["u_levelZeroWorldScaleFactor"])->get_data());
		};

		////////////////////////////////////////////////////////////////////////////////////////////
		// SET TEXTURES
		////////////////////////////////////////////////////////////////////////////////////////////

		// Set height texture.
		(*uniform_map)["og_texture0"] = [&, id](IUniform* value)
		{
			static_cast<UniformSampler*>(value)->set_texture(static_cast<UniformValue<Texture*>*>(
				(*m_render_resources.uniform_values[id])["og_texture0"])->get_data());
		};

		(*uniform_map)["og_texture1"] = [&, id](IUniform* value)
		{
			static_cast<UniformSampler*>(value)->set_texture(static_cast<UniformValue<Texture*>*>(
				(*m_render_resources.uniform_values[id])["og_texture1"])->get_data());
		};

		// Set the normal texture.
		(*uniform_map)["og_fineNormalTexture"] = [&, id](IUniform* value)
		{
			static_cast<UniformSampler*>(value)->set_texture(static_cast<UniformValue<Texture*>*>(
				(*m_render_resources.uniform_values[id])["og_fineNormalTexture"])->get_data());
		};

		(*uniform_map)["og_coarseNormalTexture"] = [&, id](IUniform* value)
		{
			static_cast<UniformSampler*>(value)->set_texture(static_cast<UniformValue<Texture*>*>(
				(*m_render_resources.uniform_values[id])["og_coarseNormalTexture"])->get_data());
		};

		// Set the imagery texture.
		(*uniform_map)["og_texture4"] = [&, id](IUniform* value)
		{
			static_cast<UniformSampler*>(value)->set_texture(static_cast<UniformValue<Texture*>*>(
				(*m_render_resources.uniform_values[id])["og_texture4"])->get_data());
		};

		////////////////////////////////////////////////////////////////////////////////////////////
		// SET LEVEL SCALE AND OFFSET
		////////////////////////////////////////////////////////////////////////////////////////////

        (*uniform_map)["u_oneOverBlockSize"] = [&, id](IUniform* value)
        {
            static_cast<Uniform<vec2>*>(value)->set_value(static_cast<UniformValue<vec2>*>(
             (*m_render_resources.uniform_values[id])["u_oneOverBlockSize"])->get_data());
        };

		// Compute the level scale. 2 to the -L
		(*uniform_map)["u_levelScaleFactor"] = [&, id](IUniform* value)
		{
			static_cast<Uniform<vec2>*>(value)->set_value(static_cast<UniformValue<vec2>*>(
				(*m_render_resources.uniform_values[id])["u_levelScaleFactor"])->get_data());
		};

		// Compute the level offset.
		// Is the real-world space between post at the coarsest level.
		// Multiplying these two values gives the real-world space between post at the current level.
		(*uniform_map)["u_levelOffsetFromWorldOrigin"] = [&, id](IUniform* value)
		{
			static_cast<Uniform<vec2>*>(value)->set_value(static_cast<UniformValue<vec2>*>(
				(*m_render_resources.uniform_values[id])["u_levelOffsetFromWorldOrigin"])->get_data());
		};

		(*uniform_map)["u_fineLevelOriginInCoarse"] = [&, id](IUniform* value)
		{
			static_cast<Uniform<vec2>*>(value)->set_value(static_cast<UniformValue<vec2>*>(
				(*m_render_resources.uniform_values[id])["u_fineLevelOriginInCoarse"])->get_data());
		};

		(*uniform_map)["u_viewPosInClippedLevel"] = [&, id](IUniform* value)
		{
			static_cast<Uniform<vec2>*>(value)->set_value(static_cast<UniformValue<vec2>*>(
				(*m_render_resources.uniform_values[id])["u_viewPosInClippedLevel"])->get_data());
		};

		(*uniform_map)["u_fineTextureOrigin"] = [&, id](IUniform* value)
		{
			static_cast<Uniform<vec2>*>(value)->set_value(static_cast<UniformValue<vec2>*>(
				(*m_render_resources.uniform_values[id])["u_fineTextureOrigin"])->get_data());
		};

		////////////////////////////////////////////////////////////////////////////////////////////
		// SET IMAGERY SCALE RELATION
		////////////////////////////////////////////////////////////////////////////////////////////

		(*uniform_map)["u_terrainToImageryResolutionRatio"] = [&, id](IUniform* value)
		{
			static_cast<Uniform<vec2>*>(value)->set_value(static_cast<UniformValue<vec2>*>(
				(*m_render_resources.uniform_values[id])["u_terrainToImageryResolutionRatio"])->get_data());
		};

		(*uniform_map)["u_terrainOffsetInImagery"] = [&, id](IUniform* value)
		{
			static_cast<Uniform<vec2>*>(value)->set_value(static_cast<UniformValue<vec2>*>(
				(*m_render_resources.uniform_values[id])["u_terrainOffsetInImagery"])->get_data());
		};

		(*uniform_map)["u_oneOverImagerySize"] = [&, id](IUniform* value)
		{
			static_cast<Uniform<vec2>*>(value)->set_value(static_cast<UniformValue<vec2>*>(
				(*m_render_resources.uniform_values[id])["u_oneOverImagerySize"])->get_data());
		};

		////////////////////////////////////////////////////////////////////////////////////////////
		// SET PATCH UNIFORMS
		////////////////////////////////////////////////////////////////////////////////////////////

		(*uniform_map)["u_patchOriginInClippedLevel"] = [&, id](IUniform* value)
		{
			static_cast<Uniform<vec2>*>(value)->set_value(static_cast<UniformValue<vec2>*>(
				(*m_render_resources.uniform_values[id])["u_patchOriginInClippedLevel"])->get_data());
		};

        (*uniform_map)["u_center3DLow"] = [&, id](IUniform* value)
        {
            static_cast<Uniform<vec3>*>(value)->set_value(static_cast<UniformValue<vec3>*>(
                (*m_render_resources.uniform_values[id])["u_center3DLow"])->get_data());
        };

        (*uniform_map)["u_center3DHigh"] = [&, id](IUniform* value)
        {
            static_cast<Uniform<vec3>*>(value)->set_value(static_cast<UniformValue<vec3>*>(
                (*m_render_resources.uniform_values[id])["u_center3DHigh"])->get_data());
        };

        (*uniform_map)["u_center3D"] = [&, id](IUniform* value)
        {
            static_cast<Uniform<vec3>*>(value)->set_value(static_cast<UniformValue<vec3>*>(
                 (*m_render_resources.uniform_values[id])["u_center3D"])->get_data());
        };

		////////////////////////////////////////////////////////////////////////////////////////////
		// GENERAL UNIFORMS
		////////////////////////////////////////////////////////////////////////////////////////////

		(*uniform_map)["u_oneOverClipmapSize"] = [&](IUniform* value)
		{
			static_cast<Uniform<float>*>(value)->set_value(1.0f / m_clipmap_posts);
		};

		(*uniform_map)["u_heightExaggeration"] = [&](IUniform* value)
		{
			static_cast<Uniform<float>*>(value)->set_value(m_height_exaggeration);
		};

		(*uniform_map)["u_oneOverBlendedRegionSize"] = [&](IUniform* value)
		{
			float one_over_blended_region_size = (float)(10.0 / m_clipmap_posts);
			static_cast<Uniform<vec2>*>(value)->set_value(vec2(one_over_blended_region_size, one_over_blended_region_size));
		};

		(*uniform_map)["u_unblendedRegionSize"] = [&](IUniform* value)
		{
			float unblended_region_size = (float)(m_clipmap_segments / 2 - m_clipmap_posts / 10.0 - 1);
			static_cast<Uniform<vec2>*>(value)->set_value(vec2(unblended_region_size, unblended_region_size));
		};

		(*uniform_map)["u_useBlendRegions"] = [&, id](IUniform* value)
		{
			static_cast<Uniform<bool>*>(value)->set_value(static_cast<UniformValue<bool>*>(
				(*m_render_resources.uniform_values[id])["u_useBlendRegions"])->get_data());
		};

		(*uniform_map)["u_shade"] = [&](IUniform* value)
		{
			static_cast<Uniform<bool>*>(value)->set_value(m_lighting);
		};

		(*uniform_map)["u_showImagery"] = [&](IUniform* value)
		{
			static_cast<Uniform<bool>*>(value)->set_value(m_show_imagery);
		};

		(*uniform_map)["u_maxMinHeights"] = [&, id](IUniform* value)
		{
			static_cast<Uniform<vec2>*>(value)->set_value(static_cast<UniformValue<vec2>*>(
				(*m_render_resources.uniform_values[id])["u_maxMinHeights"])->get_data());
		};

		(*uniform_map)["u_globeRadiiSquared"] = [&](IUniform* value)
		{
			static_cast<Uniform<vec3>*>(value)->set_value(
				to_vec3(m_ellipsoid.get_radii_squared()));
		};

		UniformMap material_uniform_map = m_material->get_uniform_map();

		uniform_map->insert(
			material_uniform_map.begin(),
			material_uniform_map.end());

		m_render_resources.uniform_maps[id]   = uniform_map;
		m_render_resources.uniform_values[id] = new UniformValues();
	}

	void ClipmapTerrain::update_uniform_values(
		const int& level_index,
		const int& block_index,
        const int& block_west,
        const int& block_south,
        const int& overall_west,
        const int& overall_south,
		FrameState* frame_state)
	{
		std::stringstream id_stream;
		id_stream << level_index << "." << block_index;
		std::string id = id_stream.str();

		ClipmapLevel* level         = m_clipmap_levels[level_index].get();
		ClipmapLevel* coarser_level = level_index == 0 ? level : m_clipmap_levels[level_index - 1].get();

		UniformValues* uniform_values = m_render_resources.uniform_values[id];

        int texture_west  = block_west  - overall_west;
        int texture_south = block_south - overall_south;

		////////////////////////////////////////////////////////////////////////////////////////////
		// SET TEXTURES
		////////////////////////////////////////////////////////////////////////////////////////////

		// Set height texture.
		(*uniform_values)["og_texture0"] = new UniformValue<Texture*>(
			level->get_height_texture(),
			UniformType::Sampler2D);

		(*uniform_values)["og_texture1"] = new UniformValue<Texture*>(
			coarser_level->get_height_texture(),
			UniformType::Sampler2D);

		(*uniform_values)["og_fineNormalTexture"] = new UniformValue<Texture*>(
			level->get_normal_texture(),
			UniformType::Sampler2D);

		(*uniform_values)["og_coarseNormalTexture"] = new UniformValue<Texture*>(
                coarser_level->get_normal_texture(),
				UniformType::Sampler2D);

		(*uniform_values)["og_texture4"] = new UniformValue<Texture*>(
			level->get_imagery_texture(),
			UniformType::Sampler2D);

		////////////////////////////////////////////////////////////////////////////////////////////
		// SET LEVEL SCALE AND OFFSET
		////////////////////////////////////////////////////////////////////////////////////////////

		int west  = level->get_current_extent().west;
		int south = level->get_current_extent().south;

        vec2 block_size = get_block_size(block_index);
        (*uniform_values)["u_oneOverBlockSize"] = new UniformValue<vec2>(vec2(
                1.0 / block_size.x,
                1.0 / block_size.y),
                UniformType::FloatVector2);

		// Set the level zero scale factor.
		(*uniform_values)["u_levelZeroWorldScaleFactor"] = new UniformValue<vec2>(vec2(
			m_clipmap_levels[0]->get_terrain()->get_post_delta_longitude(),
			m_clipmap_levels[0]->get_terrain()->get_post_delta_latitude()),
			UniformType::FloatVector2);

		// Compute the level scale. 2 to the -L
        double level_scale_factor = ::powf(2.0, -level_index);
		(*uniform_values)["u_levelScaleFactor"] = new UniformValue<vec2>(vec2(
			level_scale_factor, 
			level_scale_factor),
			UniformType::FloatVector2);

		// Compute the level offset.
		// Is the real-world space between post at the coarsest level.
		// Multiplying these two values gives the real-world space between post at the current level.
		double level_offset_longitude = static_cast<double>(west)  - level->get_terrain()->longitude_to_index(0.0);
		double level_offset_latitude  = static_cast<double>(south) - level->get_terrain()->latitude_to_index(0.0);
		(*uniform_values)["u_levelOffsetFromWorldOrigin"] = new UniformValue<vec2>(vec2(
			level_offset_longitude,
			level_offset_latitude),
			UniformType::FloatVector2);

		int coarser_west  = coarser_level->get_current_extent().west;
		int coarser_south = coarser_level->get_current_extent().south;
		(*uniform_values)["u_fineLevelOriginInCoarse"] = new UniformValue<vec2>(
			to_vec2(coarser_level->get_origin_in_textures()) + vec2(
			static_cast<double>(west)  / 2.0 - coarser_west  + 0.5,
			static_cast<double>(south) / 2.0 - coarser_south + 0.5),
			UniformType::FloatVector2);

		(*uniform_values)["u_viewPosInClippedLevel"] = new UniformValue<vec2>(vec2(
			level->get_terrain()->longitude_to_index(ArgeoMath::to_degrees(m_clipmap_center.get_longitude())) - west,
			level->get_terrain()->latitude_to_index(ArgeoMath::to_degrees(m_clipmap_center.get_latitude()))   - south),
			UniformType::FloatVector2);

		(*uniform_values)["u_fineTextureOrigin"] = new UniformValue<vec2>(
			to_vec2(level->get_origin_in_textures()) + vec2(0.5f, 0.5f),
			UniformType::FloatVector2);

		(*uniform_values)["u_useBlendRegions"] = new UniformValue<bool>(
			level->get_coarser_level() != nullptr,
			UniformType::Bool);


        ////////////////////////////////////////////////////////////////////////////////////////////
		// SET IMAGERY SCALE RELATION
		////////////////////////////////////////////////////////////////////////////////////////////

		(*uniform_values)["u_terrainToImageryResolutionRatio"] = new UniformValue<vec2>(vec2(
			(float)(level->get_terrain()->get_post_delta_longitude() / level->get_imagery()->get_post_delta_longitude()),
			(float)(level->get_terrain()->get_post_delta_latitude() / level->get_imagery()->get_post_delta_latitude())),
			UniformType::FloatVector2);

		double terrain_south       = level->get_terrain()->index_to_latitude(south);
		double imagery_south_index = level->get_imagery()->latitude_to_index(terrain_south);

		vec2i terrain_offset_in_imagery =
			level->get_origin_in_textures() +
			vec2i(imagery_south_index - level->get_current_imagery_extent().west,
				  imagery_south_index - level->get_current_imagery_extent().south);

		(*uniform_values)["u_terrainOffsetInImagery"] = new UniformValue<vec2>(
			to_vec2(terrain_offset_in_imagery),
			UniformType::FloatVector2);

		(*uniform_values)["u_oneOverImagerySize"] = new UniformValue<vec2>(vec2(
			1.0 / level->get_imagery_width(),
			1.0 / level->get_imagery_height()),
			UniformType::FloatVector2);

		(*uniform_values)["u_maxMinHeights"] = new UniformValue<vec2>(vec2(
			m_raster_source->get_maximum_height(),
			m_raster_source->get_minimum_height()),
			UniformType::FloatVector2);

		////////////////////////////////////////////////////////////////////////////////////////////
		// SET PATCH UNIFORMS
		////////////////////////////////////////////////////////////////////////////////////////////

		(*uniform_values)["u_patchOriginInClippedLevel"] = new UniformValue<vec2>(vec2(
			texture_west,
			texture_south),
			UniformType::FloatVector2);

        // Only used for Mercator projections.

        double south_latitude = 0.0;
        double north_latitude = 0.0;
        double south_mercator_y = 0.0;
        double one_over_mercator_height = 0.0;

        bool   use_web_mercator_projection = false;

        GeodeticExtent block_extent = get_block_geodetic_extent(level_index, block_index);
        vec3d block_center          = m_ellipsoid.to_geocentric3D(block_extent.get_center()).to_vec3d();
        vec3d rtc                   = block_center;
        vec4d tile_rectangle;

        if (frame_state->get_mode() != SceneMode::Scene3D)
        {
            MapProjection* projection = frame_state->get_map_projection();

            Geocentric3D southwest = projection->project(block_extent.get_south_west());
            Geocentric3D northeast = projection->project(block_extent.get_north_east());

            tile_rectangle = vec4d(
                    southwest.get_x(),
                    southwest.get_y(),
                    northeast.get_x(),
                    northeast.get_y());

            // In 2D and Columbus View, use the center of the tile for RTC rendering.
            if (frame_state->get_mode() != SceneMode::Morphing)
            {
                rtc = vec3d(
                        0.0,
                        (tile_rectangle.z + tile_rectangle.x) * 0.5,
                        (tile_rectangle.w + tile_rectangle.y) * 0.5);

                tile_rectangle.x -= rtc.y;
                tile_rectangle.y -= rtc.z;
                tile_rectangle.z -= rtc.y;
                tile_rectangle.w -= rtc.z;
            }
        }

        (*uniform_values)["u_tileRectangle"] = new UniformValue<vec4>(
                to_vec4(tile_rectangle),
                UniformType::FloatVector4);

        evec3d emulated_block_center(block_center);
        (*uniform_values)["u_center3DHigh"] = new UniformValue<vec3>(
                emulated_block_center.high,
                UniformType::FloatVector3);

        (*uniform_values)["u_center3DLow"] = new UniformValue<vec3>(
                emulated_block_center.low,
                UniformType::FloatVector3);

        (*uniform_values)["u_center3D"] = new UniformValue<vec3>(
                to_vec3(block_center),
                UniformType::FloatVector3);

        Context* context        = m_scene->get_context();
        mat4d view_matrix       = context->get_uniform_state()->get_view_matrix();
        mat4d projection_matrix = context->get_uniform_state()->get_projection_matrix();

        vec3d center_eye = mat_multiply_by_point(view_matrix, rtc);
        const mat4d modified_model_view_matrix = mat_set_translation(view_matrix, center_eye);
        const mat4d modified_model_view_projection_matrix = projection_matrix * modified_model_view_matrix;

        (*uniform_values)["u_modifiedModelViewMatrix"] = new UniformValue<mat4>(
                to_mat4(modified_model_view_matrix),
                UniformType::FloatMatrix44);

        (*uniform_values)["u_modifiedModelViewProjectionMatrix"] = new UniformValue<mat4>(
                to_mat4(modified_model_view_projection_matrix),
                UniformType::FloatMatrix44);

        (*uniform_values)["u_southAndNorthLatitude"] = new UniformValue<vec2>(
                vec2((float) south_latitude, (float) north_latitude),
                UniformType::FloatVector2);

        (*uniform_values)["u_southAndNorthLatitude"] = new UniformValue<vec2>(
                vec2((float) south_latitude, (float) north_latitude),
                UniformType::FloatVector2);

        (*uniform_values)["u_southAndNorthLatitude"] = new UniformValue<vec2>(
                vec2((float) south_latitude, (float) north_latitude),
                UniformType::FloatVector2);

        (*uniform_values)["u_southMercatorYAndOneOverHeight"] = new UniformValue<vec2>(
                vec2((float) south_mercator_y, (float) one_over_mercator_height),
                UniformType::FloatVector2);
	}

	void ClipmapTerrain::generate_command_queue(FrameState* frame_state)
	{
		m_render_resources.primitive_type = m_wireframe ?
			PrimitiveType::Lines :
			PrimitiveType::Triangles;

		int max_level = m_clipmap_levels.size() - 1;

        if (m_mode != frame_state->get_mode())
        {
            for (auto entry : m_block_bounding_volume_dirty)
            {
                m_block_bounding_volume_dirty[entry.first] = true;
            }
        }

		bool rendered = false;
		for (int i = max_level; i >= 0; --i)
		{
			ClipmapLevel* this_level = m_clipmap_levels[i].get();
			rendered = generate_level_command_queue(
				i, 
				this_level, 
				!rendered,
				frame_state);
		}

        m_mode = frame_state->get_mode();
	}

	void ClipmapTerrain::pre_render(FrameState* frame_state)
	{
		// Get the new clipmap center.
		m_clipmap_center = frame_state->get_camera()->get_position_geodetic();

		////////////////////////////////////////////////////////////////////////////////////////////
		// UPDATE FINEST LEVEL
		////////////////////////////////////////////////////////////////////////////////////////////

		// The extent of the finest level within the terrain is computed such that is approximately centered around the view. 
		
		Geodetic2D center(m_clipmap_center.get_longitude(), m_clipmap_center.get_latitude());
		double center_longitude = ArgeoMath::to_degrees(center.get_longitude());
		double center_latitude  = ArgeoMath::to_degrees(center.get_latitude());

		// Get the finest level that will be centered within the view.
		ClipmapLevel* level			   = m_clipmap_levels[m_clipmap_levels.size() - 1].get();
		double longitude_index		   = level->get_terrain()->longitude_to_index(center_longitude);
		double latitude_index		   = level->get_terrain()->latitude_to_index(center_latitude);
		double imagery_longitude_index = level->get_imagery()->longitude_to_index(center_longitude);
		double imagery_latitude_index  = level->get_imagery()->latitude_to_index(center_latitude);

		int32_t west = std::ceil(longitude_index - m_clipmap_posts / 2);
		if ((west % 2) != 0)
		{
			++west;
		}

		int32_t south = std::ceil(latitude_index - m_clipmap_posts / 2);
		if ((south % 2) != 0)
		{
			++south;
		}

		int32_t imagery_west  = std::ceil(imagery_longitude_index - level->get_imagery_width() / 2);
		int32_t imagery_south = std::ceil(imagery_latitude_index - level->get_imagery_height() / 2);

		// Now set the new view extents.
		level->set_next_extent(Extent(
			west,
			south,
			west + m_clipmap_segments,
			south + m_clipmap_segments));

		level->set_next_imagery_extent(Extent(
			imagery_west,
			imagery_south,
			imagery_west + level->get_imagery_width(),
			imagery_south + level->get_imagery_height()));


		// Update the origin coordinate to use toroidal addressing.
		update_origin_in_textures(level);
		update_imagery_origin_in_textures(level);

		////////////////////////////////////////////////////////////////////////////////////////////
		// UPDATE COARSER LEVELS
		////////////////////////////////////////////////////////////////////////////////////////////

		// Update the coarser levels with the new finer level state.
		// The extent of the coarser level is computed from the extent of the finer level that it encloses.
		for (int i = m_clipmap_levels.size() - 2; i >= 0; --i)
		{
			level = m_clipmap_levels[i].get();
			ClipmapLevel* finer_level = m_clipmap_levels[i + 1].get();

			// Set the new terrain extent.
			Extent next_extent;

			next_extent.west = finer_level->get_next_extent().west / 2 - m_fill_patch_segments;
			level->set_offset_strip_on_east((next_extent.west % 2) == 0);
			if (!level->get_offset_strip_on_east())
			{
				--next_extent.west;
			}
			next_extent.east = next_extent.west + m_clipmap_segments;

			next_extent.south = finer_level->get_next_extent().south / 2 - m_fill_patch_segments;
			level->set_offset_strip_on_north((next_extent.south % 2) == 0);
			if (!level->get_offset_strip_on_north())
			{
				--next_extent.south;
			}
			next_extent.north = next_extent.south + m_clipmap_segments;

			level->set_next_extent(next_extent);

			// Imagery.
			imagery_west  = std::ceil(level->get_imagery()->longitude_to_index(level->get_terrain()->index_to_longitude(level->get_next_extent().west)));
			imagery_south = std::ceil(level->get_imagery()->latitude_to_index(level->get_terrain()->index_to_latitude(level->get_next_extent().south)));

			level->set_next_imagery_extent(Extent(
				imagery_west,
				imagery_south,
				imagery_west + level->get_imagery_width() - 1,
				imagery_south + level->get_imagery_height() - 1));

			// Update the origin coordinate to use toroidal addressing.
			update_origin_in_textures(level);
			update_imagery_origin_in_textures(level);
		}

		////////////////////////////////////////////////////////////////////////////////////////////
		// APPLY NEW DATA
		////////////////////////////////////////////////////////////////////////////////////////////

		m_updater->apply_new_data(frame_state);

		////////////////////////////////////////////////////////////////////////////////////////////
		// REQUEST TILE RESIDENCY AND UPDATE TERRAIN AND IMAGERY FOR EACH LEVEL
		////////////////////////////////////////////////////////////////////////////////////////////

		for (unsigned int i = 0; i < m_clipmap_levels.size(); ++i)
		{
			ClipmapLevel* this_level = m_clipmap_levels[i].get();

			m_updater->request_tile_residency(frame_state, this_level);

			update_terrain(i, frame_state);
			update_imagery(i, frame_state);
		}
	}


	bool ClipmapTerrain::generate_level_command_queue(
		int level_index,
		ClipmapLevel* level,
		bool fill_ring,
		FrameState* frame_state)
	{
		// TODO: check active regions
        // Specifically, we deactivate levels for which the grid extent is smaller than 2.5h,
        // where h is the viewer height above the terrain.
        const double h =
                frame_state->get_camera()->get_position_geodetic().get_height() -
                level->get_maximum_height();

        const double width = level->get_width();

		if (0.25 * h > width)
		{
            return false;
		}

		////////////////////////////////////////////////////////////////////////////////////////////
		// DRAW THE LEVEL BLOCKS
		////////////////////////////////////////////////////////////////////////////////////////////

		int west  = level->get_current_extent().west;
		int south = level->get_current_extent().south;
		int east  = level->get_current_extent().east;
		int north = level->get_current_extent().north;


		////////////////////////////////////////////////////////////////////////////////////////////
		//									M x M BLOCKS
		////////////////////////////////////////////////////////////////////////////////////////////

		draw_block(level_index, (unsigned int) BlockIndex::FillBlock0, west, south, west, south, frame_state);
		draw_block(level_index, (unsigned int) BlockIndex::FillBlock1, west, south, west + m_fill_patch_segments, south, frame_state);
		draw_block(level_index, (unsigned int) BlockIndex::FillBlock2, west, south, east - 2 * m_fill_patch_segments, south, frame_state);
		draw_block(level_index, (unsigned int) BlockIndex::FillBlock3, west, south, east - m_fill_patch_segments, south, frame_state);
		
		
		draw_block(level_index, (unsigned int) BlockIndex::FillBlock4, west, south, west, south + m_fill_patch_segments, frame_state);
		draw_block(level_index, (unsigned int) BlockIndex::FillBlock5, west, south, east - m_fill_patch_segments, south + m_fill_patch_segments, frame_state);
		draw_block(level_index, (unsigned int) BlockIndex::FillBlock6, west, south, west, north - 2 * m_fill_patch_segments, frame_state);
		draw_block(level_index, (unsigned int) BlockIndex::FillBlock7, west, south, east - m_fill_patch_segments, north - 2 * m_fill_patch_segments, frame_state);
		
		
		draw_block(level_index, (unsigned int) BlockIndex::FillBlock8, west, south, west, north - m_fill_patch_segments, frame_state);
		draw_block(level_index, (unsigned int) BlockIndex::FillBlock9, west, south, west + m_fill_patch_segments, north - m_fill_patch_segments, frame_state);
		draw_block(level_index, (unsigned int) BlockIndex::FillBlock10, west, south, east - 2 * m_fill_patch_segments, north - m_fill_patch_segments, frame_state);
		draw_block(level_index, (unsigned int) BlockIndex::FillBlock11, west, south, east - m_fill_patch_segments, north - m_fill_patch_segments, frame_state);

		////////////////////////////////////////////////////////////////////////////////////////////
		//									FIX-UP BLOCKS
		////////////////////////////////////////////////////////////////////////////////////////////


		draw_block(level_index, (unsigned int) BlockIndex::HorizontalFixup0, west, south, west, south + 2 * m_fill_patch_segments, frame_state);
		draw_block(level_index, (unsigned int) BlockIndex::HorizontalFixup1, west, south, east - m_fill_patch_segments, south + 2 * m_fill_patch_segments, frame_state);
		
		draw_block(level_index, (unsigned int) BlockIndex::VerticalFixup0, west, south, west + 2 * m_fill_patch_segments, south, frame_state);
		draw_block(level_index, (unsigned int) BlockIndex::VerticalFixup1, west, south, west + 2 * m_fill_patch_segments, north - m_fill_patch_segments, frame_state);

		////////////////////////////////////////////////////////////////////////////////////////////
		//									DEGENERATE TRIANGLES
		////////////////////////////////////////////////////////////////////////////////////////////

		draw_block(level_index, (unsigned int)BlockIndex::DegenerateTrianglesOuterRing, west, south, west, south, frame_state);


		// Fill the center of the highest-detail ring
		if (fill_ring)
		{
			draw_block(level_index, (unsigned int) BlockIndex::CenterFillBlock0, west, south, west + m_fill_patch_segments, south + m_fill_patch_segments, frame_state);
			draw_block(level_index, (unsigned int) BlockIndex::CenterFillBlock1, west, south, west + 2 * m_fill_patch_posts, south + m_fill_patch_segments, frame_state);
			draw_block(level_index, (unsigned int) BlockIndex::CenterFillBlock2, west, south, west + m_fill_patch_segments, south + 2 * m_fill_patch_posts, frame_state);
			draw_block(level_index, (unsigned int) BlockIndex::CenterFillBlock3, west, south, west + 2 * m_fill_patch_posts, south + 2 * m_fill_patch_posts, frame_state);

			draw_block(level_index, (unsigned int) BlockIndex::CenterHorizontalFixup0, west, south, west + m_fill_patch_segments, south + 2 * m_fill_patch_segments, frame_state);
			draw_block(level_index, (unsigned int) BlockIndex::CenterHorizontalFixup1, west, south, west + 2 * m_fill_patch_posts, south + 2 * m_fill_patch_segments, frame_state);

			draw_block(level_index, (unsigned int) BlockIndex::CenterVerticalFixup0, west, south, west + 2 * m_fill_patch_segments, south + m_fill_patch_segments, frame_state);
			draw_block(level_index, (unsigned int) BlockIndex::CenterVerticalFixup1, west, south, west + 2 * m_fill_patch_segments, south + 2 * m_fill_patch_posts, frame_state);

			draw_block(level_index, (unsigned int) BlockIndex::CenterBlock, west, south, west + 2 * m_fill_patch_segments, south + 2 * m_fill_patch_segments, frame_state);
		}
		else
		{
			////////////////////////////////////////////////////////////////////////////////////////////
			//									INTERIOR TRIM BLOCKS
			////////////////////////////////////////////////////////////////////////////////////////////

			int offset = level->get_offset_strip_on_north()
				? north - m_fill_patch_posts
				: south + m_fill_patch_segments;
			draw_block(level_index, (unsigned int)BlockIndex::HorizontalInteriorTrim, west, south, west + m_fill_patch_segments, offset, frame_state);

			int south_offset = level->get_offset_strip_on_north() ? 0 : 1;
			offset = level->get_offset_strip_on_east()
				? east - m_fill_patch_posts
				: west + m_fill_patch_segments;
			draw_block(level_index, (unsigned int)BlockIndex::VerticalInteriorTrim, west, south, offset, south + m_fill_patch_segments + south_offset, frame_state);
		}

		return true;
	}

	void ClipmapTerrain::draw_block(
        const unsigned int& level_index,
        const unsigned int& block_index,
        const unsigned int& overall_west,
        const unsigned int& overall_south,
        const unsigned int& block_west,
        const unsigned int& block_south,
		FrameState* frame_state)
	{
		std::stringstream id_stream;
		id_stream << level_index << "." << block_index;
		std::string id = id_stream.str();

		// Update the uniform values for this block.
		update_uniform_values(
			level_index,
			block_index,
			block_west,
            block_south,
			overall_west,
            overall_south,
			frame_state);
		
		if (m_block_bounding_volume_dirty[id])
		{
			update_bounding_volume(
				level_index,
				block_index,
                frame_state);
            m_block_bounding_volume_dirty[id] = false;
		}

		DrawCommand* draw_command = m_block_commands[id];

        draw_command->set_shader_program(m_shader_set->get_shader_program(this, frame_state, m_use_double_precision));
		draw_command->set_primitive_type(m_render_resources.primitive_type);

		frame_state->get_command_queue().push_back(draw_command);
	}

    void ClipmapTerrain::set_use_double_precision(const bool &value)
    {
        m_use_double_precision = value;
    }

    bool ClipmapTerrain::get_use_double_precision()
    {
        return m_use_double_precision;
    }

     TerrainShader* ClipmapTerrain::get_shader_program()
     {
         return m_render_resources.shader_program;
     }

     void ClipmapTerrain::set_shader_program(TerrainShader* value)
     {
         m_render_resources.shader_program = value;
     }

    void ClipmapTerrain::set_depth_test_against(const bool &value)
    {
        m_depth_test_against = value;
    }

    bool ClipmapTerrain::get_depth_test_against() const
    {
        return m_depth_test_against;
    }

	bool ClipmapTerrain::get_wireframe() const
	{
		return m_wireframe;
	}

	void ClipmapTerrain::set_wireframe(bool value)
	{
		m_wireframe = value;
	}

	bool ClipmapTerrain::get_show() const
	{
		return m_show;
	}

	void ClipmapTerrain::set_show(bool value)
	{
		m_show = value;
	}

	bool ClipmapTerrain::get_show_imagery() const
	{
		return m_show_imagery;
	}

	void ClipmapTerrain::set_show_imagery(bool value)
	{
		m_show_imagery = value;
	}

	bool ClipmapTerrain::get_lighting() const
	{
		return m_lighting;
	}

	void ClipmapTerrain::set_lighting(bool value)
	{
		m_lighting = value;
	}

	double ClipmapTerrain::get_height_exaggeration() const
	{
		return m_height_exaggeration;
	}
	
	void ClipmapTerrain::set_height_exaggeration(double value)
	{
		if (m_height_exaggeration != value)
		{
			m_height_exaggeration = value;
			m_height_exaggeration_dirty = true;
		}
	}

	Ellipsoid ClipmapTerrain::get_ellipsoid() const
	{
		return m_ellipsoid; 
	}
	
	void ClipmapTerrain::set_ellipsoid(const Ellipsoid& value)
	{
		m_ellipsoid = value;
	}

	Material* ClipmapTerrain::get_material()
	{
		return m_material.get();
	}

	double ClipmapTerrain::get_point_height(const Geodetic2D& point) const
	{
		double height = 0;

		double center_longitude = ArgeoMath::to_degrees(point.get_longitude());
		double center_latitude  = ArgeoMath::to_degrees(point.get_latitude());

		unsigned int max_level = m_clipmap_levels.size() - 1;
		for (int i = max_level; i >= 0; --i)
		{
			ClipmapLevel* level    = m_clipmap_levels[i].get();

			double longitude_index = level->get_terrain()->longitude_to_index(center_longitude);
			double latitude_index  = level->get_terrain()->latitude_to_index(center_latitude);

			Extent level_extent	   = level->get_current_extent();

			if (level_extent.south < latitude_index   && level_extent.north > latitude_index &&
				level_extent.west  < longitude_index  && level_extent.east  > longitude_index)
			{
				vec2i fine_texture_origin = level->get_origin_in_textures();
				const Texture2D* texture  = level->get_height_texture();

				const double column = longitude_index - level_extent.west   + fine_texture_origin.x;
				const double row	= latitude_index  - level_extent.south  + fine_texture_origin.y;
				
				height = get_point_height(
					column,
					row,
					m_raster_source->get_maximum_height(),
					m_raster_source->get_minimum_height(),
					texture) *
					m_height_exaggeration;

				break;
			}
		}

		return height;
	}

	// http://gamedev.stackexchange.com/questions/24572/how-does-terrain-following-work-on-height-map
	double ClipmapTerrain::get_point_height(
		double column,
		double row,
		double max,
		double min,
		const Texture2D* texture) const
	{
		// we first get the height of four points of the quad underneath the point
		// Check to make sure this point is not off the map at all
		int x = static_cast<int>(column) % m_clipmap_posts;
		int z = static_cast<int>(row)    % m_clipmap_posts;

        std::unique_ptr<FrameBuffer> frame_buffer = m_scene->get_context()->create_frame_buffer();
        frame_buffer->get_color_attachments()->set_attachment(0, texture);
        const std::vector<vec4ub> quad = m_scene->get_context()->read_pixels(
                x,
                z,
                2,
                2,
                frame_buffer.get());

        double triZ0 = ArgeoMath::denormalize_float(
                ArgeoMath::float_from_vec4(quad[0]),
                max,
                min);

        double triZ1 = ArgeoMath::denormalize_float(
                ArgeoMath::float_from_vec4(quad[1]),
                max,
                min);

        double triZ2 = ArgeoMath::denormalize_float(
                ArgeoMath::float_from_vec4(quad[2]),
                max,
                min);

        double triZ3 = ArgeoMath::denormalize_float(
                ArgeoMath::float_from_vec4(quad[3]),
                max,
                min);

		double height = 0.0;
		double sqX    = column - static_cast<int>(column);
		double sqZ    = row    - static_cast<int>(row);
		
		if ((sqX + sqZ) < 1)
		{
			height  = triZ0;
			height += (triZ1 - triZ0) * sqX;
			height += (triZ2 - triZ0) * sqZ;
		}
		else
		{
			height  = triZ3;
			height += (triZ1 - triZ3) * (1.0f - sqZ);
			height += (triZ2 - triZ3) * (1.0f - sqX);
		}

		return height;
	}
}