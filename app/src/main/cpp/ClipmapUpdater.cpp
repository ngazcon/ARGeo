#include "ClipmapUpdater.h"
#include "RectangleTessellator.h"

#include "ClipmapComputeNormalsShaders.h"
#include "ClipmapUpdateShaders.h"
#include "ClipmapUpsampleShaders.h"
#include "Device.h"
#include "RasterSource.h"

namespace argeo
{
	ClipmapUpdater::ClipmapUpdater(
		Context* context,
		std::vector<ClipmapLevel*> clipmap_levels)
		: m_terrain(new RasterDataDetails(RasterType::Terrain))
		, m_imagery(new RasterDataDetails(RasterType::Imagery))
		, m_pass_state(context)
	{
		///////////////////////////////////////////////////////////////////////////////////
		// Unit quad mesh to render into the frame buffer
		///////////////////////////////////////////////////////////////////////////////////

		ShaderVertexAttributeCollection* vertex_attributes = new ShaderVertexAttributeCollection();
		(*vertex_attributes)["position"] = new ShaderVertexAttribute(
			"position",
			0,
			1, 
			ShaderVertexAttributeType::FloatVector2);

		std::unique_ptr<Mesh> unit_quad =
			RectangleTessellator::compute(RectangleD(vec2d(0.0, 0.0), vec2d(1.0, 1.0)), 1, 1);

		m_unit_quad = context->create_vertex_array(
			unit_quad.get(),
			vertex_attributes, 
			BufferHint::StaticDraw);

		///////////////////////////////////////////////////////////////////////////////////
		// PASS STATE
		///////////////////////////////////////////////////////////////////////////////////
		
		m_framebuffer = context->create_frame_buffer();
		m_pass_state.set_framebuffer(m_framebuffer.get());

		///////////////////////////////////////////////////////////////////////////////////
		// RENDER STATE
		///////////////////////////////////////////////////////////////////////////////////

		RenderStateParameters parameters;
		parameters.facet_culling = FacetCulling(
			true,
			CullFace::Back,
			unit_quad->get_front_face_winding_order());
		m_render_state = RenderState::from_cache(parameters);
		
		///////////////////////////////////////////////////////////////////////////////////
		// UPDATE SHADER
		///////////////////////////////////////////////////////////////////////////////////

		m_update_shader = Device::create_shader_program(
			clipmap_update_vs,
			clipmap_update_fs);
		m_update_texel_output = m_update_shader->get_fragments_output("texelOutput");

		m_update_draw_command = std::unique_ptr<DrawCommand>(new DrawCommand(
			unit_quad->get_primitive_type(),
			m_unit_quad.get(),
			m_update_shader.get(),			
			m_render_state,
			m_framebuffer.get()));

		
		m_update_max_min_heights		  = (Uniform<vec2>*)m_update_shader->get_uniform("u_maxMinHeights");
		m_update_destination_offset       = (Uniform<vec2>*)m_update_shader->get_uniform("u_destinationOffset");
		m_update_update_size              = (Uniform<vec2>*)m_update_shader->get_uniform("u_updateSize");
		m_update_source_origin            = (Uniform<vec2>*)m_update_shader->get_uniform("u_sourceOrigin");
		m_update_one_over_texture_size    = (Uniform<vec2>*)m_update_shader->get_uniform("u_oneOverTextureSize");
		m_update_texture_sampler		  = (UniformSampler*)m_update_shader->get_uniform("u_heightMapSampler");

		///////////////////////////////////////////////////////////////////////////////////
		// Upsample shader
		///////////////////////////////////////////////////////////////////////////////////

		m_upsample_shader = Device::create_shader_program(
			clipmap_upsample_vs,
			clipmap_upsample_fs);
		m_upsample_texel_output = m_upsample_shader->get_fragments_output("texelOutput");

		m_upsample_draw_command = std::unique_ptr<DrawCommand>(new DrawCommand(
			unit_quad->get_primitive_type(),
			m_unit_quad.get(),
			m_upsample_shader.get(),
			m_render_state,
			m_framebuffer.get()));

		m_upsample_source_origin		  = (Uniform<vec2>*)m_upsample_shader->get_uniform("u_sourceOrigin");
		m_upsample_update_size			  = (Uniform<vec2>*)m_upsample_shader->get_uniform("u_updateSize");
		m_upsample_destination_offset     = (Uniform<vec2>*)m_upsample_shader->get_uniform("u_destinationOffset");
		m_upsample_one_over_texture_size  = (Uniform<vec2>*)m_upsample_shader->get_uniform("u_oneOverTextureSize");
		m_upsample_texture_sampler		  = (UniformSampler*)m_upsample_shader->get_uniform("u_heightMapSampler");

		///////////////////////////////////////////////////////////////////////////////////
		// Normal compute shader
		///////////////////////////////////////////////////////////////////////////////////

		m_compute_normals_shader = Device::create_shader_program(
			clipmap_compute_normals_vs,
			clipmap_compute_normals_fs);

        m_normal_output = m_compute_normals_shader->get_fragments_output("normalOutput");

		m_compute_normals_draw_command = std::unique_ptr<DrawCommand>(new DrawCommand(
			unit_quad->get_primitive_type(),
			m_unit_quad.get(),
			m_compute_normals_shader.get(),			
			m_render_state,
			m_framebuffer.get()));

		m_compute_normals_origin				   = (Uniform<vec2>*)m_compute_normals_shader->get_uniform("u_origin");
		m_compute_normals_update_size			   = (Uniform<vec2>*)m_compute_normals_shader->get_uniform("u_updateSize");
		m_compute_normals_max_min_heights          = (Uniform<vec2>*)m_compute_normals_shader->get_uniform("u_maxMinHeights");

		m_compute_normals_one_over_height_map_size = (Uniform<vec2>*)m_compute_normals_shader->get_uniform("u_oneOverHeightMapSize");
		m_compute_normals_texture_sampler		   = (UniformSampler*)m_compute_normals_shader->get_uniform("u_heightMapSampler");
		m_height_exaggeration = (Uniform<float>*)m_compute_normals_shader->get_uniform("u_heightExaggeration");
		m_post_delta          = (Uniform<float>*)m_compute_normals_shader->get_uniform("u_postDelta");

		m_height_exaggeration->set_value(1.0);

		ClipmapLevel* level_zero = clipmap_levels[0];
		initialize_request_threads(context, m_terrain.get(), level_zero, level_zero->get_terrain());
		//initialize_request_threads(context, m_imagery.get(), level_zero, level_zero->get_imagery());
	}

