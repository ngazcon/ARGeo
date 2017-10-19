#include "ProjectionMatrixUniformFactory.h"

#include "ProjectionMatrixUniform.h"

namespace argeo
{
	std::string ProjectionMatrixUniformFactory::get_name() const
	{
		return "og_projectionMatrix";
	}

	std::unique_ptr<DrawAutomaticUniform> ProjectionMatrixUniformFactory::create(IUniform* uniform)
	{
		return std::unique_ptr<DrawAutomaticUniform>(
			new ProjectionMatrixUniform(uniform));
	}
}
