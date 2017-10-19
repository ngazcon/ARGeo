#include "ModelViewMatrixUniformFactory.h"

#include "ModelViewMatrixUniform.h"

namespace argeo
{
	std::string ModelViewMatrixUniformFactory::get_name() const
	{
		return "og_modelViewMatrix";
	}

	std::unique_ptr<DrawAutomaticUniform> ModelViewMatrixUniformFactory::create(IUniform* uniform)
	{
		return std::unique_ptr<DrawAutomaticUniform>(
			new ModelViewMatrixUniform(uniform));
	}
}
