#include "logging.h"
#include <cstdio>

loggerFn logger = &defaultLogger;

void defaultLogger(eLogLevel level, const char *message) {
    const char *levelName;
    switch (level) {
        case 0:
            levelName = "DEBUG";
            break;
        default:
        case 1:
            levelName = "INFO";
            break;
        case 2:
            levelName = "WARN";
            break;
        case 3:
            levelName = "ERROR";
            break;
    }
    printf("[%-5s]: %s", levelName, message);
}

void loggerEx(eLogLevel level, const std::string &message) {
    logger(level, message.c_str());
}


