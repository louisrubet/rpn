#include "program.hpp"

/// @brief re keyword implementation
/// the result is stacked on current program stack
///
void program::rpn_re() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_complex);
    _stack->push_front(new number(real(_stack->value<ocomplex>(0))));
    _stack->erase(1);
}

/// @brief im keyword implementation
/// the result is stacked on current program stack
///
void program::rpn_im() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_complex);
    _stack->push_front(new number(imag(_stack->value<ocomplex>(0))));
    _stack->erase(1);
}

/// @brief arg keyword implementation
/// the result is stacked on current program stack
///
void program::rpn_arg() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_complex);
    _stack->push_front(new number(arg(_stack->value<ocomplex>(0))));
    _stack->erase(1);
}

/// @brief conj keyword implementation
/// the result is stacked on current program stack
///
void program::rpn_conj() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_complex);
    _stack->value<ocomplex>(0) = conj(_stack->value<ocomplex>(0));
}

/// @brief r2c keyword implementation
/// the result is stacked on current program stack
///
void program::rpn_r2c() {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ARG_MUST_BE_OF_TYPE(1, cmd_number);
    _stack->push(new ocomplex(_stack->value<number>(1), _stack->value<number>(0), _stack->obj<ocomplex>(1)->reBase, _stack->obj<ocomplex>(0)->reBase));
    _stack->erase(1, 2);
}

/// @brief c2r keyword implementation
/// the result is stacked on current program stack
///
void program::rpn_c2r() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_complex);
    _stack->push(new number(real(_stack->value<ocomplex>(0)), _stack->obj<ocomplex>(0)->reBase));
    _stack->push(new number(imag(_stack->value<ocomplex>(1)), _stack->obj<ocomplex>(1)->imBase));
    _stack->erase(2);
}

/// @brief r2p keyword implementation
/// the result is stacked on current program stack
///
void program::rpn_r2p() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_complex);
    mpreal rho = abs(_stack->value<ocomplex>(0));
    mpreal theta = arg(_stack->value<ocomplex>(0));
    _stack->value<ocomplex>(0).real(rho);
    _stack->value<ocomplex>(0).imag(theta);
}

/// @brief p2r keyword implementation
/// the result is stacked on current program stack
///
void program::rpn_p2r() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_complex);
    _stack->value<ocomplex>(0) = polar(abs(_stack->value<ocomplex>(0)), arg(_stack->value<ocomplex>(0)));
}
