// Copyright (c) 2014-2022 Louis Rubet

#include <chrono>
#include <ctime>
using namespace std::chrono;

#include "program.h"

/// @brief time keyword implementation
///
void Program::RpnTime() {
    std::time_t rawtime = system_clock::to_time_t(system_clock::now());
    struct tm tm;

    if (localtime_r(&rawtime, &tm) != nullptr) {
        char buffer[80];
        size_t sz = strftime(buffer, sizeof(buffer), "%T", &tm);
        if (sz > 0)
            stack_.push(new String(buffer));
        else
            ERROR_CONTEXT(kInternalError);
    } else {
        ERROR_CONTEXT(kInternalError);
    }
}

/// @brief date keyword implementation
///
void Program::RpnDate() {
    std::time_t rawtime = system_clock::to_time_t(system_clock::now());
    struct tm tm;

    if (localtime_r(&rawtime, &tm) != nullptr) {
        char buffer[80];
        size_t sz = strftime(buffer, sizeof(buffer), "%F", &tm);
        if (sz > 0)
            stack_.push(new String(buffer));
        else
            ERROR_CONTEXT(kInternalError);
    } else {
        ERROR_CONTEXT(kInternalError);
    }
}

/// @brief ticks keyword implementation
///
void Program::RpnTicks() {
    uint64_t time_span = (uint64_t)duration_cast<microseconds>(high_resolution_clock::now().time_since_epoch()).count();
    stack_.push(new Number(time_span));
}
