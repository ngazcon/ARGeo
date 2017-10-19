#pragma once

#include "Rectangle.h"
#include "IPrimitive.h"
#include "RenderState.h"
#include "DrawCommand.h"
#include "Material.h"

namespace argeo
{
	class ViewportQuad : public IPrimitive
	{
	public:
		ViewportQuad(
			const Rectangle& rectangle,
			Material*  material = Material::from_name(Material::color_material_name));
		~ViewportQuad();

		void       set_rectangle(Rectangle value);
		Rectangle& get_rectangle();

		void      set_material(Material* value);
		Material* get_material();

		void update(FrameState* frame_state);

	private:
		Material*     m_material;
		Rectangle    m_rectangle;
		RenderState* m_render_state;
		std::unique_ptr<DrawCommand> m_draw_command;
	};
}
