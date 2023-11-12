#pragma once

#include <cstdint>
#include <format>

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

