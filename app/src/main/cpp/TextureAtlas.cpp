#include "TextureAtlas.h"

#include "TextureHelper.h"
#include "Device.h"
#include "UniformSampler.h"
#include "guid.h"

#include "android/include/JNIHelper.h"
#include <future>


namespace argeo
{
	TextureAtlas::TextureAtlas(
		Context* context,
		const vec2i& initial_size,
		const unsigned int& border_width_in_pixels,
		const TextureFormat& texture_format)
		: m_context(context)
		, m_border_width_in_pixels(border_width_in_pixels)
		, m_texture_format(texture_format)
	{ 
		if (context == nullptr) 
		{
			throw std::invalid_argument("context is required.");
		}

		if (initial_size.x < 1 || initial_size.y < 1)
		{
			throw std::invalid_argument("initialSize must be greater than zero.");
		}

		// Create initial texture and root.
		m_texture = Device::create_texture2D(TextureDescription(
			initial_size.x,
			initial_size.y,
			texture_format));

		m_root = std::unique_ptr<TextureAtlasNode>(new TextureAtlasNode(
			vec2i::zero(),
			vec2i(initial_size.x, initial_size.y)));

		m_uniform_map["u_texture"] = [&] (IUniform* uniform)
		{
			static_cast<UniformSampler*>(uniform)->set_texture(m_texture.get());
		};

		std::string fs =
			R"(
			uniform sampler2D u_texture;
			in  vec2 fsTextureCoordinates;
			out vec4 fragColor;

			void main()
			{
			    fragColor = texture(u_texture, fsTextureCoordinates);
			}
			)";


		m_copy_command = std::unique_ptr<DrawCommand>(context->create_viewport_quad_command(
			fs,
			nullptr,
			&m_uniform_map));

		std::ostringstream stream;

#ifdef __ANDROID__
		JNIEnv* jni;
		jint status = JNIHelper::get_instance().attach_current_thread(&jni);
		GuidGenerator generator(jni);
		stream << generator.newGuid();
		JNIHelper::get_instance().detach_current_thread(status);
#else
		GuidGenerator generator;
		stream << generator.newGuid();
#endif

