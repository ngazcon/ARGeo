#include "Device.h"

#include "ShaderProgramGL.h"
#include "VertexBufferGL.h"
#include "IndexBufferGL.h"
#include "UniformBufferGL.h"
#include "Texture2DGL.h"
#include "Texture2DArrayGL.h"
#include "WritePixelBufferGL.h"
#include "FenceGL.h"
#include "IndicesUnsignedShort.h"
#include "IndicesUnsignedInt.h"
#include "include/SurfaceWindowEGL.h"
#include "include/PixelBufferSurfaceEGL.h"
#include "VertexAttribute.h"
#include "ReadPixelBufferGL.h"
#include "ShaderObjectGL.h"
#include "TextureUnitsGL.h"
#include "CubeMapGL.h"
#include "include/ContextEGL.h"

#include "BlittableRGBA.h"
#include "TextureHelper.h"
#include "RenderBufferGL.h"
#include "ModelZToClipCoordinatesUniformFactory.h"
#include "ModelMatrixUniformFactory.h"
#include "ModelViewMatrixUniformFactory.h"
#include "ModelViewRelativeToEyeMatrixUniformFactory.h"
#include "LightPropertiesUniformFactory.h"
#include "ModelViewProjectionMatrixUniformFactory.h"
#include "NormalMatrixUniformFactory.h"
#include "SunDirectionWCUniformFactory.h"
#include "SunDirectionECUniformFactory.h"
#include "ViewportOrthographicMatrixUniformFactory.h"
#include "CameraLightPositionUniformFactory.h"
#include "CameraEyeUniformFactory.h"
#include "CameraEyeHighModelCoordinatesUniformFactory.h"
#include "CameraEyeLowModelCoordinatesUniformFactory.h"
#include "CurrentFrustumUniformFactory.h"
#include "ProjectionMatrixUniformFactory.h"
#include "HighResolutionSnapScaleUniformFactory.h"
#include "InverseProjectionMatrixUniformFactory.h"
#include "InverseModelViewMatrixUniformFactory.h"
#include "ViewportTransformationMatrixUniformFactory.h"
#include "ViewportUniformFactory.h"
#include "MorphTimeUniformFactory.h"
#include "InverseModelMatrixUniformFactory.h"

#include "VertexAttributeDoubleVector3.h"
#include "VertexAttributeRGBA.h"
#include "VertexAttributeRGB.h"
#include "VertexAttributeByte.h"

#include "Log.h"

