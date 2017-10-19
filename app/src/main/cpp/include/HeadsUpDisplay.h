#pragma once

#include "Uniform.h"
#include "Color.h"
#include "Vector.h"
#include "DrawCommand.h"
#include "Billboard.h"
#include "Texture2D.h"

#include <vector>
#include <memory>

namespace argeo
{
	class HeadsUpDisplay
	{
	public:
		HeadsUpDisplay();
		~HeadsUpDisplay();

		Texture2D* get_texture();
		void set_texture(Texture2D* value);
		
		Color get_color();
		void set_color(Color value);

		HorizontalOrigin get_horizontal_origin();
		void set_horizontal_origin(HorizontalOrigin value);

		VerticalOrigin get_vertical_origin();
		void set_vertical_origin(VerticalOrigin value);

		vec2 get_position();
		void set_position(vec2 value);


		void render(Context* context, const UniformState& uniform_state);

	private:
		void dispose_vertex_array();
		void create_vertex_array(Context* context);
		void update(Context* context);

	private:
		
		Uniform<vec3>* m_color_uniform;
		Uniform<vec2>* m_origin_scale_uniform;
		Uniform<vec3>* m_center_position_uniform;
		
		Color m_color;
		
		vec2 m_position;
		bool m_position_dirty;

		HorizontalOrigin m_horizontal_origin;
		VerticalOrigin   m_vertical_origin;

		std::unique_ptr<VertexArray>   m_vertex_array;
		std::unique_ptr<ShaderProgram> m_shader_program;
		std::unique_ptr<DrawCommand>   m_draw_command;
		std::unique_ptr<VertexBuffer>  m_position_buffer;
		

		Texture2D* m_texture;

		static float s_origin_scale[];
	};
}