	ClipmapUpdater::~ClipmapUpdater()
	{ }

	void ClipmapUpdater::request_thread_entry_point(
		RasterDataDetails* details)
	{
		details->worker_window->get_context()->attach();

		while (true)
		{
			TileLoadRequest* request = nullptr;
			{
				// Lock Request List
				std::unique_lock<std::mutex> request_list_lock(details->request_list_mutex);

				std::list<TileLoadRequest*>::iterator last_node;

				if (details->request_list.size() == 0)
				{
					details->request_list_condition.wait(request_list_lock);
				}
				else
				{
					std::unique_lock<std::mutex> loading_tiles_lock(details->loading_tiles_mutex);

					// Get the last node of the request list.
					last_node = std::prev(details->request_list.end());

					request = *last_node;
					remove_request(last_node, details);
				}
			} // UnLock Request List


			if (request != nullptr)
			{
				const RasterTile* tile			= request->tile;
				const RasterSourceTileData data = tile->load_data();
				RasterSourceTileData* non_const_data = const_cast<RasterSourceTileData*>(&data);

				request->texture = std::move(non_const_data->texture);
				request->maximum = non_const_data->maximum;
				request->minimum = non_const_data->minimum;

				std::unique_ptr<Fence> fence(Device::create_fence());
				ClientWaitResult result = fence->client_wait();

				// Notify the raster source that new data is available.
				if (result == ClientWaitResult::Signaled || 
					result == ClientWaitResult::AlreadySignaled) 
				{
					tile->on_changed();
					details->done_queue->post(request);
				}				
			}
		}
	}

