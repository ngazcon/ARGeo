#include "include/JNIEllipsoidTransformations.h"
#include "include/JNIGeodetic3D.h"
#include "include/JNIGeocentric3D.h"

namespace argeo
{
	namespace jni
	{
		void JNIEllipsoidTransformations::register_natives(JNIEnv *jni)
		{
			//Declaring the methods. This will be used by registerNatives

			std::string geocentric3D_from_geodetic3D_signature;
			JavaClassUtils::makeSignature(
				geocentric3D_from_geodetic3D_signature,
				MAKE_CANONICAL_NAME(PACKAGE, Geocentric3D),
				MAKE_CANONICAL_NAME(PACKAGE, Geodetic3D),
				nullptr);

			std::string geodetic3D_from_geocentric3D_signature;
			JavaClassUtils::makeSignature(
				geodetic3D_from_geocentric3D_signature,
				MAKE_CANONICAL_NAME(PACKAGE, Geodetic3D),				
				MAKE_CANONICAL_NAME(PACKAGE, Geocentric3D),
				nullptr);

			JNINativeMethod methods[] =
			{
				{ "geocentric3DFromGeodetic3D",  geocentric3D_from_geodetic3D_signature.c_str(), (void*)&JNIEllipsoidTransformations::geocentric3D_from_geodetic3D },
				{ "geodetic3DFromGeocentric3D",  geodetic3D_from_geocentric3D_signature.c_str(), (void*)&JNIEllipsoidTransformations::geodetic3D_from_geocentric3D },
			};

			jclass clazz = jni->FindClass(getCanonicalName());

			// Register the native methods.
			int methods_size = sizeof(methods) / sizeof(methods[0]);
			if (jni->RegisterNatives(clazz, methods, methods_size) < 0)
			{
				throw std::runtime_error("Error running RegisterNatives.");
			}
		}

		jobject JNIEllipsoidTransformations::geocentric3D_from_geodetic3D(
			JNIEnv* jni,
			jobject clazz,
			jobject java_geodetic)
		{
			JNIGeodetic3D* geodetic   = gClasses.getNativeInstance<JNIGeodetic3D>(jni, java_geodetic);
			Geocentric3D   geocentric = EllipsoidTransformations::geocentric3D_from_geodetic3D(*geodetic->getNativeObject());
			return JNIGeocentric3D::create(jni, geocentric);
		}

		jobject JNIEllipsoidTransformations::geodetic3D_from_geocentric3D(
			JNIEnv* jni,
			jobject clazz,
			jobject java_geocentric)
		{
			JNIGeocentric3D* geocentric = gClasses.getNativeInstance<JNIGeocentric3D>(jni, java_geocentric);
			Geodetic3D   geodetic       = EllipsoidTransformations::geodetic3D_from_geocentric3D(*geocentric->getNativeObject());
			return JNIGeodetic3D::create(jni, geodetic);
		}
	}
}
