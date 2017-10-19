#include "ModelViewProjectionMatrixUniformFactory.h"

#include "ModelViewProjectionMatrixUniform.h"

namespace argeo
{
	std::string ModelViewProjectionMatrixUniformFactory::get_name() const
	{
		return "og_modelViewProjectionMatrix";
	}

	std::unique_ptr<DrawAutomaticUniform> ModelViewProjectionMatrixUniformFactory::create(IUniform* uniform)
	{
		return std::unique_ptr<DrawAutomaticUniform>(
			new ModelViewProjectionMatrixUniform(uniform));
	}
}
