#include "program.hpp"

/// @brief e keyword implementation
///
void program::rpn_e(void) {
    number* euler = new number();
    _stack->push_back(euler);
    euler->_value = 1L;
    CHECK_MPFR(mpfr_exp(euler->_value.mpfr, euler->_value.mpfr, floating_t::s_mpfr_rnd));
}

/// @brief log10 keyword implementation
///
void program::rpn_log10() {
    MIN_ARGUMENTS(1);

    if (_stack->at(0)->_type == cmd_number || _stack->at(0)->_type == cmd_complex) {
        // log10(z)=ln(z)/ln(10)
        rpn_ln();

        number* ten = new number();
        _stack->push_back(ten);
        CHECK_MPFR(mpfr_set_d(ten->_value.mpfr, 10.0, floating_t::s_mpfr_rnd));
        rpn_ln();
        rpn_div();
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief alog10 keyword implementation
///
void program::rpn_alog10() {
    MIN_ARGUMENTS(1);

    if (_stack->at(0)->_type == cmd_number || _stack->at(0)->_type == cmd_complex) {
        floating_t* left = &((number*)_stack->at(0))->_value;
        number* ten = new number();
        _stack->push_back(ten);
        CHECK_MPFR(mpfr_set_d(ten->_value.mpfr, 10.0, floating_t::s_mpfr_rnd));
        rpn_ln();
        rpn_mul();
        rpn_exp();
    }
}

/// @brief log2 keyword implementation
///
void program::rpn_log2() {
    MIN_ARGUMENTS(1);

    if (_stack->at(0)->_type == cmd_number || _stack->at(0)->_type == cmd_complex) {
        // log2(z)=ln(z)/ln(2)
        rpn_ln();

        number* two = new number();
        _stack->push_back(two);
        CHECK_MPFR(mpfr_set_d(two->_value.mpfr, 2.0, floating_t::s_mpfr_rnd));
        rpn_ln();
        rpn_div();
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief alog2 keyword implementation
///
void program::rpn_alog2() {
    MIN_ARGUMENTS(1);

    if (_stack->at(0)->_type == cmd_number || _stack->at(0)->_type == cmd_complex) {
        floating_t* left = &((number*)_stack->at(0))->_value;
        number* two = new number();
        _stack->push_back(two);
        CHECK_MPFR(mpfr_set_d(two->_value.mpfr, 2.0, floating_t::s_mpfr_rnd));
        rpn_ln();
        rpn_mul();
        rpn_exp();
    }
}

/// @brief ln keyword implementation
///
void program::rpn_ln() {
    MIN_ARGUMENTS(1);

    if (_stack->at(0)->_type == cmd_number) {
        number* left = (number*)_stack->back();

        // x<0 -> ln(x) = ln(-x)+i*pi
        if (mpfr_cmp_si(left->_value.mpfr, 0) < 0) {
            rpnstack::copy_and_push_back(*_stack, _stack->size() - 1, _calc_stack);
            _stack->pop_back();
            left = (number*)_calc_stack.back();

            complex* cplx = new complex();
            _stack->push_back(cplx);
            CHECK_MPFR(mpfr_neg(cplx->re()->mpfr, left->_value.mpfr, floating_t::s_mpfr_rnd));
            CHECK_MPFR(mpfr_log(cplx->re()->mpfr, cplx->re()->mpfr, floating_t::s_mpfr_rnd));
            CHECK_MPFR(mpfr_const_pi(cplx->im()->mpfr, floating_t::s_mpfr_rnd));
            _calc_stack.pop_back();
        } else
            CHECK_MPFR(mpfr_log(left->_value.mpfr, left->_value.mpfr, floating_t::s_mpfr_rnd));
    } else if (_stack->at(0)->_type == cmd_complex) {
        // ln(x+iy) = 0.5*ln(x*x+y*y) + i atan(x/y)
        rpnstack::copy_and_push_back(*_stack, _stack->size() - 1, _calc_stack);

        floating_t* x = ((complex*)_calc_stack.at(0))->re();
        floating_t* y = ((complex*)_calc_stack.at(0))->im();

        floating_t* re = ((complex*)_stack->at(0))->re();
        floating_t* im = ((complex*)_stack->at(0))->im();

        // 1. atan(x/y)
        CHECK_MPFR(mpfr_atan2(im->mpfr, y->mpfr, x->mpfr, floating_t::s_mpfr_rnd));

        // 2. 0.5*ln(x*x+y*y)
        CHECK_MPFR(mpfr_mul(x->mpfr, x->mpfr, x->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_mul(y->mpfr, y->mpfr, y->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_add(re->mpfr, x->mpfr, y->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_log(re->mpfr, re->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_mul_d(re->mpfr, re->mpfr, 0.5, floating_t::s_mpfr_rnd));

        _calc_stack.pop_back();
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief exp keyword implementation
///
void program::rpn_exp() {
    MIN_ARGUMENTS(1);

    if (_stack->at(0)->_type == cmd_number) {
        floating_t* left = &((number*)_stack->at(0))->_value;
        CHECK_MPFR(mpfr_exp(left->mpfr, left->mpfr, floating_t::s_mpfr_rnd));
    } else if (_stack->at(0)->_type == cmd_complex) {
        // exp(x)*(cos(y)+i sin(y))
        rpnstack::copy_and_push_back(*_stack, _stack->size() - 1, _calc_stack);

        floating_t* x = ((complex*)_calc_stack.at(0))->re();
        floating_t* y = ((complex*)_calc_stack.at(0))->im();

        floating_t* re = ((complex*)_stack->at(0))->re();
        floating_t* im = ((complex*)_stack->at(0))->im();

        CHECK_MPFR(mpfr_cos(re->mpfr, y->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_sin(im->mpfr, y->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_exp(x->mpfr, x->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_mul(re->mpfr, re->mpfr, x->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_mul(im->mpfr, im->mpfr, x->mpfr, floating_t::s_mpfr_rnd));

        _calc_stack.pop_back();
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief expm keyword implementation
///
void program::rpn_expm() {
    MIN_ARGUMENTS(1);

    if (_stack->at(0)->_type == cmd_number || _stack->at(0)->_type == cmd_complex) {
        // exp(x)-1
        rpn_exp();

        number* one = new number();
        _stack->push_back(one);
        CHECK_MPFR(mpfr_set_d(one->_value.mpfr, 1.0, floating_t::s_mpfr_rnd));
        rpn_minus();
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief lnp1 keyword implementation
///
void program::rpn_lnp1() {
    MIN_ARGUMENTS(1);

    if (_stack->at(0)->_type == cmd_number || _stack->at(0)->_type == cmd_complex) {
        // ln(x+1)
        number* one = new number();
        _stack->push_back(one);
        CHECK_MPFR(mpfr_set_d(one->_value.mpfr, 1.0, floating_t::s_mpfr_rnd));
        rpn_plus();

        rpn_ln();
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief sinh keyword implementation
///
void program::rpn_sinh() {
    MIN_ARGUMENTS(1);

    if (_stack->at(0)->_type == cmd_number) {
        floating_t* left = &((number*)_stack->at(0))->_value;
        CHECK_MPFR(mpfr_sinh(left->mpfr, left->mpfr, floating_t::s_mpfr_rnd));
    } else if (_stack->at(0)->_type == cmd_complex) {
        // sinh(x+iy)=sinh(x)cos(y)+icosh(x)sin(y)
        rpnstack::copy_and_push_back(*_stack, _stack->size() - 1, _calc_stack);

        number* num = new number();
        _stack->push_back(num);
        floating_t* tmp = &num->_value;
        floating_t* x = ((complex*)_calc_stack.at(1))->re();
        floating_t* y = ((complex*)_calc_stack.at(1))->im();

        floating_t* re = ((complex*)_stack->at(0))->re();
        floating_t* im = ((complex*)_stack->at(0))->im();

        CHECK_MPFR(mpfr_sinh(re->mpfr, x->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_cos(tmp->mpfr, y->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_mul(re->mpfr, re->mpfr, tmp->mpfr, floating_t::s_mpfr_rnd));

        CHECK_MPFR(mpfr_cosh(im->mpfr, x->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_sin(tmp->mpfr, y->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_mul(im->mpfr, im->mpfr, tmp->mpfr, floating_t::s_mpfr_rnd));

        _calc_stack.pop_back(2);
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief asinh keyword implementation
///
void program::rpn_asinh() {
    MIN_ARGUMENTS(1);

    if (_stack->at(0)->_type == cmd_number) {
        floating_t* left = &((number*)_stack->at(0))->_value;
        CHECK_MPFR(mpfr_asinh(left->mpfr, left->mpfr, floating_t::s_mpfr_rnd));
    } else if (_stack->at(0)->_type == cmd_complex) {
        // asinh(z)=ln(z+sqrt(1+z*z))
        rpn_dup();
        rpn_square();
        number* num = new number();
        _stack->push_back(num);
        CHECK_MPFR(mpfr_set_d(num->_value.mpfr, 1.0, floating_t::s_mpfr_rnd));
        rpn_plus();
        rpn_squareroot();
        rpn_plus();
        rpn_ln();
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief cosh keyword implementation
///
void program::rpn_cosh() {
    MIN_ARGUMENTS(1);

    if (_stack->at(0)->_type == cmd_number) {
        floating_t* left = &((number*)_stack->at(0))->_value;
        CHECK_MPFR(mpfr_cosh(left->mpfr, left->mpfr, floating_t::s_mpfr_rnd));
    } else if (_stack->at(0)->_type == cmd_complex) {
        // acosh(x+iy)=cosh(x)cos(y)+isinh(x)sin(y)
        rpnstack::copy_and_push_back(*_stack, _stack->size() - 1, _calc_stack);

        number* num = new number();
        _stack->push_back(num);
        floating_t* tmp = &num->_value;
        floating_t* x = ((complex*)_calc_stack.at(1))->re();
        floating_t* y = ((complex*)_calc_stack.at(1))->im();

        floating_t* re = ((complex*)_stack->at(0))->re();
        floating_t* im = ((complex*)_stack->at(0))->im();

        CHECK_MPFR(mpfr_cosh(re->mpfr, x->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_cos(tmp->mpfr, y->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_mul(re->mpfr, re->mpfr, tmp->mpfr, floating_t::s_mpfr_rnd));

        CHECK_MPFR(mpfr_sinh(im->mpfr, x->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_sin(tmp->mpfr, y->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_mul(im->mpfr, im->mpfr, tmp->mpfr, floating_t::s_mpfr_rnd));

        _calc_stack.pop_back(2);
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief acosh keyword implementation
///
void program::rpn_acosh() {
    MIN_ARGUMENTS(1);

    if (_stack->at(0)->_type == cmd_number) {
        floating_t* left = &((number*)_stack->at(0))->_value;
        CHECK_MPFR(mpfr_acosh(left->mpfr, left->mpfr, floating_t::s_mpfr_rnd));
    } else if (_stack->at(0)->_type == cmd_complex) {
        // acosh(z)=ln(z+sqrt(z+1)sqrt(z-1))
        rpn_dup();
        number* num = new number();
        _stack->push_back(num);
        CHECK_MPFR(mpfr_set_d(num->_value.mpfr, 1.0, floating_t::s_mpfr_rnd));
        rpn_plus();
        rpn_dup();
        num = (number*)new number();
        _stack->push_back(num);
        CHECK_MPFR(mpfr_set_d(num->_value.mpfr, 2.0, floating_t::s_mpfr_rnd));
        rpn_minus();
        rpn_mul();
        rpn_squareroot();
        rpn_plus();
        rpn_ln();
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief tanh keyword implementation
///
void program::rpn_tanh() {
    MIN_ARGUMENTS(1);

    if (_stack->at(0)->_type == cmd_number) {
        floating_t* left = &((number*)_stack->at(0))->_value;
        CHECK_MPFR(mpfr_tanh(left->mpfr, left->mpfr, floating_t::s_mpfr_rnd));
    } else if (_stack->at(0)->_type == cmd_complex) {
        // tanh(x+iy)=(tanh(x)+itan(y)) / (1 + itanh(x)tan(y))
        rpn_dup();

        floating_t* x = ((complex*)_stack->at(1))->re();
        floating_t* y = ((complex*)_stack->at(1))->im();

        floating_t* re = ((complex*)_stack->at(1))->re();
        floating_t* im = ((complex*)_stack->at(1))->im();

        CHECK_MPFR(mpfr_tanh(re->mpfr, x->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_tan(im->mpfr, y->mpfr, floating_t::s_mpfr_rnd));

        CHECK_MPFR(mpfr_tanh(x->mpfr, x->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_tan(y->mpfr, y->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_mul(y->mpfr, y->mpfr, x->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_set_d(x->mpfr, 1.0, floating_t::s_mpfr_rnd));
        rpn_div();
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief atanh keyword implementation
///
void program::rpn_atanh() {
    MIN_ARGUMENTS(1);

    if (_stack->at(0)->_type == cmd_number) {
        floating_t* left = &((number*)_stack->at(0))->_value;
        CHECK_MPFR(mpfr_atanh(left->mpfr, left->mpfr, floating_t::s_mpfr_rnd));
    } else if (_stack->at(0)->_type == cmd_complex) {
        // atanh(z)=0.5*ln((1+z)/(1-z))
        rpn_dup();

        number* num;
        _stack->push_back(num = new number);
        CHECK_MPFR(mpfr_set_d(num->_value.mpfr, 1.0, floating_t::s_mpfr_rnd));
        rpn_plus();
        rpn_swap();
        _stack->push_back(num = new number);
        CHECK_MPFR(mpfr_set_d(num->_value.mpfr, 1.0, floating_t::s_mpfr_rnd));
        rpn_minus();
        rpn_neg();
        rpn_div();
        rpn_ln();
        _stack->push_back(num = new number);
        CHECK_MPFR(mpfr_set_d(num->_value.mpfr, 0.5, floating_t::s_mpfr_rnd));
        rpn_mul();
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}
