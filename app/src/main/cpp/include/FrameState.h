#pragma once

#include "Camera.h"
#include "Ellipsoid.h"
#include "EngineClock.h"
#include "MapProjection.h"

#include <vector>

namespace argeo
{
	class Context;
	class CullingVolume;
	class ShadowMap;
	class DrawCommand;
	
	typedef std::vector<DrawCommand*> CommandQueue;
	typedef std::vector<ShadowMap*> ShadowMaps;

	enum class Pass : unsigned int
	{
		Render = 0,
		Pick   = 1,
	};

	struct ShadowHints
	{
		ShadowHints()
			: shadows_enabled(true)
			, near_plane(1.0)
			, far_plane(5000.0)
			, closest_object_size(1000.0)
			, out_of_view(true)
		{ }

		bool shadows_enabled;

		double near_plane;
		double far_plane;

		double closest_object_size;
		bool out_of_view;

		std::vector<ShadowMap*> shadow_maps;
		std::vector<ShadowMap*> light_shadow_maps;

		std::time_t last_time_dirty;
	};

	class FrameState
	{
		///
		/// The state for a particular frame.
		///

	public:

		///
		/// Create a frame state for a given context.
		/// @constructor.
		/// @param {Context*} context The context
		///
		FrameState(Context* context);

		///
		/// @destructor.
		///
		~FrameState();


		///
		/// Get the context of this frame state.
		/// @return {Context*} The context of this frame state.
		///
		Context* get_context();


		///
		/// Get the pass of this frame state.
		/// @return {Pass} The pass of this frame state.
		///
		Pass get_pass();


		///
		/// Set the pass of this frame state. 
		/// @param {Pass} value The pass.
		///
		void set_pass(const Pass& value);


		///
		/// Get the camera of this frame state.
		/// @return {Camera*} The camera of this frame state.
		///
		Camera* get_camera() const;


		///
		/// Set the camera of this frame state. 
		/// @param {Camera*} value The camera.
		///
		void set_camera(Camera* value);


		///
		/// Get the culling volume of this frame state.
		/// @return {CullingVolume*} The culling volume of this frame state.
		///
		CullingVolume* get_culling_volume() const;


		///
		/// Set the culling volume of this frame state. 
		/// @param {CullingVolume*} value The culling volume.
		///
		void set_culling_volume(CullingVolume* value);


		///
		/// Get the frame number of this frame state.
		/// @return {unsigned int} The frame number of this frame state.
		///
		unsigned int get_frame_number() const;


		///
		/// Set the culling volume of this frame state. 
		/// @param {CullingVolume*} value The culling volume.
		///
		void set_frame_number(const unsigned int& value);

		///
		/// Get the map projection of this frame state.
		/// @return {MapProjection*} The map projection of this frame state.
		///
		MapProjection* get_map_projection() const;


		ShadowHints& get_shadow_hints();


		ShadowMaps& get_shadow_maps();

		void set_shadow_hints(const ShadowHints& value);

		///
		/// Set themap projection of this frame state. 
		/// @param {MapProjection*} value The map projection.
		///
		void set_map_projection(MapProjection* value);

		///
		/// Get the current time of this frame state.
		/// @return {EngineClockTick} The current time of this frame state.
		///
		EngineClockTick get_time() const;


		///
		/// Set the current time of this frame state.
		/// @param {EngineClockTick} value The current time of this frame state.
		///
		void set_time(const EngineClockTick& value);


		///
		/// Get the command queue of this frame state.
		/// @return {EngineClockTick} The command queue of this frame state.
		///
		CommandQueue& get_command_queue();


        double get_morph_time() const;
        void   set_morph_time(const double& value);

		bool get_scene_3D_only() const;
		void set_scene_3D_only(const bool& value);

		SceneMode get_mode() const;

		void set_mode(const SceneMode& value);

	private:
		bool m_scene_3D_only;

		EngineClockTick m_time;
		unsigned int m_frame_number;
		double m_morph_time;
		CommandQueue m_command_queue;
		Pass m_pass;
		ShadowHints m_shadow_hints;
		ShadowMaps  m_shadow_maps;
        SceneMode m_mode;

		MapProjection* m_map_projection;
		CullingVolume* m_culling_volume;
		Context*	   m_context;
		Camera*		   m_video_capture_object;		
	};
}