namespace argeo
{
	Device::Device()
		: g_shared_surface(nullptr)
	{
		// First we initialize the display
		EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
		
		if (display == EGL_NO_DISPLAY)
		{
			throw std::logic_error("No display available.");
		}

		// Initialize EGL
		if (!eglInitialize(display, 0, 0))
		{
			throw std::logic_error("Cannot initialize the display.");
		}

		g_shared_surface = create_pixel_buffer_surface(1, 1, nullptr);

		glGetIntegerv(
			GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
			&g_number_of_texture_units);

		glGetIntegerv(
			GL_MAX_VERTEX_ATTRIBS,
			&g_maximum_number_of_vertex_attributes);

		glGetIntegerv(
			GL_MAX_COLOR_ATTACHMENTS,
			&g_maximum_number_of_color_attachments);

		glGetIntegerv(
			GL_MAX_CUBE_MAP_TEXTURE_SIZE,
			&g_maximum_cube_map_size);

		glGetIntegerv(
			GL_MAX_TEXTURE_SIZE,
			&g_maximum_texture_size);

		///////////////////////////////////////////////////////////////

		g_draw_automatic_uniform_factory_collection =
			std::unique_ptr<AutomaticUniformFactoryCollection>(new AutomaticUniformFactoryCollection());

		AutomaticUniformFactory* factory;

		factory = new InverseModelMatrixUniformFactory();
		g_draw_automatic_uniform_factory_collection->emplace(factory->get_name(), std::unique_ptr<AutomaticUniformFactory>(factory));

        factory = new MorphTimeUniformFactory();
        g_draw_automatic_uniform_factory_collection->emplace(factory->get_name(), std::unique_ptr<AutomaticUniformFactory>(factory));

		factory = new SunDirectionECUniformFactory();
		g_draw_automatic_uniform_factory_collection->emplace(factory->get_name(), std::unique_ptr<AutomaticUniformFactory>(factory));

		factory = new InverseModelViewMatrixUniformFactory();
		g_draw_automatic_uniform_factory_collection->emplace(factory->get_name(), std::unique_ptr<AutomaticUniformFactory>(factory));

		factory = new NormalMatrixUniformFactory();
		g_draw_automatic_uniform_factory_collection->emplace(factory->get_name(), std::unique_ptr<AutomaticUniformFactory>(factory));

		factory = new CameraEyeHighModelCoordinatesUniformFactory();
		g_draw_automatic_uniform_factory_collection->emplace(factory->get_name(), std::unique_ptr<AutomaticUniformFactory>(factory));

		factory = new CameraEyeLowModelCoordinatesUniformFactory();
		g_draw_automatic_uniform_factory_collection->emplace(factory->get_name(), std::unique_ptr<AutomaticUniformFactory>(factory));

		factory = new ModelMatrixUniformFactory();
		g_draw_automatic_uniform_factory_collection->emplace(factory->get_name(), std::unique_ptr<AutomaticUniformFactory>(factory));

		factory = new InverseProjectionMatrixUniformFactory();
		g_draw_automatic_uniform_factory_collection->emplace(factory->get_name(), std::unique_ptr<AutomaticUniformFactory>(factory));

		factory = new CurrentFrustumUniformFactory();
		g_draw_automatic_uniform_factory_collection->emplace(factory->get_name(), std::unique_ptr<AutomaticUniformFactory>(factory));

		factory = new ModelViewMatrixUniformFactory();
		g_draw_automatic_uniform_factory_collection->emplace(factory->get_name(), std::unique_ptr<AutomaticUniformFactory>(factory));

		factory = new ProjectionMatrixUniformFactory();
		g_draw_automatic_uniform_factory_collection->emplace(factory->get_name(), std::unique_ptr<AutomaticUniformFactory>(factory));

		factory = new ModelViewRelativeToEyeMatrixUniformFactory();
		g_draw_automatic_uniform_factory_collection->emplace(factory->get_name(), std::unique_ptr<AutomaticUniformFactory>(factory));

		factory = new ModelViewProjectionMatrixUniformFactory();
		g_draw_automatic_uniform_factory_collection->emplace(factory->get_name(), std::unique_ptr<AutomaticUniformFactory>(factory));

		factory = new LightPropertiesUniformFactory();
		g_draw_automatic_uniform_factory_collection->emplace(factory->get_name(), std::unique_ptr<AutomaticUniformFactory>(factory));

		factory = new SunDirectionWCUniformFactory();
		g_draw_automatic_uniform_factory_collection->emplace(factory->get_name(), std::unique_ptr<AutomaticUniformFactory>(factory));

		factory = new ViewportOrthographicMatrixUniformFactory();
		g_draw_automatic_uniform_factory_collection->emplace(factory->get_name(), std::unique_ptr<AutomaticUniformFactory>(factory));

		factory = new ModelZToClipCoordinatesUniformFactory();
		g_draw_automatic_uniform_factory_collection->emplace(factory->get_name(), std::unique_ptr<AutomaticUniformFactory>(factory));

		factory = new CameraLightPositionUniformFactory();
		g_draw_automatic_uniform_factory_collection->emplace(factory->get_name(), std::unique_ptr<AutomaticUniformFactory>(factory));

		factory = new CameraEyeUniformFactory();
		g_draw_automatic_uniform_factory_collection->emplace(factory->get_name(), std::unique_ptr<AutomaticUniformFactory>(factory));

		factory = new CameraEyeUniformFactory();
		g_draw_automatic_uniform_factory_collection->emplace(factory->get_name(), std::unique_ptr<AutomaticUniformFactory>(factory));

		factory = new HighResolutionSnapScaleUniformFactory();
		g_draw_automatic_uniform_factory_collection->emplace(factory->get_name(), std::unique_ptr<AutomaticUniformFactory>(factory));

		factory = new ViewportTransformationMatrixUniformFactory();
		g_draw_automatic_uniform_factory_collection->emplace(factory->get_name(), std::unique_ptr<AutomaticUniformFactory>(factory));

		factory = new ViewportUniformFactory();
		g_draw_automatic_uniform_factory_collection->emplace(factory->get_name(), std::unique_ptr<AutomaticUniformFactory>(factory));

		g_texture_units = std::unique_ptr<TextureUnitsGL>(
			new TextureUnitsGL());

		g_texture_samplers =
			std::unique_ptr<TextureSamplers>(new TextureSamplers());

	}

