#pragma once

#include <string>

namespace argeo
{
	class FragmentOutputs
	{
	public:

		// Get the fragment output index for the given name.
		virtual int get_fragment_output(std::string name) const = 0;
	};
}
