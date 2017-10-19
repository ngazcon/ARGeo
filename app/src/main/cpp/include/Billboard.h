#pragma once

#include "Vector.h"
#include "RectangleD.h"
#include "Color.h"
#include "Bitmap.h"
#include "PickId.h"
#include "IPickable.h"
#include "HeightClamping.h"
#include "RasterSource.h"

#include <vector>
#include <string>

namespace argeo
{
	enum class HorizontalOrigin
	{
		Center = 0,
		Left = 1,
		Right = 2
	};

	enum class VerticalOrigin
	{
		Center = 0,
		Bottom = 1,
		Top = 2,
	};

	class BillboardCollection;
	class Billboard
		: public IPickable
	{
		friend class BillboardCollection;

	private:
		Billboard(
			BillboardCollection* collection,
			const std::string& image_path,
			const std::string& image_id = std::string(),
			const vec3d& position = vec3d::zero(),
			const vec2i& pixel_offset = vec2i::zero(),
			const unsigned int& width = IMAGE_WIDTH,
			const unsigned int& height = IMAGE_HEIGHT,
            const HeightClamping& height_clamping = HeightClamping::Ground);

		Billboard(
			BillboardCollection* collection,
			Bitmap* image,
			const std::string& image_id = std::string(),
			const vec3d& position	    = vec3d::zero(),
			const vec2i& pixel_offset   = vec2i::zero(),
			const unsigned int& width   = IMAGE_WIDTH,
			const unsigned int& height  = IMAGE_HEIGHT,
            const HeightClamping& height_clamping = HeightClamping::Ground);

	public:

		~Billboard();

        vec3d compute_actual_position(
                const vec3d& position,
                FrameState* frame_state,
                const mat4d& model_matrix);

        vec3d get_actual_position() const;
        void  set_actual_position(const vec3d& value);

        vec3d get_clamped_position() const;
        void  set_clamped_position(const vec3d& value);

		vec3d get_position() const;
		void  set_position(const vec3d& value);

		Color get_color() const;
		void  set_color(Color value);

		HorizontalOrigin get_horizontal_origin() const;
		void set_horizontal_origin(const HorizontalOrigin& value);

		VerticalOrigin get_vertical_origin() const;
		void set_vertical_origin(const VerticalOrigin& value);

		vec2i get_pixel_offset() const;
		void  set_pixel_offset(const vec2i& value);

		void make_dirty(const unsigned int& index);

		void set_vertex_buffer_offset(const unsigned int& value);
		unsigned int get_vertex_buffer_offset() const;

		void set_width(const unsigned int& value);
		unsigned int get_width() const;

		void set_height(const unsigned int& value);
		unsigned int get_height() const;

        void set_height_clamping(const HeightClamping& value);
        HeightClamping get_height_clamping() const;

		void set_group(BillboardCollection* value);
		BillboardCollection* get_group();

		unsigned int get_image_index() const;
		void load_image();

		PickId* get_pick_id(Context* context);

        bool get_size_in_meters() const;
        void set_size_in_meters(const bool& value);

        double get_scale() const;
        void set_scale(const double& value);

	private:
		void clean();
        void update_clamping();

	public:
		static unsigned int IMAGE_WIDTH;
		static unsigned int IMAGE_HEIGHT;

		static unsigned int SHOW_INDEX;
		static unsigned int WIDTH_INDEX;
		static unsigned int HEIGHT_INDEX;
		static unsigned int IMAGE_INDEX;
		static unsigned int POSITION_INDEX;
		static unsigned int COLOR_INDEX;
		static unsigned int PIXEL_OFFSET_INDEX;
		static unsigned int HORIZONTAL_ORIGIN_INDEX;
		static unsigned int VERTICAL_ORIGIN_INDEX;
        static unsigned int SCALE_INDEX;
		static unsigned int NUMBER_OF_PROPERTIES;

	private:
        HeightClamping m_height_clamping;
		SceneMode     m_mode;
		Bitmap*      m_image;
		std::string  m_image_path;
		std::string  m_image_id;
		unsigned int m_image_index;

        double m_scale;
        bool m_size_in_meters;
		bool m_dirty;
		BillboardCollection* m_collection;

		std::unique_ptr<PickId> m_pick_id;
        std::function<void()> m_remove_update_height_callback;
        std::function<void(vec3d&)> m_update_height_function;

		unsigned int m_width;
		unsigned int m_height;
		unsigned int m_vertex_buffer_offset;

        vec3d m_actual_clamped_position;
        vec3d m_actual_position;
		vec3d m_position;

		Color  m_color;
		vec2i  m_pixel_offset;
		unsigned char  m_origins;

		const short m_horizontal_origin_shift = 0;
		const short m_vertical_origin_shift   = 2;
		const unsigned char m_horizontal_origin_mask = 0x03 << m_horizontal_origin_shift;
		const unsigned char m_vertical_origin_mask   = 0x03 << m_vertical_origin_shift;

	};

}