	Device::~Device()
	{ }


	std::unique_ptr<ShaderProgram> Device::create_shader_program(
		std::string vertex_shader_source,
		std::string fragment_shader_source,
		ShaderVertexAttributeLocations vertex_attribute_locations)
	{
		return std::unique_ptr<ShaderProgram>(
			new ShaderProgramGL(
				vertex_shader_source,
				fragment_shader_source,
				vertex_attribute_locations));
	}


	std::unique_ptr<VertexBuffer> Device::create_vertex_buffer(
		BufferHint hint,
		int size_in_bytes)
	{
		return std::unique_ptr<VertexBuffer>(
			new VertexBufferGL(size_in_bytes, hint));
	}

	std::unique_ptr<IndexBuffer> Device::create_index_buffer(
		BufferHint hint,
		int size_in_bytes)
	{
		return std::unique_ptr<IndexBuffer>(
			new IndexBufferGL(size_in_bytes, hint));
	}

	std::unique_ptr<UniformBuffer> Device::create_uniform_buffer(
		BufferHint hint,
		int size_in_bytes)
	{
		return std::unique_ptr<UniformBuffer>(
			new UniformBufferGL(size_in_bytes, hint));
	}

	std::unique_ptr<Texture2DArray> Device::create_texture2D_array(TextureDescription description)
	{
		return std::unique_ptr<Texture2DArray>(
			new Texture2DArrayGL(description, TextureTarget::Texture2DArray));
	}

	std::unique_ptr<RenderBuffer> Device::create_render_buffer(
		RenderBufferDescription description)
	{
		return std::unique_ptr<RenderBuffer>(
			new RenderBufferGL(description));
	}

	std::unique_ptr<Texture2D> Device::create_texture2D(
		TextureDescription description,
		TextureSampler* sampler)
	{
		return std::unique_ptr<Texture2D>(
			new Texture2DGL(
				description,
				TextureTarget::Texture2D,
				sampler));
	}

	std::unique_ptr<Texture2D> Device::create_texture2D_rectangle(TextureDescription description)
	{
		return std::unique_ptr<Texture2D>(
			new Texture2DGL(description, TextureTarget::TextureRectangle));
	}

	std::unique_ptr<Texture2D> Device::create_texture2D_external(TextureDescription description)
	{
		return std::unique_ptr<Texture2D>(
			new Texture2DGL(description, TextureTarget::TextureExternal));
	}

	std::unique_ptr<ReadPixelBuffer> Device::create_read_pixel_buffer(
		const PixelBufferHint& hint,
		const std::size_t& size_in_bytes)
	{
		return std::unique_ptr<ReadPixelBuffer>(
			new ReadPixelBufferGL(hint, size_in_bytes));
	}

	std::unique_ptr<WritePixelBuffer> Device::create_write_pixel_buffer(
		const PixelBufferHint& hint,
		const std::size_t& size_in_bytes)
	{
		return std::unique_ptr<WritePixelBuffer>(
			new WritePixelBufferGL(hint, size_in_bytes));
	}

	std::unique_ptr<TextureSampler> Device::create_texture2D_sampler(
		TextureMinificationFilter minification_filter,
		TextureMagnificationFilter magnification_filter,
		TextureWrap wrap_s,
		TextureWrap wrap_t)
	{
		return std::unique_ptr<TextureSampler>(
			new TextureSamplerGL(
				minification_filter,
				magnification_filter,
				wrap_s,
				wrap_t,
				1));
	}

	std::unique_ptr<TextureSampler> Device::create_texture2D_sampler(
		TextureMinificationFilter minification_filter,
		TextureMagnificationFilter magnification_filter,
		TextureWrap wrap_s,
		TextureWrap wrap_t,
		float maximum_anisotropy)
	{
		return std::unique_ptr<TextureSampler>(
			new TextureSamplerGL(
				minification_filter,
				magnification_filter,
				wrap_s,
				wrap_t,
				maximum_anisotropy));
	}

	std::unique_ptr<Texture2D> Device::create_texture2D_rectangle(
		Bitmap* bitmap,
		TextureFormat format)
	{
		return create_texture2D_from_bitmap(
			bitmap,
			format,
			false,
			TextureTarget::TextureRectangle);
	}

