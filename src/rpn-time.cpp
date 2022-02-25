// Copyright (c) 2014-2022 Louis Rubet

#include <ctime>

#include "program.hpp"

/// @brief time keyword implementation
///
void program::rpn_time() {
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
        _stack.push(new Number(date));
        _stack.value<Number>(0) /= 10000000000.0;
    } else {
        setErrorContext(ret_internal);
    }
}

/// @brief date keyword implementation
///
void program::rpn_date() {
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

        // push it
        Number* num;
        // division after push for real precision
        _stack.push(new Number(date));
        _stack.value<Number>(0) /= 1000000.0;
    } else {
        setErrorContext(ret_internal);
    }
}

/// @brief ticks keyword implementation
///
void program::rpn_ticks() {
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
        _stack.push(new Number(date));
    } else {
        setErrorContext(ret_internal);
    }
}
