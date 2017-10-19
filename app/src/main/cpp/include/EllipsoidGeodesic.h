#pragma once

#include "Ellipsoid.h"

#include <stdexcept>

namespace argeo
{
	class EllipsoidGeodesic
	{
	public:

		void setConstants()
		{
			m_a = m_ellipsoid.get_maximum_radius();
			m_b = m_ellipsoid.get_minimum_radius();
			m_f = (m_a - m_b) / m_a;

			m_cosineHeading = std::cos(m_start_heading);
			m_sineHeading   = std::sin(m_start_heading);

			m_tanU = (1 - m_f) * std::tan(m_start.get_latitude());

			m_cosineU = 1.0 / std::sqrt(1.0 + m_tanU * m_tanU);
			m_sineU = m_cosineU * m_tanU;

			m_sigma = std::atan2(m_tanU, m_cosineHeading);

			m_sineAlpha = m_cosineU * m_sineHeading;
			m_sineSquaredAlpha = m_sineAlpha * m_sineAlpha;

			m_cosineSquaredAlpha = 1.0 - m_sineSquaredAlpha;
			m_cosineAlpha = std::sqrt(m_cosineSquaredAlpha);

			m_u2Over4   = m_u_squared / 4.0;
			m_u4Over16  = m_u2Over4 * m_u2Over4;
			m_u6Over64  = m_u4Over16 * m_u2Over4;
			m_u8Over256 = m_u4Over16 * m_u4Over16;

			m_a0 = (1.0 + m_u2Over4 - 3.0  * m_u4Over16 / 4.0 + 5.0  * m_u6Over64 / 4.0 - 175.0 * m_u8Over256 / 64.0);
			m_a1 = (1.0 - m_u2Over4 + 15.0 * m_u4Over16 / 8.0 - 35.0 * m_u6Over64 / 8.0);
			m_a2 = (1.0 - 3.0 * m_u2Over4 + 35.0 * m_u4Over16 / 4.0);
			m_a3 = (1.0 - 5.0 * m_u2Over4);

			m_distanceRatio = m_a0 * m_sigma - m_a1 * std::sin(2.0 * m_sigma) * m_u2Over4 / 2.0 - m_a2 * std::sin(4.0 * m_sigma) * m_u4Over16 / 16.0 -
				m_a3 * std::sin(6.0 * m_sigma) * m_u6Over64 / 48.0 - std::sin(8.0 * m_sigma) * 5.0 * m_u8Over256 / 512;
		}

		double computeC(
			const double& f,
			const double& cosineSquaredAlpha) const
		{
			return f * cosineSquaredAlpha * (4.0 + f * (4.0 - 3.0 * cosineSquaredAlpha)) / 16.0;
		}

		double computeDeltaLambda(
			const double& f,
			const double& sineAlpha,
			const double& cosineSquaredAlpha,
			const double& sigma,
			const double& sineSigma,
			const double& cosineSigma,
			const double& cosineTwiceSigmaMidpoint) const
		{
			double C = computeC(f, cosineSquaredAlpha);

			return (1.0 - C) * f * sineAlpha * (sigma + C * sineSigma * (cosineTwiceSigmaMidpoint +
				C * cosineSigma * (2.0 * cosineTwiceSigmaMidpoint * cosineTwiceSigmaMidpoint - 1.0)));
		}