	std::unique_ptr<CubeMap> Device::create_cube_map(
		TextureDescription description,
		const bool& generate_mipmaps,
		TextureSampler* sampler)
	{
		return std::unique_ptr<CubeMapGL>(new CubeMapGL(
			description,
			generate_mipmaps,
			sampler));
	}

	std::unique_ptr<Bitmap> Device::create_bitmap_from_text(
		std::string text,
		Font font)
	{
		return Bitmap::create_bitmap_from_text(
			text,
			font);
	}

	std::unique_ptr<Texture2D> Device::create_texture2D(
		Bitmap* bitmap,
		TextureFormat format,
		bool generate_mipmaps,
		TextureSampler* sampler,
		const bool& flip)
	{
		return create_texture2D_from_bitmap(
			bitmap,
			format,
			generate_mipmaps,
			TextureTarget::Texture2D,
		flip);
	}


	std::unique_ptr<Texture2D> Device::create_texture2D_from_bitmap(
		Bitmap* bitmap,
		TextureFormat format,
		bool generate_mipmaps,
		TextureTarget texture_target,
		const bool& flip)
	{
		std::unique_ptr<WritePixelBuffer> pixel_buffer = create_write_pixel_buffer(
			PixelBufferHint::Stream,
			bitmap->get_size_in_bytes());

		pixel_buffer->copy_from_bitmap(bitmap, flip);
		
		TextureDescription description(
			bitmap->get_width(),
			bitmap->get_height(),
			format,
			generate_mipmaps);

		std::unique_ptr<Texture2D> texture(new Texture2DGL(description, texture_target));

		texture->copy_from_buffer(
			pixel_buffer.get(),
			TextureHelper::imaging_pixel_format_to_image_format(bitmap->get_pixel_format()),
			TextureHelper::imaging_pixel_format_to_data_type(bitmap->get_pixel_format()));

		return texture;
	}

