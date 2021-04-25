//
// Created by mikita on 08.04.2021.
//

#include "LoggerImpl.h"

std::map<RC, std::string> LoggerImpl::RCtoString;
std::map<ILogger::Level, std::string> LoggerImpl::LevelToString;

RC LoggerImpl::fillRCMap() {
    std::string str("UNKNOWN");
    RCtoString.insert((std::pair<RC, std::string>) {RC::UNKNOWN, str});
    str = "SUCCESS";
    RCtoString.insert((std::pair<RC, std::string>) {RC::SUCCESS, str});
    str = "INVALID_ARGUMENT";
    RCtoString.insert((std::pair<RC, std::string>) {RC::INVALID_ARGUMENT, str});
    str = "MISMATCHING_DIMENSIONS";
    RCtoString.insert((std::pair<RC, std::string>) {RC::MISMATCHING_DIMENSIONS, str});
    str = "INDEX_OUT_OF_BOUND";
    RCtoString.insert((std::pair<RC, std::string>) {RC::INDEX_OUT_OF_BOUND, str});
    str = "INFINITY_OVERFLOW";
    RCtoString.insert((std::pair<RC, std::string>) {RC::INFINITY_OVERFLOW, str});
    str = "NOT_NUMBER";
    RCtoString.insert((std::pair<RC, std::string>) {RC::NOT_NUMBER, str});
    str = "ALLOCATION_ERROR";
    RCtoString.insert((std::pair<RC, std::string>) {RC::ALLOCATION_ERROR, str});
    str = "NULLPTR_ERROR";
    RCtoString.insert((std::pair<RC, std::string>) {RC::NULLPTR_ERROR, str});
    str = "FILE_NOT_FOUND";
    RCtoString.insert((std::pair<RC, std::string>) {RC::FILE_NOT_FOUND, str});
    str = "VECTOR_NOT_FOUND";
    RCtoString.insert((std::pair<RC, std::string>) {RC::VECTOR_NOT_FOUND, str});
    str = "IO_ERROR";
    RCtoString.insert((std::pair<RC, std::string>) {RC::IO_ERROR, str});
    str = "MEMORY_INTERSECTION";
    RCtoString.insert((std::pair<RC, std::string>) {RC::MEMORY_INTERSECTION, str});
    str = "AMOUNT";
    RCtoString.insert((std::pair<RC, std::string>) {RC::AMOUNT, str});
    return RC::SUCCESS;
}

RC LoggerImpl::fillLevelMap() {
    std::string str("SEVERE");
    LevelToString.insert((std::pair<ILogger::Level, std::string>) {Level::SEVERE, str});
    str = "WARNING";
    LevelToString.insert((std::pair<ILogger::Level, std::string>) {Level::WARNING, str});
    str = "INFO";
    LevelToString.insert((std::pair<ILogger::Level, std::string>) {Level::INFO, str});
    return RC::SUCCESS;
}

LoggerImpl::LoggerImpl() {
    stream = stdout;
    if (RCtoString.size() == 0)
        fillRCMap();
    if (LevelToString.size() == 0)
        fillLevelMap();
}

RC LoggerImpl::setStream(FILE *stream) {
    this->stream = stream;
    return RC::SUCCESS;
}

RC LoggerImpl::log(RC code, Level level, const char *const &srcfile, const char *const &function, int line) {
    if (stream == nullptr)
        return RC::IO_ERROR;
    fprintf(stream, "%s %s", LevelToString.operator[](level).data(), RCtoString.operator[](code).data());
    int flag = 1;
    if (srcfile != nullptr) {
        fprintf(stream, ": %s", *&srcfile);
        flag = 0;
    }
    if (function != nullptr) {
        if (flag)
            fprintf(stream, ":");
        fprintf(stream, " %s", *&function);
        flag = 0;
    }
    if (line >= 1) {
        if (flag)
            fprintf(stream, ":");
        fprintf(stream, " %i", line);
    }
    fprintf(stream, ";\n");
    return RC::SUCCESS;
}

RC LoggerImpl::log(RC code, Level level) {
    return log(code, level, nullptr, nullptr, 0);
}