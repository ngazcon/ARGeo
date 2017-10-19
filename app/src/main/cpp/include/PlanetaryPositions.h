#pragma once

#include "Vector.h"
#include "ArgeoMath.h"

#include <chrono>

namespace argeo
{
	class PlanetaryPositions
	{
	public:

		//inline double computeTdbMinusTtSpice(int daysSinceJ2000InTerrestrialTime) 
		//{
		//	/* STK Comments ------------------------------------------------------
		//	* This function uses constants designed to be consistent with
		//	* the SPICE Toolkit from JPL version N0051 (unitim.c)
		//	* M0 = 6.239996
		//	* M0Dot = 1.99096871e-7 rad/s = 0.01720197 rad/d
		//	* EARTH_ECC = 1.671e-2
		//	* TDB_AMPL = 1.657e-3 secs
		//	*--------------------------------------------------------------------*/
		//
		//	//* Values taken as specified in STK Comments except: 0.01720197 rad/day = 1.99096871e-7 rad/sec
		//	//* Here we use the more precise value taken from the SPICE value 1.99096871e-7 rad/sec converted to rad/day
		//	//* All other constants are consistent with the SPICE implementation of the TDB conversion
		//	//* except where we treat the independent time parameter to be in TT instead of TDB.
		//	//* This is an approximation made to facilitate performance due to the higher prevalance of
		//	//* the TT2TDB conversion over TDB2TT in order to avoid having to iterate when converting to TDB for the JPL ephemeris.
		//	//* Days are used instead of seconds to provide a slight improvement in numerical precision.
		//
		//	//* For more information see:
		//	//* http://www.cv.nrao.edu/~rfisher/Ephemerides/times.html#TDB
		//	//* ftp://ssd.jpl.nasa.gov/pub/eph/planets/ioms/ExplSupplChap8.pdf
		//
		//	double g = 6.239996 + (0.0172019696544) * daysSinceJ2000InTerrestrialTime;
		//	return 1.657e-3 * std::sin(g + 1.671e-2 * std::sin(g));
		//}
		//
		//
		//double TdtMinusTai = 32.184;
		//int J2000d      = 2451545;
		//
		//double epoch = new JulianDate(2451545, 0, TimeStandard.TAI); //Actually TDB (not TAI)
		//double GravitationalParameterOfEarth = 3.98600435e14;
		//double GravitationalParameterOfSun   = GravitationalParameterOfEarth * (1.0 + 0.012300034) * 328900.56;
		//double MetersPerKilometer			 = 1000.0;
		//double RadiansPerDegree				 = RADIANS_PER_DEGREE;
		//double RadiansPerArcSecond			 = RADIANS_PER_ARCSECOND;
		//double MetersPerAstronomicalUnit	 = 1.49597870e+11; // IAU 1976 value
		//
		//
		//// TBD: Temps Dynamique Barycentrique
		//// Barycentric Dynamical Time.Used as a time - scale of ephemerides
		//// referred to the barycentre of the solar system
		//// TAI: International Atomic Time (Temps Atomique International = TAI) is
		//// defined as the weighted average of the time kept by about 200
		//// atomic clocks in over 50 national laboratories worldwide.
		//// TAI - UT1 was approximately 0 on 1958 Jan 1.
		//inline std::tm taiToTdb(std::tm time)
		//{
		//	//Converts TAI to TT
		//	//JulianDate.addSeconds(date, TdtMinusTai, result);
		//	std::tm result = time;
		//	result.tm_sec += TdtMinusTai;
		//	result.tm
		//
		//	//Converts TT to TDB
		//	int days = result.tm_mday - J2000d;
		//	result   += computeTdbMinusTtSpice(days);
		//
		//	return result;
		//}
		//
		//
		///**
		//* Gets a point describing the motion of the Earth-Moon barycenter according to the equations
		//* described in section 6.
		//*/
		//inline vec3d computeSimonEarthMoonBarycenter(time_t date)
		//{
		//	// t is thousands of years from J2000 TDB
		//	taiToTdb(date, scratchDate);
		//	double x = (scratchDate.dayNumber - epoch.dayNumber) + ((scratchDate.secondsOfDay - epoch.secondsOfDay) / TimeConstants.SECONDS_PER_DAY);
		//	double t = x / (TimeConstants.DAYS_PER_JULIAN_CENTURY * 10.0);
		//
		//	double u = 0.35953620 * t;
		//	double semimajorAxis = semiMajorAxis0 +
		//		Ca1 * std::cos(p1u * u) + Sa1 * std::sin(p1u * u) +
		//		Ca2 * std::cos(p2u * u) + Sa2 * std::sin(p2u * u) +
		//		Ca3 * std::cos(p3u * u) + Sa3 * std::sin(p3u * u) +
		//		Ca4 * std::cos(p4u * u) + Sa4 * std::sin(p4u * u) +
		//		Ca5 * std::cos(p5u * u) + Sa5 * std::sin(p5u * u) +
		//		Ca6 * std::cos(p6u * u) + Sa6 * std::sin(p6u * u) +
		//		Ca7 * std::cos(p7u * u) + Sa7 * std::sin(p7u * u) +
		//		Ca8 * std::cos(p8u * u) + Sa8 * std::sin(p8u * u);
		//	double meanLongitude = meanLongitude0 + meanLongitude1 * t +
		//		Cl1 * Math.cos(q1u * u) + Sl1 * Math.sin(q1u * u) +
		//		Cl2 * Math.cos(q2u * u) + Sl2 * Math.sin(q2u * u) +
		//		Cl3 * Math.cos(q3u * u) + Sl3 * Math.sin(q3u * u) +
		//		Cl4 * Math.cos(q4u * u) + Sl4 * Math.sin(q4u * u) +
		//		Cl5 * Math.cos(q5u * u) + Sl5 * Math.sin(q5u * u) +
		//		Cl6 * Math.cos(q6u * u) + Sl6 * Math.sin(q6u * u) +
		//		Cl7 * Math.cos(q7u * u) + Sl7 * Math.sin(q7u * u) +
		//		Cl8 * Math.cos(q8u * u) + Sl8 * Math.sin(q8u * u);
		//
		//	// All constants in this part are from section 5.8
		//	var eccentricity = 0.0167086342 - 0.0004203654 * t;
		//	var longitudeOfPerigee = 102.93734808 * RadiansPerDegree + 11612.35290 * RadiansPerArcSecond * t;
		//	var inclination = 469.97289 * RadiansPerArcSecond * t;
		//	var longitudeOfNode = 174.87317577 * RadiansPerDegree - 8679.27034 * RadiansPerArcSecond * t;
		//
		//	return elementsToCartesian(
		//		semimajorAxis, eccentricity, inclination, longitudeOfPerigee,
		//		longitudeOfNode, meanLongitude, GravitationalParameterOfSun, result);
		//}
		//
		//
		///**
		//* Computes the position of the Sun in the Earth-centered inertial frame
		//*
		//* @param {JulianDate} [julianDate] The time at which to compute the Sun's position, if not provided the current system time is used.
		//* @param {Cartesian3} [result] The object onto which to store the result.
		//* @returns {Cartesian3} Calculated sun position
		//*/
		//inline vec3d computeSunPositionInEarthInertialFrame(time_t date)
		//{
		//	//first forward transformation
		//	vec3d translation = computeSimonEarthMoonBarycenter(date);
		//	vec3d result      = -translation;
		//
		//	//second forward transformation
		//	computeSimonEarth(date, translation);
		//
		//	result = result - translation;
		//	Matrix3.multiplyByVector(axesTransformation, result, result);
		//
		//	return result;
		//};

	};
}