	void ClipmapUpdater::remove_request(std::list<TileLoadRequest*>::iterator request_iterator, RasterDataDetails* details)
	{
		// We have to remove the request from the loading tiles because the iterator is invalidated.
		{
			// First check if we are removing the insertion point, if so then invalidate the iterator.
			if (details->request_insertion_point == request_iterator)
			{
				details->request_insertion_point = details->request_list_empty_iterator;
			}

			// Mark the tile as loaded.
			TileLoadRequest* request = *request_iterator;

			details->loading_tiles[request->tile->get_identifier()] =
				details->request_list_empty_iterator;

			// Remove the tile from the request list.
			details->request_list.erase(request_iterator);
		}
	}

	void ClipmapUpdater::initialize_request_threads(
		Context* context,
		RasterDataDetails* details,
		ClipmapLevel * clipmap_level_zero,
		RasterLevel * raster_level_zero)
	{
		details->worker_window = Device::create_pixel_buffer_surface(1, 1);
		context->attach();
		std::thread thread(&ClipmapUpdater::request_thread_entry_point, this, details);
		thread.detach();
	}


	void ClipmapUpdater::apply_new_data(FrameState* frame_state)
	{
		apply_new_data(frame_state, m_terrain.get());
		//apply_new_data(frame_state, m_imagery.get());
	}

	void ClipmapUpdater::apply_new_data(
		FrameState* frame_state,
		RasterDataDetails* details)
	{
		// This is the start of a new frame, so the next request goes at the end
		details->request_insertion_point = details->request_list_empty_iterator;

		std::vector<TileLoadRequest*> tiles;

		class Handler : public MessageQueueListener
		{
		public:
			Handler(
				std::vector<TileLoadRequest*>& tiles,
				RasterDataDetails* details)
				: tiles(tiles)
				, details(details)
			{ }

			inline void on_received(MessageQueueEventArgs args)
			{
				TileLoadRequest* request = reinterpret_cast<TileLoadRequest*>(args.message);
				details->loaded_tiles[request->tile->get_identifier()] = request->texture.get();
				details->loading_tiles.erase(request->tile->get_identifier());
				tiles.push_back(request);
			}

		private:
			RasterDataDetails* details;
			std::vector<TileLoadRequest*>& tiles;

		} handler(tiles, details);

		details->done_queue->add_listener(&handler);
 		details->done_queue->process_queue();
		details->done_queue->remove_listener(&handler);

		for (TileLoadRequest* request : tiles)
		{
			ClipmapLevel* level = request->level;
			RasterTile* tile    = request->tile;

			if (details->type == RasterType::Terrain)
			{
                tile->set_maximum_height(
                        ::fmax(tile->get_maximum_height(), request->maximum));
                tile->set_minimum_height(
                        ::fmin(tile->get_minimum_height(), request->minimum));

				level->set_maximum_height(
					::fmax(level->get_maximum_height(), request->maximum));
				level->set_minimum_height(
					::fmin(level->get_minimum_height(), request->minimum));
			}

			apply_new_tile(frame_state, details, request->level, request->tile);
		}
	}