	std::unique_ptr<MeshBuffers> Device::create_mesh_buffers(
			Mesh* mesh,
            const ShaderVertexAttributeLocations& locations,
			const BufferHint& hint)
	{
		if (mesh == nullptr)
		{
			throw std::invalid_argument("mesh");
		}

		MeshBuffers* mesh_buffers = new MeshBuffers(false);

		if (mesh->get_indices() != nullptr)
		{
			if (mesh->get_indices()->get_type() == IndicesType::UnsignedShort)
			{
				std::vector<unsigned short> mesh_indices = static_cast<IndicesUnsignedShort*>(mesh->get_indices())->get_values();

				std::unique_ptr<IndexBuffer> index_buffer =
						create_index_buffer(hint, mesh_indices.size() * sizeof(unsigned short));

				index_buffer->copy_from_system(mesh_indices);

				// Release the unique ptr and assign the raw to the buffers.
				mesh_buffers->set_index_buffer(index_buffer.release());
			}
			else
			{
				std::vector<unsigned int> mesh_indices = static_cast<IndicesUnsignedInt*>(mesh->get_indices())->get_values();

				std::unique_ptr<IndexBuffer> index_buffer =
						create_index_buffer(hint, mesh_indices.size() * sizeof(unsigned int));

				index_buffer->copy_from_system(mesh_indices);

				// Release the unique ptr and assign the raw to the buffers.
				mesh_buffers->set_index_buffer(index_buffer.release());
			}
		}

		//
		// Emulated double precision vectors are a special case:  one mesh vertex attribute
		// yields two shader vertex attributes.  As such, these are handled separately before
		// normal attributes.
		//
		std::unordered_set<std::string> ignore_attributes;
		VertexAttributeCollection attributes = mesh->get_attributes();
		for (auto attribute : attributes)
		{
			VertexAttributeDoubleVector3* emulated = dynamic_cast<VertexAttributeDoubleVector3*>(attribute.second);

            if (emulated != nullptr)
			{
				int high_location = -1;
				int low_location = -1;

                if (locations.find(emulated->get_name() + "High") != locations.end())
                {
                    high_location = locations.at(emulated->get_name() + "High");
                }

                if (locations.find(emulated->get_name() + "Low") != locations.end())
                {
                    low_location = locations.at(emulated->get_name() + "Low");
                }

				if ((high_location == -1) && (low_location == -1))
				{
					// The shader did not have either attribute.  No problem.
					continue;
				}
				else if ((high_location == -1) || (low_location == -1))
				{
					throw std::invalid_argument("An emulated double vec3 mesh attribute requires both " + emulated->get_name() + "High and " + emulated->get_name() + "Low vertex attributes, but the shader only contains one matching attribute.");
				}

				// Copy both high and low parts into a single vertex buffer.
				std::vector<vec3d> values = emulated->get_values();

				std::vector<vec3> vertices(2 * values.size());

				int j = 0;
				for (int i = 0; i < values.size(); ++i)
				{
					evec3d v(values[i]);
					vertices[j++] = v.high;
					vertices[j++] = v.low;
				}

				VertexBuffer* vertex_buffer = Device::create_vertex_buffer(
						hint,
						vertices.size() * sizeof(vec3)).release();
				vertex_buffer->copy_from_system(vertices);

				int stride = 2 * sizeof(vec3);

				mesh_buffers->get_attributes()->set_attribute(
						new VertexBufferAttribute(vertex_buffer, ComponentDataType::Float, 3, false, 0, stride),
						high_location);

				mesh_buffers->get_attributes()->set_attribute(
						new VertexBufferAttribute(vertex_buffer, ComponentDataType::Float, 3, false, sizeof(vec3), stride),
						low_location);

				ignore_attributes.insert(emulated->get_name() + "High");
				ignore_attributes.insert(emulated->get_name() + "Low");
			}
		}

		for (auto entry : locations)
		{
			if (ignore_attributes.find(entry.first) != ignore_attributes.end())
			{
				continue;
			}

            if (attributes.find(entry.first) == attributes.end())
            {
                continue;
            }

            IVertexAttribute* attribute = mesh->get_attributes()[entry.first];

			std::unique_ptr<VertexBuffer> vertex_buffer;
			std::vector<vec3d> values;
			std::vector<vec3>  values_array;

			switch (attribute->get_type())
			{

				case VertexAttributeType::EmulatedDoubleVector3:
					values = static_cast<VertexAttribute<vec3d>*>(attribute)->get_values();
					values_array.resize(values.size());
					for (int i = 0; i < values.size(); ++i)
					{
						values_array[i] = to_vec3(values[i]);
					}

					vertex_buffer = Device::create_vertex_buffer(
							hint,
							values_array.size() * sizeof(vec3));
					vertex_buffer->copy_from_system(values_array);

					mesh_buffers->get_attributes()->set_attribute(
							new VertexBufferAttribute(vertex_buffer.release(), ComponentDataType::Float, 3),
							entry.second);
					break;

				case VertexAttributeType::HalfFloat:
				case VertexAttributeType::HalfFloatVector2:
				case VertexAttributeType::HalfFloatVector3:
				case VertexAttributeType::HalfFloatVector4:
					break;

				case VertexAttributeType::Float:
					vertex_buffer = create_vertex_buffer(((VertexAttribute< float >*)attribute)->get_values(), hint);
					(*mesh_buffers->get_attributes())[entry.second] =
							new VertexBufferAttribute(vertex_buffer.release(), ComponentDataType::Float, 1);
					break;

				case VertexAttributeType::FloatVector2:
					vertex_buffer = create_vertex_buffer(((VertexAttribute< vec2 >*)attribute)->get_values(), hint);
					(*mesh_buffers->get_attributes())[entry.second] =
							new VertexBufferAttribute(vertex_buffer.release(), ComponentDataType::Float, 2);
					break;

				case VertexAttributeType::FloatVector3:
					vertex_buffer = create_vertex_buffer(((VertexAttribute< vec3 >*)attribute)->get_values(), hint);
					(*mesh_buffers->get_attributes())[entry.second] =
							new VertexBufferAttribute(vertex_buffer.release(), ComponentDataType::Float, 3);
					break;

				case VertexAttributeType::FloatVector4:
					vertex_buffer = create_vertex_buffer(((VertexAttribute< vec4 >*)attribute)->get_values(), hint);
					(*mesh_buffers->get_attributes())[entry.second] =
							new VertexBufferAttribute(vertex_buffer.release(), ComponentDataType::Float, 4);
					break;

				case VertexAttributeType::UnsignedByte:
					if (VertexAttributeRGBA* v = dynamic_cast<VertexAttributeRGBA*>(attribute))
					{
						vertex_buffer = create_vertex_buffer(((VertexAttribute< BlittableRGBA >*)attribute)->get_values(), hint);
						(*mesh_buffers->get_attributes())[entry.second] =
								new VertexBufferAttribute(vertex_buffer.release(), ComponentDataType::UnsignedByte, 4, true, 0, 0);
					}

					if (VertexAttributeRGB* v = dynamic_cast<VertexAttributeRGB*>(attribute))
					{
						vertex_buffer = create_vertex_buffer(((VertexAttribute< unsigned char >*)attribute)->get_values(), hint);
						(*mesh_buffers->get_attributes())[entry.second] =
								new VertexBufferAttribute(vertex_buffer.release(), ComponentDataType::UnsignedByte, 3, true, 0, 0);
					}
					else
					{
						vertex_buffer = create_vertex_buffer(((VertexAttribute< unsigned char >*)attribute)->get_values(), hint);
						(*mesh_buffers->get_attributes())[entry.second] =
								new VertexBufferAttribute(vertex_buffer.release(), ComponentDataType::UnsignedByte, 1);
					}
					break;
			}
		}

		return std::unique_ptr<MeshBuffers>(mesh_buffers);
	}

