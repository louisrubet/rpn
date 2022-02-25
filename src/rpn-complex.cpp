// Copyright (c) 2014-2022 Louis Rubet

#include "program.hpp"

/// @brief re keyword implementation
/// the result is stacked on current program stack
///
void program::rpn_re() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kComplex);
    _stack.push_front(new Number(real(_stack.value<Complex>(0))));
    _stack.erase(1);
}

/// @brief im keyword implementation
/// the result is stacked on current program stack
///
void program::rpn_im() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kComplex);
    _stack.push_front(new Number(imag(_stack.value<Complex>(0))));
    _stack.erase(1);
}

/// @brief arg keyword implementation
/// the result is stacked on current program stack
///
void program::rpn_arg() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kComplex);
    _stack.push_front(new Number(arg(_stack.value<Complex>(0))));
    _stack.erase(1);
}

/// @brief conj keyword implementation
/// the result is stacked on current program stack
///
void program::rpn_conj() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kComplex);
    _stack.value<Complex>(0) = conj(_stack.value<Complex>(0));
}

/// @brief r2c keyword implementation
/// the result is stacked on current program stack
///
void program::rpn_r2c() {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, kNumber);
    ARG_MUST_BE_OF_TYPE(1, kNumber);
    _stack.push(new Complex(_stack.value<Number>(1), _stack.value<Number>(0), _stack.obj<Complex>(1).reBase, _stack.obj<Complex>(0).reBase));
    _stack.erase(1, 2);
}

/// @brief c2r keyword implementation
/// the result is stacked on current program stack
///
void program::rpn_c2r() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kComplex);
    _stack.push(new Number(real(_stack.value<Complex>(0)), _stack.obj<Complex>(0).reBase));
    _stack.push(new Number(imag(_stack.value<Complex>(1)), _stack.obj<Complex>(1).imBase));
    _stack.erase(2);
}

/// @brief r2p keyword implementation
/// the result is stacked on current program stack
///
void program::rpn_r2p() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kComplex);
    mpreal rho = abs(_stack.value<Complex>(0));
    mpreal theta = arg(_stack.value<Complex>(0));
    _stack.value<Complex>(0).real(rho);
    _stack.value<Complex>(0).imag(theta);
}

/// @brief p2r keyword implementation
/// the result is stacked on current program stack
///
void program::rpn_p2r() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kComplex);
    _stack.value<Complex>(0) = polar(abs(_stack.value<Complex>(0)), arg(_stack.value<Complex>(0)));
}
