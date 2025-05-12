#include "logging.h"
#include <cstdio>

loggerFn logger = &defaultLogger;
eLogLevel logLevel = eLogLevel::INFO;

void defaultLogger(eLogLevel level, const char *message) {
    const char *levelName;
    switch (level) {
        case eLogLevel::DEBUG:
            levelName = "DEBUG";
            break;
        default:
        case eLogLevel::INFO:
            levelName = "INFO";
            break;
        case eLogLevel::WARN:
            levelName = "WARN";
            break;
        case eLogLevel::ERROR:
            levelName = "ERROR";
            break;
    }
    printf("[%-5s]: %s", levelName, message);
}

void loggerEx(eLogLevel level, const std::string &message) {
    if(level<logLevel){
        return;
    }
    logger(level, message.c_str());
}


