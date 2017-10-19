#include "UniformSampler.h"
#include "Device.h"

#include "TextureUnit.h"

#include <sstream>

#include "GL.h"

namespace argeo
{
	UniformSampler::UniformSampler(
		std::string name,
		int location,
		UniformType type,
		int texture_unit_index,
		ICleanableObserver* observer)
		: Uniform<int>(name, type, location, observer)
		, m_sampler(nullptr)
		, m_texture(nullptr)
	{
		set_value(texture_unit_index);
	}


	UniformSampler::~UniformSampler()
	{ }

	void UniformSampler::set_sampler(TextureSampler* sampler)
	{
		m_sampler = sampler;
		
		if (!m_dirty)
		{
			m_observer->notify_dirty(this);
			m_dirty = true;
		}
	}

	void UniformSampler::set_texture(Texture* texture)
	{
		m_texture = texture;
		
		if (!m_dirty)
		{
			m_observer->notify_dirty(this);
			m_dirty = true;
		}
	}

	void UniformSampler::clean()	
	{
		// Set the sampler to the texture unit.
		// If not set any then use the texture sampler.
		TextureUnit* unit = Device::get_texture_units()->get_texture_unit(m_value);

		TextureSampler* sampler = m_sampler;
		if (sampler == nullptr)
		{
			sampler = m_texture != nullptr ? 
				m_texture->get_sampler() :
				nullptr;
		}
		
		unit->set_texture_sampler(sampler);

		// Set the texture to the texture unit.
		unit->set_texture(m_texture);

		// Set the uniform value to match the texture unit.
		glUniform1i(
			m_location,
			m_value);

		// Set as clean.
		m_dirty = false;
	}
}