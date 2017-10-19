#pragma once

#include "Ellipsoid.h"
#include "Vector.h"
#include "Geodetic3D.h"

namespace argeo
{
	class MapProjection
	{
	public:

		virtual Ellipsoid    get_ellipsoid() const = 0;
		virtual Geocentric3D project(const Geodetic3D& geodetic) const = 0;
		virtual Geodetic3D   unproject(const Geocentric3D& geocentric) const = 0;
	};
}
