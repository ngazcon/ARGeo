# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_CXX_STANDARD 14)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
             argeo-lib

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             src/main/cpp/ARBuffer.cpp
             src/main/cpp/AssimpIOStream.cpp
             src/main/cpp/AssimpIOSystem.cpp
             src/main/cpp/Billboard.cpp
             src/main/cpp/BillboardCollection.cpp
             src/main/cpp/BillboardGraphics.cpp
             src/main/cpp/BillboardVisualizer.cpp
             src/main/cpp/BoundingSphere.cpp
             src/main/cpp/Camera.cpp
             src/main/cpp/CameraEyeHighModelCoordinatesUniform.cpp
             src/main/cpp/CameraEyeHighModelCoordinatesUniformFactory.cpp
             src/main/cpp/CameraEyeLowModelCoordinatesUniform.cpp
             src/main/cpp/CameraEyeLowModelCoordinatesUniformFactory.cpp
             src/main/cpp/CameraEyeUniform.cpp
             src/main/cpp/CameraEyeUniformFactory.cpp
             src/main/cpp/CameraLightPositionUniform.cpp
             src/main/cpp/CameraLightPositionUniformFactory.cpp
             src/main/cpp/ClearCommand.cpp
             src/main/cpp/ClipmapLevel.cpp
             src/main/cpp/ClipmapTerrain.cpp
             src/main/cpp/ClipmapUpdate.cpp
             src/main/cpp/ClipmapUpdater.cpp
             src/main/cpp/CullingVolume.cpp
             src/main/cpp/CurrentFrustumUniform.cpp
             src/main/cpp/CurrentFrustumUniformFactory.cpp
             src/main/cpp/DepthPlane.cpp
             src/main/cpp/DrawCommand.cpp
             src/main/cpp/Ellipsoid.cpp
             src/main/cpp/EllipsoidTangentPlane.cpp
             src/main/cpp/Entity.cpp
             src/main/cpp/EntityCollection.cpp
             src/main/cpp/Font.cpp
             src/main/cpp/FrameState.cpp
             src/main/cpp/Geocentric2D.cpp
             src/main/cpp/Geocentric3D.cpp
             src/main/cpp/Geodetic2D.cpp
             src/main/cpp/Geodetic3D.cpp
             src/main/cpp/GeodeticExtent.cpp
             src/main/cpp/GeographicProjection.cpp
             src/main/cpp/GeologicalImageSource.cpp
             src/main/cpp/guid.cpp
             src/main/cpp/HeadsUpDisplay.cpp
             src/main/cpp/HighResolutionSnapScaleUniform.cpp
             src/main/cpp/HighResolutionSnapScaleUniformFactory.cpp
             src/main/cpp/IIndices.cpp
             src/main/cpp/IndicesUnsignedInt.cpp
             src/main/cpp/IndicesUnsignedShort.cpp
             src/main/cpp/InverseModelMatrixUniform.cpp
             src/main/cpp/InverseModelMatrixUniformFactory.cpp
             src/main/cpp/InverseModelViewMatrixUniform.cpp
             src/main/cpp/InverseModelViewMatrixUniformFactory.cpp
             src/main/cpp/InverseProjectionMatrixUniform.cpp
             src/main/cpp/InverseProjectionMatrixUniformFactory.cpp
             src/main/cpp/LightPropertiesUniform.cpp
             src/main/cpp/LightPropertiesUniformFactory.cpp
             src/main/cpp/Material.cpp
             src/main/cpp/MaterialCache.cpp
             src/main/cpp/Mesh.cpp
             src/main/cpp/MeshBuffers.cpp
             src/main/cpp/MeshVertexBufferAttributeCollection.cpp
             src/main/cpp/MessageQueue.cpp
             src/main/cpp/Model.cpp
             src/main/cpp/ModelGraphics.cpp
             src/main/cpp/ModelMatrixUniform.cpp
             src/main/cpp/ModelMatrixUniformFactory.cpp
             src/main/cpp/ModelNode.cpp
             src/main/cpp/ModelViewMatrixUniform.cpp
             src/main/cpp/ModelViewMatrixUniformFactory.cpp
             src/main/cpp/ModelViewProjectionMatrixUniform.cpp
             src/main/cpp/ModelViewProjectionMatrixUniformFactory.cpp
             src/main/cpp/ModelViewProjectionRelativeToEyeMatrixUniform.cpp
             src/main/cpp/ModelViewRelativeToEyeMatrixUniformFactory.cpp
             src/main/cpp/ModelViewRelativeToEyeMatrixUniform.cpp
             src/main/cpp/ModelViewProjectionRelativeToEyeMatrixUniformFactory.cpp
             src/main/cpp/ModelVisualizer.cpp
             src/main/cpp/ModelZToClipCoordinatesUniform.cpp
             src/main/cpp/ModelZToClipCoordinatesUniformFactory.cpp
             src/main/cpp/MorphTimeUniform.cpp
             src/main/cpp/MorphTimeUniformFactory.cpp
             src/main/cpp/NormalMatrixUniform.cpp
             src/main/cpp/NormalMatrixUniformFactory.cpp
             src/main/cpp/OrientedBoundingBox.cpp
             src/main/cpp/OrthographicOffCenterFrustum.cpp
             src/main/cpp/OrthographicFrustum.cpp
             src/main/cpp/PassState.cpp
             src/main/cpp/PboDownloader.cpp
             src/main/cpp/PerspectiveFrustum.cpp
             src/main/cpp/PerspectiveOffCenterFrustum.cpp
             src/main/cpp/PickableNode.cpp
             src/main/cpp/PickDepth.cpp
             src/main/cpp/PickFramebuffer.cpp
             src/main/cpp/Plane.cpp
             src/main/cpp/PlaneGraphics.cpp
             src/main/cpp/PlanePrimitive.cpp
             src/main/cpp/PlaneVisualizer.cpp
             src/main/cpp/Polyline.cpp
             src/main/cpp/PolylineBucket.cpp
             src/main/cpp/PolylineCollection.cpp
             src/main/cpp/PolylineGraphics.cpp
             src/main/cpp/PolylineVisualizer.cpp
             src/main/cpp/ProjectionMatrixUniform.cpp
             src/main/cpp/ProjectionMatrixUniformFactory.cpp
             src/main/cpp/RasterLevel.cpp
             src/main/cpp/RasterSource.cpp
             src/main/cpp/RasterTile.cpp
             src/main/cpp/RectangleTessellator.cpp
             src/main/cpp/RenderState.cpp
             src/main/cpp/Scene.cpp
             src/main/cpp/SceneNode.cpp
             src/main/cpp/SceneTransitioner.cpp
             src/main/cpp/ShaderCache.cpp
             src/main/cpp/ShaderProgram.cpp
             src/main/cpp/ShaderVertexAttribute.cpp
             src/main/cpp/ShadowMap.cpp
             src/main/cpp/ShadowMapShader.cpp
             src/main/cpp/Snapshot.cpp
             src/main/cpp/SrtmTerrainSource.cpp
             src/main/cpp/SunDirectionECUniform.cpp
             src/main/cpp/SunDirectionECUniformFactory.cpp
             src/main/cpp/SunDirectionWCUniform.cpp
             src/main/cpp/SunDirectionWCUniformFactory.cpp
             src/main/cpp/TerrainDepth.cpp
             src/main/cpp/TerrainShaderSet.cpp
             src/main/cpp/TextureAtlas.cpp
             src/main/cpp/TextureSamplers.cpp
             src/main/cpp/TriangleIndicesUnsignedInt.cpp
             src/main/cpp/TriangleIndicesUnsignedShort.cpp
             src/main/cpp/UniformState.cpp
             src/main/cpp/VertexAttributeByte.cpp
             src/main/cpp/VertexAttributeDoubleVector3.cpp
             src/main/cpp/VertexAttributeFloatVector2.cpp
             src/main/cpp/VertexAttributeFloatVector3.cpp
             src/main/cpp/VertexAttributeFloatVector4.cpp
             src/main/cpp/VertexAttributeRGB.cpp
             src/main/cpp/VertexAttributeRGBA.cpp
             src/main/cpp/VertexBufferAttribute.cpp
             src/main/cpp/Viewer.cpp
             src/main/cpp/ViewportOrthographicMatrixUniform.cpp
             src/main/cpp/ViewportOrthographicMatrixUniformFactory.cpp
             src/main/cpp/ViewportQuad.cpp
             src/main/cpp/ViewportTransformationMatrixUniform.cpp
             src/main/cpp/ViewportTransformationMatrixUniformFactory.cpp
             src/main/cpp/ViewportUniform.cpp
             src/main/cpp/ViewportUniformFactory.cpp


             src/main/cpp/BufferGL.cpp
             src/main/cpp/ColorAttachmentsGL.cpp
             src/main/cpp/CubeMapFaceGL.cpp
             src/main/cpp/CubeMapGL.cpp
             src/main/cpp/FenceGL.cpp
             src/main/cpp/FragmentOutputsGL.cpp
             src/main/cpp/FrameBufferGL.cpp
             src/main/cpp/IndexBufferGL.cpp
             src/main/cpp/PixelBufferGL.cpp
             src/main/cpp/ReadPixelBufferGL.cpp
             src/main/cpp/RenderBufferGL.cpp
             src/main/cpp/ShaderObjectGL.cpp
             src/main/cpp/ShaderProgramGL.cpp
             src/main/cpp/Texture2DArrayGL.cpp
             src/main/cpp/Texture2DGL.cpp
             src/main/cpp/TextureGL.cpp
             src/main/cpp/TextureSamplerGL.cpp
             src/main/cpp/TextureUnitGL.cpp
             src/main/cpp/TextureUnitsGL.cpp
             src/main/cpp/UniformBlockGL.cpp
             src/main/cpp/UniformBoolArrayGL.cpp
             src/main/cpp/UniformBoolGL.cpp
             src/main/cpp/UniformBufferGL.cpp
             src/main/cpp/UniformFloatArrayGL.cpp
             src/main/cpp/UniformFloatGL.cpp
             src/main/cpp/UniformFloatMatrix3ArrayGL.cpp
             src/main/cpp/UniformFloatMatrix3GL.cpp
             src/main/cpp/UniformFloatMatrix4ArrayGL.cpp
             src/main/cpp/UniformFloatMatrix4GL.cpp
             src/main/cpp/UniformFloatMatrix42GL.cpp
             src/main/cpp/UniformFloatVector2ArrayGL.cpp
             src/main/cpp/UniformFloatVector2GL.cpp
             src/main/cpp/UniformFloatVector3ArrayGL.cpp
             src/main/cpp/UniformFloatVector3GL.cpp
             src/main/cpp/UniformFloatVector4ArrayGL.cpp
             src/main/cpp/UniformFloatVector4GL.cpp
             src/main/cpp/UniformIntArrayGL.cpp
             src/main/cpp/UniformIntGL.cpp
             src/main/cpp/UniformSampler.cpp
             src/main/cpp/UniformSampler.cpp
             src/main/cpp/VertexArrayGL.cpp
             src/main/cpp/VertexBufferAttributeCollectionGL.cpp
             src/main/cpp/VertexBufferGL.cpp
             src/main/cpp/WritePixelBufferGL.cpp


             # Android
             src/main/cpp/android/AndroidNativeAppGlue.c
             src/main/cpp/android/Application.cpp
             src/main/cpp/android/ArgeoActivity.cpp
             src/main/cpp/android/ArgeoFragment.cpp
             src/main/cpp/android/ArgeoNativeActivity.cpp
             src/main/cpp/android/ArgeoTest.cpp
             src/main/cpp/android/Bitmap.cpp
             src/main/cpp/android/Bootstrap.cpp
             src/main/cpp/android/ByteArray.cpp
             src/main/cpp/android/ClassRegistry.cpp
             src/main/cpp/android/Device.cpp
             src/main/cpp/android/DevicePoseCameraController.cpp
             src/main/cpp/android/EmbeddedResources.cpp
             src/main/cpp/android/Engine.cpp
             src/main/cpp/android/EventLoop.cpp
             src/main/cpp/android/InputManager.cpp
             src/main/cpp/android/GestureDetector.cpp
             src/main/cpp/android/JavaClass.cpp
             src/main/cpp/android/JavaClassUtils.cpp
             src/main/cpp/android/JavaExceptionUtils.cpp
             src/main/cpp/android/JavaString.cpp
             src/main/cpp/android/JavaStringArray.cpp
             src/main/cpp/android/JavaThreadUtils.cpp

             src/main/cpp/android/JNIArgeoActivity.cpp
             src/main/cpp/android/JNIArgeoFragment.cpp
             src/main/cpp/android/JNIBillboardGraphics.cpp
             src/main/cpp/android/JNICamera.cpp
             src/main/cpp/android/JNIDevicePoseCameraController.cpp
             src/main/cpp/android/JNIEllipsoidTangentPlane.cpp
             src/main/cpp/android/JNIEllipsoidTransformations.cpp
             src/main/cpp/android/JNIEngine.cpp
             src/main/cpp/android/JNIEntity.cpp
             src/main/cpp/android/JNIEntityCollection.cpp
             src/main/cpp/android/JNIGeocentric3D.cpp
             src/main/cpp/android/JNIGeodetic3D.cpp
             src/main/cpp/android/JNIHelper.cpp
             src/main/cpp/android/JniHelpers.cpp
             src/main/cpp/android/JNIIGraphics.cpp
             src/main/cpp/android/JNIITerrain.cpp
             src/main/cpp/android/JNIMatrix4D.cpp
             src/main/cpp/android/JNIModelGraphics.cpp
             src/main/cpp/android/JNIPlaneGraphics.cpp
             src/main/cpp/android/JNIPolylineGraphics.cpp
             src/main/cpp/android/JNIQuaternion.cpp
             src/main/cpp/android/JNIScene.cpp
             src/main/cpp/android/JNIScreenSpaceCameraController.cpp
             src/main/cpp/android/JNIViewer.cpp

             src/main/cpp/android/LocationManager.cpp
             src/main/cpp/android/NativeObject.cpp
             src/main/cpp/android/ScreenSpaceCameraController.cpp
             src/main/cpp/android/Sensor.cpp
             src/main/cpp/android/SensorManager.cpp
             src/main/cpp/android/ShortArray.cpp
             src/main/cpp/android/TextureExternal.cpp
             src/main/cpp/android/VideoCapture.cpp

             src/main/cpp/android/ContextEGL.cpp
             src/main/cpp/android/ExtensionsES.cpp
             src/main/cpp/android/PixelBufferSurfaceEGL.cpp
             src/main/cpp/android/SurfaceWindowEGL.cpp
             src/main/cpp/android/TypeConverterGL.cpp

             # tween
             tween/catch.cpp
             tween/tween.cpp)

# Specifies a path to native header files.
include_directories(src/main/cpp/include/)
include_directories(src/main/cpp/Android/include/)
include_directories(assimp/include/)
include_directories(tween/)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

find_library( android-lib
              android )

find_library( jnigraphics-lib
              jnigraphics )

find_library( m-lib
              m )

find_library( z-lib
              z )

find_library( GLESv3-lib
              GLESv3 )

find_library( EGL-lib
              EGL )

# Add static link external libraries
add_library( assimp-lib
             STATIC
             IMPORTED )


set_target_properties( # Specifies the target library.
                       assimp-lib

                       # Specifies the parameter you want to define.
                       PROPERTIES IMPORTED_LOCATION

                       # Provides the path to the library you want to import.
                       ${CMAKE_SOURCE_DIR}/assimp/bin/Android/libassimp.a)



# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       argeo-lib

                       # Links the target library to static libs.
                       assimp-lib

                       # Links the target library to libraries
                       # included in the NDK.
                       ${log-lib}
                       ${android-lib}
                       ${m-lib}
                       ${jnigraphics-lib}
                       ${GLESv3-lib}
                       ${EGL-lib}
                       ${z-lib}
                       ${assimp-lib})