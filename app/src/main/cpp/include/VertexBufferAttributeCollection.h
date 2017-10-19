#pragma once

#include "VertexBufferAttribute.h"

namespace argeo
{
	class VertexBufferAttributeCollection
	{
		class Proxy
		{
		public:

			Proxy(
				VertexBufferAttributeCollection* collection, 
				const unsigned int& index)
				: m_collection(collection)
				, m_index(index)
			{ }

			VertexBufferAttribute* operator=(VertexBufferAttribute* attribute)
			{
				m_collection->set_attribute(
					attribute,
					m_index);

				return m_collection->get_attribute(m_index);
			}

		private:

			VertexBufferAttributeCollection* m_collection;
			unsigned int m_index;
		};

	public:

		/// Get the attribute on the given index.
		virtual VertexBufferAttribute* get_attribute(const unsigned int& index) = 0;

		/// Set the attribute on the given index.
		virtual void set_attribute(
			VertexBufferAttribute* attribute, 
			const unsigned int& index) = 0;

		/// Get the actual count of attributes on this buffer.
		virtual unsigned int get_count() const = 0;

		/// Get the max count of attributes on this buffer.
		virtual unsigned int get_maximum_count() const = 0;


		inline Proxy operator[](int index)
		{
			return Proxy(this, index);
		}
	};
}
