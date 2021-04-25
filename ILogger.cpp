//
// Created by mikita on 25.04.2021.
//

#include "LoggerImpl.h"

ILogger *ILogger::createLogger() {
    return (ILogger *) (new LoggerImpl);
}

ILogger *ILogger::createLogger(const char *const &filename, bool overwrite) {
    FILE *stream = fopen(filename, overwrite ? "w" : "a");
    if (stream == nullptr)
        return nullptr;
    LoggerImpl *newLogger = new LoggerImpl;
    newLogger->setStream(stream);
    return (ILogger *) newLogger;
}