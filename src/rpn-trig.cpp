// Copyright (c) 2014-2022 Louis Rubet

#include "program.hpp"

/// @brief pi keyword implementation
///
void program::rpn_pi(void) {
    stack_.push_front(new Number(mpfr::const_pi()));
}

/// @brief d->r keyword implementation
///
void program::rpn_d2r(void) {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kNumber);
    stack_.value<Number>(0) *= mpfr::const_pi();
    stack_.value<Number>(0) /= 180;
}

/// @brief r->d keyword implementation
///
void program::rpn_r2d(void) {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kNumber);
    stack_.value<Number>(0) /= mpfr::const_pi();
    stack_.value<Number>(0) *= 180;
}

/// @brief sin keyword implementation
///
void program::rpn_sin(void) {
    MIN_ARGUMENTS(1);
    if (stack_.type(0) == kNumber)
        stack_.value<Number>(0) = sin(stack_.value<Number>(0));
    else if (stack_.type(0) == kComplex)
        stack_.value<Complex>(0) = sin(stack_.value<Complex>(0));
    else
        setErrorContext(kBadOperandType);
}

/// @brief asin keyword implementation
///
void program::rpn_asin(void) {
    MIN_ARGUMENTS(1);
    if (stack_.type(0) == kNumber)
        stack_.value<Number>(0) = asin(stack_.value<Number>(0));
    else if (stack_.type(0) == kComplex)
        stack_.value<Complex>(0) = asin(stack_.value<Complex>(0));
    else
        setErrorContext(kBadOperandType);
}

/// @brief cos keyword implementation
///
void program::rpn_cos(void) {
    MIN_ARGUMENTS(1);
    if (stack_.type(0) == kNumber)
        stack_.value<Number>(0) = cos(stack_.value<Number>(0));
    else if (stack_.type(0) == kComplex)
        stack_.value<Complex>(0) = cos(stack_.value<Complex>(0));
    else
        setErrorContext(kBadOperandType);
}

/// @brief acos keyword implementation
///
void program::rpn_acos(void) {
    MIN_ARGUMENTS(1);
    if (stack_.type(0) == kNumber)
        stack_.value<Number>(0) = acos(stack_.value<Number>(0));
    else if (stack_.type(0) == kComplex)
        stack_.value<Complex>(0) = acos(stack_.value<Complex>(0));
    else
        setErrorContext(kBadOperandType);
}

/// @brief tan keyword implementation
///
void program::rpn_tan(void) {
    MIN_ARGUMENTS(1);
    if (stack_.type(0) == kNumber)
        stack_.value<Number>(0) = tan(stack_.value<Number>(0));
    else if (stack_.type(0) == kComplex)
        stack_.value<Complex>(0) = tan(stack_.value<Complex>(0));
    else
        setErrorContext(kBadOperandType);
}

/// @brief atan keyword implementation
///
void program::rpn_atan(void) {
    MIN_ARGUMENTS(1);
    if (stack_.type(0) == kNumber)
        stack_.value<Number>(0) = atan(stack_.value<Number>(0));
    else if (stack_.type(0) == kComplex)
        stack_.value<Complex>(0) = atan(stack_.value<Complex>(0));
    else
        setErrorContext(kBadOperandType);
}
