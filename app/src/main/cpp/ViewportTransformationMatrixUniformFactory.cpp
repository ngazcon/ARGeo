#include "ViewportTransformationMatrixUniformFactory.h"

#include "ViewportTransformationMatrixUniform.h"

namespace argeo
{
	std::string ViewportTransformationMatrixUniformFactory::get_name() const
	{
		return "og_viewportTransformationMatrix";
	}

	std::unique_ptr<DrawAutomaticUniform> ViewportTransformationMatrixUniformFactory::create(IUniform* uniform)
	{
		return std::unique_ptr<DrawAutomaticUniform>(
			new ViewportTransformationMatrixUniform(uniform));
	}
}
