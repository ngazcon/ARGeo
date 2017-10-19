#pragma once

#include "IGraphics.h"
#include "ITerrain.h"

#include "HeightClamping.h"
#include "OrientationClamping.h"

#include <string>

namespace argeo
{
	class ModelGraphics : public IGraphics
	{
	public:
		ModelGraphics(
			const std::string& path,
			const HeightClamping& height_clamping = HeightClamping::None,
			const OrientationClamping& orientation_clamping = OrientationClamping::None);
		~ModelGraphics();

		std::string get_path() const;

		Material* get_material();
		void set_material(Material* value);
		
		bool get_show() const;
		void set_show(const bool& value);

		HeightClamping get_height_clamping() const;
		void set_height_clamping(const HeightClamping& value);

		OrientationClamping get_orientation_clamping() const;
		void set_orientation_clamping(const OrientationClamping& value);

	private:
		HeightClamping m_height_clamping;
		OrientationClamping m_orientation_clamping;
		std::string m_path;
		bool m_show;
		Material* m_material;
		ITerrain* m_terrain;
	};
}
