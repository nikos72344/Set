//
// Created by mikita on 08.04.2021.
//

#ifndef IVECTOR_LOGGERIMPL_H
#define IVECTOR_LOGGERIMPL_H

#include "ILogger.h"
#include <stdio.h>
#include <string>
#include <map>

class LoggerImpl : public ILogger {
private:
    FILE *stream;
    static std::map<RC, std::string> RCtoString;
    static std::map<ILogger::Level, std::string> LevelToString;

    static RC fillRCMap(void);

    static RC fillLevelMap(void);


    LoggerImpl(const LoggerImpl &);

    LoggerImpl &operator=(const LoggerImpl &);

public:
    LoggerImpl();

    RC setStream(FILE *stream);

    RC log(RC code, Level level, const char *const &srcfile, const char *const &function, int line);

    RC log(RC code, Level level);

};


#endif //IVECTOR_LOGGERIMPL_H