	// Loading the tiles in the order they are requested is not a great strategy.
	// When the viewer is moving quickly and generating a lot of cache misses, the worker thread is simply unable to keep up.
	// By the time an tile is loaded, the viewer may have moved such that the tile is not even visible anymore.
	// The strategy is to firt load the tile that was requested most recently and work backward in time from there.
	void ClipmapUpdater::request_tile_load(
		RasterDataDetails* details,
		ClipmapLevel * level,
		RasterTile * tile)
	{
		std::list<TileLoadRequest*>::iterator request_node;
		TileLoadRequest* request = nullptr;

		bool exists = false;


		{ // Lock list mutex
			std::unique_lock<std::mutex> request_list_lock(details->request_list_mutex);

			{
				std::unique_lock<std::mutex> loading_tiles_lock(details->loading_tiles_mutex);

				// Try to find the request node in the loading tiles.
				std::unordered_map<RasterTileIdentifier, std::list<TileLoadRequest*>::iterator, RasterTileIdentifierHasher>::const_iterator loading_node_itr =
					details->loading_tiles.find(tile->get_identifier());

				exists = loading_node_itr != details->loading_tiles.end();

				if (!exists)
				{
					// Create a new request.
					request = new TileLoadRequest();
					request->level = level;
					request->tile = tile;

					// Get the final position of the request list.
					request_node = details->request_list.end();
				}
				else
				{
					if (loading_node_itr->second == details->request_list_empty_iterator)
					{
						// Request was in the queue at one point, but it's not anymore.
						// That means it's been loaded,  so we don't need to do anything.
						return;
					}

					// The node is in the loading tiles list.
					request_node = loading_node_itr->second;
					request = *request_node;

					// Remove the request from the position.
					remove_request(request_node, details);

					// Now add to the last position.
					request_node = details->request_list.end();
				}
			}

			if (details->request_insertion_point == details->request_list_empty_iterator)
			{
				// Set the new insertion point.
				request_node =
					details->request_list.insert(
						details->request_list.end(),
						request);
			}
			else
			{
				// Inserts before request insertion point.
				request_node =
					details->request_list.insert(
						details->request_insertion_point,
						request);
			}

			// Set the new insertion point.
			details->request_insertion_point = request_node;

			// If the request list has too many entries, delete from the beginning
			{
				std::unique_lock<std::mutex> lock(details->loading_tiles_mutex);

				// TODO: ACHICAR ESTO
				const int max_requests = 20;
				while (details->request_list.size() > max_requests)
				{
                    std::list<TileLoadRequest*>::iterator node_to_remove = std::next(details->request_list.begin());

                    // First check if we are removing the insertion point, if so then invalidate the iterator.
                    if (details->request_insertion_point == node_to_remove)
                    {
                        details->request_insertion_point = details->request_list_empty_iterator;
                    }

                    // Remove the tile from the request list.
                    details->request_list.erase(node_to_remove);
                    details->loading_tiles.erase((*node_to_remove)->tile->get_identifier());

					delete (*node_to_remove);
				}

				// Add the request to the loading tiles.
				details->loading_tiles[tile->get_identifier()] =
					request_node;
			}
		} // Unlock List mutex

		  // Notify the request thread.
		details->request_list_condition.notify_one();
	}

	void ClipmapUpdater::apply_if_not_loaded(
		FrameState* frame_state,
		RasterDataDetails * details,
		ClipmapLevel * level,
		RasterTile * tile)
	{
		std::unordered_map<RasterTileIdentifier, Texture2D*, RasterTileIdentifierHasher>::const_iterator itr =
			details->loaded_tiles.find(tile->get_identifier());

		bool exists = itr != details->loaded_tiles.end();

		if (!exists || itr->second == nullptr)
		{
			apply_new_tile(frame_state, details, level, tile);
		}
	}

	ClipmapUpdate ClipmapUpdater::intersect_updates(
		ClipmapUpdate& first,
		ClipmapUpdate& second)
	{
		int west = fmax(first.get_west(), second.get_west());
		int south = fmax(first.get_south(), second.get_south());
		int east = fmin(first.get_east(), second.get_east());
		int north = fmin(first.get_north(), second.get_north());

		return ClipmapUpdate(first.get_level(), west, south, east, north);
	}

