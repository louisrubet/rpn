// Copyright (c) 2014-2022 Louis Rubet

#include <fcntl.h>

#include "program.hpp"

/// @brief ->str keyword implementation
///
void program::rpn_instr() {
    MIN_ARGUMENTS(1);

    // stringify only if not already a string
    if (_stack.type(0) != kString) {
        stringstream ss;
        ss << _stack.at(0);
        _stack.pop();
        _stack.push(new String(ss.str()));
    }
}

/// @brief str-> keyword implementation
///
void program::rpn_strout() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kString);

    string entry(_stack.value<String>(0));
    program prog(_stack, _heap);
    _stack.pop();

    // make program from string in stack level 1
    if (prog.parse(entry) == kOk)
        // run it
        prog.run();
}

/// @brief chr keyword implementation
///
void program::rpn_chr() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kNumber);
    char the_chr = static_cast<char>(_stack.value<Number>(0).toLong());
    _stack.pop();
    if (the_chr < 32 || the_chr > 126) the_chr = '.';
    _stack.push_front(new String(string(1, the_chr)));
}

/// @brief num keyword implementation
///
void program::rpn_num() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kString);
    if (_stack.value<String>(0).size() > 0)
        _stack.push_front(new Number(_stack.value<String>(0)[0]));
    else
        _stack.push_front(new Number(0));
    _stack.erase(1);
}

/// @brief size keyword implementation
///
void program::rpn_strsize() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kString);
    _stack.push_front(new Number(_stack.value<String>(0).size()));
    _stack.erase(1);
}

/// @brief pos keyword implementation
///
void program::rpn_strpos() {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, kString);
    ARG_MUST_BE_OF_TYPE(1, kString);

    size_t pos = _stack.value<String>(1).find(_stack.value<String>(0)) + 1;
    _stack.erase(0, 2);
    _stack.push_front(new Number(pos));
}

/// @brief sub keyword implementation
///
void program::rpn_strsub() {
    MIN_ARGUMENTS(3);
    ARG_MUST_BE_OF_TYPE(0, kNumber);
    ARG_MUST_BE_OF_TYPE(1, kNumber);
    ARG_MUST_BE_OF_TYPE(2, kString);

    size_t first = _stack.value<Number>(1).toULong();
    size_t len = _stack.value<Number>(0).toULong() - first + 1;
    first--;

    if (first > _stack.value<String>(2).size()) first = len = 0;
    _stack.push(new String(_stack.value<String>(2).substr(first, len)));
    _stack.erase(1, 3);
}
