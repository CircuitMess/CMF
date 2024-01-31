#ifndef CMF_LOG_H
#define CMF_LOG_H

#include <cstdio>
#include <string>
#include <esp_log.h>
#include <source_location>

#define TRACE_LOG(format, ...) printf(std::string("%s(%d:%d) '%s' ").append(format).append("\n").c_str(), std::source_location::current().file_name(),std::source_location::current().line(), std::source_location::current().column(), std::source_location::current().function_name() __VA_OPT__(,) __VA_ARGS__)

#endif //CMF_LOG_H