	std::unique_ptr<MeshBuffers> Device::create_mesh_buffers(
		Mesh* mesh,
		ShaderVertexAttributeCollection* shader_attributes,
		const BufferHint& hint)
	{
		if (mesh == nullptr)
		{
			throw std::invalid_argument("mesh");
		}

		if (shader_attributes == nullptr)
		{
			throw std::invalid_argument("shaderAttributes");
		}

		MeshBuffers* mesh_buffers = new MeshBuffers(false);

		if (mesh->get_indices() != nullptr)
		{
			if (mesh->get_indices()->get_type() == IndicesType::UnsignedShort)
			{
				std::vector<unsigned short> mesh_indices = static_cast<IndicesUnsignedShort*>(mesh->get_indices())->get_values();

				std::unique_ptr<IndexBuffer> index_buffer =
					create_index_buffer(hint, mesh_indices.size() * sizeof(unsigned short));

				index_buffer->copy_from_system(mesh_indices);

				// Release the unique ptr and assign the raw to the buffers.
				mesh_buffers->set_index_buffer(index_buffer.release());
			}
			else
			{
				std::vector<unsigned int> mesh_indices = static_cast<IndicesUnsignedInt*>(mesh->get_indices())->get_values();

				std::unique_ptr<IndexBuffer> index_buffer =
					create_index_buffer(hint, mesh_indices.size() * sizeof(unsigned int));

				index_buffer->copy_from_system(mesh_indices);

				// Release the unique ptr and assign the raw to the buffers.
				mesh_buffers->set_index_buffer(index_buffer.release());
			}
		}

		//
		// Emulated double precision vectors are a special case:  one mesh vertex attribute
		// yields two shader vertex attributes.  As such, these are handled separately before
		// normal attributes.
		//
		std::unordered_set<std::string> ignore_attributes;
		VertexAttributeCollection attributes = mesh->get_attributes();
		for (auto attribute : attributes)
		{
			VertexAttributeDoubleVector3* emulated = dynamic_cast<VertexAttributeDoubleVector3*>(attribute.second);
			if (emulated != nullptr)
			{
				int high_location = -1;
				int low_location = -1;

				for (ShaderVertexAttributeCollection::iterator it = shader_attributes->begin();
					it != shader_attributes->end();
					it++)
				{
					ShaderVertexAttribute* shader_attribute = it->second;

					if (shader_attribute->get_name() == emulated->get_name() + "High")
					{
						high_location = shader_attribute->get_location();
					}
					else if (shader_attribute->get_name() == emulated->get_name() + "Low")
					{
						low_location = shader_attribute->get_location();
					}

					if ((high_location != -1) && (low_location != -1))
					{
						break;
					}
				}

				if ((high_location == -1) && (low_location == -1))
				{
					// The shader did not have either attribute.  No problem.
					continue;
				}
				else if ((high_location == -1) || (low_location == -1))
				{
					throw std::invalid_argument("An emulated double vec3 mesh attribute requires both " + emulated->get_name() + "High and " + emulated->get_name() + "Low vertex attributes, but the shader only contains one matching attribute.");
				}

				// Copy both high and low parts into a single vertex buffer.
				std::vector<vec3d> values = emulated->get_values();

				std::vector<vec3> vertices(2 * values.size());

				int j = 0;
				for (int i = 0; i < values.size(); ++i)
				{
					evec3d v(values[i]);
					vertices[j++] = v.high;
					vertices[j++] = v.low;
				}

				VertexBuffer* vertex_buffer = Device::create_vertex_buffer(
					hint,
					vertices.size() * sizeof(vec3)).release();
				vertex_buffer->copy_from_system(vertices);

				int stride = 2 * sizeof(vec3);

				mesh_buffers->get_attributes()->set_attribute(
					new VertexBufferAttribute(vertex_buffer, ComponentDataType::Float, 3, false, 0, stride),
					high_location);

				mesh_buffers->get_attributes()->set_attribute(
					new VertexBufferAttribute(vertex_buffer, ComponentDataType::Float, 3, false, sizeof(vec3), stride),
					low_location);

				ignore_attributes.insert(emulated->get_name() + "High");
				ignore_attributes.insert(emulated->get_name() + "Low");
			}
		}

		for (auto entry : *shader_attributes)
		{
			ShaderVertexAttribute* shader_attribute = entry.second;

			if (ignore_attributes.find(shader_attribute->get_name()) != ignore_attributes.end())
			{
				continue;
			}

			if (attributes.find(shader_attribute->get_name()) == attributes.end())
			{
				continue;
			}

			IVertexAttribute* attribute = mesh->get_attributes()[shader_attribute->get_name()];

			std::unique_ptr<VertexBuffer> vertex_buffer;
			std::vector<vec3d> values;
			std::vector<vec3>  values_array;
			switch (attribute->get_type())
			{

			case VertexAttributeType::EmulatedDoubleVector3:
				values = static_cast<VertexAttribute<vec3d>*>(attribute)->get_values();
				values_array.resize(values.size());
				for (int i = 0; i < values.size(); ++i)
				{
					values_array[i] = to_vec3(values[i]);
				}

				vertex_buffer = Device::create_vertex_buffer(
					hint,
					values_array.size() * sizeof(vec3));
				vertex_buffer->copy_from_system(values_array);

				mesh_buffers->get_attributes()->set_attribute(
					new VertexBufferAttribute(vertex_buffer.release(), ComponentDataType::Float, 3),
					shader_attribute->get_location());

				break;

			case VertexAttributeType::HalfFloat:
			case VertexAttributeType::HalfFloatVector2:
			case VertexAttributeType::HalfFloatVector3:
			case VertexAttributeType::HalfFloatVector4:
				break;

			case VertexAttributeType::Float:
				vertex_buffer = create_vertex_buffer(((VertexAttribute< float >*)attribute)->get_values(), hint);
				(*mesh_buffers->get_attributes())[shader_attribute->get_location()] =
					new VertexBufferAttribute(vertex_buffer.release(), ComponentDataType::Float, 1);
				break;

			case VertexAttributeType::FloatVector2:
				vertex_buffer = create_vertex_buffer(((VertexAttribute< vec2 >*)attribute)->get_values(), hint);
				(*mesh_buffers->get_attributes())[shader_attribute->get_location()] =
					new VertexBufferAttribute(vertex_buffer.release(), ComponentDataType::Float, 2);
				break;

			case VertexAttributeType::FloatVector3:
				vertex_buffer = create_vertex_buffer(((VertexAttribute< vec3 >*)attribute)->get_values(), hint);
				(*mesh_buffers->get_attributes())[shader_attribute->get_location()] =
					new VertexBufferAttribute(vertex_buffer.release(), ComponentDataType::Float, 3);
				break;

			case VertexAttributeType::FloatVector4:
				vertex_buffer = create_vertex_buffer(((VertexAttribute< vec4 >*)attribute)->get_values(), hint);
				(*mesh_buffers->get_attributes())[shader_attribute->get_location()] =
					new VertexBufferAttribute(vertex_buffer.release(), ComponentDataType::Float, 4);
				break;

			case VertexAttributeType::UnsignedByte:
				if (VertexAttributeRGBA* v = dynamic_cast<VertexAttributeRGBA*>(attribute))
				{
					vertex_buffer = create_vertex_buffer(((VertexAttribute< BlittableRGBA >*)attribute)->get_values(), hint);
					(*mesh_buffers->get_attributes())[shader_attribute->get_location()] =
						new VertexBufferAttribute(vertex_buffer.release(), ComponentDataType::UnsignedByte, 4, true, 0, 0);
				}

				if (VertexAttributeRGB* v = dynamic_cast<VertexAttributeRGB*>(attribute))
				{
					vertex_buffer = create_vertex_buffer(((VertexAttribute< unsigned char >*)attribute)->get_values(), hint);
					(*mesh_buffers->get_attributes())[shader_attribute->get_location()] =
						new VertexBufferAttribute(vertex_buffer.release(), ComponentDataType::UnsignedByte, 3, true, 0, 0);
				}
				else
				{
					vertex_buffer = create_vertex_buffer(((VertexAttribute< unsigned char >*)attribute)->get_values(), hint);
					(*mesh_buffers->get_attributes())[shader_attribute->get_location()] =
						new VertexBufferAttribute(vertex_buffer.release(), ComponentDataType::UnsignedByte, 1);
				}
				break;
			}
		}

		return std::unique_ptr<MeshBuffers>(mesh_buffers);
	}

