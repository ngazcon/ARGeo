#include "ShaderObjectGL.h"
#include "ArgeoMath.h"

#include "Structs.h"
#include "Functions.h"
#include "Uniforms.h"

#include <limits>
#include <stdexcept>
#include <sstream>
#include <regex>

#include "GL.h"

namespace argeo
{
	const std::vector<std::string> ShaderObjectGL::g_position_output_names = { "fsPositionEC" };
	const std::vector<std::string> ShaderObjectGL::g_normal_output_names   = { "fsNormalEC" };

	ShaderObjectGL::ShaderObjectGL(std::string source, ShaderObjectType type)
		: m_source(source)
		, m_type(type)
	{
		std::string modified_source = modify_shader_source(source, type);
		const GLchar* shader_source	= modified_source.c_str();

		// Use the openGL equivalent for shader type.
		GLuint shader_type = GL_VERTEX_SHADER;
		switch (type)
		{
			case ShaderObjectType::VertexShader:
				shader_type = GL_VERTEX_SHADER;
				break;
			case ShaderObjectType::FragmentShader:
				shader_type = GL_FRAGMENT_SHADER;
				break;
			case ShaderObjectType::GeometryShader:
				shader_type = GL_GEOMETRY_SHADER_EXT;
				break;
		}

		// Create the shader.
		GLuint shader = glCreateShader(shader_type);

		// Bind the shader source to the shader object.
		glShaderSource(shader, 1, &shader_source, nullptr);

		// Compile the shader using the given source.
		glCompileShader(shader);

		// Check shader status.
		GLint status = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		if (!status)
		{
			GLint info_len = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_len);
			std::string str_buffer;
			if (info_len)
			{
				char *buffer = new char[info_len];
				GLint actual_len;
				glGetShaderInfoLog(shader, info_len, &actual_len, buffer);
				str_buffer = std::string(buffer);
				delete[] buffer;
			}
			throw std::runtime_error("Shader Object: " + str_buffer);
		}
		// Finally cache the shader object handle.
		m_handle = shader;
	}


	ShaderObjectGL::~ShaderObjectGL()
	{ }

	std::string ShaderObjectGL::builtin_constants()
	{
		std::stringstream builtin_constants_stream;

		builtin_constants_stream <<
			"const float og_epsilon0  =                " << EPSILON0 << "; \n"
			"const float og_epsilon1  =                " << EPSILON1 << "; \n"
			"const float og_epsilon2  =                " << EPSILON2 << "; \n"
			"const float og_epsilon3  =                " << EPSILON3 << "; \n"
			"const float og_epsilon4  =                " << EPSILON4 << "; \n"
			"const float og_epsilon5  =                " << EPSILON5 << "; \n"
			"const float og_epsilon6  =                " << EPSILON6 << "; \n"
			"const float og_epsilon7  =                " << EPSILON7 << "; \n"
			"const float og_epsilon8  =                " << EPSILON8 << "; \n"
			"const float og_epsilon9  =                " << EPSILON9 << "; \n"
			"const float og_epsilon10 =                " << EPSILON10 << "; \n"
			"const float og_epsilon11 =                " << EPSILON11 << "; \n"
			"const float og_epsilon12 =                " << EPSILON12 << "; \n"
			"const float og_epsilon13 =                " << EPSILON13 << "; \n"
			"const float og_epsilon14 =                " << EPSILON14 << "; \n"
			"const float og_epsilon15 =                " << EPSILON15 << "; \n"

			"const float og_webMercatorMaxLatitude  =  " << 1.4844222297453324 << "; \n"

			"const float og_infinity =         " << std::numeric_limits<float>::max() << "; \n"

			"const float og_E =                " << EULER << "; \n"
			"const float og_pi =               " << PI << "; \n"
			"const float og_oneOverPi =        " << ONE_OVER_PI << "; \n"
			"const float og_piOverTwo =        " << PI_OVER_TWO << "; \n"
			"const float og_piOverThree =      " << PI_OVER_THREE << "; \n"
			"const float og_piOverFour =       " << PI_OVER_FOUR << "; \n"
			"const float og_piOverSix =        " << PI_OVER_SIX << "; \n"
			"const float og_threePiOver2 =     " << THREE_PI_OVER_TWO << "; \n"
			"const float og_twoPi =            " << TWO_PI << "; \n"
			"const float og_oneOverTwoPi =     " << ONE_OVER_TWO_PI << "; \n"
			"const float og_radiansPerDegree = " << RADIANS_PER_DEGREE << "; \n"
			"const float og_maximumFloat =     " <<  std::numeric_limits<float>::max() << "; \n"
			"const float og_minimumFloat =     " << -std::numeric_limits<float>::max() << "; \n";

		return builtin_constants_stream.str();
	}

	std::string ShaderObjectGL::remove_comments(std::string source)
	{
		return source;
	}

	std::string ShaderObjectGL::replace_main(std::string source, std::string new_name)
	{
		std::string renamed_main = "void " + new_name + "()";
		
		// Search for the main definition and replace with the new name.
		// Finds expresions that starts with void then has one or more spaces and then the main secuence,
		// then one or more white spaces followed by a parenthesis opening that may have a void secuence in the middle
		// finally a close parentesis.
		std::regex e(
			"void\\s+main\\s*\\(\\s*(?:void)?\\s*\\)");

		return std::regex_replace(source, e, renamed_main);
	}

	std::string ShaderObjectGL::builtin_uniforms()
	{
		std::string builtin_uniforms = uniforms;
		return builtin_uniforms;
	}

	std::string ShaderObjectGL::builtin_structs()
	{
		std::string builtin_structs =
			material_input +
			material +
			ellipsoid +
			ray +
			ray_segment;

		return builtin_structs;
	}

	std::string ShaderObjectGL::builtin_functions()
	{
		std::string builtin_functions =
			equals_epsilon +
			antialias +
			eye_to_window_coordinates +
			model_to_window_coordinates +
			compute_position +
			translate_relative_to_eye +
			window_to_eye_coordinates +
			polyline_common + 
			tangent_to_eye_space_matrix +
			get_default_material +
			normalize_float +
			denormalize_float +
			get_lambert_diffuse +
			get_specular +
			pack_float_to_vec4 +
			unpack_float_from_vec4 +
			is_empty +
			ray_ellipsoid_intersection_interval +
			get_wgs84_ellipsoid_ec+
			phong +
            latitude_to_web_mercator_fraction +
            columbus_view_morph;

		return builtin_functions;
	}

	std::string ShaderObjectGL::modify_shader_source(
		std::string source,
		ShaderObjectType type)
	{
		std::stringstream vertex_shader_source_stream;

		// This requires that #version be the first line in the shader.  This
		// doesn't follow the spec exactly, which allows whitespace and
		// comments to come beforehand.
		if (source.find("#version") == 0)
		{

#ifdef __ANDROID__
			if (source.find("#version 300 es") == 0)
			{
				vertex_shader_source_stream <<
					"#version 300 es\n" <<
					"#extension GL_OES_EGL_image_external_essl3 : require\n";
			}
			else
			{
				throw std::invalid_argument("Only GLSL version 300 es is supported.");
			}
#else 
			if (source.find("#version 330") == 0)
			{
				vertex_shader_source_stream <<
					"#version 330\n";
			}
			else
			{
				throw std::invalid_argument("Only GLSL version 330 is supported.");
			}

			// Add the raw shader source.
			// Comment the version line because we have set our own version.
			source = "//" + source;
#endif
		}
		else
		{

#ifdef __ANDROID__
			vertex_shader_source_stream <<
				"#version 300 es\n" <<
				"#extension GL_OES_EGL_image_external : require\n";
#else 
			vertex_shader_source_stream <<
				"#version 330\n";
#endif
		}

		// Add the precision qualifier.
		if (type == ShaderObjectType::FragmentShader)
		{
			vertex_shader_source_stream << "precision highp float;\n";
		}

		// Add the builtin constants.
		vertex_shader_source_stream << builtin_constants();

		// Add the builtin structs.
		vertex_shader_source_stream << builtin_structs();

		// Add the builtin uniforms
		vertex_shader_source_stream << builtin_uniforms();

		// Add the builtin functions.
		vertex_shader_source_stream << builtin_functions();

		// Add the original shader source.
		vertex_shader_source_stream <<  source;

		return vertex_shader_source_stream.str();
	}

	std::string ShaderObjectGL::create_vertex_outline_shader_source(std::string source)
	{
		std::string renamed_main = replace_main(source, "og_outline_main");

		// Modified from http://forum.unity3d.com/threads/toon-outline-but-with-diffuse-surface.24668/
		std::string outline_main =
			R"(
				uniform float og_outlineStroke;	

				void main()
				{
					og_outline_main();

					vec3 n			= normalize(og_normalMatrix * normal);
					n.x *= og_projectionMatrix[0][0];
					n.y *= og_projectionMatrix[1][1];
					vec4 clip = gl_Position;
					clip.xy += n.xy * clip.w * og_outlineStroke / og_viewport.z;
					gl_Position = clip;
				}	
			)";

		return renamed_main + "\n" + outline_main;
	}

	std::string ShaderObjectGL::create_fragment_outline_shader_source(std::string source)
	{
		std::string renamed_main = replace_main(source, "og_outline_main");
		std::string outline_main =
			R"(
				uniform vec3 og_outlineColor;	
				void main()
				{
					og_outline_main();
					fragColor = vec4(og_outlineColor, 1.0);
				}	
			)";

		return renamed_main + "\n" + outline_main;
	}

	std::string ShaderObjectGL::create_fragment_pick_shader_source(
		std::string source, 
		std::string pick_color_qualifier)
	{
		std::string renamed_main = replace_main(source, "og_old_main");
		std::string pick_main = pick_color_qualifier + " vec4 og_pickColor;" +
			R"(
				void main()
				{
					og_old_main();					
					fragColor = og_pickColor;
				}	
			)";

		return renamed_main + "\n" + pick_main;
	}

	std::string ShaderObjectGL::find_output(
		const std::string& source,
		const std::vector<std::string>& names)
	{
		for (const std::string name : names)
		{
			std::string::size_type pos = source.find(name, 0);
			if (pos != std::string::npos)
			{
				return name;
			}
		}

		return std::string();
	}

	std::string ShaderObjectGL::find_position_output(const std::string& source)
	{
		return find_output(
			source, 
			g_position_output_names);
	}

	std::string ShaderObjectGL::find_normal_output(const std::string& source)
	{
		return find_output(
			source,
			g_normal_output_names);
	}

	int ShaderObjectGL::get_handle() const
	{
		return m_handle;
	}

	ShaderObjectType ShaderObjectGL::get_type() const
	{
		return m_type;
	}

	std::string ShaderObjectGL::get_source() const
	{
		return m_source;
	}
}
