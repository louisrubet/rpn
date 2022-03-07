// Copyright (c) 2014-2022 Louis Rubet

#include <fcntl.h>

#include "program.h"

/// @brief ->str keyword implementation
///
void Program::RpnInstr() {
    MIN_ARGUMENTS(1);

    // stringify only if not already a string
    if (stack_.type(0) != kString) {
        stringstream ss;
        ss << stack_.at(0);
        stack_.pop();
        stack_.push(new String(ss.str()));
    }
}

/// @brief str-> keyword implementation
///
void Program::RpnStrout() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kString);

    string entry(stack_.value<String>(0));
    Program prog(stack_, heap_);
    stack_.pop();

    // make program from string in stack_ level 1
    if (prog.Parse(entry) == kOk)
        // run it
        prog.Run();
}

/// @brief chr keyword implementation
///
void Program::RpnChr() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kNumber);
    char the_chr = static_cast<char>(stack_.value<Number>(0).toLong());
    stack_.pop();
    if (the_chr < 32 || the_chr > 126) the_chr = '.';
    stack_.push_front(new String(string(1, the_chr)));
}

/// @brief num keyword implementation
///
void Program::RpnNum() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kString);
    if (stack_.value<String>(0).size() > 0)
        stack_.push_front(new Number(stack_.value<String>(0)[0]));
    else
        stack_.push_front(new Number(0));
    stack_.erase(1);
}

/// @brief size keyword implementation
///
void Program::RpnStrsize() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kString);
    stack_.push_front(new Number(stack_.value<String>(0).size()));
    stack_.erase(1);
}

/// @brief pos keyword implementation
///
void Program::RpnStrpos() {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, kString);
    ARG_MUST_BE_OF_TYPE(1, kString);

    size_t pos = stack_.value<String>(1).find(stack_.value<String>(0)) + 1;
    stack_.erase(0, 2);
    stack_.push_front(new Number(pos));
}

/// @brief sub keyword implementation
///
void Program::RpnStrsub() {
    MIN_ARGUMENTS(3);
    ARG_MUST_BE_OF_TYPE(0, kNumber);
    ARG_MUST_BE_OF_TYPE(1, kNumber);
    ARG_MUST_BE_OF_TYPE(2, kString);

    size_t first = stack_.value<Number>(1).toULong();
    size_t len = stack_.value<Number>(0).toULong() - first + 1;
    first--;

    if (first > stack_.value<String>(2).size()) first = len = 0;
    stack_.push(new String(stack_.value<String>(2).substr(first, len)));
    stack_.erase(1, 3);
}