	std::unique_ptr<SurfaceWindow> Device::create_surface_window(void* reserved)
	{
		return create_surface_window(
			Device::get_instance().g_shared_surface.get()->get_context(),
			reserved);
	}

	std::unique_ptr<SurfaceWindow> Device::create_surface_window(
		Context* shared_context,
		void* reserved)
	{
		return std::unique_ptr<SurfaceWindow>(new SurfaceWindowEGL(
			shared_context, 
			static_cast<NativeWindowType>(reserved)));
	}

	std::unique_ptr<PixelBufferSurface> Device::create_pixel_buffer_surface(int width, int height)
	{
		return create_pixel_buffer_surface(
			width,
			height,
			Device::get_instance().g_shared_surface.get()->get_context());
	}

	std::unique_ptr<PixelBufferSurface> Device::create_pixel_buffer_surface(int width, int height, Context* shared_context)
	{
		return std::unique_ptr<PixelBufferSurface>(
			new PixelBufferSurfaceEGL(width, height, shared_context));
	}

	std::unique_ptr<Context> Device::create_context(
		Surface* surface, 
		int width,
		int height)
	{
		return create_context(
			surface,
			width,
			height,
			Device::get_instance().g_shared_surface.get()->get_context());
	}

	std::unique_ptr<Context> Device::create_context(
		Surface* surface,
		int width,
		int height,
		Context* shared_context)
	{
		return std::unique_ptr<Context>(
			new ContextEGL(surface, shared_context, width, height));
	}

