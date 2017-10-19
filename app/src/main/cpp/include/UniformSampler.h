#pragma once

#include "Uniform.h"
#include "Context.h"

namespace argeo
{
	class UniformSampler : public Uniform<int>
	{
	public:
		UniformSampler(
			std::string name,
			int location,
			UniformType type,
			int texture_unit_index, 
			ICleanableObserver* observer);

		~UniformSampler();

		void set_sampler(TextureSampler* sampler);
		void set_texture(Texture* texture);
		
		void clean();

	private:
		TextureSampler* m_sampler;
		Texture* m_texture;
	};
}
