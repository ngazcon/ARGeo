#pragma once

#include "ShaderObject.h"

#include <vector>
#include <string>

namespace argeo
{
	class ShaderObjectGL : public ShaderObject
	{
	public:

		ShaderObjectGL(std::string source, ShaderObjectType type);
		~ShaderObjectGL();

		int get_handle() const;

		ShaderObjectType get_type() const;

		std::string get_source() const;

		static std::string create_vertex_outline_shader_source(std::string source);
		static std::string create_fragment_outline_shader_source(std::string source);

		static std::string create_fragment_pick_shader_source(
			std::string source,
			std::string pick_color_qualifier);

		static std::string find_output(
			const std::string& source,
			const std::vector<std::string>& names);

		static std::string find_position_output(const std::string& source);
		static std::string find_normal_output(const std::string& source);

		static std::string remove_comments(std::string source);
		static std::string replace_main(std::string source, std::string new_name);

	private:
		static std::string modify_shader_source(std::string source, ShaderObjectType type);

		static std::string builtin_structs();
		static std::string builtin_constants();
		static std::string builtin_functions();
		static std::string builtin_uniforms();


		static const std::vector<std::string> g_position_output_names;
		static const std::vector<std::string> g_normal_output_names;

		// The handle to gpu memory.
		int m_handle;

		// The shader object type.
		ShaderObjectType m_type;

		// The shader object source code.
		std::string m_source;

	};
}

