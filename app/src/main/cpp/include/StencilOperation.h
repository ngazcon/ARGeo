#pragma once

namespace argeo
{
	enum class StencilOperation
	{
		Zero,
		Invert,
		Keep,
		Replace,
		Increment,
		Decrement,
		IncrementWrap,
		DecrementWrap
	};
}