		m_guid = stream.str();
	}


	TextureAtlas::~TextureAtlas()
	{ }

	std::string TextureAtlas::get_guid() const
	{
		return m_guid;
	}

	void TextureAtlas::resize_atlas(Bitmap* image)
	{
		int scaling_factor = 2;
		
		if (get_number_of_images() > 0) 
		{
			int old_atlas_width  = m_texture->get_description().get_width();
			int old_atlas_height = m_texture->get_description().get_height();
			int atlas_width      = scaling_factor * (old_atlas_width  + image->get_width()  + m_border_width_in_pixels);
			int atlas_height     = scaling_factor * (old_atlas_height + image->get_height() + m_border_width_in_pixels);
			
			double one_over_atlas_width  = 1.0 / static_cast<double>(atlas_width);
			double one_over_atlas_height = 1.0 / static_cast<double>(atlas_height);

			double width_ratio  = static_cast<double>(old_atlas_width)  * one_over_atlas_width;
			double height_ratio = static_cast<double>(old_atlas_height) * one_over_atlas_height;

			// Create new node structure, putting the old root node in the bottom left.
			TextureAtlasNode* node_bottom_right = new TextureAtlasNode(
				vec2i(old_atlas_width + m_border_width_in_pixels, 0),
				vec2i(atlas_width, old_atlas_height));

			TextureAtlasNode* node_bottom_half = new TextureAtlasNode(
				vec2i::zero(),
				vec2i(atlas_width, old_atlas_height),
				std::move(m_root),
				std::unique_ptr<TextureAtlasNode>(node_bottom_right));

			TextureAtlasNode* node_top_half = new TextureAtlasNode(
				vec2i(0, old_atlas_height + m_border_width_in_pixels),
				vec2i(atlas_width, atlas_height));

			TextureAtlasNode* node_main = new TextureAtlasNode(
				vec2i::zero(),
				vec2i(atlas_width, atlas_height),
				std::unique_ptr<TextureAtlasNode>(node_bottom_half),
				std::unique_ptr<TextureAtlasNode>(node_top_half));

			m_root = std::unique_ptr<TextureAtlasNode>(node_main);

			// Resize texture coordinates.
			for(int i = 0; i < m_texture_coordinates.size(); i++)
			{
				RectangleD tex_coord     = m_texture_coordinates.at(i);
				m_texture_coordinates[i] = RectangleD(
					tex_coord.get_left()   * width_ratio,
					tex_coord.get_bottom() * height_ratio,
					tex_coord.get_width()  * width_ratio,
					tex_coord.get_height() * height_ratio);
			}

			// Copy larger texture.
			std::unique_ptr<Texture2D> new_texture = Device::create_texture2D(TextureDescription(
				atlas_width,
				atlas_height,
				m_texture_format));

			std::unique_ptr<FrameBuffer> framebuffer = m_context->create_frame_buffer();
			framebuffer->get_color_attachments()->set_attachment(0, new_texture.get());

			RenderStateParameters render_params;
			render_params.depth_test.enabled = false;
			render_params.viewport = Rectangle(0, 0, old_atlas_width, old_atlas_height);
			
			m_copy_command->set_render_state(RenderState::from_cache(render_params));
			m_copy_command->set_framebuffer(framebuffer.get());
			m_copy_command->execute(m_context);

			m_texture = std::move(new_texture);
			RenderState::remove_from_cache(render_params);
		}
		else 
		{
			// First image exceeds initialSize
			int initial_width  = scaling_factor * (image->get_width()  + m_border_width_in_pixels);
			int initial_height = scaling_factor * (image->get_height() + m_border_width_in_pixels);

			m_texture = Device::create_texture2D(TextureDescription(
				initial_width,
				initial_height,
				m_texture_format));
			
			m_root = std::unique_ptr<TextureAtlasNode>(new TextureAtlasNode(
				vec2i::zero(),
				vec2i(initial_width, initial_height)));
		}
	}


	TextureAtlasNode* TextureAtlas::find_node(
		TextureAtlasNode* node,
		Bitmap* image)
	{
		if (node == nullptr)
		{
			return nullptr;
		}

		// If a leaf node
		if (node->child_node1 == nullptr &&
			node->child_node2 == nullptr)
		{
			// Node already contains an image, don't add to it.
			if (node->image_index != INDEX_NOT_DEFINED)
			{
				return nullptr;
			}

			int node_width  = node->top_right.x  - node->bottom_left.x;
			int node_height = node->top_right.y  - node->bottom_left.y;
			int width_difference  = node_width   - image->get_width();
			int height_difference = node_height  - image->get_height();

			// Node is smaller than the image.
			if (width_difference < 0 || height_difference < 0)
			{
				return nullptr;
			}

			// If the node is the same size as the image, return the node
			if (width_difference == 0 && height_difference == 0) 
			{
				return node;
			}

			// Vertical split (childNode1 = left half, childNode2 = right half).
			if (width_difference > height_difference)
			{
				node->child_node1 = std::unique_ptr<TextureAtlasNode>(new TextureAtlasNode(
					vec2i(node->bottom_left.x, node->bottom_left.y), 
					vec2i(node->bottom_left.x + image->get_width(), node->top_right.y)));

				// Only make a second child if the border gives enough space.
				int child_node2_bottom_left_x = 
					node->bottom_left.x + image->get_width() + m_border_width_in_pixels;

				if (child_node2_bottom_left_x < node->top_right.x)
				{
					node->child_node2 = std::unique_ptr<TextureAtlasNode>(new TextureAtlasNode(
						vec2i(child_node2_bottom_left_x, node->bottom_left.y),
						vec2i(node->top_right.x, node->top_right.y)));
				}
			}
			// Horizontal split (childNode1 = bottom half, childNode2 = top half).
			else 
			{
				node->child_node1 = std::unique_ptr<TextureAtlasNode>(new TextureAtlasNode(
					vec2i(node->bottom_left.x, node->bottom_left.y),
					vec2i(node->top_right.x, node->bottom_left.y + image->get_height())));

				// Only make a second child if the border gives enough space.
				int child_node2_bottom_left_y = 
					node->bottom_left.y + image->get_height() + m_border_width_in_pixels;
				if (child_node2_bottom_left_y < node->top_right.y)
				{
					node->child_node2 = std::unique_ptr<TextureAtlasNode>(new TextureAtlasNode(
						vec2i(node->bottom_left.x, child_node2_bottom_left_y),
						vec2i(node->top_right.x, node->top_right.y)));
				}
			}

			return find_node(node->child_node1.get(), image);
		}

		// If not a leaf node
		TextureAtlasNode* found = nullptr;
		found = find_node(node->child_node1.get(), image);
		if (found == nullptr)
		{
			found = find_node(node->child_node2.get(), image);
		}
		return found;
	}

	void TextureAtlas::add_image(
		Bitmap* image,
		const unsigned int& index)
	{
		TextureAtlasNode* node = find_node(m_root.get(), image);

		if (node != nullptr) 
		{
			// Found a node that can hold the image.
			node->image_index = index;

			// Add texture coordinate and write to texture
			int atlas_width  = m_texture->get_description().get_width();
			int atlas_height = m_texture->get_description().get_height();
			int node_width   = node->top_right.x - node->bottom_left.x;
			int node_height  = node->top_right.y - node->bottom_left.y;

			double one_over_atlas_width  = 1.0 / static_cast<double>(atlas_width);
			double one_over_atlas_height = 1.0 / static_cast<double>(atlas_height);

			double x = static_cast<double>(node->bottom_left.x) * one_over_atlas_width;
			double y = static_cast<double>(node->bottom_left.y) * one_over_atlas_height;
			double w = static_cast<double>(node_width)  * one_over_atlas_width;
			double h = static_cast<double>(node_height) * one_over_atlas_height;

			m_texture_coordinates[index] = RectangleD(x, y, w, h);

			std::unique_ptr<WritePixelBuffer> write_buffer = Device::create_write_pixel_buffer(
				PixelBufferHint::Static,
				image->get_size_in_bytes());

			write_buffer->copy_from_bitmap(image, true);

			m_texture->copy_from_buffer(
				write_buffer.get(),
				node->bottom_left.x,
				node->bottom_left.y,
				image->get_width(),
				image->get_height(),
				TextureHelper::imaging_pixel_format_to_image_format(image->get_pixel_format()),
				TextureHelper::imaging_pixel_format_to_data_type(image->get_pixel_format()),
				4);
		}
		else 
		{
			// No node found, must resize the texture atlas.
			resize_atlas(image);
			add_image(image, index);
		}

		std::ostringstream stream;

#ifdef __ANDROID__
		JNIEnv* jni;
		jint status = JNIHelper::get_instance().attach_current_thread(&jni);
		GuidGenerator generator(jni);
		stream << generator.newGuid();
		JNIHelper::get_instance().detach_current_thread(status);
#else
		GuidGenerator generator;
		stream << generator.newGuid();
#endif

		m_guid = stream.str();
	}

	Defer TextureAtlas::add_image(
		const std::string& id,
		const std::string& image_path)
	{
		if (id.empty())
		{
			throw std::invalid_argument("id is required.");
		}

		if (image_path.empty())
		{
			throw std::invalid_argument("image path is required.");
		}

		if (m_indices.find(id) != m_indices.end())
		{
			// we're already aware of this source
			Defer future_index =
				newPromise(
					[&](Defer d)
			{
				std::async(
					std::launch::async,
					[&]()
				{
					d.resolve(m_indices.at(id));
				});
			});

			return future_index;
		}
		

		// not in atlas, create the promise for the index
		Defer future_index =
			newPromise(
				[&, image_path](Defer d)
		{
			std::async(
				std::launch::async,
				[&, image_path]()
			{
				std::unique_ptr<Surface> surface = Device::create_pixel_buffer_surface(1, 1);
				std::unique_ptr<Bitmap> image    = Bitmap::create_bitmap_from_asset(image_path);
				unsigned int index = get_number_of_images();
				add_image(image.get(), index);
				
				// store the promise
				m_indices[id] = index;

				d.resolve(index);
			});			
		});

		
		return future_index;
	}

	Defer TextureAtlas::add_image(
		const std::string& id,
		Bitmap* image)
	{
		if (id.empty())
		{
			throw std::invalid_argument("id is required.");
		}

		if (image == nullptr) 
		{
			throw std::invalid_argument("image is required.");
		}

		if (m_indices.find(id) != m_indices.end())
		{
			// we're already aware of this source
			Defer future_index =
				newPromise(
					[&](Defer d)
			{
				std::async(
					std::launch::async,
					[&]()
				{
					d.resolve(m_indices.at(id));
				});
			});

			return future_index;
		}

		
		// not in atlas, create the promise for the index
		Defer future_index =
			newPromise(
				[&, image](Defer d)
		{
			std::async(
				std::launch::async,
				[&, image]()
			{
				std::unique_ptr<Surface> surface = Device::create_pixel_buffer_surface(1, 1);

				unsigned int index = get_number_of_images();
				add_image(image, index);

				// store the promise
				m_indices[id] = index;

				d.resolve(index);
			});
		});

		return future_index;
	}

	unsigned int TextureAtlas::get_number_of_images() const
	{
		return m_texture_coordinates.size();
	}

	Texture* TextureAtlas::get_texture() const
	{
		return m_texture.get();
	}

	std::map<unsigned int, RectangleD> TextureAtlas::get_texture_coordinates() const
	{
		return m_texture_coordinates;
	}

	RectangleD TextureAtlas::get_texture_coordinates(unsigned int& image_index) const
	{
		return m_texture_coordinates.at(image_index);
	}
}