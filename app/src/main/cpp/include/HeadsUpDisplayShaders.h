#pragma once

namespace argeo
{
	static const char heads_up_display_vs[] =
		R"(
			layout(location = og_positionVertexLocation) in vec2 position;

			uniform sampler2D og_texture0;
			uniform float og_highResolutionSnapScale;
			uniform mat4  og_viewportOrthographicMatrix;


			uniform vec2 u_originScale;
			uniform vec3 u_centerPosition;

			out vec2 fsTextureCoordinates;

			void main()
			{
				vec2 halfSize = vec2(textureSize(og_texture0, 0)) * 0.5 * og_highResolutionSnapScale;

				vec4 center = vec4(u_centerPosition, 0.0);
				center.xy += (u_originScale * halfSize);

				gl_Position = og_viewportOrthographicMatrix * vec4(center.xy + position * halfSize, center.z, 1.0);

			   fsTextureCoordinates = vec2((position.x + 1.0) / 2.0, 1.0 - (position.y + 1.0) / 2.0);
			}
		)";

	static const char heads_up_display_fs[] =
		R"(
		precision highp float;

		in vec2 fsTextureCoordinates;

		out vec4 fragmentColor;

		uniform sampler2D og_texture0;
		uniform vec3 u_color;

		void main()
		{
			vec4 color = texture(og_texture0, fsTextureCoordinates);
		
			if (color.a == 0.0)
			{
				discard;
			}

			fragmentColor = vec4(color.rgb * u_color.rgb, color.a);
		}
	)";
}

