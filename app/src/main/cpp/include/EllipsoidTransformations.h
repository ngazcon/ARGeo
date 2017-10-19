#pragma once

#include "Ellipsoid.h"
#include "Geodetic2D.h"
#include "Geodetic3D.h"
#include "Geocentric3D.h"

namespace argeo
{
	class EllipsoidTransformations
	{
	public:
		static Geocentric3D geocentric3D_from_geodetic2D(
			const Geodetic2D& geodetic,
			const Ellipsoid& ellipsoid = Ellipsoid::wgs84())
		{
			return ellipsoid.to_geocentric3D(geodetic);
		}

		static Geocentric3D geocentric3D_from_geodetic3D(
			const Geodetic3D& geodetic,
			const Ellipsoid& ellipsoid = Ellipsoid::wgs84())
		{
			return ellipsoid.to_geocentric3D(geodetic);
		}

		static Geodetic3D geodetic3D_from_geocentric3D(
			const Geocentric3D& geocentric,
			const Ellipsoid& ellipsoid = Ellipsoid::wgs84())
		{
			return ellipsoid.to_geodetic3D(geocentric);
		}

		static Geodetic2D geodetic2D_from_geocentric3D(
			const Geocentric3D& geocentric,
			const Ellipsoid& ellipsoid = Ellipsoid::wgs84())
		{
			return ellipsoid.to_geodetic2D(geocentric);
		}
	};
}
