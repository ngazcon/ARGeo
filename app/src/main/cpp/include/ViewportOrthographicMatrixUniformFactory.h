#pragma once

#include "AutomaticUniformFactory.h"

namespace argeo
{
	class ViewportOrthographicMatrixUniformFactory : public AutomaticUniformFactory
	{
	public:

		std::string get_name() const;
		std::unique_ptr<DrawAutomaticUniform> create(IUniform* uniform);
	};
}
