#include "Bitmap.h"
#include "BitmapAlgorithms.h"
#include "include/JNIHelper.h"
#include "StringHelper.h"
#include "ColorHelper.h"
#include "EmbeddedResources.h"

#include <android/bitmap.h>
#include <algorithm>

namespace argeo
{
	Bitmap::Bitmap(FILE* file)
	{ 
		JNIEnv* jni;
		JavaVM* jvm = JNIHelper::get_instance().get_java_vm();

		jint result = jvm->GetEnv(reinterpret_cast<void**>(&jni), JNI_VERSION_1_6);
		if (result == JNI_EDETACHED)
		{
			jvm->AttachCurrentThread(&jni, 0);
		}

		jclass bitmap_factory_class = jni->FindClass("android/graphics/BitmapFactory");
		jmethodID constructor_id = jni->GetStaticMethodID(
			bitmap_factory_class,
			"decodeByteArray",
			"([BII)Landroid/graphics/Bitmap;");

		if (file != nullptr)
		{
			int buf_length = EmbeddedResources::resource_size(file);

			// Buffer used to store file data
			std::unique_ptr<signed char> buf(new signed char[buf_length]);
			
			fread(
				reinterpret_cast<signed char*>(buf.get()),
				sizeof(unsigned char),
				buf_length,
				file);

			jbyteArray jdata = jni->NewByteArray(buf_length);
			jni->SetByteArrayRegion(jdata, 0, buf_length, buf.get());

			jobject bitmap = jni->CallStaticObjectMethod(
				bitmap_factory_class,
				constructor_id,
				jdata,
				0,
				buf_length);

			jclass bitmap_class = jni->FindClass("android/graphics/Bitmap");
			jmethodID bitmap_get_row_bytes_id = jni->GetMethodID(
				bitmap_class,
				"getRowBytes",
				"()I");

			m_stride = jni->CallIntMethod(
				bitmap,
				bitmap_get_row_bytes_id);

			jmethodID bitmap_get_width = jni->GetMethodID(
				bitmap_class,
				"getWidth",
				"()I");

			m_width = jni->CallIntMethod(
				bitmap,
				bitmap_get_width);

			jmethodID bitmap_get_height = jni->GetMethodID(
				bitmap_class,
				"getHeight",
				"()I");

			m_height = jni->CallIntMethod(
				bitmap,
				bitmap_get_height);

			m_handle = jni->NewGlobalRef(bitmap);

			jmethodID bitmap_get_config= jni->GetMethodID(
				bitmap_class,
				"getConfig",
				"()Landroid/graphics/Bitmap$Config;");

			jobject bcfg = jni->CallObjectMethod(
				bitmap,
				bitmap_get_config);

			jclass bcfg_class = jni->FindClass("android/graphics/Bitmap$Config");
			jmethodID bcfg_name_id = jni->GetMethodID(
				bcfg_class,
				"name",
				"()Ljava/lang/String;");

			jstring name = static_cast<jstring>(jni->CallObjectMethod(
				bcfg,
				bcfg_name_id));

			const char *name_str = jni->GetStringUTFChars(name, 0);

			if (!strcmp(name_str, "RGB_565"))
			{
				m_pixel_format = PixelFormat::Format16bppRgb565;
			}
			else if (!strcmp(name_str, "ALPHA_8"))
			{
				m_pixel_format = PixelFormat::Alpha;
			}
			else if (!strcmp(name_str, "ARGB_4444"))
			{
				m_pixel_format = PixelFormat::Format32bppArgb;
			}
			else if (!strcmp(name_str, "ARGB_8888"))
			{
				m_pixel_format = PixelFormat::Format32bppArgb;
			}
			else
			{
				m_pixel_format = PixelFormat::Undefined;
			}	

			jni->ReleaseStringUTFChars(name, name_str);
		}

		if (result == JNI_EDETACHED)
		{
			jvm->DetachCurrentThread();
		}
	}