	std::string Device::create_vertex_outline_shader_source(
		std::string source)
	{
		return ShaderObjectGL::create_vertex_outline_shader_source(source);
	}

	std::string Device::create_fragment_outline_shader_source(
		std::string source)
	{
		return ShaderObjectGL::create_fragment_outline_shader_source(source);
	}

	std::string Device::create_fragment_pick_shader_source(
		std::string source,
		std::string pick_color_qualifier)
	{
		return ShaderObjectGL::create_fragment_pick_shader_source(
			source,
			pick_color_qualifier);
	}

	std::unique_ptr<Fence> Device::create_fence()
	{
		return std::unique_ptr<Fence>(
			new FenceGL());
	}

	void Device::finish()
	{
		glFinish();
	}

	void Device::flush()
	{
		glFlush();
	}

	TextureUnits* Device::get_texture_units()
	{
		return Device::get_instance().g_texture_units.get();
	}

	TextureSamplers* Device::get_texture_samplers()
	{
		return Device::get_instance().g_texture_samplers.get();
	}

	AutomaticUniformFactoryCollection* Device::get_draw_automatic_uniform_factories()
	{
		return Device::get_instance().g_draw_automatic_uniform_factory_collection.get();
	}

	int Device::get_maximum_number_of_vertex_attributes()
	{
		return Device::get_instance().g_maximum_number_of_vertex_attributes;
	}

	int Device::get_maximum_number_of_color_attachments()
	{
		return Device::get_instance().g_maximum_number_of_color_attachments;
	}

	int Device::get_number_of_texture_units()
	{
		return Device::get_instance().g_number_of_texture_units;
	}

	int Device::get_maximum_cube_map_size()
	{
		return Device::get_instance().g_maximum_cube_map_size;
	}

	int Device::get_maximum_texture_size()
	{
		return Device::get_instance().g_maximum_texture_size;
	}

	Device& Device::get_instance()
	{
		static Device instance;
		return instance;
	}
}