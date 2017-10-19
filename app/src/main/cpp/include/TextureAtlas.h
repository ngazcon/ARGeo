#pragma once

#include "Texture2D.h"
#include "RectangleD.h"
#include "Vector.h"
#include "Bitmap.h"
#include "Context.h"
#include "DrawCommand.h"
#include "Promise.h"

#include <map>
#include <string>
#include <memory>

namespace argeo
{
	const unsigned int INDEX_NOT_DEFINED = -1;

	struct TextureAtlasNode
	{
		TextureAtlasNode(
			vec2i bottom_left = vec2i::zero(),
			vec2i top_right   = vec2i::zero(),
			std::unique_ptr<TextureAtlasNode> child_node1 = nullptr,
			std::unique_ptr<TextureAtlasNode> child_node2 = nullptr,
			const unsigned int& image_index = INDEX_NOT_DEFINED)
			: image_index(image_index)
			, bottom_left(bottom_left)
			, top_right(top_right)
			, child_node1(std::move(child_node1))
			, child_node2(std::move(child_node2))
		{ }

		vec2i bottom_left;
		vec2i top_right;
		std::unique_ptr<TextureAtlasNode> child_node1;
		std::unique_ptr<TextureAtlasNode> child_node2;
		unsigned int image_index;
	};

	class TextureAtlas
	{
	public:
		TextureAtlas(
			Context* context,
			const vec2i& initial_size = vec2i(16, 16),
			const unsigned int& border_width_in_pixels = 1,
			const TextureFormat& texture_format = TextureFormat::RedGreenBlueAlpha8);
		~TextureAtlas();

		Texture* get_texture() const;
		std::string get_guid() const;

		/**
		* Adds an image to the atlas.  If the image is already in the atlas, the atlas is unchanged and
		* the existing index is used.
		*
		* @param {String} id An identifier to detect whether the image already exists in the atlas.
		* @param {Image|Canvas|String|Promise|TextureAtlas~CreateImageCallback} image An image or canvas to add to the texture atlas,
		*        or a URL to an Image, or a Promise for an image, or a function that creates an image.
		* @returns {Promise.<Number>} A Promise for the image index.
		*/
		Defer add_image(
			const std::string& id,
			Bitmap* image);	

		Defer add_image(
			const std::string& id,
			const std::string& image_path);

		RectangleD get_texture_coordinates(unsigned int& image_index) const;

		std::map<unsigned int, RectangleD> get_texture_coordinates() const;

		unsigned int get_number_of_images() const;

	private:
		void resize_atlas(Bitmap* image);

		// A recursive function that finds the best place to insert
		// a new image based on existing image 'nodes'.
		// Inspired by: http://blackpawn.com/texts/lightmaps/default.html
		TextureAtlasNode* find_node(
			TextureAtlasNode* node,
			Bitmap* image);

		void add_image(
			Bitmap* image,
			const unsigned int& index);

	private:
		unsigned int m_border_width_in_pixels;
		std::string  m_guid;

		std::map<std::string, unsigned int> m_indices;
		std::map<unsigned int, RectangleD> m_texture_coordinates;
		
		std::unique_ptr<Texture2D> m_texture;
		std::unique_ptr<DrawCommand> m_copy_command;

		UniformMap m_uniform_map;
		TextureFormat m_texture_format;
		Context* m_context;
		std::unique_ptr<TextureAtlasNode> m_root;
	};
}
