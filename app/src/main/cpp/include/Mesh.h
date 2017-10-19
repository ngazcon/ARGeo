#pragma once

#include "IIndices.h"
#include "WindingOrder.h"
#include "PrimitiveType.h"
#include "VertexAttributeCollection.h"

#include <memory>

namespace argeo
{
	class Mesh
	{
		///
		/// Represent a collection of vertices, edges and faces that defines the shape of a polyhedral object in 3D space. 
		///

	public:

		///
		/// 
		///
		Mesh(
			const bool& dispose_attributes = true,
			const bool& dispose_indices    = true);

		///
		/// @destructor
		///
		~Mesh();

		///
		/// Get the indices of this mesh.
		/// @return {IIndices*} The indices.
		///
		IIndices* get_indices();

		///
		/// Set the indices of this mesh.
		/// @param {IIndices*} value The indices.
		///
		void set_indices(IIndices* value);

		///
		/// Get the primitive type of this mesh.
		/// @return {PrimitiveType} The primitive type.
		///
		PrimitiveType get_primitive_type();

		///
		/// Set the primitive type of this mesh.
		/// @param {PrimitiveType} value The primitive type.
		///
		void set_primitive_type(const PrimitiveType& value);

		///
		/// Get the indices of this mesh.
		/// @return {IIndices*} The indices.
		///
		WindingOrder get_front_face_winding_order();

		///
		/// Set the winding order of this mesh.
		/// @param {WindingOrder} value The winding order.
		///
		void set_front_face_winding_order(const WindingOrder& value);

		///
		/// Get the vertex attributes of this mesh.
		/// @return {VertexAttributeCollection*} The vertex attributes.
		///
		VertexAttributeCollection get_attributes();

		///
		/// Add a vertex attribute to this mesh.
		/// @param {IVertexAttribute*} attribute The vertex attribute to add.
		/// @return {bool} False if the attribute was already in this mesh, otherwise True.
		///
		bool add_attribute(IVertexAttribute* attribute);

		///
		/// Remove a vertex attribute from this mesh.
		/// @param {IVertexAttribute*} attribute The vertex attribute to remove.
		/// @return {bool} True if the attribute was correctly remove from his mesh, otherwise False.
		///
		bool remove_attribute(IVertexAttribute* attribute);


	private:
		bool m_dispose_attributes;
		bool m_dispose_indices;

		IIndices* m_indices;
		VertexAttributeCollection m_attributes;
		PrimitiveType m_primitive_type;
		WindingOrder  m_front_face_winding_order;
	};
}
