#include "NormalMatrixUniformFactory.h"

#include "NormalMatrixUniform.h"

namespace argeo
{
	std::string NormalMatrixUniformFactory::get_name() const
	{
		return "og_normalMatrix";
	}

	std::unique_ptr<DrawAutomaticUniform> NormalMatrixUniformFactory::create(IUniform* uniform)
	{
		return std::unique_ptr<DrawAutomaticUniform>(
			new NormalMatrixUniform(uniform));
	}
}