	void ClipmapUpdater::apply_new_tile(
		FrameState* frame_state,
		RasterDataDetails * details,
		ClipmapLevel * level,
		RasterTile * tile)
	{
		Extent next_extent = details->type == RasterType::Terrain ?
			level->get_next_extent() :
			level->get_next_imagery_extent();

		RasterLevel* raster_level = details->type == RasterType::Terrain ?
			level->get_terrain() :
			level->get_imagery();

		ClipmapUpdate entire_level(
			level,
			next_extent.west,
			next_extent.south,
			next_extent.east,
			next_extent.north);

		ClipmapUpdate this_tile(
			level,
			tile->get_west()  - 1,
			tile->get_south() - 1,
			tile->get_east()  + 1,
			tile->get_north() + 1);

		ClipmapUpdate intersection =
			intersect_updates(entire_level, this_tile);

		
		if (intersection.get_width() > 0 && intersection.get_height() > 0)
		{
			update(frame_state, intersection, level, details, raster_level);

			// Recurse on child tiles if they're NOT loaded.
			// Unloaded children will use data from this tile.
			ClipmapLevel * finer = level->get_finer_level();

			if (finer != nullptr)
			{
				apply_if_not_loaded(frame_state, details, finer, tile->get_southwest_child());
				apply_if_not_loaded(frame_state, details, finer, tile->get_southeast_child());
				apply_if_not_loaded(frame_state, details, finer, tile->get_northwest_child());
				apply_if_not_loaded(frame_state, details, finer, tile->get_northeast_child());
			}
		}
	}

	std::vector<ClipmapUpdate> ClipmapUpdater::split_update_to_avoid_wrapping(
		ClipmapUpdate & update,
		RasterDataDetails * details)
	{
		ClipmapLevel * level = update.get_level();

		vec2i origin = details->type == RasterType::Terrain ?
			level->get_origin_in_textures() :
			level->get_origin_in_imagery();

		Extent extent = details->type == RasterType::Terrain ?
			level->get_next_extent() :
			level->get_next_imagery_extent();

		int clipmap_size_x = extent.east  - extent.west + 1;
		int clipmap_size_y = extent.north - extent.south + 1;

		int west  = (origin.x + (update.get_west()  - extent.west))  % clipmap_size_x;
		int east  = (origin.x + (update.get_east()  - extent.west))  % clipmap_size_x;
		int south = (origin.y + (update.get_south() - extent.south)) % clipmap_size_y;
		int north = (origin.y + (update.get_north() - extent.south)) % clipmap_size_y;

		std::vector<ClipmapUpdate> result;

		if (east < west && north < south)
		{
			// Horizontal AND vertical wrap
			ClipmapUpdate bottom_left_update(
				level,
				update.get_west(),
				update.get_south(),
				extent.west  + (clipmap_size_x - origin.x - 1),
				extent.south + (clipmap_size_y - origin.y - 1));

			ClipmapUpdate bottom_right_update(
				level,
				extent.west + clipmap_size_x - origin.x,
				update.get_south(),
				update.get_east(),
				extent.south + (clipmap_size_y - origin.y - 1));

			ClipmapUpdate top_left_update(
				level,
				update.get_west(),
				extent.south + clipmap_size_y - origin.y,
				extent.west  + (clipmap_size_x - origin.x - 1),
				update.get_north());

			ClipmapUpdate top_right_update(
				level,
				extent.west  + clipmap_size_x - origin.x,
				extent.south + clipmap_size_y - origin.y,
				update.get_east(),
				update.get_north());

			result.push_back(bottom_left_update);
			result.push_back(bottom_right_update);
			result.push_back(top_left_update);
			result.push_back(top_right_update);
		}
		else if (east < west)
		{
			// Horizontal wrap
			ClipmapUpdate left_update(
				level,
				update.get_west(),
				update.get_south(),
				extent.west + (clipmap_size_x - origin.x - 1),
				update.get_north());

			ClipmapUpdate right_update(
				level,
				extent.west + clipmap_size_x - origin.x,
				update.get_south(),
				update.get_east(),
				update.get_north());

			result.push_back(left_update);
			result.push_back(right_update);
		}
		else if (north < south)
		{
			// Vertical wrap
			ClipmapUpdate bottom_update(
				level,
				update.get_west(),
				update.get_south(),
				update.get_east(),
				extent.south + (clipmap_size_y - origin.y - 1));

			ClipmapUpdate top_update(
				level,
				update.get_west(),
				extent.south + clipmap_size_y - origin.y,
				update.get_east(),
				update.get_north());

			result.push_back(bottom_update);
			result.push_back(top_update);
		}
		else
		{
			// No wrap
			result.push_back(update);
		}

		return result;
	}

