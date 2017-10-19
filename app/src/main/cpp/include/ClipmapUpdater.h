#pragma once

#include "MessageQueue.h"

#include "Context.h"
#include "VertexArray.h"
#include "FrameBuffer.h"
#include "Uniform.h"

#include "PassState.h"
#include "DrawCommand.h"
#include "ClearCommand.h"
#include "UniformSampler.h"

#include "Vector.h"

#include "ClipmapLevel.h"
#include "ClipmapUpdate.h"
#include "RasterTile.h"
#include "Surface.h"

#include <unordered_map>
#include <list>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <memory>

namespace argeo
{
	enum class RasterType
	{
		Terrain,
		Imagery
	};

	struct TileLoadRequest
	{
	public:
		ClipmapLevel* level;
		RasterTile*   tile;
		float		  maximum;
		float		  minimum;
		std::unique_ptr<Texture2D>  texture;
	};

	class RasterDataDetails
	{
	public:
		RasterDataDetails(RasterType type)
			: done_queue(new MessageQueue)
			, type(type)
			, notified(false)
		{
			request_list_empty_iterator = request_list.end();
			request_insertion_point		= request_list_empty_iterator;
		}

		RasterType type;
		std::unordered_map<RasterTileIdentifier, std::list<TileLoadRequest*>::iterator, RasterTileIdentifierHasher> loading_tiles;
		std::unordered_map<RasterTileIdentifier, Texture2D*, RasterTileIdentifierHasher> loaded_tiles;

		std::mutex loading_tiles_mutex;
		std::mutex request_list_mutex;
		std::condition_variable request_list_condition;
		bool notified;

		std::list<TileLoadRequest*> request_list;
		std::list<TileLoadRequest*>::iterator request_insertion_point;
		std::list<TileLoadRequest*>::iterator request_list_empty_iterator;

		std::unique_ptr<Surface> worker_window;
		std::unique_ptr<MessageQueue> done_queue;
	};

	class ClipmapUpdater
	{
	public:
		ClipmapUpdater(
			Context* context,
			std::vector<ClipmapLevel*> clipmap_levels);

		~ClipmapUpdater();


		float get_height_exaggeration();
		
		void  set_height_exaggeration(
			float value,
			ClipmapLevel* level);

		void apply_new_data(FrameState* frame_state);

		void request_tile_residency(
			FrameState* frame_state,
			ClipmapLevel* level);

		void update_terrain(FrameState* frame_state, ClipmapUpdate& update);
		void update_imagery(FrameState* frame_state, ClipmapUpdate& update);
		void update_normals(FrameState* frame_state, ClipmapUpdate& update);

		void remove_request(
			std::list<TileLoadRequest*>::iterator request_iterator,
			RasterDataDetails* details);

	private:

		void request_tile_residency(
			FrameState* frame_state,
			ClipmapLevel * level,
			RasterDataDetails* details,
			RasterLevel * raster_level,
			Extent next_extent);

		void request_tile_load(
			RasterDataDetails* details,
			ClipmapLevel * level,
			RasterTile * tile);

		void update(
			FrameState* frame_state,
			ClipmapUpdate & update,
			ClipmapLevel * level,
			RasterDataDetails * details,
			RasterLevel * raster_level);

		void upsample_tile_data(
			FrameState* frame_state,
			ClipmapLevel * level,
			RasterDataDetails * details,
			RasterTileRegion region);

		void render_tile_to_level_texture(
			FrameState* frame_state,
			ClipmapLevel * level,
			RasterDataDetails * details,
			RasterTileRegion region,
			Texture2D * tile_texture);

		void apply_new_data(
			FrameState* frame_state,
			RasterDataDetails* details);

		void apply_new_tile(
			FrameState* frame_state,
			RasterDataDetails * details,
			ClipmapLevel * level,
			RasterTile * tile);

		void apply_if_not_loaded(
			FrameState* frame_state,
			RasterDataDetails * details,
			ClipmapLevel * level,
			RasterTile * tile);

		std::vector<ClipmapUpdate> split_update_to_avoid_wrapping(
			ClipmapUpdate & update,
			RasterDataDetails * details);

		ClipmapUpdate intersect_updates(
			ClipmapUpdate& first,
			ClipmapUpdate& second);

		void request_thread_entry_point(RasterDataDetails* details);

		void initialize_request_threads(
			Context* context,
			RasterDataDetails* details,
			ClipmapLevel * clipmap_level_zero,
			RasterLevel * raster_level_zero);


	private:

		RenderState*  m_render_state;
		PassState     m_pass_state;

		std::unique_ptr<VertexArray> m_unit_quad;
		std::unique_ptr<FrameBuffer> m_framebuffer;

		std::unique_ptr<DrawCommand> m_update_draw_command;
		
		std::unique_ptr<ShaderProgram> m_update_shader;
		UniformSampler* m_update_texture_sampler;
		Uniform<vec2>*  m_update_destination_offset;
		Uniform<vec2>*  m_update_max_min_heights;
		Uniform<vec2>*  m_update_update_size;
		Uniform<vec2>*  m_update_source_origin;
		Uniform<vec2>*  m_update_one_over_texture_size;
		int m_update_texel_output;

		std::unique_ptr<DrawCommand> m_upsample_draw_command;
		std::unique_ptr<ShaderProgram> m_upsample_shader;
		UniformSampler* m_upsample_texture_sampler;
		Uniform<vec2>*  m_upsample_source_origin;
		Uniform<vec2>*  m_upsample_update_size;
		Uniform<vec2>*  m_upsample_destination_offset;
		Uniform<vec2>*  m_upsample_one_over_texture_size;
		int m_upsample_texel_output;

		std::unique_ptr<DrawCommand> m_compute_normals_draw_command;
		std::unique_ptr<ShaderProgram> m_compute_normals_shader;
		UniformSampler* m_compute_normals_texture_sampler;
		Uniform<vec2>*  m_compute_normals_origin;
		Uniform<vec2>*  m_compute_normals_update_size;
		Uniform<vec2>*  m_compute_normals_max_min_heights;
		Uniform<vec2>*  m_compute_normals_one_over_height_map_size;
		Uniform<float>* m_height_exaggeration;
		Uniform<float>* m_post_delta;
		int m_normal_output;

		std::unique_ptr<RasterDataDetails> m_terrain;
		std::unique_ptr<RasterDataDetails> m_imagery;
	};
}
