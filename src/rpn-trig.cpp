#include "program.hpp"

/// @brief pi keyword implementation
///
void program::rpn_pi(void) {
    number* pi;
    _stack->push_front(pi = static_cast<number*>(new number));
    CHECK_MPFR(mpfr_const_pi(pi->_value.mpfr, mpreal::get_default_rnd()));
}

/// @brief d->r keyword implementation
///
void program::rpn_d2r(void) {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    // add pi on stack
    rpn_pi();

    floating_t* pi = &((number*)_stack->pop_front())->_value;
    floating_t* left = &((number*)_stack->at(0))->_value;

    CHECK_MPFR(mpfr_mul(left->mpfr, left->mpfr, pi->mpfr, mpreal::get_default_rnd()));
    CHECK_MPFR(mpfr_div_si(left->mpfr, left->mpfr, 180, mpreal::get_default_rnd()));
}

/// @brief r->d keyword implementation
///
void program::rpn_r2d(void) {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    // add pi on stack
    rpn_pi();

    floating_t* pi = &((number*)_stack->pop_front())->_value;
    floating_t* left = &((number*)_stack->at(0))->_value;

    CHECK_MPFR(mpfr_div(left->mpfr, left->mpfr, pi->mpfr, mpreal::get_default_rnd()));
    CHECK_MPFR(mpfr_mul_si(left->mpfr, left->mpfr, 180, mpreal::get_default_rnd()));
}

