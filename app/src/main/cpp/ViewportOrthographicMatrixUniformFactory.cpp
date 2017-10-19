#include "ViewportOrthographicMatrixUniformFactory.h"

#include "ViewportOrthographicMatrixUniform.h"

namespace argeo
{
	std::string ViewportOrthographicMatrixUniformFactory::get_name() const
	{
		return "og_viewportOrthographicMatrix";
	}

	std::unique_ptr<DrawAutomaticUniform> ViewportOrthographicMatrixUniformFactory::create(IUniform* uniform)
	{
		return std::unique_ptr<DrawAutomaticUniform>(
			new ViewportOrthographicMatrixUniform(uniform));
	}
}
