#pragma once

#include "ShaderProgram.h"
#include "ShaderObject.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "UniformBuffer.h"
#include "MeshBuffers.h"
#include "Mesh.h"
#include "Texture2D.h"
#include "Texture2DArray.h"
#include "TextureSampler.h"
#include "TextureMagnificationFilter.h"
#include "TextureMinificationFilter.h"
#include "TextureWrap.h"
#include "Fence.h"
#include "SurfaceWindow.h"
#include "PixelBufferSurface.h"
#include "Bitmap.h"
#include "Font.h"
#include "TextureUnits.h"
#include "ShaderVertexAttributeLocations.h"
#include "RenderBuffer.h"
#include "TextureSamplers.h"
#include "TextureUnits.h"
#include "CubeMap.h"

#include "AutomaticUniformFactoryCollection.h"

#include <string>
#include <vector>
#include <memory>

namespace argeo
{
	class TextureSamplers;

	class Device
	{
	public:

		static Device& get_instance();

		// Create a new shader program with a vertex a fragment and a geometry shader.
		// The shaders objects are created using the given sources.
		// The owner of the pointer is now the caller.
		static std::unique_ptr<ShaderProgram> create_shader_program(
			std::string vertex_shader_source,
			std::string fragment_shader_source,
			ShaderVertexAttributeLocations vertex_attribute_locations = ShaderVertexAttributeLocations());

		// Create a new vertex buffer of data < T > from the given vector.
		// Utilizes the given buffer hint for the creation of the buffer.
		// The owner of the pointer is now the caller.
		template<class T>
		static std::unique_ptr<VertexBuffer> create_vertex_buffer(
			std::vector<T> data,
			BufferHint hint);

		// Create a new vertex buffer with the given size.
		// Utilizes the given buffer hint for the creation of the buffer.
		// The owner of the pointer is now the caller.
		static std::unique_ptr<VertexBuffer> create_vertex_buffer(
			BufferHint hint,
			int size_in_bytes);

		// Create a new index buffer with the given size.
		// Utilizes the given buffer hint for the creation of the buffer.
		// The owner of the pointer is now the caller.
		static std::unique_ptr<IndexBuffer> create_index_buffer(
			BufferHint hint,
			int size_in_bytes);

		// Create a new uniform buffer with the given size.
		// Utilizes the given buffer hint for the creation of the buffer.
		// The owner of the pointer is now the caller.
		static std::unique_ptr<UniformBuffer> create_uniform_buffer(
			BufferHint hint,
			int size_in_bytes);

		// Create a 2D texture array using the given description.
		// The owner of the pointer is now the caller.
		static std::unique_ptr<Texture2DArray> create_texture2D_array(TextureDescription description);

		// Create a 2D texture using the given description.
		// The owner of the pointer is now the caller.
		static std::unique_ptr<RenderBuffer> create_render_buffer(
			RenderBufferDescription description);

		// Create a 2D texture using the given description.
		// The owner of the pointer is now the caller.
		static std::unique_ptr<Texture2D> create_texture2D(
			TextureDescription description,
			TextureSampler* sampler = nullptr);


		static std::unique_ptr<WritePixelBuffer> create_write_pixel_buffer(
			const PixelBufferHint& hint,
			const std::size_t& size_in_bytes);

		static std::unique_ptr<ReadPixelBuffer> create_read_pixel_buffer(
			const PixelBufferHint& hint,
			const std::size_t& size_in_bytes);

		// Create a 2D texture rectangle using the given description.
		// The owner of the pointer is now the caller.
		static std::unique_ptr<Texture2D> create_texture2D_rectangle(TextureDescription description);

		static std::unique_ptr<Texture2D> create_texture2D_external(TextureDescription description);
		
		static std::unique_ptr<CubeMap>  create_cube_map(
			TextureDescription description,
			const bool& generate_mipmaps = false,
			TextureSampler* sampler      = nullptr);

		static std::unique_ptr<Texture2D> create_texture2D(
			Bitmap* bitmap,
			TextureFormat format,
			bool generate_mipmaps   = false,
			TextureSampler* sampler = nullptr,
            const bool& flip = false);

