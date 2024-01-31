#ifndef CMF_LOG_H
#define CMF_LOG_H

#include <cstdio>
#include <string>
#include <esp_log.h>

#define TRACE_LOG(format, ...) printf(std::string("%s: %s (%d) ").append(format).append("\n").c_str(), __FILE__, __FUNCTION__, __LINE__ __VA_OPT__(,) __VA_ARGS__)

#endif //CMF_LOG_H