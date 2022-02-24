// Copyright (c) 2014-2022 Louis Rubet

#include "program.hpp"

/// @brief pi keyword implementation
///
void program::rpn_pi(void) {
    _stack.push_front(new number(const_pi()));
}

/// @brief d->r keyword implementation
///
void program::rpn_d2r(void) {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    _stack.value<number>(0) *= const_pi();
    _stack.value<number>(0) /= 180;
}

/// @brief r->d keyword implementation
///
void program::rpn_r2d(void) {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    _stack.value<number>(0) /= const_pi();
    _stack.value<number>(0) *= 180;
}

/// @brief sin keyword implementation
///
void program::rpn_sin(void) {
    MIN_ARGUMENTS(1);
    if (_stack.type(0) == cmd_number)
        _stack.value<number>(0) = sin(_stack.value<number>(0));
    else if (_stack.type(0) == cmd_complex)
        _stack.value<ocomplex>(0) = sin(_stack.value<ocomplex>(0));
    else
        setErrorContext(ret_bad_operand_type);
}

/// @brief asin keyword implementation
///
void program::rpn_asin(void) {
    MIN_ARGUMENTS(1);
    if (_stack.type(0) == cmd_number)
        _stack.value<number>(0) = asin(_stack.value<number>(0));
    else if (_stack.type(0) == cmd_complex)
        _stack.value<ocomplex>(0) = asin(_stack.value<ocomplex>(0));
    else
        setErrorContext(ret_bad_operand_type);
}

/// @brief cos keyword implementation
///
void program::rpn_cos(void) {
    MIN_ARGUMENTS(1);
    if (_stack.type(0) == cmd_number)
        _stack.value<number>(0) = cos(_stack.value<number>(0));
    else if (_stack.type(0) == cmd_complex)
        _stack.value<ocomplex>(0) = cos(_stack.value<ocomplex>(0));
    else
        setErrorContext(ret_bad_operand_type);
}

/// @brief acos keyword implementation
///
void program::rpn_acos(void) {
    MIN_ARGUMENTS(1);
    if (_stack.type(0) == cmd_number)
        _stack.value<number>(0) = acos(_stack.value<number>(0));
    else if (_stack.type(0) == cmd_complex)
        _stack.value<ocomplex>(0) = acos(_stack.value<ocomplex>(0));
    else
        setErrorContext(ret_bad_operand_type);
}

/// @brief tan keyword implementation
///
void program::rpn_tan(void) {
    MIN_ARGUMENTS(1);
    if (_stack.type(0) == cmd_number)
        _stack.value<number>(0) = tan(_stack.value<number>(0));
    else if (_stack.type(0) == cmd_complex)
        _stack.value<ocomplex>(0) = tan(_stack.value<ocomplex>(0));
    else
        setErrorContext(ret_bad_operand_type);
}

/// @brief atan keyword implementation
///
void program::rpn_atan(void) {
    MIN_ARGUMENTS(1);
    if (_stack.type(0) == cmd_number)
        _stack.value<number>(0) = atan(_stack.value<number>(0));
    else if (_stack.type(0) == cmd_complex)
        _stack.value<ocomplex>(0) = atan(_stack.value<ocomplex>(0));
    else
        setErrorContext(ret_bad_operand_type);
}
