#include "include/Bootstrap.h"

#include "include/JNIEngine.h"
#include "include/JNIArgeoFragment.h"
#include "include/JNIArgeoActivity.h"
#include "include/JNIEntity.h"
#include "include/JNIEntityCollection.h"
#include "include/JNIModelGraphics.h"
#include "include/JNIBillboardGraphics.h"
#include "include/JNIPolylineGraphics.h"
#include "include/JNIGeocentric3D.h"
#include "include/JNIGeodetic3D.h"
#include "include/JNIEllipsoidTransformations.h"
#include "include/JNIScene.h"
#include "include/JNIITerrain.h"
#include "include/JNIViewer.h"
#include "include/JNIDevicePoseCameraController.h"
#include "include/JNIScreenSpaceCameraController.h"
#include "include/JNIEllipsoidTangentPlane.h"
#include "include/JNIPlaneGraphics.h"
#include "include/JNIQuaternion.h"

using namespace argeo;

ClassRegistry gClasses;

jint JNI_OnLoad(
	JavaVM* jvm, 
	void* aReserved)
{
	JNIEnv* jni = jniHelpersInitialize(jvm);
	
	if (jni == NULL)
	{
		return -1;
	}

	gClasses.add(jni, new JNIGeocentric3D(jni));
	gClasses.add(jni, new JNIGeodetic3D(jni));
	gClasses.add(jni, new JNIArgeoFragment(jni));
	gClasses.add(jni, new JNIArgeoActivity(jni));
	gClasses.add(jni, new JNIEntityCollection(jni));
	gClasses.add(jni, new JNIIGraphics(jni));
	gClasses.add(jni, new JNIBillboardGraphics(jni));
	gClasses.add(jni, new JNIPolylineGraphics(jni));
	gClasses.add(jni, new JNIModelGraphics(jni));
	gClasses.add(jni, new JNIPlaneGraphics(jni));
	gClasses.add(jni, new JNIEntity(jni));
	gClasses.add(jni, new JNIITerrain(jni));
	gClasses.add(jni, new JNIScene(jni));
	gClasses.add(jni, new JNIViewer(jni));
	gClasses.add(jni, new VideoCapture(jni));
	gClasses.add(jni, new JNIEllipsoidTangentPlane(jni));
	gClasses.add(jni, new JNIScreenSpaceCameraController(jni));
	gClasses.add(jni, new JNIDevicePoseCameraController(jni));
	gClasses.add(jni, new JNICamera(jni));
	gClasses.add(jni, new JNIQuaternion(jni));
	
	JNIEngine::register_natives(jni);
	JNIEllipsoidTransformations::register_natives(jni);

	return JNI_VERSION_1_6;
}

void JNI_OnUnload(
	JavaVM* jvm,
	void* aReserved)
{ }


