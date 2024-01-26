#ifndef CMF_LOG_H
#define CMF_LOG_H

#include <cstdio>
#include <string>

#define TRACE_LOG(format, ...) printf(std::string("%s: %s (%d) -> ").append(format).c_str(), __FILE__, __func__, __LINE__ __VA_OPT__(,) __VA_ARGS__)

#endif //CMF_LOG_H