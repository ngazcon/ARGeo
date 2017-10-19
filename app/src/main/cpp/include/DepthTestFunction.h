#pragma once

namespace argeo
{
	enum class DepthTestFunction : unsigned int
	{
		Never		   = 0, 
		Less		   = 1, 
		LessOrEqual	   = 2, 
		Equal		   = 3, 
		Greater		   = 4, 
		GreaterOrEqual = 5, 
		NotEqual	   = 6, 
		Always		   = 7, 
	};
}