#pragma once

#include <jni.h>
#include <android/log.h>

namespace argeo
{
	class Log
	{
	public:
		static inline void error(const char* pMessage, ...)
		{
			va_list varArgs;
			va_start(varArgs, pMessage);

#ifdef __ANDROID__
			__android_log_vprint(
				ANDROID_LOG_ERROR,
				"PACKT",
				pMessage,
				varArgs);

			__android_log_print(
				ANDROID_LOG_ERROR,
				"PACKT",
				"\n");
#endif 
			va_end(varArgs);
		}

		static inline void warn(const char* pMessage, ...)
		{
			va_list varArgs;
			va_start(varArgs, pMessage);

#ifdef __ANDROID__
			__android_log_vprint(
				ANDROID_LOG_WARN,
				"PACKT",
				pMessage,
				varArgs);

			__android_log_print(
				ANDROID_LOG_WARN,
				"PACKT",
				"\n");
#endif 
			va_end(varArgs);
		}
			   
		static inline void info(const char* pMessage, ...)
		{
			va_list varArgs;
			va_start(varArgs, pMessage);

#ifdef __ANDROID__
			__android_log_vprint(
				ANDROID_LOG_INFO,
				"PACKT",
				pMessage,
				varArgs);

			__android_log_print(
				ANDROID_LOG_INFO,
				"PACKT",
				"\n");
#endif 
			va_end(varArgs);
		}
			   
		static inline void debug(const char* pMessage, ...)
		{
			va_list varArgs;
			va_start(varArgs, pMessage);

#ifdef __ANDROID__
			__android_log_vprint(
				ANDROID_LOG_DEBUG,
				"PACKT",
				pMessage,
				varArgs);

			__android_log_print(
				ANDROID_LOG_DEBUG,
				"PACKT",
				"\n");
#endif 
			va_end(varArgs);
		}
	};

#ifndef NDEBUG
#define packt_Log_debug(...) Log::debug(__VA_ARGS__)
#else
#define packt_Log_debug(...)
#endif

}