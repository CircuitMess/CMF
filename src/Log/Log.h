#ifndef CMF_LOG_H
#define CMF_LOG_H

#include <cstdio>
#include <string>
#include <esp_log.h>
#include <source_location>

enum LogLevel {
	None = (uint8_t) ESP_LOG_NONE,
	Error = (uint8_t) ESP_LOG_ERROR,
	Warning = (uint8_t) ESP_LOG_WARN,
	Info = (uint8_t) ESP_LOG_INFO,
	Debug = (uint8_t) ESP_LOG_DEBUG,
	Verbose = (uint8_t) ESP_LOG_VERBOSE
};

struct LogType {
	const std::string Tag;
};

#define DEFINE_LOG(name) inline static const LogType name = {#name};
DEFINE_LOG(LogTemp)
DEFINE_LOG(LogCMF)

#define TRACE_LOG(format, ...) printf(std::string("%s(%d:%d) '%s' ").append(format).append("\n").c_str(), std::source_location::current().file_name(),std::source_location::current().line(), std::source_location::current().column(), std::source_location::current().function_name() __VA_OPT__(,) __VA_ARGS__)

#define CMF_LOG(log, level, format, ...)																		\
	do {																										\
		if(level == Error){																						\
			ESP_LOGE(log.Tag.c_str(), format __VA_OPT__(,) __VA_ARGS__);										\
		}else if(level == LogLevel::Warning){																	\
			ESP_LOGW(log.Tag.c_str(), format __VA_OPT__(,) __VA_ARGS__);										\
		}else if(level == Info){																				\
			ESP_LOGI(log.Tag.c_str(), format __VA_OPT__(,) __VA_ARGS__);										\
		}else if(level == Debug){																				\
			ESP_LOGD(log.Tag.c_str(), format __VA_OPT__(,) __VA_ARGS__);										\
		}else if(level == Verbose){																				\
			ESP_LOGV(log.Tag.c_str(), format __VA_OPT__(,) __VA_ARGS__);										\
		}else{}																									\
    } while(false)																								\

#endif //CMF_LOG_H