		void vincentyInverseFormula(
			const double& major,
			const double& minor,
			const double& firstLongitude,
			const double& firstLatitude,
			const double& secondLongitude,
			const double& secondLatitude)
		{
			double eff = (major - minor) / major;
			double l = secondLongitude - firstLongitude;

			double u1 = std::atan((1 - eff) * std::tan(firstLatitude));
			double u2 = std::atan((1 - eff) * std::tan(secondLatitude));

			double cosineU1 = std::cos(u1);
			double sineU1   = std::sin(u1);
			double cosineU2 = std::cos(u2);
			double sineU2   = std::sin(u2);

			double cc = cosineU1 * cosineU2;
			double cs = cosineU1 * sineU2;
			double ss = sineU1 * sineU2;
			double sc = sineU1 * cosineU2;

			double lambda = l;
			double lambdaDot = TWO_PI;

			double cosineLambda = std::cos(lambda);
			double sineLambda   = std::sin(lambda);

			double sigma;
			double cosineSigma;
			double sineSigma;
			double cosineSquaredAlpha;
			double cosineTwiceSigmaMidpoint;

			do 
			{
				cosineLambda = std::cos(lambda);
				sineLambda   = std::sin(lambda);

				double temp = cs - sc * cosineLambda;
				sineSigma   = std::sqrt(cosineU2 * cosineU2 * sineLambda * sineLambda + temp * temp);
				cosineSigma = ss + cc * cosineLambda;

				sigma = std::atan2(sineSigma, cosineSigma);

				double sineAlpha;

				if (sineSigma == 0.0)
				{
					sineAlpha = 0.0;
					cosineSquaredAlpha = 1.0;
				}
				else 
				{
					sineAlpha = cc * sineLambda / sineSigma;
					cosineSquaredAlpha = 1.0 - sineAlpha * sineAlpha;
				}

				lambdaDot = lambda;

				cosineTwiceSigmaMidpoint = cosineSigma - 2.0 * ss / cosineSquaredAlpha;

				if (std::isnan(cosineTwiceSigmaMidpoint))
				{
					cosineTwiceSigmaMidpoint = 0.0;
				}

				lambda = l + computeDeltaLambda(eff, sineAlpha, cosineSquaredAlpha,
					sigma, sineSigma, cosineSigma, cosineTwiceSigmaMidpoint);
			}
			while (std::abs(lambda - lambdaDot) > EPSILON12);

			double uSquared = cosineSquaredAlpha * (major * major - minor * minor) / (minor * minor);
			double A = 1.0 + uSquared * (4096.0 + uSquared * (uSquared * (320.0 - 175.0 * uSquared) - 768.0)) / 16384.0;
			double B = uSquared * (256.0 + uSquared * (uSquared * (74.0 - 47.0 * uSquared) - 128.0)) / 1024.0;

			double cosineSquaredTwiceSigmaMidpoint = cosineTwiceSigmaMidpoint * cosineTwiceSigmaMidpoint;
			double deltaSigma = B * sineSigma * (cosineTwiceSigmaMidpoint + B * (cosineSigma *
				(2.0 * cosineSquaredTwiceSigmaMidpoint - 1.0) - B * cosineTwiceSigmaMidpoint *
				(4.0 * sineSigma * sineSigma - 3.0) * (4.0 * cosineSquaredTwiceSigmaMidpoint - 3.0) / 6.0) / 4.0);

			double distance = minor * A * (sigma - deltaSigma);

			double startHeading = std::atan2(cosineU2 * sineLambda, cs - sc * cosineLambda);
			double endHeading = std::atan2(cosineU1 * sineLambda, cs * cosineLambda - sc);

			m_distance = distance;
			m_start_heading = startHeading;
			m_end_heading = endHeading;
			m_u_squared = uSquared;
		}

		void computeProperties(
			const Geodetic3D& start,
			const Geodetic3D& end,
			const Ellipsoid& ellipsoid)
		{
			Geocentric3D firstCartesian = ellipsoid.to_geocentric3D(start).normalize();
			Geocentric3D lastCartesian  = ellipsoid.to_geocentric3D(end).normalize();

			if (std::abs(std::abs(std::acos(vec_dot(firstCartesian.to_vec3d(), lastCartesian.to_vec3d()))) - PI) < 0.0125)
			{
				throw std::runtime_error("geodesic position is not unique");
			}

			vincentyInverseFormula(
				ellipsoid.get_maximum_radius(),
				ellipsoid.get_minimum_radius(),
				start.get_longitude(), 
				start.get_latitude(),
				end.get_longitude(),
				end.get_latitude());

			m_start = Geodetic3D(
				start.get_longitude(),
				start.get_latitude(),
				0.0);

			m_end = Geodetic3D(
				end.get_longitude(),
				end.get_latitude(),
				0.0);

			setConstants();
		}

		/**
		* Initializes a geodesic on the ellipsoid connecting the two provided planetodetic points.
		*
		* @alias EllipsoidGeodesic
		* @constructor
		*
		* @param {Cartographic} [start] The initial planetodetic point on the path.
		* @param {Cartographic} [end] The final planetodetic point on the path.
		* @param {Ellipsoid} [ellipsoid=Ellipsoid.WGS84] The ellipsoid on which the geodesic lies.
		*/
		EllipsoidGeodesic(
			const Geodetic3D& start,
			const Geodetic3D& end,
			const Ellipsoid& ellipsoid = Ellipsoid::wgs84())
			: m_ellipsoid(ellipsoid)
		{
			computeProperties(start, end, ellipsoid);
		}

		/**
		* Sets the start and end points of the geodesic
		*
		* @param {Cartographic} start The initial planetodetic point on the path.
		* @param {Cartographic} end The final planetodetic point on the path.
		*/
		void setEndPoints(
			const Geodetic3D& start,
			const Geodetic3D& end) 
		{
			computeProperties(start, end, m_ellipsoid);
		};

