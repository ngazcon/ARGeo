#pragma once

#include "ColorHelper.h"
#include "Context.h"
#include "BillboardCollection.h"
#include "FrameState.h"
#include "PassState.h"
#include "Model.h"
#include "ITerrain.h"
#include "RasterSource.h"
#include "ViewportQuad.h"
#include "ClearCommand.h"
#include "PickFramebuffer.h"
#include "MapProjection.h"
#include "TweenCollection.h"

#include "SnapshotListener.h"
#include "RasterSourceChangedListener.h"
#include "PickListener.h"
#include "Bitmap.h"
#include "ShadowMap.h"
#include "PboDownloader.h"

#include "IVideoCapturePreview.h"
#include "PickDepth.h"
#include "TerrainDepth.h"
#include "DepthPlane.h"
#include "ARBuffer.h"
#include "MapMode.h"
#include "SceneTransitioner.h"

#include <string>
#include <map>
#include <chrono>
#include <unordered_set>
#include <mutex>
#include <time.h>

namespace argeo
{
	enum class PickingMode : unsigned char
	{
		Single = 0,
		Multiple = 1
	};

	struct FrustumCommands
	{
		FrustumCommands(
			const double& near = 0.0,
			const double& far = 0.0)
			: near(near)
			, far(far)
		{ }


		double near;
		double far;

		std::map<CommandPass, std::vector<DrawCommand*>> commands;
	};

	typedef std::vector<FrustumCommands> FrustumCommandsQueue;

