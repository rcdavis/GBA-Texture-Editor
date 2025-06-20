
#pragma once

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#pragma warning(pop)

#include "Core.h"

class Log {
public:
	static void Init();

	static Ref<spdlog::logger> &GetLogger() { return s_Logger; }

private:
	static Ref<spdlog::logger> s_Logger;
};

#if LOGGING_ENABLED
#	define LOG_TRACE(...) Log::GetLogger()->trace(__VA_ARGS__);
#	define LOG_INFO(...) Log::GetLogger()->info(__VA_ARGS__);
#	define LOG_WARN(...) Log::GetLogger()->warn(__VA_ARGS__);
#	define LOG_ERROR(...) Log::GetLogger()->error(__VA_ARGS__);
#	define LOG_CRITICAL(...) Log::GetLogger()->critical(__VA_ARGS__);
#else
#	define LOG_TRACE(...)
#	define LOG_INFO(...)
#	define LOG_WARN(...)
#	define LOG_ERROR(...)
#	define LOG_CRITICAL(...)
#endif
