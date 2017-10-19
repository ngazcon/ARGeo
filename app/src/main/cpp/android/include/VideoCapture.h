#pragma once

#include "Matrix.h"

#include "NativeObject.h"
#include "JniHelpers.h"
#include "Bootstrap.h"
#include "ShaderProgram.h"
#include "RenderState.h"
#include "PrimitiveType.h"
#include "VertexArray.h"
#include "Uniform.h"
#include "DrawCommand.h"
#include "VideoCapture.h"
#include "TextureExternal.h"

#include "IVideoCapturePreview.h"

#include <jni.h>
#include <string>
#include <mutex>

namespace argeo
{
	namespace jni
	{
		class VideoCapture
			: public NativeObject
			, public IVideoCapturePreview
		{
		public:
			VideoCapture();
			VideoCapture(JNIEnv* jni);

			~VideoCapture();

			const char* getCanonicalName() const {
				return MAKE_CANONICAL_NAME(PACKAGE, VideoCapture);
			}

			void initialize(JNIEnv *env);
			void mapFields();

			static void nativeDestroy(JNIEnv *env, jobject java_this);

			bool is_running() const;
			void start(JNIEnv* jenv);
			void stop(JNIEnv* jenv);

			unsigned int get_texture_id() const;
			mat4 get_transform() const;

			Texture2D* get_texture();

			void frame_available();

			void   set_current_aspect_ratio(const double& value);
			double get_current_aspect_ratio() const;
			void   set_current_vertical_view_angle(const double& value);
			double get_current_vertical_view_angle() const;

			void set_orientation(float value);
			
			void set_viewport_size(Rectangle value);
			Rectangle get_viewport_size() const;

			void set_current_preview_size(Rectangle value);
			Rectangle get_current_preview_size() const;

			void update(FrameState* frame_state);

			static void jni_frame_available(
				JNIEnv* env,
				jobject java_this);

			static void jni_set_current_preview_size(
				JNIEnv* env,
				jobject java_this,
				jdouble width,
				jdouble height);

			static void jni_set_current_aspect_ratio(
				JNIEnv* env,
				jobject java_this,
				jdouble value);

			static jdouble jni_get_current_aspect_ratio(
				JNIEnv* env,
				jobject java_this);

			static void jni_set_current_vertical_view_angle(
				JNIEnv* env,
				jobject java_this,
				jdouble value);
			
			static jdouble jni_get_current_vertical_view_angle(
				JNIEnv* env,
				jobject java_this);

			static jboolean jni_is_running(
				JNIEnv* env,
				jobject java_this);

		private:
			void update_texture(JNIEnv* jenv);

			void create_resources(
				JNIEnv* jenv,
				Context* context);

			void create_external_texture();
			void create_surface_texture(JNIEnv* jenv);
			void create_camera(JNIEnv* jenv);

		private:
			double m_current_aspect_ratio;
			double m_current_vertical_view_angle;

			bool m_running;
			bool m_resources_created;
			
			mat4  m_transform;
			
			float m_orientation;
			Rectangle  m_current_preview_size;
			Rectangle  m_viewport_size;

			unsigned int m_frame_available_count;
			unsigned int m_texture_id;

			struct RenderResources
			{
				std::unique_ptr<TextureExternal> texture;
				std::unique_ptr<VertexArray>     vertex_array;
				ShaderProgram* shader_program;
				RenderState*   render_state;
			} m_render_resources;

			DrawCommand m_draw_command;
			UniformMap  m_uniform_map;

			std::mutex m_update_mutex;

			jobject m_java_this;
			jobject m_surface_texture;
			jclass  m_surface_texture_class;

			jmethodID m_surface_texture_update_texture_mid;
			jmethodID m_surface_texture_attach_context_mid;
			jmethodID m_surface_texture_detach_context_mid;
			jmethodID m_surface_texture_get_transform_mid;
		};
	}
}