	void ClipmapUpdater::update(
		FrameState* frame_state,
		ClipmapUpdate & update,
		ClipmapLevel * level,
		RasterDataDetails * details,
		RasterLevel * raster_level)
	{
		std::vector<ClipmapUpdate> updates = split_update_to_avoid_wrapping(update, details);

		for (ClipmapUpdate non_wrapping_update : updates)
		{
			std::vector<RasterTileRegion> tile_regions = raster_level->get_tiles_in_extent(
				non_wrapping_update.get_west(),
				non_wrapping_update.get_south(),
				non_wrapping_update.get_east(),
				non_wrapping_update.get_north());

			for (RasterTileRegion region : tile_regions)
			{
				std::unordered_map<RasterTileIdentifier, Texture2D*, RasterTileIdentifierHasher>::const_iterator tile_texture =
					details->loaded_tiles.find(region.tile->get_identifier());

				if (tile_texture != details->loaded_tiles.end())
				{
					render_tile_to_level_texture(frame_state, level, details, region, tile_texture->second);
				}
				else
				{
					upsample_tile_data(frame_state, level, details, region);
				}
			}
		}

		if (details->type == RasterType::Terrain)
		{
			// Normals at edges are incorrect, so include a one-post buffer around the update region
			// when updating normals in order to update normals that were previously at the edge.
			ClipmapUpdate update_with_buffer =
				update.add_buffer_within_level_next_extent();

			const std::vector<ClipmapUpdate>& normal_updates =
				split_update_to_avoid_wrapping(update_with_buffer, details);

			for (ClipmapUpdate normal_update : normal_updates)
			{
				update_normals(frame_state, normal_update);
			}
		}
	}

	void ClipmapUpdater::update_terrain(
		FrameState* frame_state,
		ClipmapUpdate& update)
	{
		ClipmapLevel * level = update.get_level();
		this->update(frame_state, update, level, m_terrain.get(), level->get_terrain());
	}

	void ClipmapUpdater::update_imagery(
		FrameState* frame_state,
		ClipmapUpdate& update)
	{
		ClipmapLevel* level = update.get_level();
		this->update(frame_state, update, level, m_imagery.get(), level->get_imagery());
	}

	void ClipmapUpdater::update_normals(
		FrameState* frame_state,
		ClipmapUpdate& update)
	{
		ClipmapLevel * level = update.get_level();

		Texture2D * level_texture = level->get_height_texture();
		Texture2D * level_normal_texture = level->get_normal_texture();

		m_compute_normals_texture_sampler->set_sampler(Device::get_texture_samplers()->get_nearest_repeat());
		m_compute_normals_texture_sampler->set_texture(level_texture);

		m_framebuffer->get_color_attachments()->set_attachment(m_normal_output, level_normal_texture);

		int clipmap_size = level->get_next_extent().east - level->get_next_extent().west + 1;
		int west = (level->get_origin_in_textures().x + (update.get_west() - level->get_next_extent().west)) % clipmap_size;
		int south = (level->get_origin_in_textures().y + (update.get_south() - level->get_next_extent().south)) % clipmap_size;

		m_compute_normals_max_min_heights->set_value(vec2(level->get_maximum_height(), level->get_minimum_height()));
		m_compute_normals_update_size->set_value(vec2(update.get_width(), update.get_height()));
		m_compute_normals_origin->set_value(vec2(west, south));
		m_compute_normals_one_over_height_map_size->set_value(vec2(1.0f / (float) level_texture->get_description().get_width(), 1.0f / (float) level_texture->get_description().get_height()));
		m_post_delta->set_value(update.get_level()->get_terrain()->get_post_delta_longitude());
		
		Rectangle viewport(
			0,
			0,
            level_texture->get_description().get_width(),
            level_texture->get_description().get_height());

		m_pass_state.set_viewport(&viewport);
		
		m_compute_normals_draw_command->execute(
			frame_state->get_context(),
			&m_pass_state);

		m_pass_state.set_viewport(nullptr);
	}

