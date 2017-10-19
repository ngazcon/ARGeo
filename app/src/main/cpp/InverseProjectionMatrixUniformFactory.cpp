#include "InverseProjectionMatrixUniformFactory.h"

#include "InverseProjectionMatrixUniform.h"

namespace argeo
{
	std::string InverseProjectionMatrixUniformFactory::get_name() const
	{
		return "og_inverseProjectionMatrix";
	}

	std::unique_ptr<DrawAutomaticUniform> InverseProjectionMatrixUniformFactory::create(IUniform* uniform)
	{
		return std::unique_ptr<DrawAutomaticUniform>(
			new InverseProjectionMatrixUniform(uniform));
	}
}
