// Copyright (c) 2014-2022 Louis Rubet

#include <fcntl.h>

#include "program.hpp"

/// @brief ->str keyword implementation
///
void program::rpn_instr() {
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
void program::rpn_strout() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kString);

    string entry(stack_.value<String>(0));
    program prog(stack_, heap_);
    stack_.pop();

    // make program from string in stack_ level 1
    if (prog.parse(entry) == kOk)
        // run it
        prog.run();
}

/// @brief chr keyword implementation
///
void program::rpn_chr() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kNumber);
    char the_chr = static_cast<char>(stack_.value<Number>(0).toLong());
    stack_.pop();
    if (the_chr < 32 || the_chr > 126) the_chr = '.';
    stack_.push_front(new String(string(1, the_chr)));
}

/// @brief num keyword implementation
///
void program::rpn_num() {
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
void program::rpn_strsize() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kString);
    stack_.push_front(new Number(stack_.value<String>(0).size()));
    stack_.erase(1);
}

/// @brief pos keyword implementation
///
void program::rpn_strpos() {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, kString);
    ARG_MUST_BE_OF_TYPE(1, kString);

    size_t pos = stack_.value<String>(1).find(stack_.value<String>(0)) + 1;
    stack_.erase(0, 2);
    stack_.push_front(new Number(pos));
}

/// @brief sub keyword implementation
///
void program::rpn_strsub() {
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
