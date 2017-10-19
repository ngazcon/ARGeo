#pragma once

#include "ITerrain.h"
#include "RasterSource.h"
#include "ClipmapUpdater.h"
#include "RasterLevelCollection.h"
#include "RasterSourceChangedListener.h"

#include "Ellipsoid.h"
#include "Geodetic3D.h"

#include "Uniform.h"
#include "Material.h"
#include "TerrainShaderSet.h"

#include <vector>
#include <memory>

namespace argeo
{
	class Scene;

	class ClipmapTerrain :
		public ITerrain,
		public RasterSourceChangedListener
	{
		enum BlockIndex : unsigned int
		{
			FillBlock0					 = 0,
			FillBlock1					 = 1,
			FillBlock2					 = 2,
			FillBlock3					 = 3,
			FillBlock4					 = 4,
			FillBlock5					 = 5,
			FillBlock6					 = 6,
			FillBlock7					 = 7,
			FillBlock8					 = 8,
			FillBlock9					 = 9,
			FillBlock10					 = 10,
			FillBlock11					 = 11,
			HorizontalFixup0			 = 12,
			HorizontalFixup1			 = 13,
			VerticalFixup0				 = 14,
			VerticalFixup1				 = 15,
			DegenerateTrianglesOuterRing = 16,

			// Non center specific
			HorizontalInteriorTrim		 = 17,
			VerticalInteriorTrim		 = 18,

			// Center specific
			CenterFillBlock0			 = 19,
			CenterFillBlock1			 = 20,
			CenterFillBlock2			 = 21,
			CenterFillBlock3			 = 22,
										 
			CenterHorizontalFixup0		 = 23,
			CenterHorizontalFixup1		 = 24,
			CenterVerticalFixup0		 = 25,
			CenterVerticalFixup1		 = 26,
										 
			CenterBlock					 = 27,
			NumberOfBlocks				 = 28,
		};

	public:
		ClipmapTerrain(
			Scene* scene,
			const Ellipsoid& ellipsoid,
			const unsigned int& clipmap_posts);

		~ClipmapTerrain();

		void update(FrameState* frame_state);
		void pre_render(FrameState* frame_state);
		void generate_command_queue(FrameState* frame_state);

		Material* get_material();

		bool get_wireframe() const;
		void set_wireframe(bool value);

		bool get_show_imagery() const;
		void set_show_imagery(bool value);

		bool get_show() const;
		void set_show(bool value);

		bool get_lighting() const;
		void set_lighting(bool value);

        bool get_use_double_precision();
        void set_use_double_precision(const bool& value);

		double get_height_exaggeration() const;
		void   set_height_exaggeration(double value);

		void set_raster_source(RasterSource* value);
		RasterSource* get_raster_source();

        void set_imagery_source(RasterSource* value);
        RasterSource* get_imagery_source();

		Ellipsoid get_ellipsoid() const;
		void set_ellipsoid(const Ellipsoid& value);

		double get_point_height(const Geodetic2D& point) const;

        TerrainShader* get_shader_program();
        void set_shader_program(TerrainShader* value);

		void on_raster_source_changed(RasterSource* value);
        void notify_dirty(const RasterTile* tile);

	private:

		double get_point_height(
			double column,
			double row,
			double max,
			double min,
			const Texture2D* texture) const;

		std::unique_ptr<Mesh> create_degenerate_triangle_mesh();

		bool generate_level_command_queue(
			int level_index,
			ClipmapLevel* level, 
			bool fill_ring,
			FrameState* frame_state);
		
		void draw_block(
            const unsigned int& level_index,
            const unsigned int& block_index,
            const unsigned int& overall_west,
            const unsigned int& overall_south,
            const unsigned int& block_west,
            const unsigned int& block_south,
			FrameState* frame_state);


		void update_uniform_values(
            const int& level_index,
            const int& block_index,
            const int& block_west,
            const int& block_south,
            const int& overall_west,
            const int& overall_south,
            FrameState* frame_state);

		void update_origin_in_textures(ClipmapLevel* level);
		void update_imagery_origin_in_textures(ClipmapLevel* level);
		void update_terrain(unsigned int& level_index, FrameState* frame_state);
		void update_imagery(unsigned int& level_index, FrameState* frame_state);

		void update_clipmap_updater(Context* context);
		void update_bounding_volume(
			const unsigned int& level_index,
			const unsigned int& block_index,
			FrameState* frame_state);

		void create_block_command(
			unsigned int level_index,
			unsigned int block_index);
		void create_block_commands();
		void create_resources(Context* context);
		void create_uniform_map(std::string id);
		void create_vertex_arrays(Context* context);

		VertexArray* get_block_vertex_array(unsigned int block_index);
		GeodeticExtent get_block_geodetic_extent(
			unsigned int level_index,
			unsigned int block_index);

        vec2 get_block_size(const unsigned int& block_index);

        bool get_depth_test_against() const;
        void set_depth_test_against(const bool& value);

		vec3d pick(
            const Ray& ray,
            Scene* scene);

        vec3d pick_from_tile(
                const RasterTile* tile,
                const Ray &ray,
                const SceneMode &mode,
                MapProjection *projection,
                const bool &cull_back_faces);


	private:
        void set_level_bounding_volume_dirty(const unsigned int& level);

        static const ShaderVertexAttributeLocations attribute_locations;

		std::vector<std::unique_ptr<ClipmapLevel>> m_clipmap_levels;
		std::unique_ptr<Material> m_material;
		
		bool							m_updater_dirty;
		std::unique_ptr<ClipmapUpdater> m_updater;

		Scene* m_scene;
		RasterSource*   m_imagery_source;
		RasterSource*   m_raster_source;

        SceneMode m_mode;
		std::map<std::string, DrawCommand*>  m_block_commands;
		std::map<std::string, bool> m_block_bounding_volume_dirty;


        std::unique_ptr<TerrainShaderSet> m_shader_set;
		struct RenderResource
		{
            RenderResource()
            : render_state(nullptr)
            , shader_program(nullptr)
            { }

			RenderState*   render_state;
			PrimitiveType  primitive_type;
			TerrainShader* shader_program;

			// Uniform maps and values of the patchs.
			// The ids are in the form: <level>.<patch>
			std::map<std::string, UniformMap*>    uniform_maps;
			std::map<std::string, UniformValues*> uniform_values;

			// M x M block.
			std::unique_ptr<VertexArray> fill_patch;

			// Horizontal fix-up
			std::unique_ptr<VertexArray> horizontal_fixup_patch;
			
			// Vertical fix-up
			std::unique_ptr<VertexArray> vertical_fixup_patch;

			// Horizontal interior trim.
			std::unique_ptr<VertexArray> horizontal_offset_patch;

			// Vertical interior trim.
			std::unique_ptr<VertexArray> vertical_offset_patch;

			// Degenerate triangle patch.
			std::unique_ptr<VertexArray> degenerate_triangle_patch;

			// Center patch.
			std::unique_ptr<VertexArray> center_patch;

			
		} m_render_resources;

		bool m_resources_dirty;
		bool m_use_double_precision;

		int m_clipmap_posts;
		int m_clipmap_segments;

		int m_fill_patch_posts;
		int m_fill_patch_segments;

		bool   m_show;
		bool   m_show_imagery;
		bool   m_lighting;
		bool   m_wireframe;
        bool   m_depth_test_against;

		bool   m_height_exaggeration_dirty;
		double m_height_exaggeration;

		Geodetic3D m_clipmap_center;
		Ellipsoid  m_ellipsoid;

	};
}
