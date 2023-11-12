#pragma once

#include <cstdint>
#include <string>

#if defined(__cpp_lib_format)
#include <format>
#else

    #include "fmt/format.h"

namespace std {
    using namespace fmt;
}
#endif
enum eLogLevel {
    DEBUG,
    INFO,
    WARN,
    ERROR
};

void defaultLogger(eLogLevel level, const char *message);

typedef void (*loggerFn)(eLogLevel, const char *);

extern loggerFn logger;

void loggerEx(eLogLevel level, const std::string &message);

