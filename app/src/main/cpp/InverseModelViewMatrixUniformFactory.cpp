#include "InverseModelViewMatrixUniformFactory.h"

#include "InverseModelViewMatrixUniform.h"

namespace argeo
{
	std::string InverseModelViewMatrixUniformFactory::get_name() const
	{
		return "og_inverseModelViewMatrix";
	}

	std::unique_ptr<DrawAutomaticUniform> InverseModelViewMatrixUniformFactory::create(IUniform* uniform)
	{
		return std::unique_ptr<DrawAutomaticUniform>(
			new InverseModelViewMatrixUniform(uniform));
	}
}
