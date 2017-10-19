#pragma once

#include "Vector.h"
#include "PickId.h"

#include "IPickable.h"
#include "Material.h"
#include "VertexArrayBucketLocator.h"

#include <vector>

namespace argeo
{
	class PolylineCollection;
	
	struct LocatorBucket
	{
		LocatorBucket(
			VertexArrayBucketLocator* vertex_array_bucket_locator,
			const unsigned int& count)
			: vertex_array_bucket_locator(vertex_array_bucket_locator)
			, count(count)
		{ }

		VertexArrayBucketLocator* vertex_array_bucket_locator;
		unsigned int count;
	};

	class Polyline 
		: public IPickable
	{
		friend class PolylineCollection;
		friend class PolylineBucket;

	public:
		Polyline(
			const std::vector<vec3d>& points,
			const bool& loop    = false,
			const double& width = 1.0);

		~Polyline();

		void update();
		void clean();

		std::vector<vec3d> get_actual_points() const;
		std::vector<vec3d> get_points() const;
		void set_points(const std::vector<vec3d>& value);

		void set_loop(bool value);
		bool get_loop() const;

		void set_show(bool value);
		bool get_show() const;

		void   set_width(double value);
		double get_width() const;

		unsigned int get_length() const;

		std::vector<bool> get_properties() const;
		void make_dirty(const unsigned int& index);

		void set_vertex_buffer_offset(int value);
		int  get_vertex_buffer_offset();

		void set_group(PolylineCollection* value);
		PolylineCollection* get_group();

		void set_material(Material* value);
		Material* get_material();

		PickId* get_pick_id(Context* context);

	public:
		static unsigned int show_index;
		static unsigned int width_index;
		static unsigned int position_index;
		static unsigned int material_index;
		static unsigned int position_size_index;
		static unsigned int number_of_properties;

	protected:
		void add_locator_bucket(const LocatorBucket& locator_bucket);
		std::vector<LocatorBucket> get_locator_buckets();

	private:
		
		std::vector<bool> m_properties;

		bool m_show;
		bool m_loop;
		double m_width;

		std::unique_ptr<PickId> m_pick_id;

		int m_vertex_buffer_offset;

		PolylineCollection* m_collection;

		Material* m_material;
		std::unique_ptr<UniformValue<float>> m_outline_width;

		unsigned int m_length;
		std::vector<vec3d> m_actual_points;
		std::vector<vec3d> m_points;

		std::vector<LocatorBucket> m_locator_buckets;
	};
}
