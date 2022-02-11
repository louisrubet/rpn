#include "program.hpp"

/// @brief + keyword implementation
///
void program::rpn_plus() {
    MIN_ARGUMENTS(2);

    // adding strings
    if (_stack->at(0)->_type == cmd_string && _stack->at(1)->_type == cmd_string) {
        ostring* right = (ostring*)_stack->pop_front();
        ostring* left = (ostring*)_stack->front();
        left->value += right->value;
    }
    // adding numbers
    else if (_stack->at(0)->_type == cmd_number && _stack->at(1)->_type == cmd_number) {
        number* right = (number*)_stack->pop_front();
        number* left = (number*)_stack->front();
        left->value += right->value;
    }
    // adding complexes
    else if (_stack->at(0)->_type == cmd_complex && _stack->at(1)->_type == cmd_complex) {
        ocomplex* right = (ocomplex*)_stack->pop_front();
        ocomplex* left = (ocomplex*)_stack->front();
        left->value += right->value;
    }
    // adding complex+number
    else if (_stack->at(0)->_type == cmd_number && _stack->at(1)->_type == cmd_complex) {
        number* right = (number*)_stack->pop_front();
        ocomplex* left = (ocomplex*)_stack->back();
        left->value += right->value;
    }
    // adding number+complex
    else if (_stack->at(0)->_type == cmd_complex && _stack->at(1)->_type == cmd_number) {
        rpn_swap();
        number* right = (number*)_stack->pop_front();
        ocomplex* left = (ocomplex*)_stack->back();
        left->value += right->value;
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief - keyword implementation
///
void program::rpn_minus() {
    MIN_ARGUMENTS(2);

    // substracting numbers
    if (_stack->at(0)->_type == cmd_number && _stack->at(1)->_type == cmd_number) {
        number* right = (number*)_stack->pop_front();
        number* left = (number*)_stack->back();
        left->value -= right->value;
    }
    // substracting complexes
    else if (_stack->at(0)->_type == cmd_complex && _stack->at(1)->_type == cmd_complex) {
        ocomplex* right = (ocomplex*)_stack->pop_front();
        ocomplex* left = (ocomplex*)_stack->back();
        left->value -= right->value;
    }
    // substracting complex-number
    else if (_stack->at(0)->_type == cmd_number && _stack->at(1)->_type == cmd_complex) {
        number* right = (number*)_stack->pop_front();
        ocomplex* left = (ocomplex*)_stack->back();
        left->value -= right->value;
    }
    // substracting number-complex
    else if (_stack->at(0)->_type == cmd_complex && _stack->at(1)->_type == cmd_number) {
        rpn_swap();
        number* right = (number*)_stack->pop_front();
        ocomplex* left = (ocomplex*)_stack->back();
        left->value -= right->value;
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief * keyword implementation
///
void program::rpn_mul() {
    MIN_ARGUMENTS(2);

    // multiplying numbers
    if (_stack->at(0)->_type == cmd_number && _stack->at(1)->_type == cmd_number) {
        number* right = (number*)_stack->pop_front();
        number* left = (number*)_stack->back();
        left->value *= right->value;
    }
    // multiplying complexes
    else if (_stack->at(0)->_type == cmd_complex && _stack->at(1)->_type == cmd_complex) {
        ocomplex* right = (ocomplex*)_stack->pop_front();
        ocomplex* left = (ocomplex*)_stack->back();
        left->value *= right->value;
    }
    // multiplying complex*number
    else if (_stack->at(0)->_type == cmd_number && _stack->at(1)->_type == cmd_complex) {
        number* right = (number*)_stack->pop_front();
        ocomplex* left = (ocomplex*)_stack->back();
        left->value *= right->value;
    }
    // multiplying number*complex
    else if (_stack->at(0)->_type == cmd_complex && _stack->at(1)->_type == cmd_number) {
        rpn_swap();
        number* right = (number*)_stack->pop_front();
        ocomplex* left = (ocomplex*)_stack->back();
        left->value *= right->value;
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief / keyword implementation
///
void program::rpn_div() {
    MIN_ARGUMENTS(2);

    // dividing numbers
    if (_stack->at(0)->_type == cmd_number && _stack->at(1)->_type == cmd_number) {
        number* right = (number*)_stack->pop_front();
        number* left = (number*)_stack->back();
        left->value /= right->value;
    }
    // dividing complexes
    else if (_stack->at(0)->_type == cmd_complex && _stack->at(1)->_type == cmd_complex) {
        number* right = (number*)_stack->pop_front();
        number* left = (number*)_stack->back();
        left->value /= right->value;
    }
    // dividing complex/number
    else if (_stack->at(0)->_type == cmd_number && _stack->at(1)->_type == cmd_complex) {
        number* right = (number*)_stack->pop_front();
        ocomplex* left = (ocomplex*)_stack->back();
        left->value /= right->value;
    }
    // dividing number/complex
    else if (_stack->at(0)->_type == cmd_complex && _stack->at(1)->_type == cmd_number) {
        rpn_swap();
        number* right = (number*)_stack->pop_front();
        ocomplex* left = (ocomplex*)_stack->back();
        left->value /= right->value;
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief neg keyword implementation
///
void program::rpn_neg() {
    MIN_ARGUMENTS(1);

    if (_stack->at(0)->_type == cmd_number) {
        number* left = (number*)_stack->back();
        left->value = -left->value;
    } else if (_stack->at(0)->_type == cmd_complex) {
        ocomplex* left = (ocomplex*)_stack->back();
        left->value = -left->value;
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief inv keyword implementation
///
void program::rpn_inv() {
    MIN_ARGUMENTS(1);

    if (_stack->at(0)->_type == cmd_number) {
        number* left = (number*)_stack->back();
        left->value = 1 / left->value;
    } else if (_stack->at(0)->_type == cmd_complex) {
        ocomplex* left = (ocomplex*)_stack->back();
        left->value = mpreal{1} / left->value;
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief % (purcent) keyword implementation
///
void program::rpn_purcent() {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ARG_MUST_BE_OF_TYPE(1, cmd_number);
    number* right = (number*)_stack->pop_front();
    number* left = (number*)_stack->back();
    left->value *= right->value;
    left->value /= 100;
}

/// @brief %CH keyword implementation
///
void program::rpn_purcentCH() {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ARG_MUST_BE_OF_TYPE(1, cmd_number);
    number* right = (number*)_stack->pop_front();
    number* left = (number*)_stack->back();
    right->value *= 100;
    left->value /= right->value;
}

/// @brief power keyword implementation
///
void program::rpn_power() {
    MIN_ARGUMENTS(2);
    bool done_on_real = false;

    if (_stack->at(0)->_type == cmd_number && _stack->at(1)->_type == cmd_number) {
        ARG_MUST_BE_OF_TYPE(0, cmd_number);
        number* right = (number*)_stack->pop_front();
        number* left = (number*)_stack->back();
        // TODO pow seems not to be a friend of mpreal
        //left->value.mpfr_ptr = pow(right->value);
        mpfr_pow(left->value.mpfr_ptr(), left->value.mpfr_ptr(), right->value.mpfr_ptr(), mpreal::get_default_rnd());
    } else
        ERR_CONTEXT(ret_bad_operand_type);
    // TODO manage complex case
}

#if 0
/// @brief sqrt keyword implementation
///
void program::rpn_squareroot() {
    if (_stack->at(0)->_type == cmd_number) {
        number* left = (number*)_stack->back();

        if (left->value >= 0)
            left->value = rec_sqrt(left->value);
        else {
            // negative number -> complex square root
            _stack->push_front(new number);
            CHECK_MPFR(mpfr_set_d(((number*)_stack->back())->value.mpfr, 0.0, mpreal::get_default_rnd()));
            rpn_r2c();
            _stack->push_front(new number);
            CHECK_MPFR(mpfr_set_d(((number*)_stack->back())->value.mpfr, 0.5, mpreal::get_default_rnd()));
            rpn_power();
        }
    } else if (_stack->at(0)->_type == cmd_complex) {
        // calc cplx^0.5
        _stack->push_front(new number);
        CHECK_MPFR(mpfr_set_d(((number*)_stack->back())->value.mpfr, 0.5, mpreal::get_default_rnd()));
        rpn_power();
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief sq keyword implementation
///
void program::rpn_square() {
    MIN_ARGUMENTS(1);

    if (_stack->at(0)->_type == cmd_number) {
        number* left = (number*)_stack->back();
        CHECK_MPFR(mpfr_sqr(left->value.mpfr, left->value.mpfr, mpreal::get_default_rnd()));
    } else if (_stack->at(0)->_type == cmd_complex) {
        rpn_dup();
        rpn_mul();
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief mod keyword implementation
///
void program::rpn_modulo() {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ARG_MUST_BE_OF_TYPE(1, cmd_number);

    number* right = (number*)_stack->pop_front();
    number* left = (number*)_stack->back();

    CHECK_MPFR(mpfr_fmod(left->value.mpfr, left->value.mpfr, right->value.mpfr, mpreal::get_default_rnd()));
}
#endif

#if 0
/// @brief abs keyword implementation
///
void program::rpn_abs() {
    MIN_ARGUMENTS(1);

    if (_stack->at(0)->_type == cmd_number) {
        number* left = (number*)_stack->back();
        CHECK_MPFR(mpfr_abs(left->value.mpfr, left->value.mpfr, mpreal::get_default_rnd()));
    } else if (_stack->at(0)->_type == cmd_complex) {
        // 1. copy out -> calc x2+iy2
        rpnstack::copy_and_push_front(*_stack, _stack->size() - 1, _calc_stack);
        _stack->pop_front();

        // 2. calc x2+iy2
        ocomplex* cplx = (ocomplex*)_calc_stack.back();
        CHECK_MPFR(mpfr_mul(cplx->re.mpfr, cplx->re.mpfr, cplx->re.mpfr, mpreal::get_default_rnd()));
        CHECK_MPFR(mpfr_mul(cplx->im.mpfr, cplx->im.mpfr, cplx->im.mpfr, mpreal::get_default_rnd()));

        // 3. new real on stack
        _stack->push_front(new number);
        number* module = (number*)_stack->back();

        // 4. set it to |x2+y2| then take sqrt
        CHECK_MPFR(mpfr_set(module->value.mpfr, cplx->re.mpfr, mpreal::get_default_rnd()));
        CHECK_MPFR(mpfr_add(module->value.mpfr, module->value.mpfr, cplx->im.mpfr, mpreal::get_default_rnd()));
        CHECK_MPFR(mpfr_sqrt(module->value.mpfr, module->value.mpfr, mpreal::get_default_rnd()));

        _calc_stack.pop_front();
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief hex keyword implementation
///
void program::rpn_hex() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ((number*)_stack->back())->_representation = number::hex;
    number::s_decimal_digits = 0;
}

/// @brief bin keyword implementation
///
void program::rpn_bin() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ((number*)_stack->back())->_representation = number::bin;
}

/// @brief dec keyword implementation
///
void program::rpn_dec() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ((number*)_stack->back())->_representation = number::dec;
}

/// @brief base keyword implementation
///
void program::rpn_base() {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ARG_MUST_BE_OF_TYPE(1, cmd_number);
    if (mpfr_cmp_d(((number*)_stack->back())->value.mpfr, (double)BASE_MIN) >= 0 &&
        mpfr_cmp_d(((number*)_stack->back())->value.mpfr, (double)BASE_MAX) <= 0) {
        int base = (int)mpfr_get_d(((number*)_stack->pop_front())->value.mpfr, mpreal::get_default_rnd());
        ((number*)_stack->at(0))->_base = base;
        ((number*)_stack->at(0))->_representation = number::base;
    } else
        ERR_CONTEXT(ret_out_of_range);
}

/// @brief fact (factorial) keyword implementation
///
void program::rpn_fact() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    // fact(n) = gamma(n+1)
    number* left = (number*)_stack->back();
    number* right;
    _stack->push_front(right = new number);
    right->value = 1L;
    rpn_plus();

    CHECK_MPFR(mpfr_gamma(left->value.mpfr, left->value.mpfr, mpreal::get_default_rnd()));
}

/// @brief sign keyword implementation
///
void program::rpn_sign() {
    MIN_ARGUMENTS(1);

    if (_stack->at(0)->_type == cmd_number) {
        // fact(n) = gamma(n+1)
        number* left = (number*)_stack->back();
        int result = mpfr_sgn(left->value.mpfr);
        left->value = (long)result;
    } else if (_stack->at(0)->_type == cmd_complex) {
        // calc x/sqrt(x*x+y*y) +iy/sqrt(x*x+y*y)
        rpn_dup();
        rpn_abs();
        rpn_div();
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief mant keyword implementation
///
void program::rpn_mant() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    number* left = (number*)_stack->back();

    if (mpfr_number_p(left->value.mpfr)) {
        if (mpfr_zero_p(left->value.mpfr))
            left->value = 0.0;
        else {
            mpfr_abs(left->value.mpfr, left->value.mpfr, mpreal::get_default_rnd());

            number* one;
            _stack->push_front(one = new number);
            number* ten;
            _stack->push_front(ten = new number);
            ten->value = 10L;

            one->value = 1L;
            while (mpfr_greaterequal_p(left->value.mpfr, one->value.mpfr))
                mpfr_div(left->value.mpfr, left->value.mpfr, ten->value.mpfr, mpreal::get_default_rnd());

            one->value = 0.1;
            while (mpfr_less_p(left->value.mpfr, one->value.mpfr))
                mpfr_mul(left->value.mpfr, left->value.mpfr, ten->value.mpfr, mpreal::get_default_rnd());

            _calc_stack.pop_front(2);
        }
    } else
        ERR_CONTEXT(ret_out_of_range);
}

/// @brief xpon keyword implementation
///
void program::rpn_xpon() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    number* left = (number*)_stack->back();

    if (mpfr_number_p(left->value.mpfr)) {
        if (mpfr_zero_p(left->value.mpfr))
            left->value = 0.0;
        else {
            double exponant = 0.0;
            mpfr_abs(left->value.mpfr, left->value.mpfr, mpreal::get_default_rnd());

            number* one;
            _stack->push_front(one = new number);
            number* ten;
            _stack->push_front(ten = new number);
            ten->value = 10L;

            one->value = 1L;
            while (mpfr_greaterequal_p(left->value.mpfr, one->value.mpfr)) {
                mpfr_div(left->value.mpfr, left->value.mpfr, ten->value.mpfr, mpreal::get_default_rnd());
                exponant += 1.0;
            }

            one->value = 0.1;
            while (mpfr_less_p(left->value.mpfr, one->value.mpfr)) {
                mpfr_mul(left->value.mpfr, left->value.mpfr, ten->value.mpfr, mpreal::get_default_rnd());
                exponant -= 1.0;
            }

            left->value = exponant;

            _calc_stack.pop_front();
            _calc_stack.pop_front();
        }
    } else
        ERR_CONTEXT(ret_out_of_range);
}

/// @brief floor keyword implementation
///
void program::rpn_floor() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    number* left = (number*)_stack->back();

    CHECK_MPFR(mpfr_floor(left->value.mpfr, left->value.mpfr));
}

/// @brief ceil keyword implementation
///
void program::rpn_ceil() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    number* left = (number*)_stack->back();

    CHECK_MPFR(mpfr_ceil(left->value.mpfr, left->value.mpfr));
}

/// @brief fp keyword implementation
///
void program::rpn_fp() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    number* left = (number*)_stack->back();

    CHECK_MPFR(mpfr_frac(left->value.mpfr, left->value.mpfr, mpreal::get_default_rnd()));
}

/// @brief ip keyword implementation
///
void program::rpn_ip() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    number* left = (number*)_stack->back();

    CHECK_MPFR(mpfr_trunc(left->value.mpfr, left->value.mpfr));
}

/// @brief min keyword implementation
///
void program::rpn_min() {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ARG_MUST_BE_OF_TYPE(1, cmd_number);

    number* right = (number*)_stack->pop_front();
    number* left = (number*)_stack->back();

    CHECK_MPFR(mpfr_min(left->value.mpfr, left->value.mpfr, right->value.mpfr, mpreal::get_default_rnd()));
}

/// @brief max keyword implementation
///
void program::rpn_max() {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ARG_MUST_BE_OF_TYPE(1, cmd_number);

    number* right = (number*)_stack->pop_front();
    number* left = (number*)_stack->back();

    CHECK_MPFR(mpfr_max(left->value.mpfr, left->value.mpfr, right->value.mpfr, mpreal::get_default_rnd()));
}
#endif
