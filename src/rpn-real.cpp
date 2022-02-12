#include "program.hpp"

/// @brief + keyword implementation
///
void program::rpn_plus() {
    MIN_ARGUMENTS(2);

    // strings
    if (_stack->type(0) == cmd_string && _stack->type(1) == cmd_string) {
        _stack->value<ostring>(1) += _stack->value<ostring>(0);
        _stack->pop();
    }
    // numbers
    else if (_stack->type(0) == cmd_number && _stack->type(1) == cmd_number) {
        _stack->value<number>(1) += _stack->value<number>(0);
        _stack->pop();
    }
    // complexes
    else if (_stack->type(0) == cmd_complex && _stack->type(1) == cmd_complex) {
        _stack->value<ocomplex>(1) += _stack->value<ocomplex>(0);
        _stack->pop();
    }
    // complex+number
    else if (_stack->type(0) == cmd_number && _stack->type(1) == cmd_complex) {
        _stack->value<ocomplex>(1) += _stack->value<number>(0);
        _stack->pop();
    }
    // number+complex
    else if (_stack->type(0) == cmd_complex && _stack->type(1) == cmd_number) {
        rpn_swap();
        _stack->value<ocomplex>(1) += _stack->value<number>(0);
        _stack->pop();
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief - keyword implementation
///
void program::rpn_minus() {
    MIN_ARGUMENTS(2);

    // numbers
    if (_stack->type(0) == cmd_number && _stack->type(1) == cmd_number) {
        _stack->value<number>(1) -= _stack->value<number>(0);
        _stack->pop();
    }
    // complexes
    else if (_stack->type(0) == cmd_complex && _stack->type(1) == cmd_complex) {
        _stack->value<ocomplex>(1) -= _stack->value<ocomplex>(0);
        _stack->pop();
    }
    // subbing complex-number
    else if (_stack->type(0) == cmd_number && _stack->type(1) == cmd_complex) {
        _stack->value<ocomplex>(1) -= _stack->value<number>(0);
        _stack->pop();
    }
    // number-complex
    else if (_stack->type(0) == cmd_complex && _stack->type(1) == cmd_number) {
        rpn_swap();
        _stack->value<ocomplex>(1) = _stack->value<number>(0) - _stack->value<ocomplex>(1);
        _stack->pop();
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief * keyword implementation
///
void program::rpn_mul() {
    MIN_ARGUMENTS(2);

    // mutiplying numbers
    if (_stack->type(0) == cmd_number && _stack->type(1) == cmd_number) {
        _stack->value<number>(1) *= _stack->value<number>(0);
        _stack->pop();
    }
    // mutiplying complexes
    else if (_stack->type(0) == cmd_complex && _stack->type(1) == cmd_complex) {
        _stack->value<ocomplex>(1) *= _stack->value<ocomplex>(0);
        _stack->pop();
    }
    // mutiplying complex*number
    else if (_stack->type(0) == cmd_number && _stack->type(1) == cmd_complex) {
        _stack->value<ocomplex>(1) *= _stack->value<number>(0);
        _stack->pop();
    }
    // mutiplying number*complex
    else if (_stack->type(0) == cmd_complex && _stack->type(1) == cmd_number) {
        rpn_swap();
        _stack->value<ocomplex>(1) *= _stack->value<number>(0);
        _stack->pop();
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief / keyword implementation
///
void program::rpn_div() {
    MIN_ARGUMENTS(2);

    // dividing numbers
    if (_stack->type(0) == cmd_number && _stack->type(1) == cmd_number) {
        _stack->value<number>(1) /= _stack->value<number>(0);
        _stack->pop();
    }
    // dividing complexes
    else if (_stack->type(0) == cmd_complex && _stack->type(1) == cmd_complex) {
        _stack->value<ocomplex>(1) /= _stack->value<ocomplex>(0);
        _stack->pop();
    }
    // dividing complex/number
    else if (_stack->type(0) == cmd_number && _stack->type(1) == cmd_complex) {
        _stack->value<ocomplex>(1) /= _stack->value<number>(0);
        _stack->pop();
    }
    // dividing number/complex
    else if (_stack->type(0) == cmd_complex && _stack->type(1) == cmd_number) {
        rpn_swap();
        _stack->value<ocomplex>(1) = _stack->value<number>(0) / _stack->value<ocomplex>(1);
        _stack->pop();
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief neg keyword implementation
///
void program::rpn_neg() {
    MIN_ARGUMENTS(1);

    if (_stack->type(0) == cmd_number)
        _stack->value<number>(0) = -_stack->value<number>(0);
    else if (_stack->type(0) == cmd_complex)
        _stack->value<ocomplex>(0) = -_stack->value<ocomplex>(0);
    else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief inv keyword implementation
///
void program::rpn_inv() {
    MIN_ARGUMENTS(1);

    if (_stack->type(0) == cmd_number)
        _stack->value<number>(0) = 1 / _stack->value<number>(0);
    else if (_stack->type(0) == cmd_complex)
        _stack->value<ocomplex>(0) = mpreal{1} / _stack->value<ocomplex>(0);
    else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief power keyword implementation
///
void program::rpn_power() {
    if (_stack->type(0) == cmd_number && _stack->type(1) == cmd_number) {
        _stack->value<number>(1) = pow(_stack->value<number>(1), _stack->value<number>(0));
        _stack->pop();
    } else if (_stack->type(0) == cmd_complex && _stack->type(1) == cmd_complex) {
        _stack->value<ocomplex>(1) = pow(_stack->value<ocomplex>(1), _stack->value<ocomplex>(0));
        _stack->pop();
    } else if (_stack->type(0) == cmd_number && _stack->type(1) == cmd_complex) {
        _stack->value<ocomplex>(1) = pow(_stack->value<ocomplex>(1), _stack->value<number>(0));
        _stack->pop();
    } else if (_stack->type(0) == cmd_complex && _stack->type(1) == cmd_number) {
        rpn_swap();
        _stack->value<ocomplex>(1) = pow(_stack->value<number>(0), _stack->value<ocomplex>(1));
        _stack->pop();
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief sqrt keyword implementation
///
void program::rpn_squareroot() {
    if (_stack->type(0) == cmd_number) {
        if (_stack->value<number>(0) >= 0) {
            _stack->value<number>(0) = rec_sqrt(_stack->value<number>(0));
        } else {
            // negative number -> square root is compl ex
            _stack->push(new ocomplex);// TODO manage new errors
            _stack->value<ocomplex>(0) = sqrt(_stack->value<number>(1));
            _stack->pop_front(1);
        }
    } else if (_stack->type(0) == cmd_complex)
        _stack->value<ocomplex>(0) = sqrt(_stack->value<ocomplex>(0));
    else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief hex keyword implementation
///
void program::rpn_hex() {
    MIN_ARGUMENTS(1);
    if (_stack->type(0) == cmd_number)
        _stack->obj<number>(0)->repr = number::hex;
    else if (_stack->type(0) == cmd_complex)
        _stack->obj<ocomplex>(0)->repr = number::hex;
    else {
        ERR_CONTEXT(ret_bad_operand_type);
        return;
    }
    number::s_decimal_digits = 0;
}

#if 0

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

/// @brief bin keyword implementation
///
void program::rpn_bin() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ((number*)_stack->back())->repr = number::bin;
}

/// @brief dec keyword implementation
///
void program::rpn_dec() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ((number*)_stack->back())->repr = number::dec;
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
        ((number*)_stack->at(0))->repr = number::base;
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