	Bitmap::Bitmap(void* handle)
	{
		JNIEnv* jni;
		JavaVM* jvm = JNIHelper::get_instance().get_java_vm();

		jint result = jvm->GetEnv(reinterpret_cast<void**>(&jni), JNI_VERSION_1_6);
		if (result == JNI_EDETACHED)
		{
			jvm->AttachCurrentThread(&jni, 0);
		}

		jobject bitmap = reinterpret_cast<jobject>(handle);

		jclass bitmap_class = jni->FindClass("android/graphics/Bitmap");
		jmethodID bitmap_get_row_bytes_id = jni->GetMethodID(
			bitmap_class,
			"getRowBytes",
			"()I");

		m_stride = jni->CallIntMethod(
			bitmap,
			bitmap_get_row_bytes_id);

		jmethodID bitmap_get_width = jni->GetMethodID(
			bitmap_class,
			"getWidth",
			"()I");

		m_width = jni->CallIntMethod(
			bitmap,
			bitmap_get_width);

		jmethodID bitmap_get_height = jni->GetMethodID(
			bitmap_class,
			"getHeight",
			"()I");

		m_height = jni->CallIntMethod(
			bitmap,
			bitmap_get_height);

		m_handle = jni->NewGlobalRef(bitmap);

		jmethodID bitmap_get_config = jni->GetMethodID(
			bitmap_class,
			"getConfig",
			"()Landroid/graphics/Bitmap$Config;");

		jobject bcfg = jni->CallObjectMethod(
			bitmap,
			bitmap_get_config);

		jclass bcfg_class = jni->FindClass("android/graphics/Bitmap$Config");
		jmethodID bcfg_name_id = jni->GetMethodID(
			bcfg_class,
			"name",
			"()Ljava/lang/String;");

		jstring name = static_cast<jstring>(jni->CallObjectMethod(
			bcfg,
			bcfg_name_id));

		const char *name_str = jni->GetStringUTFChars(name, 0);

		if (!strcmp(name_str, "RGB_565"))
		{
			m_pixel_format = PixelFormat::Format16bppRgb565;
		}
		else if (!strcmp(name_str, "ALPHA_8"))
		{
			m_pixel_format = PixelFormat::Alpha;
		}
		else if (!strcmp(name_str, "ARGB_4444"))
		{
			m_pixel_format = PixelFormat::Format32bppArgb;
		}
		else if (!strcmp(name_str, "ARGB_8888"))
		{
			m_pixel_format = PixelFormat::Format32bppArgb;
		}
		else
		{
			m_pixel_format = PixelFormat::Undefined;
		}

		jni->ReleaseStringUTFChars(name, name_str);

		if (result == JNI_EDETACHED)
		{
			jvm->DetachCurrentThread();
		}
	}

	Bitmap::Bitmap(
		const unsigned int& width,
		const unsigned int& height,
		const PixelFormat& pixel_format)
		: m_pixel_format(pixel_format)
		, m_width(width)
		, m_height(height)
	{
		JNIEnv* jni;
		JavaVM* jvm = JNIHelper::get_instance().get_java_vm();
		jint result = jvm->GetEnv(reinterpret_cast<void**>(&jni), JNI_VERSION_1_6);
		if (result == JNI_EDETACHED)
		{
			jvm->AttachCurrentThread(&jni, 0);
		}

		jclass bitmap_class      = jni->FindClass("android/graphics/Bitmap");
		jmethodID constructor_id = jni->GetStaticMethodID(
			bitmap_class,
			"createBitmap",
			"(IILandroid/graphics/Bitmap$Config;)Landroid/graphics/Bitmap;");

		jstring j_config_name;
		
		switch (pixel_format)
		{
		case argeo::PixelFormat::Alpha:
			j_config_name = jni->NewStringUTF("ARGB_8888");
			break;
		case argeo::PixelFormat::Format16bppRgb555:
			j_config_name = jni->NewStringUTF("RGB_565");
			break;
		case argeo::PixelFormat::Format24bppRgb:
			j_config_name = jni->NewStringUTF("RGB_888");
			break;
		case argeo::PixelFormat::Format32bppArgb:
			j_config_name = jni->NewStringUTF("ARGB_8888");
			break;
		}

		jclass bcfg_class = jni->FindClass("android/graphics/Bitmap$Config");
		jmethodID bcfg_value_of_id = jni->GetStaticMethodID(
			bcfg_class,
			"valueOf",
			"(Ljava/lang/String;)Landroid/graphics/Bitmap$Config;");

		jobject java_bitmap_config = jni->CallStaticObjectMethod(
			bcfg_class,
			bcfg_value_of_id,
			j_config_name);

		jobject bitmap  = jni->CallStaticObjectMethod(
			bitmap_class,
			constructor_id,
			width,
			height,
			java_bitmap_config);

		jmethodID bitmap_get_row_bytes_id = jni->GetMethodID(
			bitmap_class,
			"getRowBytes",
			"()I");

		m_stride = jni->CallIntMethod(
			bitmap,
			bitmap_get_row_bytes_id);

		m_handle = jni->NewGlobalRef(bitmap);

		if (result == JNI_EDETACHED)
		{
			jvm->DetachCurrentThread();
		}
	}
	
