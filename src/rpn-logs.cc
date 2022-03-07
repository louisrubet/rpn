// Copyright (c) 2014-2022 Louis Rubet

#include "program.h"

/// @brief e keyword implementation
///
void Program::RpnE(void) { stack_.push(new Number(mpfr::const_euler())); }

/// @brief log10 keyword implementation
///
void Program::RpnLog10() {
    MIN_ARGUMENTS(1);
    if (stack_.type(0) == kNumber)
        stack_.value<Number>(0) = log10(stack_.value<Number>(0));
    else if (stack_.type(0) == kComplex)
        stack_.value<Complex>(0) = log10(stack_.value<Complex>(0));
    else
        ERROR_CONTEXT(kBadOperandType);
}

/// @brief alog10 keyword implementation
///
void Program::RpnAlog10() {
    MIN_ARGUMENTS(1);
    if (stack_.type(0) == kNumber)
        stack_.value<Number>(0) = exp(log(mpreal(10)) * stack_.value<Number>(0));
    else if (stack_.type(0) == kComplex)
        stack_.value<Complex>(0) = exp(log(mpreal(10)) * stack_.value<Complex>(0));
    else
        ERROR_CONTEXT(kBadOperandType);
}

/// @brief log2 keyword implementation
///
void Program::RpnLog2() {
    MIN_ARGUMENTS(1);
    if (stack_.type(0) == kNumber)
        stack_.value<Number>(0) = log(stack_.value<Number>(0)) / mpfr::const_log2();
    else if (stack_.type(0) == kComplex)
        stack_.value<Complex>(0) = log(stack_.value<Complex>(0)) / mpfr::const_log2();
    else
        ERROR_CONTEXT(kBadOperandType);
}

/// @brief alog2 keyword implementation
///
void Program::RpnAlog2() {
    MIN_ARGUMENTS(1);
    if (stack_.type(0) == kNumber)
        stack_.value<Number>(0) = exp(mpfr::const_log2() * stack_.value<Number>(0));
    else if (stack_.type(0) == kComplex)
        stack_.value<Complex>(0) = exp(mpfr::const_log2() * stack_.value<Complex>(0));
    else
        ERROR_CONTEXT(kBadOperandType);
}

/// @brief ln keyword implementation
///
void Program::RpnLn() {
    MIN_ARGUMENTS(1);
    if (stack_.type(0) == kNumber)
        stack_.value<Number>(0) = log(stack_.value<Number>(0));
    else if (stack_.type(0) == kComplex)
        stack_.value<Complex>(0) = log(stack_.value<Complex>(0));
    else
        ERROR_CONTEXT(kBadOperandType);
}

/// @brief exp keyword implementation
///
void Program::RpnExp() {
    MIN_ARGUMENTS(1);
    if (stack_.type(0) == kNumber)
        stack_.value<Number>(0) = exp(stack_.value<Number>(0));
    else if (stack_.type(0) == kComplex)
        stack_.value<Complex>(0) = exp(stack_.value<Complex>(0));
    else
        ERROR_CONTEXT(kBadOperandType);
}

/// @brief expm keyword implementation
///
void Program::RpnExpm() {
    MIN_ARGUMENTS(1);
    if (stack_.type(0) == kNumber)
        stack_.value<Number>(0) = exp(stack_.value<Number>(0)) - mpreal(1);
    else if (stack_.type(0) == kComplex)
        stack_.value<Complex>(0) = exp(stack_.value<Complex>(0)) - mpreal(1);
    else
        ERROR_CONTEXT(kBadOperandType);
}

/// @brief lnp1 keyword implementation
///
void Program::RpnLnp1() {
    MIN_ARGUMENTS(1);
    if (stack_.type(0) == kNumber)
        stack_.value<Number>(0) = log(stack_.value<Number>(0) + 1);
    else if (stack_.type(0) == kComplex)
        stack_.value<Complex>(0) = log(stack_.value<Complex>(0) + mpreal(1));
    else
        ERROR_CONTEXT(kBadOperandType);
}

/// @brief sinh keyword implementation
///
void Program::RpnSinh() {
    MIN_ARGUMENTS(1);
    if (stack_.type(0) == kNumber)
        stack_.value<Number>(0) = sinh(stack_.value<Number>(0));
    else if (stack_.type(0) == kComplex)
        stack_.value<Complex>(0) = sinh(stack_.value<Complex>(0));
    else
        ERROR_CONTEXT(kBadOperandType);
}

/// @brief asinh keyword implementation
///
void Program::RpnAsinh() {
    MIN_ARGUMENTS(1);
    if (stack_.type(0) == kNumber)
        stack_.value<Number>(0) = asinh(stack_.value<Number>(0));
    else if (stack_.type(0) == kComplex)
        stack_.value<Complex>(0) = asinh(stack_.value<Complex>(0));
    else
        ERROR_CONTEXT(kBadOperandType);
}

/// @brief cosh keyword implementation
///
void Program::RpnCosh() {
    MIN_ARGUMENTS(1);
    if (stack_.type(0) == kNumber)
        stack_.value<Number>(0) = cosh(stack_.value<Number>(0));
    else if (stack_.type(0) == kComplex)
        stack_.value<Complex>(0) = cosh(stack_.value<Complex>(0));
    else
        ERROR_CONTEXT(kBadOperandType);
}

/// @brief acosh keyword implementation
///
void Program::RpnAcosh() {
    MIN_ARGUMENTS(1);
    if (stack_.type(0) == kNumber)
        stack_.value<Number>(0) = acosh(stack_.value<Number>(0));
    else if (stack_.type(0) == kComplex)
        stack_.value<Complex>(0) = acosh(stack_.value<Complex>(0));
    else
        ERROR_CONTEXT(kBadOperandType);
}

/// @brief tanh keyword implementation
///
void Program::RpnTanh() {
    MIN_ARGUMENTS(1);
    if (stack_.type(0) == kNumber)
        stack_.value<Number>(0) = tanh(stack_.value<Number>(0));
    else if (stack_.type(0) == kComplex)
        stack_.value<Complex>(0) = tanh(stack_.value<Complex>(0));
    else
        ERROR_CONTEXT(kBadOperandType);
}

/// @brief atanh keyword implementation
///
void Program::RpnAtanh() {
    MIN_ARGUMENTS(1);
    if (stack_.type(0) == kNumber)
        stack_.value<Number>(0) = atanh(stack_.value<Number>(0));
    else if (stack_.type(0) == kComplex)
        stack_.value<Complex>(0) = atanh(stack_.value<Complex>(0));
    else
        ERROR_CONTEXT(kBadOperandType);
}
