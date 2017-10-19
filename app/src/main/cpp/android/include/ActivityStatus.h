#pragma once

#include <cstdint>

namespace argeo
{
	typedef int32_t status;

	const status STATUS_OK = 0;
	const status STATUS_KO = -1;
	const status STATUS_EXIT = -2;
}