// Copyright (c) 2014-2022 Louis Rubet

#include <ctime>

#include "program.h"

/// @brief time keyword implementation
///
void program::RpnTime() {
    struct timespec ts;
    struct tm* tm;
    double date;

    // get local date
    clock_gettime(CLOCK_REALTIME, &ts);
    time_t time = (time_t)ts.tv_sec;
    tm = localtime(&time);
    if (tm != nullptr) {
        // date format = HH.MMSSssssss
        date = (static_cast<double>(tm->tm_hour) * 10000000000.0 + static_cast<double>(tm->tm_min) * 100000000.0 +
                static_cast<double>(tm->tm_sec) * 1000000.0 + static_cast<double>(ts.tv_nsec / 1000));

        // push it
        // division after push for real precision
        stack_.push(new Number(date));
        stack_.value<Number>(0) /= 10000000000.0;
    } else {
        ERROR_CONTEXT(kInternalError);
    }
}

/// @brief date keyword implementation
///
void program::RpnDate() {
    struct timespec ts;
    struct tm* tm;
    double date;

    // get local date
    clock_gettime(CLOCK_REALTIME, &ts);
    time_t time = (time_t)ts.tv_sec;
    tm = localtime(&time);
    if (tm != nullptr) {
        // date format = (M)M.DDYYYY
        date = static_cast<double>(tm->tm_mon + 1) * 1000000.0 + static_cast<double>(tm->tm_mday) * 10000.0 +
               static_cast<double>(tm->tm_year + 1900);
        // division after push for real precision
        stack_.push(new Number(date));
        stack_.value<Number>(0) /= 1000000.0;
    } else {
        ERROR_CONTEXT(kInternalError);
    }
}

/// @brief ticks keyword implementation
///
void program::RpnTicks() {
    struct timespec ts;
    struct tm* tm;
    double date;

    // get local date
    clock_gettime(CLOCK_REALTIME, &ts);
    time_t time = (time_t)ts.tv_sec;
    tm = localtime(&time);
    if (tm != nullptr) {
        // date in µs
        date = 1000000.0 * static_cast<double>(ts.tv_sec) + static_cast<double>(ts.tv_nsec / 1000);
        stack_.push(new Number(date));
    } else {
        ERROR_CONTEXT(kInternalError);
    }
}
