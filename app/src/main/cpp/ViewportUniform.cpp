#include "ViewportUniform.h"

#include "Context.h"
#include "UniformState.h"
#include "DrawCommand.h"

namespace argeo
{
	ViewportUniform::ViewportUniform(IUniform* uniform)
		: m_uniform(static_cast<Uniform<vec4>*>(uniform))
	{ }

	void ViewportUniform::set(UniformState* uniform_state)
	{
		// viewport.Bottom should really be used but Rectangle goes top to botom, not bottom to top. 
		const Rectangle viewport = uniform_state->get_viewport();
		m_uniform->set_value(vec4(
			viewport.get_left(),
			viewport.get_top(),
			viewport.get_width(),
			viewport.get_height()));
	}
}