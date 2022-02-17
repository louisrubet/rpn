#include <fcntl.h>

#include "program.hpp"

/// @brief ->str keyword implementation
///
void program::rpn_instr() {
    MIN_ARGUMENTS(1);

    // stringify only if not already a string
    if (_stack->at(0)->_type != cmd_string) {
        stringstream ss;
        ss << _stack->at(0);
        _stack->pop();
        _stack->push(new ostring(ss.str()));
    }
}

/// @brief str-> keyword implementation
///
void program::rpn_strout() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_string);

    string entry(_stack->value<ostring>(0));
    program prog;
    _stack->pop();

    // make program from string in stack level 1
    if (program::parse(entry, prog) == ret_ok)
        // run it
        prog.run(*_stack, *_heap);
}

/// @brief chr keyword implementation
///
void program::rpn_chr() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    char the_chr = (char)(int)_stack->value<number>(0);
    _stack->pop();
    if (the_chr < 32 || the_chr > 126) the_chr = '.';
    _stack->push_front(new ostring(string(1, the_chr)));
}

/// @brief num keyword implementation
///
void program::rpn_num() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_string);
    if (_stack->value<ostring>(0).size() > 0)
        _stack->push_front(new number(_stack->value<ostring>(0)[0]));
    else
        _stack->push_front(new number(0));
    _stack->erase(1);
}

/// @brief size keyword implementation
///
void program::rpn_strsize() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_string);
    _stack->push_front(new number(_stack->value<ostring>(0).size()));
    _stack->erase(1);
}

/// @brief pos keyword implementation
///
void program::rpn_strpos() {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_string);
    ARG_MUST_BE_OF_TYPE(1, cmd_string);

    size_t pos = _stack->value<ostring>(1).find(_stack->value<ostring>(0)) + 1;
    _stack->pop_front(2);
    _stack->push_front(new number(pos));
}

/// @brief sub keyword implementation
///
void program::rpn_strsub() {
    MIN_ARGUMENTS(3);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ARG_MUST_BE_OF_TYPE(1, cmd_number);
    ARG_MUST_BE_OF_TYPE(2, cmd_string);

    size_t first = (size_t)_stack->value<number>(1);
    size_t len = (size_t)_stack->value<number>(0) - first + 1;
    first--;

    if (first > _stack->value<ostring>(2).size()) first = len = 0;
    _stack->push(new ostring(_stack->value<ostring>(2).substr(first, len)));
    _stack->erase(1, 3);
}
