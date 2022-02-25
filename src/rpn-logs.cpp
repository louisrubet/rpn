// Copyright (c) 2014-2022 Louis Rubet

#include "program.hpp"

/// @brief e keyword implementation
///
void program::rpn_e(void) { _stack.push(new Number(mpfr::const_euler())); }

/// @brief log10 keyword implementation
///
void program::rpn_log10() {
    MIN_ARGUMENTS(1);
    if (_stack.type(0) == kNumber)
        _stack.value<Number>(0) = log10(_stack.value<Number>(0));
    else if (_stack.type(0) == kComplex)
        _stack.value<Complex>(0) = log10(_stack.value<Complex>(0));
    else
        setErrorContext(kBadOperandType);
}

/// @brief alog10 keyword implementation
///
void program::rpn_alog10() {
    MIN_ARGUMENTS(1);
    if (_stack.type(0) == kNumber)
        _stack.value<Number>(0) = exp(log(mpreal(10)) * _stack.value<Number>(0));
    else if (_stack.type(0) == kComplex)
        _stack.value<Complex>(0) = exp(log(mpreal(10)) * _stack.value<Complex>(0));
    else
        setErrorContext(kBadOperandType);
}

/// @brief log2 keyword implementation
///
void program::rpn_log2() {
    MIN_ARGUMENTS(1);
    if (_stack.type(0) == kNumber)
        _stack.value<Number>(0) = log(_stack.value<Number>(0)) / mpfr::const_log2();
    else if (_stack.type(0) == kComplex)
        _stack.value<Complex>(0) = log(_stack.value<Complex>(0)) / mpfr::const_log2();
    else
        setErrorContext(kBadOperandType);
}

/// @brief alog2 keyword implementation
///
void program::rpn_alog2() {
    MIN_ARGUMENTS(1);
    if (_stack.type(0) == kNumber)
        _stack.value<Number>(0) = exp(mpfr::const_log2() * _stack.value<Number>(0));
    else if (_stack.type(0) == kComplex)
        _stack.value<Complex>(0) = exp(mpfr::const_log2() * _stack.value<Complex>(0));
    else
        setErrorContext(kBadOperandType);
}

/// @brief ln keyword implementation
///
void program::rpn_ln() {
    MIN_ARGUMENTS(1);
    if (_stack.type(0) == kNumber)
        _stack.value<Number>(0) = log(_stack.value<Number>(0));
    else if (_stack.type(0) == kComplex)
        _stack.value<Complex>(0) = log(_stack.value<Complex>(0));
    else
        setErrorContext(kBadOperandType);
}

/// @brief exp keyword implementation
///
void program::rpn_exp() {
    MIN_ARGUMENTS(1);
    if (_stack.type(0) == kNumber)
        _stack.value<Number>(0) = exp(_stack.value<Number>(0));
    else if (_stack.type(0) == kComplex)
        _stack.value<Complex>(0) = exp(_stack.value<Complex>(0));
    else
        setErrorContext(kBadOperandType);
}

/// @brief expm keyword implementation
///
void program::rpn_expm() {
    MIN_ARGUMENTS(1);
    if (_stack.type(0) == kNumber)
        _stack.value<Number>(0) = exp(_stack.value<Number>(0)) - mpreal(1);
    else if (_stack.type(0) == kComplex)
        _stack.value<Complex>(0) = exp(_stack.value<Complex>(0)) - mpreal(1);
    else
        setErrorContext(kBadOperandType);
}

/// @brief lnp1 keyword implementation
///
void program::rpn_lnp1() {
    MIN_ARGUMENTS(1);
    if (_stack.type(0) == kNumber)
        _stack.value<Number>(0) = log(_stack.value<Number>(0) + 1);
    else if (_stack.type(0) == kComplex)
        _stack.value<Complex>(0) = log(_stack.value<Complex>(0) + mpreal(1));
    else
        setErrorContext(kBadOperandType);
}

/// @brief sinh keyword implementation
///
void program::rpn_sinh() {
    MIN_ARGUMENTS(1);
    if (_stack.type(0) == kNumber)
        _stack.value<Number>(0) = sinh(_stack.value<Number>(0));
    else if (_stack.type(0) == kComplex)
        _stack.value<Complex>(0) = sinh(_stack.value<Complex>(0));
    else
        setErrorContext(kBadOperandType);
}

/// @brief asinh keyword implementation
///
void program::rpn_asinh() {
    MIN_ARGUMENTS(1);
    if (_stack.type(0) == kNumber)
        _stack.value<Number>(0) = asinh(_stack.value<Number>(0));
    else if (_stack.type(0) == kComplex)
        _stack.value<Complex>(0) = asinh(_stack.value<Complex>(0));
    else
        setErrorContext(kBadOperandType);
}

/// @brief cosh keyword implementation
///
void program::rpn_cosh() {
    MIN_ARGUMENTS(1);
    if (_stack.type(0) == kNumber)
        _stack.value<Number>(0) = cosh(_stack.value<Number>(0));
    else if (_stack.type(0) == kComplex)
        _stack.value<Complex>(0) = cosh(_stack.value<Complex>(0));
    else
        setErrorContext(kBadOperandType);
}

/// @brief acosh keyword implementation
///
void program::rpn_acosh() {
    MIN_ARGUMENTS(1);
    if (_stack.type(0) == kNumber)
        _stack.value<Number>(0) = acosh(_stack.value<Number>(0));
    else if (_stack.type(0) == kComplex)
        _stack.value<Complex>(0) = acosh(_stack.value<Complex>(0));
    else
        setErrorContext(kBadOperandType);
}

/// @brief tanh keyword implementation
///
void program::rpn_tanh() {
    MIN_ARGUMENTS(1);
    if (_stack.type(0) == kNumber)
        _stack.value<Number>(0) = tanh(_stack.value<Number>(0));
    else if (_stack.type(0) == kComplex)
        _stack.value<Complex>(0) = tanh(_stack.value<Complex>(0));
    else
        setErrorContext(kBadOperandType);
}

/// @brief atanh keyword implementation
///
void program::rpn_atanh() {
    MIN_ARGUMENTS(1);
    if (_stack.type(0) == kNumber)
        _stack.value<Number>(0) = atanh(_stack.value<Number>(0));
    else if (_stack.type(0) == kComplex)
        _stack.value<Complex>(0) = atanh(_stack.value<Complex>(0));
    else
        setErrorContext(kBadOperandType);
}
