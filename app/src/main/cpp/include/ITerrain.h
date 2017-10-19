#pragma once

#include "Ellipsoid.h"
#include "Geodetic3D.h"
#include "IPrimitive.h"
#include "RasterSource.h"
#include "RasterTile.h"
#include "Material.h"
#include "Ray.h"

namespace argeo
{
    class TerrainShader;
    class Scene;
	class ITerrain : public IPrimitive
	{
	public:
		virtual void update(FrameState* frame_state) = 0;

		virtual TerrainShader* get_shader_program() = 0;
        virtual void set_shader_program(TerrainShader* value) = 0;

		virtual Ellipsoid get_ellipsoid() const = 0;
		virtual void set_ellipsoid(const Ellipsoid& value) = 0;

		virtual Material* get_material() = 0;

		virtual bool get_wireframe() const = 0;
		virtual void set_wireframe(bool value) = 0;

		virtual bool get_use_double_precision() = 0;
		virtual void set_use_double_precision(const bool& value) = 0;

		virtual bool get_show_imagery() const = 0;
		virtual void set_show_imagery(bool value) = 0;

        virtual vec3d pick(
                const Ray& ray,
                Scene* scene) = 0;

		virtual vec3d pick_from_tile(
				const RasterTile* tile,
				const Ray &ray,
				const SceneMode &mode,
				MapProjection *projection,
				const bool &cull_back_faces) = 0;

        /**
        * True if primitives such as billboards, polylines, labels, etc. should be depth-tested
        * against the terrain surface, or false if such primitives should always be drawn on top
        * of terrain unless they're on the opposite side of the globe.  The disadvantage of depth
        * testing primitives against terrain is that slight numerical noise or terrain level-of-detail
        * switched can sometimes make a primitive that should be on the surface disappear underneath it.
        */
		virtual bool get_depth_test_against() const = 0;
        virtual void set_depth_test_against(const bool& value) = 0;

		virtual bool get_show() const = 0;
		virtual void set_show(bool value) = 0;

		virtual bool get_lighting() const = 0;
		virtual void set_lighting(bool value) = 0;

		virtual double get_height_exaggeration() const = 0;
		virtual void   set_height_exaggeration(double value) = 0;

		virtual double get_point_height(const Geodetic2D& point) const = 0;

        virtual void set_imagery_source(RasterSource* value) = 0;
        virtual RasterSource* get_imagery_source() = 0;

		virtual void set_raster_source(RasterSource* value) = 0;
		virtual RasterSource* get_raster_source() = 0;

        virtual void notify_dirty(const RasterTile* tile) = 0;
	};
}
