#include "FrameState.h"

#include "Context.h"
#include "CullingVolume.h"

namespace argeo
{
	FrameState::FrameState(Context* context)
		: m_context(context)
		, m_video_capture_object(nullptr)
		, m_map_projection(nullptr)
		, m_pass(Pass::Render)
        , m_mode(SceneMode::Scene3D)
        , m_scene_3D_only(false)
	{ }

	FrameState::~FrameState()
	{ }

	Context* FrameState::get_context()
	{
		return m_context;
	}

	Pass FrameState::get_pass()
	{
		return m_pass;
	}

	void FrameState::set_pass(const Pass& value)
	{
		m_pass = value;
	}

	Camera* FrameState::get_camera() const
	{
		return m_video_capture_object;
	}

	void FrameState::set_camera(Camera* value)
	{
		m_video_capture_object = value;
	}

	CommandQueue& FrameState::get_command_queue()
	{
		return m_command_queue;
	}

	unsigned int FrameState::get_frame_number() const
	{
		return m_frame_number;
	}

	void FrameState::set_frame_number(const unsigned int& value)
	{
		m_frame_number = value;
	}

	EngineClockTick FrameState::get_time() const
	{
		return m_time;
	}

	void FrameState::set_time(const EngineClockTick& value)
	{
		m_time = value;
	}

	CullingVolume* FrameState::get_culling_volume() const
	{
		return m_culling_volume;
	}

	void FrameState::set_culling_volume(CullingVolume* value)
	{
		m_culling_volume = value;
	}

	MapProjection* FrameState::get_map_projection() const
	{
		return m_map_projection;
	}

	void FrameState::set_map_projection(MapProjection* value)
	{
		m_map_projection = value;
	}

	ShadowHints& FrameState::get_shadow_hints()
	{
		return m_shadow_hints;
	}

	void FrameState::set_shadow_hints(const ShadowHints& value)
	{
		m_shadow_hints = value;
	}

	ShadowMaps& FrameState::get_shadow_maps()
	{
		return m_shadow_maps;
	}

	double FrameState::get_morph_time() const
	{
		return m_morph_time;
	}

	void FrameState::set_morph_time(const double& value)
	{
		m_morph_time = value;
	}

	bool FrameState::get_scene_3D_only() const
	{
		return m_scene_3D_only;
	}

	void FrameState::set_scene_3D_only(const bool& value)
	{
		m_scene_3D_only = value;
	}

	SceneMode FrameState::get_mode() const
	{
		return m_mode;
	}

	void FrameState::set_mode(const SceneMode& value)
	{
		m_mode = value;
	}
}