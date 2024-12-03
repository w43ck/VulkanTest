#pragma once

#include "../defines.hpp"

#include <string>
#include <stdio.h>
#include <format>
#include <array>

#define LOG_WARN_ENABLED 1
#define LOG_INFO_ENABLED 1
#define LOG_DEBUG_ENABLED 1
#define LOG_TRACE_ENABLED 1

#if WRELEASE == 1
#define LOG_DEBUG_ENABLED 0
#define LOG_TRACE_ENABLED 0
#endif

using log_lvl = enum class log_lvl{
	LOG_LEVEL_FATAL = 0,
	LOG_LEVEL_ERROR,
	LOG_LEVEL_WARN,
	LOG_LEVEL_INFO,
	LOG_LEVEL_DEBUG,
	LOG_LEVEL_TRACE
};

b8 init_logging();
void shutdown_logging();

template <typename... Args>
void log_output(log_lvl lvl, const std::string_view mes, Args&&... args) {
	constexpr std::array<const char*, 6> lvl_strings = {
		"[FATAL]: ", "[ERROR]: ", "[WARN]: ", "[INFO]: ", "[DEBUG]: ", "[TRACE]: "
	};

	std::string formatted_message = std::vformat(mes, std::make_format_args(args...));
	printf("%s%s\n", lvl_strings[static_cast<int>(lvl)], formatted_message.data());
}

#define WFATAL(mes, ...) log_output(log_lvl::LOG_LEVEL_FATAL, mes, ##__VA_ARGS__)

#ifndef WERROR
#define WERROR(mes, ...) log_output(log_lvl::LOG_LEVEL_ERROR, mes, ##__VA_ARGS__)
#endif

#if LOG_WARN_ENABLED == 1
#define WWARN(mes, ...) log_output(log_lvl::LOG_LEVEL_WARN, mes, ##__VA_ARGS__)
#else
#define WWARN(mes, ...)
#endif

#if LOG_INFO_ENABLED == 1
#define WINFO(mes, ...) log_output(log_lvl::LOG_LEVEL_INFO, mes, ##__VA_ARGS__)
#else
#define WINFO(mes, ...)
#endif

#if LOG_DEBUG_ENABLED == 1
#define WDEBUG(mes, ...) log_output(log_lvl::LOG_LEVEL_DEBUG, mes, ##__VA_ARGS__)
#else
#define WDEBUG(mes, ...)
#endif

#if LOG_TRACE_ENABLED == 1
#define WTRACE(mes, ...) log_output(log_lvl::LOG_LEVEL_TRACE, mes, ##__VA_ARGS__)
#else
#define WTRACE(mes, ...)
#endif
