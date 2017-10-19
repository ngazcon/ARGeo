#pragma once

#include "UniformBlock.h"

#include <string>

namespace argeo
{
	class UniformBlockGL : public UniformBlock
	{
	public:

		UniformBlockGL(
			std::string name, 
			int location,
			int binding_point,
			int data_size);

		~UniformBlockGL();

		// Getters and Setters
		std::string get_name() const;

		int get_data_size() const;
		int get_location() const;

		int  get_binding_point() const;
		void set_binding_point(int value);

		UniformBlockMemberCollection* get_members();

		void bind(UniformBuffer* buffer);
		void bind(
			UniformBuffer* buffer,
			int offset_in_bytes,
			int size_in_bytes);

		void clean();

	private:

		// Round up to nearest aligned offset.
		unsigned int realign_offset(std::size_t offset, std::size_t align);

		int m_uniform_buffer_align;
		int m_data_size;
		int m_binding_point;
		int m_location;

		std::string m_name;

		std::unique_ptr<UniformBlockMemberCollection> m_members;

		bool m_dirty;

	};
}