	Bitmap::~Bitmap()
	{ 
		JNIEnv* jni;
		JavaVM* jvm = JNIHelper::get_instance().get_java_vm();
		jint result = jvm->GetEnv(reinterpret_cast<void**>(&jni), JNI_VERSION_1_6);
		if (result == JNI_EDETACHED)
		{
			jvm->AttachCurrentThread(&jni, 0);
		}

		jni->DeleteGlobalRef(static_cast<jobject>(m_handle));

		if (result == JNI_EDETACHED)
		{
			jvm->DetachCurrentThread();
		}
	}

	void Bitmap::recycle()
	{
		JNIEnv* jni;
		JavaVM* jvm = JNIHelper::get_instance().get_java_vm();
		jint result = jvm->GetEnv(reinterpret_cast<void**>(&jni), JNI_VERSION_1_6);
		if (result == JNI_EDETACHED)
		{
			jvm->AttachCurrentThread(&jni, 0);
		}

		jclass bitmap_class = jni->FindClass("android/graphics/Bitmap");
		jmethodID recycle_id = jni->GetMethodID(
			bitmap_class,
			"recycle",
			"()V");

		jni->CallVoidMethod(
			static_cast<jobject>(m_handle),
			recycle_id);

		if (result == JNI_EDETACHED)
		{
			jvm->DetachCurrentThread();
		}
	}

	std::unique_ptr<Bitmap> Bitmap::create_bitmap_from_asset(std::string asset_path)
	{
		// Create the Android Bitmap instance.
		FILE* asset_file = EmbeddedResources::resource_open(asset_path);
		Bitmap* bitmap   = new Bitmap(asset_file);
		EmbeddedResources::resource_close(asset_file);
		return std::unique_ptr<Bitmap>(bitmap);
	}