/// @brief sin keyword implementation
///
void program::rpn_sin(void) {
    MIN_ARGUMENTS(1);

    if (_stack->at(0)->_type == cmd_number) {
        floating_t* left = &((number*)_stack->at(0))->_value;
        CHECK_MPFR(mpfr_sin(left->mpfr, left->mpfr, mpreal::get_default_rnd()));
    } else if (_stack->at(0)->_type == cmd_complex) {
        // sin(x+iy)=sin(x)cosh(y)+icos(x)sinh(y)
        rpnstack::copy_and_push_front(*_stack, _stack->size() - 1, _calc_stack);

        number* num = static_cast<number*>(new number);
        _stack->push_front(num);
        floating_t* tmp = &num->_value;
        floating_t* x = ((complex*)_calc_stack.at(1))->re();
        floating_t* y = ((complex*)_calc_stack.at(1))->im();

        floating_t* re = ((complex*)_stack->at(0))->re();
        floating_t* im = ((complex*)_stack->at(0))->im();

        CHECK_MPFR(mpfr_sin(re->mpfr, x->mpfr, mpreal::get_default_rnd()));
        CHECK_MPFR(mpfr_cosh(tmp->mpfr, y->mpfr, mpreal::get_default_rnd()));
        CHECK_MPFR(mpfr_mul(re->mpfr, re->mpfr, tmp->mpfr, mpreal::get_default_rnd()));

        CHECK_MPFR(mpfr_cos(im->mpfr, x->mpfr, mpreal::get_default_rnd()));
        CHECK_MPFR(mpfr_sinh(tmp->mpfr, y->mpfr, mpreal::get_default_rnd()));
        CHECK_MPFR(mpfr_mul(im->mpfr, im->mpfr, tmp->mpfr, mpreal::get_default_rnd()));

        _calc_stack.pop_front(2);
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief asin keyword implementation
///
void program::rpn_asin(void) {
    MIN_ARGUMENTS(1);

    if (_stack->at(0)->_type == cmd_number) {
        floating_t* left = &((number*)_stack->at(0))->_value;
        CHECK_MPFR(mpfr_asin(left->mpfr, left->mpfr, mpreal::get_default_rnd()));
    } else if (_stack->at(0)->_type == cmd_complex) {
        number* num;
        complex* i;

        // asin(z)=-iln(iz+sqrt(1-z*z))
        rpnstack::copy_and_push_front(*_stack, _stack->size() - 1, _calc_stack);

        i = (complex*)_calc_stack.at(0);
        CHECK_MPFR(mpfr_set_d(i->re()->mpfr, 0.0, mpreal::get_default_rnd()));
        CHECK_MPFR(mpfr_set_d(i->im()->mpfr, 1.0, mpreal::get_default_rnd()));

        rpn_dup();
        rpn_square();
        _stack->push_front(num = static_cast<number*>(new number));
        CHECK_MPFR(mpfr_set_d(num->_value.mpfr, 1.0, mpreal::get_default_rnd()));
        rpn_minus();
        rpn_neg();
        rpn_squareroot();
        rpn_swap();
        rpnstack::copy_and_push_front(_calc_stack, 0, *_stack);
        rpn_mul();
        rpn_plus();
        rpn_ln();
        rpnstack::copy_and_push_front(_calc_stack, 0, *_stack);
        rpn_mul();
        rpn_neg();
        _calc_stack.pop_front();
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief cos keyword implementation
///
void program::rpn_cos(void) {
    MIN_ARGUMENTS(1);

    if (_stack->at(0)->_type == cmd_number) {
        floating_t* left = &((number*)_stack->at(0))->_value;
        CHECK_MPFR(mpfr_cos(left->mpfr, left->mpfr, mpreal::get_default_rnd()));
    } else if (_stack->at(0)->_type == cmd_complex) {
        // cos(x+iy) = cos(x)cosh(y) - isin(x)sinh(y)
        rpnstack::copy_and_push_front(*_stack, _stack->size() - 1, _calc_stack);

        number* num = static_cast<number*>(new number);
        _stack->push_front(num);
        floating_t* tmp = &num->_value;
        floating_t* x = ((complex*)_calc_stack.at(1))->re();
        floating_t* y = ((complex*)_calc_stack.at(1))->im();

        floating_t* re = ((complex*)_stack->at(0))->re();
        floating_t* im = ((complex*)_stack->at(0))->im();

        CHECK_MPFR(mpfr_cos(re->mpfr, x->mpfr, mpreal::get_default_rnd()));
        CHECK_MPFR(mpfr_cosh(tmp->mpfr, y->mpfr, mpreal::get_default_rnd()));
        CHECK_MPFR(mpfr_mul(re->mpfr, re->mpfr, tmp->mpfr, mpreal::get_default_rnd()));

        CHECK_MPFR(mpfr_sin(im->mpfr, x->mpfr, mpreal::get_default_rnd()));
        CHECK_MPFR(mpfr_sinh(tmp->mpfr, y->mpfr, mpreal::get_default_rnd()));
        CHECK_MPFR(mpfr_mul(im->mpfr, im->mpfr, tmp->mpfr, mpreal::get_default_rnd()));
        CHECK_MPFR(mpfr_neg(im->mpfr, im->mpfr, mpreal::get_default_rnd()));

        _calc_stack.pop_front(2);
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief acos keyword implementation
///
void program::rpn_acos(void) {
    MIN_ARGUMENTS(1);

    if (_stack->at(0)->_type == cmd_number) {
        floating_t* left = &((number*)_stack->at(0))->_value;
        CHECK_MPFR(mpfr_acos(left->mpfr, left->mpfr, mpreal::get_default_rnd()));
    } else if (_stack->at(0)->_type == cmd_complex) {
        // acos(z)=pi/2-asin(z)
        rpn_asin();
        rpn_pi();
        number* num;
        _stack->push_front(num = static_cast<number*>(new number));
        CHECK_MPFR(mpfr_set_d(num->_value.mpfr, 2.0, mpreal::get_default_rnd()));
        rpn_div();
        rpn_minus();
        rpn_neg();
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief tan keyword implementation
///
void program::rpn_tan(void) {
    MIN_ARGUMENTS(1);

    if (_stack->at(0)->_type == cmd_number) {
        floating_t* left = &((number*)_stack->at(0))->_value;
        CHECK_MPFR(mpfr_tan(left->mpfr, left->mpfr, mpreal::get_default_rnd()));
    } else if (_stack->at(0)->_type == cmd_complex) {
        // tan(x+iy) = (sin(2x)+isinh(2y)) / cosh(2y)+cos(2x)
        rpnstack::copy_and_push_front(*_stack, _stack->size() - 1, _calc_stack);

        number* num = static_cast<number*>(new number);
        _stack->push_front(num);
        floating_t* tmp = &num->_value;
        floating_t* x = ((complex*)_calc_stack.at(1))->re();
        floating_t* y = ((complex*)_calc_stack.at(1))->im();

        floating_t* re = ((complex*)_stack->at(0))->re();
        floating_t* im = ((complex*)_stack->at(0))->im();

        // x->2x
        CHECK_MPFR(mpfr_mul_si(x->mpfr, x->mpfr, 2, mpreal::get_default_rnd()));
        // y->2y
        CHECK_MPFR(mpfr_mul_si(y->mpfr, y->mpfr, 2, mpreal::get_default_rnd()));

        // sin(2x)+sinh(2y)
        CHECK_MPFR(mpfr_sin(re->mpfr, x->mpfr, mpreal::get_default_rnd()));
        CHECK_MPFR(mpfr_sinh(im->mpfr, y->mpfr, mpreal::get_default_rnd()));

        // cosh(2y)+cos(2x)
        CHECK_MPFR(mpfr_cosh(tmp->mpfr, y->mpfr, mpreal::get_default_rnd()));
        CHECK_MPFR(mpfr_cos(x->mpfr, x->mpfr, mpreal::get_default_rnd()));
        CHECK_MPFR(mpfr_add(tmp->mpfr, tmp->mpfr, x->mpfr, mpreal::get_default_rnd()));

        // sin(2x)+sinh(2y) / (cosh(2y)+cos(2x))
        CHECK_MPFR(mpfr_div(re->mpfr, re->mpfr, tmp->mpfr, mpreal::get_default_rnd()));
        CHECK_MPFR(mpfr_div(im->mpfr, im->mpfr, tmp->mpfr, mpreal::get_default_rnd()));

        _calc_stack.pop_front(2);
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief atan keyword implementation
///
void program::rpn_atan(void) {
    MIN_ARGUMENTS(1);

    if (_stack->at(0)->_type == cmd_number) {
        floating_t* left = &((number*)_stack->at(0))->_value;
        CHECK_MPFR(mpfr_atan(left->mpfr, left->mpfr, mpreal::get_default_rnd()));
    } else if (_stack->at(0)->_type == cmd_complex) {
        number* num;
        complex* i;

        // atan(z)=0.5i(ln((1-iz)/(1+iz))
        rpnstack::copy_and_push_front(*_stack, _stack->size() - 1, _calc_stack);

        i = (complex*)_calc_stack.at(0);
        CHECK_MPFR(mpfr_set_d(i->re()->mpfr, 0.0, mpreal::get_default_rnd()));
        CHECK_MPFR(mpfr_set_d(i->im()->mpfr, 1.0, mpreal::get_default_rnd()));

        rpnstack::copy_and_push_front(_calc_stack, 0, *_stack);
        rpn_mul();
        _stack->push_front(num = static_cast<number*>(new number));
        CHECK_MPFR(mpfr_set_d(num->_value.mpfr, 1.0, mpreal::get_default_rnd()));
        rpn_minus();  // iz-1
        rpn_neg();    // 1-iz
        rpn_dup();
        rpn_neg();  // iz-1
        _stack->push_front(num = static_cast<number*>(new number));
        CHECK_MPFR(mpfr_set_d(num->_value.mpfr, 2.0, mpreal::get_default_rnd()));
        rpn_plus();  // iz+1
        rpn_div();

        rpn_ln();
        CHECK_MPFR(mpfr_set_d(i->im()->mpfr, 0.5, mpreal::get_default_rnd()));
        rpnstack::copy_and_push_front(_calc_stack, 0, *_stack);
        rpn_mul();

        _calc_stack.pop_front();
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}
