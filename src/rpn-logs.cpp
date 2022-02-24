// Copyright (c) 2014-2022 Louis Rubet

#include "program.hpp"

/// @brief e keyword implementation
///
void program::rpn_e(void) { _stack.push(new number(const_euler())); }

/// @brief log10 keyword implementation
///
void program::rpn_log10() {
    MIN_ARGUMENTS(1);
    if (_stack.type(0) == cmd_number)
        _stack.value<number>(0) = log10(_stack.value<number>(0));
    else if (_stack.type(0) == cmd_complex)
        _stack.value<ocomplex>(0) = log10(_stack.value<ocomplex>(0));
    else
        setErrorContext(ret_bad_operand_type);
}

/// @brief alog10 keyword implementation
///
void program::rpn_alog10() {
    MIN_ARGUMENTS(1);
    if (_stack.type(0) == cmd_number)
        _stack.value<number>(0) = exp(log(mpreal(10)) * _stack.value<number>(0));
    else if (_stack.type(0) == cmd_complex)
        _stack.value<ocomplex>(0) = exp(log(mpreal(10)) * _stack.value<ocomplex>(0));
    else
        setErrorContext(ret_bad_operand_type);
}

/// @brief log2 keyword implementation
///
void program::rpn_log2() {
    MIN_ARGUMENTS(1);
    if (_stack.type(0) == cmd_number)
        _stack.value<number>(0) = log(_stack.value<number>(0)) / const_log2();
    else if (_stack.type(0) == cmd_complex)
        _stack.value<ocomplex>(0) = log(_stack.value<ocomplex>(0)) / const_log2();
    else
        setErrorContext(ret_bad_operand_type);
}

/// @brief alog2 keyword implementation
///
void program::rpn_alog2() {
    MIN_ARGUMENTS(1);
    if (_stack.type(0) == cmd_number)
        _stack.value<number>(0) = exp(const_log2() * _stack.value<number>(0));
    else if (_stack.type(0) == cmd_complex)
        _stack.value<ocomplex>(0) = exp(const_log2() * _stack.value<ocomplex>(0));
    else
        setErrorContext(ret_bad_operand_type);
}

/// @brief ln keyword implementation
///
void program::rpn_ln() {
    MIN_ARGUMENTS(1);
    if (_stack.type(0) == cmd_number)
        _stack.value<number>(0) = log(_stack.value<number>(0));
    else if (_stack.type(0) == cmd_complex)
        _stack.value<ocomplex>(0) = log(_stack.value<ocomplex>(0));
    else
        setErrorContext(ret_bad_operand_type);
}

/// @brief exp keyword implementation
///
void program::rpn_exp() {
    MIN_ARGUMENTS(1);
    if (_stack.type(0) == cmd_number)
        _stack.value<number>(0) = exp(_stack.value<number>(0));
    else if (_stack.type(0) == cmd_complex)
        _stack.value<ocomplex>(0) = exp(_stack.value<ocomplex>(0));
    else
        setErrorContext(ret_bad_operand_type);
}

/// @brief expm keyword implementation
///
void program::rpn_expm() {
    MIN_ARGUMENTS(1);
    if (_stack.type(0) == cmd_number)
        _stack.value<number>(0) = exp(_stack.value<number>(0)) - mpreal(1);
    else if (_stack.type(0) == cmd_complex)
        _stack.value<ocomplex>(0) = exp(_stack.value<ocomplex>(0)) - mpreal(1);
    else
        setErrorContext(ret_bad_operand_type);
}

/// @brief lnp1 keyword implementation
///
void program::rpn_lnp1() {
    MIN_ARGUMENTS(1);
    if (_stack.type(0) == cmd_number)
        _stack.value<number>(0) = log(_stack.value<number>(0) + 1);
    else if (_stack.type(0) == cmd_complex)
        _stack.value<ocomplex>(0) = log(_stack.value<ocomplex>(0) + mpreal(1));
    else
        setErrorContext(ret_bad_operand_type);
}

/// @brief sinh keyword implementation
///
void program::rpn_sinh() {
    MIN_ARGUMENTS(1);
    if (_stack.type(0) == cmd_number)
        _stack.value<number>(0) = sinh(_stack.value<number>(0));
    else if (_stack.type(0) == cmd_complex)
        _stack.value<ocomplex>(0) = sinh(_stack.value<ocomplex>(0));
    else
        setErrorContext(ret_bad_operand_type);
}

/// @brief asinh keyword implementation
///
void program::rpn_asinh() {
    MIN_ARGUMENTS(1);
    if (_stack.type(0) == cmd_number)
        _stack.value<number>(0) = asinh(_stack.value<number>(0));
    else if (_stack.type(0) == cmd_complex)
        _stack.value<ocomplex>(0) = asinh(_stack.value<ocomplex>(0));
    else
        setErrorContext(ret_bad_operand_type);
}

/// @brief cosh keyword implementation
///
void program::rpn_cosh() {
    MIN_ARGUMENTS(1);
    if (_stack.type(0) == cmd_number)
        _stack.value<number>(0) = cosh(_stack.value<number>(0));
    else if (_stack.type(0) == cmd_complex)
        _stack.value<ocomplex>(0) = cosh(_stack.value<ocomplex>(0));
    else
        setErrorContext(ret_bad_operand_type);
}

/// @brief acosh keyword implementation
///
void program::rpn_acosh() {
    MIN_ARGUMENTS(1);
    if (_stack.type(0) == cmd_number)
        _stack.value<number>(0) = acosh(_stack.value<number>(0));
    else if (_stack.type(0) == cmd_complex)
        _stack.value<ocomplex>(0) = acosh(_stack.value<ocomplex>(0));
    else
        setErrorContext(ret_bad_operand_type);
}

/// @brief tanh keyword implementation
///
void program::rpn_tanh() {
    MIN_ARGUMENTS(1);
    if (_stack.type(0) == cmd_number)
        _stack.value<number>(0) = tanh(_stack.value<number>(0));
    else if (_stack.type(0) == cmd_complex)
        _stack.value<ocomplex>(0) = tanh(_stack.value<ocomplex>(0));
    else
        setErrorContext(ret_bad_operand_type);
}

/// @brief atanh keyword implementation
///
void program::rpn_atanh() {
    MIN_ARGUMENTS(1);
    if (_stack.type(0) == cmd_number)
        _stack.value<number>(0) = atanh(_stack.value<number>(0));
    else if (_stack.type(0) == cmd_complex)
        _stack.value<ocomplex>(0) = atanh(_stack.value<ocomplex>(0));
    else
        setErrorContext(ret_bad_operand_type);
}
