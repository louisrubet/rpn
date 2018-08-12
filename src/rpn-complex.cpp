#include "program.hpp"

/// @brief re keyword implementation
/// the result is stacked on current program stack
///
void program::rpn_re() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_complex);

    stack::copy_and_push_back(*_stack, _stack->size() - 1, _calc_stack);
    _stack->pop_back();

    number* re = (number*)_stack->allocate_back(number::calc_size(), cmd_number);
    CHECK_MPFR(mpfr_set(re->_value.mpfr, ((complex*)_calc_stack.get_obj(0))->re()->mpfr, floating_t::s_mpfr_rnd));
    _calc_stack.pop_back();
}

/// @brief im keyword implementation
/// the result is stacked on current program stack
///
void program::rpn_im() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_complex);

    stack::copy_and_push_back(*_stack, _stack->size() - 1, _calc_stack);
    _stack->pop_back();

    number* im = (number*)_stack->allocate_back(number::calc_size(), cmd_number);
    CHECK_MPFR(mpfr_set(im->_value.mpfr, ((complex*)_calc_stack.get_obj(0))->im()->mpfr, floating_t::s_mpfr_rnd));
    _calc_stack.pop_back();
}

/// @brief arg keyword implementation
/// the result is stacked on current program stack
///
void program::rpn_arg() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_complex);

    // calc atan2(x/y)
    complex* cplx = (complex*)_stack->pop_back();
    number* num = (number*)_calc_stack.allocate_back(number::calc_size(), cmd_number);

    CHECK_MPFR(mpfr_atan2(num->_value.mpfr, cplx->im()->mpfr, cplx->re()->mpfr, floating_t::s_mpfr_rnd));

    stack::copy_and_push_back(_calc_stack, _calc_stack.size() - 1, *_stack);
    _calc_stack.pop_back();
}

/// @brief conj keyword implementation
/// the result is stacked on current program stack
///
void program::rpn_conj() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_complex);

    complex* cplx = (complex*)_stack->back();
    CHECK_MPFR(mpfr_neg(cplx->im()->mpfr, cplx->im()->mpfr, floating_t::s_mpfr_rnd));
}

/// @brief r2c keyword implementation
/// the result is stacked on current program stack
///
void program::rpn_r2c() {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ARG_MUST_BE_OF_TYPE(1, cmd_number);

    stack::copy_and_push_back(*_stack, _stack->size() - 2, _calc_stack);
    stack::copy_and_push_back(*_stack, _stack->size() - 1, _calc_stack);
    _stack->pop_back();
    _stack->pop_back();

    complex* cplx = (complex*)_stack->allocate_back(complex::calc_size(), cmd_complex);
    CHECK_MPFR(mpfr_set(cplx->re()->mpfr, ((number*)_calc_stack.get_obj(1))->_value.mpfr, floating_t::s_mpfr_rnd));
    CHECK_MPFR(mpfr_set(cplx->im()->mpfr, ((number*)_calc_stack.get_obj(0))->_value.mpfr, floating_t::s_mpfr_rnd));
    _calc_stack.pop_back(2);
}

/// @brief c2r keyword implementation
/// the result is stacked on current program stack
///
void program::rpn_c2r() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_complex);

    stack::copy_and_push_back(*_stack, _stack->size() - 1, _calc_stack);
    _stack->pop_back();

    number* re = (number*)_stack->allocate_back(number::calc_size(), cmd_number);
    number* im = (number*)_stack->allocate_back(number::calc_size(), cmd_number);

    CHECK_MPFR(mpfr_set(re->_value.mpfr, ((complex*)_calc_stack.back())->re()->mpfr, floating_t::s_mpfr_rnd));
    CHECK_MPFR(mpfr_set(im->_value.mpfr, ((complex*)_calc_stack.back())->im()->mpfr, floating_t::s_mpfr_rnd));
    _calc_stack.pop_back();
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

    complex* cplx = (complex*)_stack->get_obj(1);
    CHECK_MPFR(mpfr_set(cplx->im()->mpfr, ((number*)_stack->back())->_value.mpfr, floating_t::s_mpfr_rnd));
    _stack->pop_back();

    rpn_swap();
    rpn_abs();
    cplx = (complex*)_stack->get_obj(1);
    CHECK_MPFR(mpfr_set(cplx->re()->mpfr, ((number*)_stack->back())->_value.mpfr, floating_t::s_mpfr_rnd));
    _stack->pop_back();
}

/// @brief p2r keyword implementation
/// the result is stacked on current program stack
///
void program::rpn_p2r() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_complex);

    stack::copy_and_push_back(*_stack, _stack->size() - 1, _calc_stack);
    _calc_stack.allocate_back(number::calc_size(), cmd_number);

    // assert complex is polar
    complex* rhotheta = (complex*)_calc_stack.get_obj(1);
    number* tmp = (number*)_calc_stack.get_obj(0);
    complex* result = (complex*)_stack->back();

    // calc cos(theta)
    CHECK_MPFR(mpfr_set(tmp->_value.mpfr, rhotheta->im()->mpfr, floating_t::s_mpfr_rnd));
    CHECK_MPFR(mpfr_cos(tmp->_value.mpfr, tmp->_value.mpfr, floating_t::s_mpfr_rnd));

    // calc rcos(theta)
    CHECK_MPFR(mpfr_mul(result->re()->mpfr, rhotheta->re()->mpfr, tmp->_value.mpfr, floating_t::s_mpfr_rnd));

    // calc sin(theta)
    CHECK_MPFR(mpfr_set(tmp->_value.mpfr, rhotheta->im()->mpfr, floating_t::s_mpfr_rnd));
    CHECK_MPFR(mpfr_sin(tmp->_value.mpfr, tmp->_value.mpfr, floating_t::s_mpfr_rnd));

    // calc rsin(theta)
    CHECK_MPFR(mpfr_mul(result->im()->mpfr, rhotheta->re()->mpfr, tmp->_value.mpfr, floating_t::s_mpfr_rnd));
}