	void ClipmapUpdater::render_tile_to_level_texture(
		FrameState* frame_state,
		ClipmapLevel * level,
		RasterDataDetails * details,
		RasterTileRegion region,
		Texture2D* tile_texture)
	{
		Texture2D* level_texture;
		vec2i origin_in_textures;
		Extent next_extent;

		if (details->type == RasterType::Terrain)
		{
			level_texture      = level->get_height_texture();
			origin_in_textures = level->get_origin_in_textures();
			next_extent        = level->get_next_extent();
		}
		else
		{
			level_texture      = level->get_imagery_texture();
			origin_in_textures = level->get_origin_in_imagery();
			next_extent        = level->get_next_imagery_extent();
		}

		m_update_texture_sampler->set_sampler(Device::get_texture_samplers()->get_nearest_clamp());
		m_update_texture_sampler->set_texture(tile_texture);
		
		m_framebuffer->get_color_attachments()->set_attachment(m_update_texel_output, level_texture);

		int clipmap_size = next_extent.east - next_extent.west + 1;
		int dest_west  = (origin_in_textures.x + (region.tile->get_west()  + region.west  - next_extent.west))  % clipmap_size;
		int dest_south = (origin_in_textures.y + (region.tile->get_south() + region.south - next_extent.south)) % clipmap_size;

		int width  = region.east - region.west + 1;
		int height = region.north - region.south + 1;

		RasterSource* source = level->get_terrain()->get_source();
		m_update_max_min_heights->set_value(vec2(
			source->get_maximum_height(),
			source->get_minimum_height()));

		m_update_one_over_texture_size->set_value(vec2(1.0f / tile_texture->get_description().get_width(), 1.0f / tile_texture->get_description().get_height()));
		m_update_update_size->set_value(vec2(width, height));
		m_update_destination_offset->set_value(vec2(dest_west, dest_south));
		m_update_source_origin->set_value(vec2(region.west, region.south));

		Rectangle viewport(
			0,
			0,
			level_texture->get_description().get_width(),
			level_texture->get_description().get_height());

		m_pass_state.set_viewport(&viewport);

		m_update_draw_command->execute(
			frame_state->get_context(),
			&m_pass_state);
		
		m_pass_state.set_viewport(nullptr);
	}

