#pragma once

namespace argeo
{
	static const char plane_vs[] =
		R"(
			in vec3 positionHigh;
			in vec3 positionLow;
			in vec3 normal;
			
			void main()
			{
				vec4 positionEC = og_modelViewRelativeToEyeMatrix *
								  og_translateRelativeToEye(positionHigh, positionLow);
    
				gl_Position     = og_projectionMatrix * positionEC;
			}
		)";

	static const char plane_fs[] =
		R"(

			uniform vec4 u_color;			
			out     vec4 fragColor;

			void main()
			{
				fragColor = u_color;
				fragColor.a = 0.4f;
			}

		)";

	static const char plane_outline_vs[] =
		R"(
			in vec3 positionHigh;
			in vec3 positionLow;
			void main()
			{
				vec4 positionEC = og_modelViewRelativeToEyeMatrix *
								  og_translateRelativeToEye(positionHigh, positionLow);
    
				gl_Position     = og_projectionMatrix * positionEC;
			}
		)";


	static const char plane_outline_fs[] =
		R"(
			
			out vec4 fragColor;

			void main()
			{
				fragColor = vec4(0.0, 0.0, 0.0, 1.0);
			}

		)";
}

