#include "program.hpp"

/// @brief re keyword implementation
/// the result is stacked on current program stack
///
void program::rpn_re() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_complex);

    rpnstack::copy_and_push_front(*_stack, _stack->size() - 1, _calc_stack);
    _stack->pop_front();

    number* re = new number();
    _stack->push_front(re);
    CHECK_MPFR(mpfr_set(re->_value.mpfr, ((complex*)_calc_stack.at(0))->re()->mpfr, mpreal::get_default_rnd()));
    _calc_stack.pop_front();
}

/// @brief im keyword implementation
/// the result is stacked on current program stack
///
void program::rpn_im() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_complex);

    rpnstack::copy_and_push_front(*_stack, _stack->size() - 1, _calc_stack);
    _stack->pop_front();

    number* im = new number();
    _stack->push_front(im);
    CHECK_MPFR(mpfr_set(im->_value.mpfr, ((complex*)_calc_stack.at(0))->im()->mpfr, mpreal::get_default_rnd()));
    _calc_stack.pop_front();
}

/// @brief arg keyword implementation
/// the result is stacked on current program stack
///
void program::rpn_arg() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_complex);

    // calc atan2(x/y)
    complex* cplx = (complex*)_stack->front();
    number* num = new number();
    _calc_stack.push_back(num);

    CHECK_MPFR(mpfr_atan2(num->_value.mpfr, cplx->im()->mpfr, cplx->re()->mpfr, mpreal::get_default_rnd()));

    rpnstack::copy_and_push_front(_calc_stack, _calc_stack.size() - 1, *_stack);
    _calc_stack.pop_front();
}

/// @brief conj keyword implementation
/// the result is stacked on current program stack
///
void program::rpn_conj() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_complex);

    complex* cplx = (complex*)_stack->back();
    CHECK_MPFR(mpfr_neg(cplx->im()->mpfr, cplx->im()->mpfr, mpreal::get_default_rnd()));
}

/// @brief r2c keyword implementation
/// the result is stacked on current program stack
///
void program::rpn_r2c() {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ARG_MUST_BE_OF_TYPE(1, cmd_number);

    rpnstack::copy_and_push_front(*_stack, _stack->size() - 2, _calc_stack);
    rpnstack::copy_and_push_front(*_stack, _stack->size() - 1, _calc_stack);
    _stack->pop_front();
    _stack->pop_front();

    complex* cplx = new complex();
    _stack->push_front(cplx);
    CHECK_MPFR(mpfr_set(cplx->re()->mpfr, ((number*)_calc_stack.at(1))->_value.mpfr, mpreal::get_default_rnd()));
    CHECK_MPFR(mpfr_set(cplx->im()->mpfr, ((number*)_calc_stack.at(0))->_value.mpfr, mpreal::get_default_rnd()));
    _calc_stack.pop_front(2);
}

/// @brief c2r keyword implementation
/// the result is stacked on current program stack
///
void program::rpn_c2r() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_complex);

    rpnstack::copy_and_push_front(*_stack, _stack->size() - 1, _calc_stack);
    _stack->pop_front();

    number* re = new number();
    _stack->push_front(re);
    number* im = new number();
    _stack->push_front(im);

    CHECK_MPFR(mpfr_set(re->_value.mpfr, ((complex*)_calc_stack.back())->re()->mpfr, mpreal::get_default_rnd()));
    CHECK_MPFR(mpfr_set(im->_value.mpfr, ((complex*)_calc_stack.back())->im()->mpfr, mpreal::get_default_rnd()));
    _calc_stack.pop_front();
}

/// @brief r2p keyword implementation
/// the result is stacked on current program stack
///
void program::rpn_r2p() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_complex);

    rpn_dup();
    rpn_dup();
    rpn_arg();

    complex* cplx = (complex*)_stack->at(1);
    CHECK_MPFR(mpfr_set(cplx->im()->mpfr, ((number*)_stack->back())->_value.mpfr, mpreal::get_default_rnd()));
    _stack->pop_front();

    rpn_swap();
    rpn_abs();
    cplx = (complex*)_stack->at(1);
    CHECK_MPFR(mpfr_set(cplx->re()->mpfr, ((number*)_stack->back())->_value.mpfr, mpreal::get_default_rnd()));
    _stack->pop_front();
}

/// @brief p2r keyword implementation
/// the result is stacked on current program stack
///
void program::rpn_p2r() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_complex);

    rpnstack::copy_and_push_front(*_stack, _stack->size() - 1, _calc_stack);
    _calc_stack.push_back(new number());

    // assert complex is polar
    complex* rhotheta = (complex*)_calc_stack.at(1);
    number* tmp = (number*)_calc_stack.at(0);
    complex* result = (complex*)_stack->back();

    // calc cos(theta)
    CHECK_MPFR(mpfr_set(tmp->_value.mpfr, rhotheta->im()->mpfr, mpreal::get_default_rnd()));
    CHECK_MPFR(mpfr_cos(tmp->_value.mpfr, tmp->_value.mpfr, mpreal::get_default_rnd()));

    // calc rcos(theta)
    CHECK_MPFR(mpfr_mul(result->re()->mpfr, rhotheta->re()->mpfr, tmp->_value.mpfr, mpreal::get_default_rnd()));

    // calc sin(theta)
    CHECK_MPFR(mpfr_set(tmp->_value.mpfr, rhotheta->im()->mpfr, mpreal::get_default_rnd()));
    CHECK_MPFR(mpfr_sin(tmp->_value.mpfr, tmp->_value.mpfr, mpreal::get_default_rnd()));

    // calc rsin(theta)
    CHECK_MPFR(mpfr_mul(result->im()->mpfr, rhotheta->re()->mpfr, tmp->_value.mpfr, mpreal::get_default_rnd()));
}
