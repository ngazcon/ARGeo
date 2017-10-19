#pragma once

#include "JniHelpers.h"

#define PACKAGE "vyglab/argeo/jni"

using namespace argeo::jni;

extern ClassRegistry gClasses;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void *reserved);