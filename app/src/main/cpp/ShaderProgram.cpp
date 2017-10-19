#include "ShaderProgram.h"

#include "Device.h"
#include "Context.h"
#include "DrawCommand.h"
#include "UniformState.h"
#include "UniformMap.h"

namespace argeo
{
	void ShaderProgram::partition_uniforms(UniformCollection* uniforms)
	{
		for (UniformCollection::iterator itr = uniforms->begin(); itr != uniforms->end(); ++itr)
		{
			AutomaticUniformFactoryCollection* draw_automatic_factory_collection = Device::get_draw_automatic_uniform_factories();

			IUniform* uniform = itr->second;
			std::string name  = uniform->get_name();

			// Check if the uniform is in the automatic uniform collection.
			if (draw_automatic_factory_collection->find(name) != draw_automatic_factory_collection->end())
			{
				m_draw_automatic_uniforms.push_back((*draw_automatic_factory_collection)[name]->create(uniform));
			}
			// Otherwise the uniform is a manual uniform.
			else
			{
				m_manual_uniforms.push_back(uniform);
			}
		}
	}

	
	void ShaderProgram::set_uniforms(
		UniformState* uniform_state,
		UniformMap* uniform_map)
	{
		for (int i = 0; i < m_draw_automatic_uniforms.size(); ++i)
		{
			m_draw_automatic_uniforms[i]->set(uniform_state);
		}

		if (uniform_map != nullptr)
		{
			for (int i = 0; i < m_manual_uniforms.size(); ++i)
			{
				IUniform* manual_uniform = m_manual_uniforms[i];
				if (uniform_map->find(manual_uniform->get_name()) != uniform_map->end())
				{
					(*uniform_map)[manual_uniform->get_name()](manual_uniform);
				}
			}
		}
	}
}