		/**
		* Provides the location of a point at the indicated portion along the geodesic.
		*
		* @param {Number} fraction The portion of the distance between the initial and final points.
		* @returns {Cartographic} The location of the point along the geodesic.
		*/
		Geodetic3D interpolateUsingFraction(const double& fraction)
		{
			return interpolateUsingSurfaceDistance(m_distance * fraction);
		};

		/**
		* Provides the location of a point at the indicated distance along the geodesic.
		*
		* @param {Number} distance The distance from the inital point to the point of interest along the geodesic
		* @returns {Cartographic} The location of the point along the geodesic.
		*
		* @exception {DeveloperError} start and end must be set before calling funciton interpolateUsingSurfaceDistance
		*/
		Geodetic3D interpolateUsingSurfaceDistance(const double& distance)
		{
			double s = m_distanceRatio + distance / m_b;

			double cosine2S = std::cos(2.0 * s);
			double cosine4S = std::cos(4.0 * s);
			double cosine6S = std::cos(6.0 * s);
			double sine2S   = std::sin(2.0 * s);
			double sine4S   = std::sin(4.0 * s);
			double sine6S   = std::sin(6.0 * s);
			double sine8S   = std::sin(8.0 * s);

			double s2 = s * s;
			double s3 = s * s2;

			double sigma = 2.0 * s3 * m_u8Over256 * cosine2S / 3.0 +
				s * (1.0 - m_u2Over4 + 7.0 * m_u4Over16 / 4.0 - 15.0 * m_u6Over64 / 4.0 + 579.0 * m_u8Over256 / 64.0 -
				(m_u4Over16 - 15.0 * m_u6Over64 / 4.0 + 187.0 * m_u8Over256 / 16.0) * cosine2S -
					(5.0 * m_u6Over64 / 4.0 - 115.0 * m_u8Over256 / 16.0) * cosine4S -
					29.0 * m_u8Over256 * cosine6S / 16.0) +
					(m_u2Over4 / 2.0 - m_u4Over16 + 71.0 * m_u6Over64 / 32.0 - 85.0 * m_u8Over256 / 16.0) * sine2S +
				(5.0 * m_u4Over16 / 16.0 - 5.0 * m_u6Over64 / 4.0 + 383.0 * m_u8Over256 / 96.0) * sine4S -
				s2 * ((m_u6Over64 - 11.0 * m_u8Over256 / 2.0) * sine2S + 5.0 * m_u8Over256 * sine4S / 2.0) +
				(29.0 * m_u6Over64 / 96.0 - 29.0 * m_u8Over256 / 16.0) * sine6S +
				539.0 * m_u8Over256 * sine8S / 1536.0;

			double theta = std::asin(std::sin(sigma) * m_cosineAlpha);
			double latitude = std::atan(m_a / m_b * std::tan(theta));

			// Redefine in terms of relative argument of latitude.
			sigma = sigma - m_sigma;

			double cosineTwiceSigmaMidpoint = std::cos(2.0 * m_sigma + sigma);

			double sineSigma = std::sin(sigma);
			double cosineSigma = std::cos(sigma);

			double cc = m_cosineU * cosineSigma;
			double ss = m_sineU   * sineSigma;

			double lambda = std::atan2(sineSigma * m_sineHeading, cc - ss * m_cosineHeading);

			double l = lambda - computeDeltaLambda(
				m_f,
				m_sineAlpha, 
				m_cosineSquaredAlpha,
				sigma,
				sineSigma,
				cosineSigma, 
				cosineTwiceSigmaMidpoint);

			return Geodetic3D(
				m_start.get_longitude() + l,
				latitude,
				0.0);
		};


		private:
			Ellipsoid m_ellipsoid;

			Geodetic3D m_start;
			Geodetic3D m_end;

			double m_distance;
			double m_u_squared;
			double m_start_heading;
			double m_end_heading;

			double m_a;
			double m_b;
			double m_f;
			double m_cosineHeading;
			double m_sineHeading;
			double m_tanU;
			double m_cosineU;
			double m_sineU;
			double m_sigma;
			double m_sineAlpha;
			double m_sineSquaredAlpha;
			double m_cosineSquaredAlpha;
			double m_cosineAlpha;
			double m_u2Over4;
			double m_u4Over16;
			double m_u6Over64;
			double m_u8Over256;
			double m_a0;
			double m_a1;
			double m_a2;
			double m_a3;
			double m_distanceRatio;
	};
}
