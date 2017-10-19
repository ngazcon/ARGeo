#include "ModelMatrixUniformFactory.h"

#include "ModelMatrixUniform.h"

namespace argeo
{
	std::string ModelMatrixUniformFactory::get_name() const
	{
		return "og_modelMatrix";
	}

	std::unique_ptr<DrawAutomaticUniform> ModelMatrixUniformFactory::create(IUniform* uniform)
	{
		return std::unique_ptr<DrawAutomaticUniform>(
			new ModelMatrixUniform(uniform));
	}
}
