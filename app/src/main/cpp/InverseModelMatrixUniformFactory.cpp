#include "InverseModelMatrixUniformFactory.h"

#include "InverseModelMatrixUniform.h"

namespace argeo
{
	std::string InverseModelMatrixUniformFactory::get_name() const
	{
		return "og_inverseModelMatrix";
	}

	std::unique_ptr<DrawAutomaticUniform> InverseModelMatrixUniformFactory::create(IUniform* uniform)
	{
		return std::unique_ptr<DrawAutomaticUniform>(
			new InverseModelMatrixUniform(uniform));
	}
}