	void ClipmapUpdater::upsample_tile_data(
		FrameState* frame_state,
		ClipmapLevel * level,
		RasterDataDetails * details,
		RasterTileRegion region)
	{
		ClipmapLevel* coarser_level = level->get_coarser_level();

		if (coarser_level == nullptr)
			return;

		Texture2D * level_texture;
		Texture2D * coarser_level_texture;

		vec2i origin_in_textures;
		vec2i coarser_origin_in_textures;

		Extent next_extent;
		Extent coarser_next_extent;

		if (details->type == RasterType::Terrain)
		{
			level_texture = level->get_height_texture();
			coarser_level_texture = coarser_level->get_height_texture();
			origin_in_textures = level->get_origin_in_textures();
			coarser_origin_in_textures = coarser_level->get_origin_in_textures();
			next_extent = level->get_next_extent();
			coarser_next_extent = coarser_level->get_next_extent();
		}
		else
		{
			level_texture = level->get_imagery_texture();
			coarser_level_texture = coarser_level->get_imagery_texture();
			origin_in_textures = level->get_origin_in_imagery();
			coarser_origin_in_textures = coarser_level->get_origin_in_imagery();
			next_extent = level->get_next_imagery_extent();
			coarser_next_extent = coarser_level->get_next_imagery_extent();
		}

		m_upsample_texture_sampler->set_sampler(Device::get_texture_samplers()->get_linear_repeat());
		m_upsample_texture_sampler->set_texture(coarser_level_texture);

		m_framebuffer->get_color_attachments()->set_attachment(m_upsample_texel_output, level_texture);

		int fine_clipmap_size = next_extent.east - next_extent.west + 1;
		int dest_west  = (origin_in_textures.x + (region.tile->get_west()  + region.west  - next_extent.west))  % fine_clipmap_size;
		int dest_south = (origin_in_textures.y + (region.tile->get_south() + region.south - next_extent.south)) % fine_clipmap_size;

		int coarse_clipmap_size = coarser_next_extent.east - coarser_next_extent.west + 1;
		double source_west  = ArgeoMath::mod((coarser_origin_in_textures.x + (static_cast<double>(region.tile->get_west()  + region.west)  / 2.0 - coarser_next_extent.west)),  coarse_clipmap_size);
		double source_south = ArgeoMath::mod((coarser_origin_in_textures.y + (static_cast<double>(region.tile->get_south() + region.south) / 2.0 - coarser_next_extent.south)), coarse_clipmap_size);
		
		int width  = region.east  - region.west  + 1;
		int height = region.north - region.south + 1;

		m_upsample_source_origin->set_value(vec2((float)source_west, (float)source_south));
		m_upsample_update_size->set_value(vec2(width, height));
		m_upsample_destination_offset->set_value(vec2(dest_west, dest_south));
		m_upsample_one_over_texture_size->set_value(vec2(1.0f / coarser_level_texture->get_description().get_width(), 1.0f / coarser_level_texture->get_description().get_height()));

		// Update the context and draw
		Rectangle viewport(
			0,
			0,
			level_texture->get_description().get_width(),
			level_texture->get_description().get_height());

		m_pass_state.set_viewport(&viewport);

		m_upsample_draw_command->execute(
			frame_state->get_context(),
			&m_pass_state);

		
		m_pass_state.set_viewport(nullptr);

		level->set_maximum_height(coarser_level->get_maximum_height());
		level->set_minimum_height(coarser_level->get_minimum_height());
	}


	void ClipmapUpdater::request_tile_residency(
		FrameState* frame_state,
		ClipmapLevel* level)
	{
		request_tile_residency(frame_state, level, m_terrain.get(), level->get_terrain(), level->get_next_extent());
		//request_tile_residency(frame_state, level, m_imagery.get(), level->get_imagery(), level->get_next_imagery_extent());
	}

	void ClipmapUpdater::request_tile_residency(
		FrameState* frame_state,
		ClipmapLevel * level,
		RasterDataDetails* details,
		RasterLevel * raster_level,
		Extent next_extent)
	{
		std::vector<RasterTileRegion> tile_regions = raster_level->get_tiles_in_extent(
			next_extent.west,
			next_extent.south,
			next_extent.east,
			next_extent.north);

		for (std::vector<RasterTileRegion>::iterator region = tile_regions.begin(); region != tile_regions.end(); ++region)
		{
			RasterTile * tile = region->tile;

			// If the tile is not loaded then request tile load.
			if (details->loaded_tiles.find(tile->get_identifier()) == details->loaded_tiles.end())
			{
				request_tile_load(details, level, tile);
			}
		}
	}

	void ClipmapUpdater::set_height_exaggeration(
		float value,
		ClipmapLevel* level)
	{
		if (value != m_height_exaggeration->get_value())
		{
			m_height_exaggeration->set_value(value);

			RasterLevel*   raster_level = level->get_terrain();
			GeodeticExtent full_extent  =  raster_level->get_extent();
			std::vector<RasterTileRegion> tile_regions = raster_level->get_tiles_in_extent(
				full_extent.get_west(),
				full_extent.get_south(),
				full_extent.get_east(),
				full_extent.get_north());
			
			for (RasterTileRegion region : tile_regions)
			{
				region.tile->on_changed();
			}
		}
	}

	float ClipmapUpdater::get_height_exaggeration()
	{
		return m_height_exaggeration->get_value();
	}
}