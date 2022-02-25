// Copyright (c) 2014-2022 Louis Rubet

#include "program.h"

/// @brief re keyword implementation
/// the result is stacked on current program stack
///
void program::RpnReal() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kComplex);
    stack_.push_front(new Number(real(stack_.value<Complex>(0))));
    stack_.erase(1);
}

/// @brief im keyword implementation
/// the result is stacked on current program stack
///
void program::RpnImag() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kComplex);
    stack_.push_front(new Number(imag(stack_.value<Complex>(0))));
    stack_.erase(1);
}

/// @brief arg keyword implementation
/// the result is stacked on current program stack
///
void program::RpnArg() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kComplex);
    stack_.push_front(new Number(arg(stack_.value<Complex>(0))));
    stack_.erase(1);
}

/// @brief conj keyword implementation
/// the result is stacked on current program stack
///
void program::RpnConj() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kComplex);
    stack_.value<Complex>(0) = conj(stack_.value<Complex>(0));
}

/// @brief r2c keyword implementation
/// the result is stacked on current program stack
///
void program::RpnR2c() {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, kNumber);
    ARG_MUST_BE_OF_TYPE(1, kNumber);
    stack_.push(new Complex(stack_.value<Number>(1), stack_.value<Number>(0), stack_.obj<Complex>(1).re_base, stack_.obj<Complex>(0).re_base));
    stack_.erase(1, 2);
}

/// @brief c2r keyword implementation
/// the result is stacked on current program stack
///
void program::RpnC2r() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kComplex);
    stack_.push(new Number(real(stack_.value<Complex>(0)), stack_.obj<Complex>(0).re_base));
    stack_.push(new Number(imag(stack_.value<Complex>(1)), stack_.obj<Complex>(1).im_base));
    stack_.erase(2);
}

/// @brief r2p keyword implementation
/// the result is stacked on current program stack
///
void program::RpnR2p() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kComplex);
    mpreal rho = abs(stack_.value<Complex>(0));
    mpreal theta = arg(stack_.value<Complex>(0));
    stack_.value<Complex>(0).real(rho);
    stack_.value<Complex>(0).imag(theta);
}

/// @brief p2r keyword implementation
/// the result is stacked on current program stack
///
void program::RpnP2r() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kComplex);
    stack_.value<Complex>(0) = polar(abs(stack_.value<Complex>(0)), arg(stack_.value<Complex>(0)));
}
