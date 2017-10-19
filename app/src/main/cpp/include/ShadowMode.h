#pragma once

namespace argeo
{
	enum class ShadowMode : unsigned char
	{
		CastAndReceive = 0,
		Cast           = 1,
		Receive		   = 2,
	};

	inline bool receive_shadows(const ShadowMode& mode)
	{
		return (mode == ShadowMode::CastAndReceive) || (mode == ShadowMode::Receive);
	}

	inline bool casts_shadows(const ShadowMode& mode)
	{
		return (mode == ShadowMode::CastAndReceive) || (mode == ShadowMode::Cast);
	}
}
