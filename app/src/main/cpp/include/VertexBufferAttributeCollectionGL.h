#pragma once

#include "VertexBufferAttributeCollection.h"
#include "VertexBufferAttributes.h"

#include <memory>
#include <map>
#include <string>

namespace argeo
{
	struct VertexBufferAttributeGL
	{
		VertexBufferAttributeGL()
			: vertex_buffer_attribute(nullptr)
			, dirty(false)
		{ }

		std::unique_ptr<VertexBufferAttribute> vertex_buffer_attribute;
		bool dirty;
	};

	class VertexBufferAttributeCollectionGL : public VertexBufferAttributeCollection
	{
	public:

		VertexBufferAttributeCollectionGL();		
		VertexBufferAttributeCollectionGL(VertexBufferAttributes attributes);

		~VertexBufferAttributeCollectionGL();

		unsigned int get_count() const;


		unsigned int get_maximum_count() const;

		void attach(const unsigned int& index);
		void detach(const unsigned int& index);

		void clean();

		VertexBufferAttribute* get_attribute(const unsigned int& index);

		void set_attribute(
			VertexBufferAttribute* attribute, 
			const unsigned int& index);

		unsigned int get_maximum_array_index() const;

	private:

		// The underlying map collection that holds the uniforms.
		std::vector<std::unique_ptr<VertexBufferAttributeGL>> m_underlying_collection;

		// The maximum number of attributes. 
		unsigned int m_maximum_count;

		bool m_dirty;

		unsigned int m_maximum_array_index;

		unsigned int m_count;
	};
}