	class PostRenderListener;
	class PreRenderListener;
    class SceneTransitioner;
	class Scene
		: public PickListener
	{
        friend class SceneTransitioner;
	public:
		Scene(Context* context);
		~Scene();

		void render(EngineClockTick time = EngineClock::now());

		bool add_post_render_listener(PostRenderListener* listener);
		bool add_pre_render_listener(PreRenderListener* listener);

		bool remove_post_render_listener(PostRenderListener* listener);
		bool remove_pre_render_listener(PreRenderListener* listener);

		IPrimitive* add_primitive(IPrimitive* primitive);
		void remove_primitive(IPrimitive* primitive);

        bool get_use_vr() const;
        void set_use_vr(const bool& value);

		Camera*		  get_camera();
		PassState*    get_pass_state();
		Context*	  get_context();
		ITerrain*	  get_terrain();
		MapProjection* get_projection();
        TweenCollection* get_tween_collection();

        unsigned int get_drawing_surface_width();
        unsigned int get_drawing_surface_height();

		void set_camera_preview(IVideoCapturePreview* value);

		void  set_terrain_opacity(const float& value);
		float get_terrain_opacity();

        void  set_map_mode_2D(const MapMode2D& value);
        MapMode2D get_map_mode_2D() const;

		void set_raster_source(RasterSource* value);
		RasterSource* get_raster_source();

		IPickable* pick_primitive(const vec2d& window_position);

		vec3d pick_position(const vec2d& window_position);
        vec3d pick_position_world_coordinates(const vec2d& window_position);

		void on_picked(IPickable* object);

		bool add_raster_source_changed_listener(RasterSourceChangedListener* listener);
		bool remove_raster_source_changed_listeners(RasterSourceChangedListener* listener);
		void on_raster_source_changed(RasterSource* value);

        std::unique_ptr<CullingVolume> get_pick_orthographic_culling_volume(
                const vec2d& surface_position,
                int width,
                int height) const;

		std::unique_ptr<CullingVolume> get_pick_perspective_culling_volume(
            const vec2d& surface_position,
			int width,
			int height) const;

		std::unique_ptr<CullingVolume> get_pick_culling_volume(
            const vec2d& surface_position,
			int width,
			int height) const;

        std::unique_ptr<PboDownloader> m_downloader;


		void take_ar_buffer_snapshot();

		void take_ttar_snapshot(
			Bitmap* reality_snapshot,
			Bitmap* ttar_snapshot,
			Camera* camera,
            const unsigned int& width,
            const unsigned int& height);

		void take_snapshot();

		bool add_snapshot_listener(SnapshotListener* listener);
		bool remove_snapshot_listener(SnapshotListener* listener);

        void morph_to_2D(const double& duration = 2);
        void morph_to_3D(const double& duration = 2);
        void morph_to_CV(const double& duration = 2);

        SceneMode get_mode() const;
        void set_mode(const SceneMode& value);

	private:
		bool camera_equals(
			Camera* camera0,
			Camera* camera1,
			const double& epsilon);

		void update_and_execute_commands(
			PassState* pass_state = nullptr,
			Color clear_color     = Color(0, 0, 0, 0),
			const bool& picking   = false,
            const bool& reality_commands = true,
            const bool& render_to_display = true);

		void update_and_clear_framebuffers(
			PassState* pass_state = nullptr,
			Color clear_color     = Color(0, 0, 0, 0),
			const bool& picking   = false,
            const bool& reality_commands = true);

		void update_frustums(
			const double& near,
			const double& far,
			const unsigned int& num_frustums,
            const bool& is_2D = false);

		void insert_into_bin(
			DrawCommand* command,
			const Interval& distances);

		void create_potentially_visible_set();

		void update_derived_commands(DrawCommand* draw_command);

		void insert_shadow_cast_commands(
			CommandQueue commands,
			ShadowMap* shadow_map);

		void execute_shadow_map_cast_commands();

        void execute_2D_commands_in_viewport(
                const bool& is_first_viewport,
                PassState* pass_state = nullptr,
                const bool& picking = false);

		void execute_commands_in_viewport(
			const bool& is_first_viewport,
			PassState* pass_state = nullptr,
			const bool& picking = false);

		void execute_command(
			DrawCommand* command,
			PassState* pass_state = nullptr);

		void execute_overlay_commands(PassState* pass_state = nullptr);
		void execute_reality_commands(PassState* pass_state = nullptr);
		void execute_mix_reality_and_overlay_commands(PassState* pass_state = nullptr);

		void execute_commands(
			PassState* pass_state = nullptr,
			const bool& picking = false);

		void update_viewports(
                Rectangle& render_target_viewport);

		void update_primitives(PassState* pass_state = nullptr);

		bool is_visible(
			DrawCommand* command,
			CullingVolume* culling_volume);

		void on_pre_render(EngineClockTick time);
		void on_post_render(EngineClockTick time);
		
		void update_frame_state(
			unsigned int frame_number,
			EngineClockTick time);

		void update_environment_state();

		PickDepth* get_pick_depth(const unsigned int& index);

        void morph_complete(
                SceneTransitioner* transitioner,
                const SceneMode& previous_mode,
                const SceneMode& mode,
                const bool& was_morphing);

        void morph_start(
                SceneTransitioner* transitioner,
                const SceneMode& previous_mode,
                const SceneMode& mode,
                const bool& was_morphing);

        void execute_translucent_commands_sorted(
                PassState* pass_state,
                std::vector<DrawCommand*>& commands);

	private:
		static double OPAQUE_FRUSTUM_NEAR_OFFSET;

		std::unique_ptr<MapProjection> m_projection;
		MapMode2D m_map_mode;

		bool m_use_vr;
        double m_saved_aspect_ratio;

		float m_terrain_opacity;
		unsigned int m_shader_frame_count;

		FrameState   m_frame_state;
		PassState    m_pass_state;

		ClearCommand m_clear_color;
		ClearCommand m_clear_depth;

		RenderState* m_viewport_render_state;
		RenderState* m_display_viewport_render_state;
		std::unique_ptr<DrawCommand>  m_render_to_viewport_command;
		
		UniformMap m_terrain_to_ar_buffer_uniform_map;
		UniformMap m_mixed_reality_to_display_uniform_map;

		IVideoCapturePreview* m_camera_preview;

		Context* m_context;
		std::unique_ptr<CullingVolume> m_culling_volume;

		std::unique_ptr<Camera> m_camera;
		std::unique_ptr<Camera> m_last_camera;
		bool m_camera_start_fired;
		std::time_t m_camera_moved_time;
		std::time_t m_camera_event_wait_time;

		Rectangle    m_render_target_viewport;
		Rectangle    m_display_viewport;

		// Shadows
		std::unique_ptr<ShadowMap> m_shadow_map;
		std::unique_ptr<Camera> m_sun_camera;

		// Picking
		PickingMode m_picking_mode;
		std::vector<IPickable*> m_picked_list;
		std::unique_ptr<PickFramebuffer> m_pick_framebuffer;
		Rectangle m_picking_rectangle;

		std::unique_ptr<ITerrain>     m_terrain;
		std::unique_ptr<RasterSource> m_default_imagery;
		std::unique_ptr<RasterSource> m_default_raster_source;
		RasterSource* m_raster_source;
		std::unordered_set<RasterSourceChangedListener*> m_raster_source_changed_listeners;

		std::unordered_set<IPrimitive*> m_primitives;
		std::unordered_set<PreRenderListener*>  m_pre_render_listeners;
		std::unordered_set<PostRenderListener*> m_post_render_listeners;

		FrustumCommandsQueue m_frustum_commands_queue;
		CommandQueue m_overlay_commands_queue;
		CommandQueue m_camera_preview_commands_queue;
		double m_far_to_near_ratio;
        double m_near_to_far_distance_2D;

		// Depth
		bool m_copy_terrain_depth;
		bool m_use_depth_picking;
		std::unique_ptr<TerrainDepth> m_terrain_depth;
		std::unique_ptr<DepthPlane> m_depth_plane;
		std::unique_ptr<ARBuffer> m_ar_buffer;
		std::unique_ptr<ARBuffer> m_ttar_buffer;

        std::map<std::string, vec3d> m_pick_position_cache;
        bool m_pick_position_cache_dirty;

		std::map<unsigned int, std::unique_ptr<PickDepth>> m_pick_depths;

		// Snapshot
		std::unordered_set<SnapshotListener*> m_snapshot_listeners;

        SceneMode m_mode;
        std::unique_ptr<SceneTransitioner> m_transitioner;
        std::unique_ptr<TweenCollection> m_tween_collection;
        double m_morph_time;

		// Keeps track of the state of a frame. FrameState is the state across
		// the primitives of the scene. This state is for internally keeping track
		// of celestial and environment effects that need to be updated/rendered in
		// a certain order as well as updating/tracking framebuffer usage.
		struct EnvironmentState
		{
			EnvironmentState()
				: clear_terrain_depth(false)
				, use_depth_plane(false)
				, use_terrain_depth_frame_buffer(false)
				, original_frame_buffer(nullptr)
			{ }

			bool clear_terrain_depth;
			bool use_depth_plane;
			bool use_terrain_depth_frame_buffer;
			FrameBuffer* original_frame_buffer;
		} m_environment_state;
	};
}