	std::unique_ptr<Bitmap> Bitmap::create_bitmap_from_text(
		std::string text,
		Font font)
	{
		JNIEnv* jni;
		JavaVM* jvm = JNIHelper::get_instance().get_java_vm();
		jint result = jvm->GetEnv(reinterpret_cast<void**>(&jni), JNI_VERSION_1_6);
		if (result == JNI_EDETACHED)
		{
			jvm->AttachCurrentThread(&jni, 0);
		}

		// Create the Type face from Font
		jclass typeface_class = jni->FindClass("android/graphics/Typeface");
		jmethodID typeface_constructor_id = jni->GetStaticMethodID(
			typeface_class,
			"create",
			"(Ljava/lang/String;I)Landroid/graphics/Typeface;");
		
		jstring name = jni->NewStringUTF(font.get_name().c_str());

		jobject typeface = jni->CallStaticObjectMethod(
			typeface_class,
			typeface_constructor_id,
			name,
			font.get_style());

		// Create Android Paint Instance
		jclass paint_class = jni->FindClass("android/graphics/Paint");
		jmethodID paint_constructor_id = jni->GetMethodID(
			paint_class,
			"<init>",
			"()V");

		jobject paint = jni->NewObject(
			paint_class,
			paint_constructor_id);
		paint = jni->NewGlobalRef(paint);

		jmethodID paint_ascent_id = jni->GetMethodID(
			paint_class,
			"ascent",
			"()F");

		jmethodID paint_descent_id = jni->GetMethodID(
			paint_class,
			"descent",
			"()F");

		jmethodID paint_measure_text_id = jni->GetMethodID(
			paint_class,
			"measureText",
			"(Ljava/lang/String;)F");

		jmethodID paint_set_anti_alias_id = jni->GetMethodID(
			paint_class,
			"setAntiAlias",
			"(Z)V");

		jmethodID paint_set_text_size_id = jni->GetMethodID(
			paint_class,
			"setTextSize",
			"(F)V");

		jmethodID paint_set_typeface_id = jni->GetMethodID(
			paint_class,
			"setTypeface",
			"(Landroid/graphics/Typeface;)Landroid/graphics/Typeface;");

		jmethodID paint_set_color_id = jni->GetMethodID(
			paint_class,
			"setColor",
			"(I)V");

		jni->CallVoidMethod(
			paint,
			paint_set_anti_alias_id,
			true);

		jni->CallVoidMethod(
			paint,
			paint_set_text_size_id,
			font.get_size());

		jni->CallVoidMethod(
			paint,
			paint_set_color_id,
			ColorHelper::color_to_hexa_argb(font.get_color()));

		jni->CallObjectMethod(
			paint,
			paint_set_typeface_id,
			typeface);

		// Get font metrics
		// ascent() is negative
		float ascent = fabs(jni->CallFloatMethod(
			paint,
			paint_ascent_id)); 

		float descent = jni->CallFloatMethod(
			paint,
			paint_descent_id);

		std::vector<std::string> split_text = StringHelper::split(text, '\n');
		int width  = 0.0;
		int height = 0.0f;
		for (std::string token : split_text)
		{
			jstring jtext = jni->NewStringUTF(token.c_str());

			width = std::max(
				width,
				static_cast<int>(
					jni->CallFloatMethod(
						paint,
						paint_measure_text_id,
						jtext)));

			height += static_cast<int>(ascent + descent);

			jni->DeleteLocalRef(jtext);
		}

		// Create the Android Bitmap instance.
		Bitmap* bitmap = new Bitmap(
			width,
			height,
			PixelFormat::Format32bppArgb);

		// Create Android Canvas Instance
		jclass canvas_class = jni->FindClass("android/graphics/Canvas");
		jmethodID canvas_constructor_id = jni->GetMethodID(
			canvas_class,
			"<init>",
			"(Landroid/graphics/Bitmap;)V");

		jmethodID canvas_draw_text_id = jni->GetMethodID(
			canvas_class,
			"drawText",
			"(Ljava/lang/String;FFLandroid/graphics/Paint;)V");

		jobject canvas = jni->NewObject(
			canvas_class,
			canvas_constructor_id,
			static_cast<jobject>(bitmap->get_handle()));


		// Split the text in line breaks tokens.
		float y = ascent;
		for (std::string t : split_text)
		{
			jstring jtext = jni->NewStringUTF(t.c_str());

			jni->CallVoidMethod(
				canvas,
				canvas_draw_text_id,
				jtext,
				0.0f,
				y,
				paint);

			y += ascent + descent;

			jni->DeleteLocalRef(jtext);
		}

		jni->DeleteGlobalRef(paint);

		if (result == JNI_EDETACHED)
		{
			jvm->DetachCurrentThread();
		}

		return std::unique_ptr<Bitmap>(bitmap);
	}