		static std::unique_ptr<Texture2D> create_texture2D_rectangle(
			Bitmap* bitmap,
			TextureFormat format);

		static std::unique_ptr<Bitmap> create_bitmap_from_text(
			std::string text, 
			Font font);

		static std::string create_vertex_outline_shader_source(
			std::string source);

		static std::string create_fragment_outline_shader_source(
			std::string source);

		static std::string create_fragment_pick_shader_source(
			std::string source,
			std::string pick_color_qualifier);

		//static std::unique_ptr<Bitmap> create_bitmap_from_point(int radius_in_pixels);

		static std::unique_ptr<MeshBuffers> create_mesh_buffers(
                Mesh* mesh,
                const ShaderVertexAttributeLocations& locations,
                const BufferHint& hint);

		static std::unique_ptr<MeshBuffers> create_mesh_buffers(
			Mesh* mesh,
			ShaderVertexAttributeCollection* shader_attributes,
			const BufferHint& hint);

		static std::unique_ptr<TextureSampler> create_texture2D_sampler(
			TextureMinificationFilter minification_filter,
			TextureMagnificationFilter magnification_filter,
			TextureWrap wrap_s,
			TextureWrap wrap_t);

		static std::unique_ptr<TextureSampler> create_texture2D_sampler(
			TextureMinificationFilter minification_filter,
			TextureMagnificationFilter magnification_filter,
			TextureWrap wrap_s,
			TextureWrap wrap_t,
			float maximum_anistropy);

		static std::unique_ptr<Fence> create_fence();

		static void finish();
		static void flush();

		static std::unique_ptr<Context> create_context(
			Surface* surface,
			int width,
			int height);

		static std::unique_ptr<Context> create_context(
			Surface* surface,
			int width,
			int height,
			Context* shared_context);

		static std::unique_ptr<SurfaceWindow> create_surface_window(void* reserved = nullptr);
		static std::unique_ptr<SurfaceWindow> create_surface_window(
			Context* shared_context,
			void*    reserved = nullptr);

		static std::unique_ptr<PixelBufferSurface> create_pixel_buffer_surface(int width, int height);
		static std::unique_ptr<PixelBufferSurface> create_pixel_buffer_surface(int width, int height, Context* shared_context);

		// A set of commonly used texture samplers.
		// Gets the current texture units.
		static TextureUnits* get_texture_units();

		static TextureSamplers* get_texture_samplers();
		static AutomaticUniformFactoryCollection* get_draw_automatic_uniform_factories();

		static int get_maximum_texture_size();
		static int get_maximum_number_of_vertex_attributes();
		static int get_maximum_number_of_color_attachments();
		static int get_number_of_texture_units();
		static int get_maximum_cube_map_size();

		public:
			// C++ 11
			// =======
			// We can use the better technique of deleting the methods
			// we don't want.
			Device(Device const&) = delete;
			void operator=(Device const&) = delete;


	private:
		Device();
		~Device();

	private:

		static std::unique_ptr<Texture2D> create_texture2D_from_bitmap(
			Bitmap* bitmap,
			TextureFormat format,
			bool generate_mipmaps,
			TextureTarget texture_target,
			const bool& flip = false);


		std::unique_ptr<TextureUnits> g_texture_units;
		std::unique_ptr<TextureSamplers>  g_texture_samplers;
		std::unique_ptr<AutomaticUniformFactoryCollection> g_draw_automatic_uniform_factory_collection;
		std::unique_ptr<Surface> g_shared_surface;

		int g_maximum_number_of_vertex_attributes;
		int g_number_of_texture_units;
		int g_maximum_number_of_color_attachments;
		int g_maximum_cube_map_size;
		int g_maximum_texture_size;
	};	
	
	 template<class T>
	 inline std::unique_ptr<VertexBuffer> Device::create_vertex_buffer(
		 std::vector<T> values,
		 BufferHint hint)
	{
		std::unique_ptr<VertexBuffer> vertexBuffer = create_vertex_buffer(hint, values.size() * sizeof(T));
		vertexBuffer->copy_from_system<T>(values);
		return vertexBuffer;
	}
}