	void Bitmap::get_pixels(
			unsigned char* pixels,
			const bool& flip) const
	{
		int width  = get_width();
		int height = get_height();

		JNIEnv* jni;
		JavaVM* jvm = JNIHelper::get_instance().get_java_vm();
		jint result = jvm->GetEnv(reinterpret_cast<void**>(&jni), JNI_VERSION_1_6);
		if (result == JNI_EDETACHED)
		{
			jvm->AttachCurrentThread(&jni, 0);
		}

        void* bitmap_pixels;
        if (AndroidBitmap_lockPixels(jni, static_cast<jobject>(m_handle), &bitmap_pixels) < 0)
        {
            throw std::runtime_error("Error locking bitmap pixels.");
        }

        switch (m_pixel_format)
		{
		case argeo::PixelFormat::Alpha:
			memcpy(
				pixels,
				reinterpret_cast<unsigned char*>(bitmap_pixels),
				width * height * sizeof(unsigned char));
			break;
		case argeo::PixelFormat::Format16bppRgb565:
			BitmapAlgorithms::rgb_int32_to_rgb_ubyte(
				reinterpret_cast<unsigned int*>(bitmap_pixels),
				pixels,
				width,
				height,
				flip);
			break;
		case argeo::PixelFormat::Format32bppArgb:
			BitmapAlgorithms::rgba_int32_to_rgba_ubyte(
				reinterpret_cast<unsigned int*>(bitmap_pixels),
				pixels,
				width,
				height,
				flip);
			break;
		}


        AndroidBitmap_unlockPixels(jni, static_cast<jobject>(m_handle));

		if (result == JNI_EDETACHED)
		{
			jvm->DetachCurrentThread();
		}
	}

	void Bitmap::set_pixels(unsigned char* pixels)
	{
		JNIEnv* jni;
		JavaVM* jvm = JNIHelper::get_instance().get_java_vm();
		jint result = jvm->GetEnv(reinterpret_cast<void**>(&jni), JNI_VERSION_1_6);
		if (result == JNI_EDETACHED)
		{
			jvm->AttachCurrentThread(&jni, 0);
		}

		int width  = get_width();
		int height = get_height();

        void* bitmap_pixels;
        if (AndroidBitmap_lockPixels(jni, static_cast<jobject>(m_handle), &bitmap_pixels) < 0)
        {
            throw std::runtime_error("Error locking bitmap pixels.");
        }

		switch (m_pixel_format)
		{
		case argeo::PixelFormat::Alpha:
			memcpy(
				reinterpret_cast<unsigned char*>(bitmap_pixels),
				pixels,
				width * height * sizeof(unsigned char));
			break;
		case argeo::PixelFormat::Format16bppRgb565:
			BitmapAlgorithms::rgb_ubyte_to_rgb_int32(
				reinterpret_cast<unsigned int*>(bitmap_pixels),
				pixels,
				width,
				height);
			break;
		case argeo::PixelFormat::Format32bppArgb:
			BitmapAlgorithms::rgba_ubyte_rgba_int32(
				pixels,
				reinterpret_cast<unsigned int*>(bitmap_pixels),
				width,
				height);
			break;
		}

        AndroidBitmap_unlockPixels(jni, static_cast<jobject>(m_handle));

		if (result == JNI_EDETACHED)
		{
			jvm->DetachCurrentThread();
		}
	}

	void Bitmap::set_pixels(unsigned int* pixels)
	{
		JNIEnv* jni;
		JavaVM* jvm = JNIHelper::get_instance().get_java_vm();
		jint result = jvm->GetEnv(reinterpret_cast<void**>(&jni), JNI_VERSION_1_6);
		if (result == JNI_EDETACHED)
		{
			jvm->AttachCurrentThread(&jni, 0);
		}

        int width  = get_width();
        int height = get_height();


        void* bitmap_pixels;
        if (AndroidBitmap_lockPixels(jni, static_cast<jobject>(m_handle), &bitmap_pixels) < 0)
        {
            throw std::runtime_error("Error locking bitmap pixels.");
        }

        memcpy(
            bitmap_pixels,
            pixels,
            width * height * sizeof(unsigned int));

        AndroidBitmap_unlockPixels(jni, static_cast<jobject>(m_handle));

		if (result == JNI_EDETACHED)
		{
			jvm->DetachCurrentThread();
		}
	}

	void* Bitmap::get_handle() const
	{
		return m_handle;
	}

	std::size_t Bitmap::get_size_in_bytes() const
	{
		return m_stride * m_height;
	}

	unsigned int Bitmap::get_width() const
	{
		return m_width;
	}

	unsigned int Bitmap::get_height() const
	{
		return m_height;
	}

	unsigned int Bitmap::get_stride() const
	{
		return m_stride;
	}

	PixelFormat Bitmap::get_pixel_format() const
	{
		return m_pixel_format;
	}
}
