#include "program.hpp"

/// @brief + keyword implementation
///
void program::rpn_plus() {
    MIN_ARGUMENTS(2);

    // strings
    if (_stack.type(0) == cmd_string && _stack.type(1) == cmd_string) {
        _stack.value<ostring>(1) += _stack.value<ostring>(0);
        _stack.pop();
    }
    // numbers
    else if (_stack.type(0) == cmd_number && _stack.type(1) == cmd_number) {
        _stack.value<number>(1) += _stack.value<number>(0);
        _stack.pop();
    }
    // complexes
    else if (_stack.type(0) == cmd_complex && _stack.type(1) == cmd_complex) {
        _stack.value<ocomplex>(1) += _stack.value<ocomplex>(0);
        _stack.pop();
    }
    // complex+number
    else if (_stack.type(0) == cmd_number && _stack.type(1) == cmd_complex) {
        _stack.value<ocomplex>(1) += _stack.value<number>(0);
        _stack.pop();
    }
    // number+complex
    else if (_stack.type(0) == cmd_complex && _stack.type(1) == cmd_number) {
        rpn_swap();
        _stack.value<ocomplex>(1) += _stack.value<number>(0);
        _stack.pop();
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief - keyword implementation
///
void program::rpn_minus() {
    MIN_ARGUMENTS(2);

    // numbers
    if (_stack.type(0) == cmd_number && _stack.type(1) == cmd_number) {
        _stack.value<number>(1) -= _stack.value<number>(0);
        _stack.pop();
    }
    // complexes
    else if (_stack.type(0) == cmd_complex && _stack.type(1) == cmd_complex) {
        _stack.value<ocomplex>(1) -= _stack.value<ocomplex>(0);
        _stack.pop();
    }
    // subbing complex-number
    else if (_stack.type(0) == cmd_number && _stack.type(1) == cmd_complex) {
        _stack.value<ocomplex>(1) -= _stack.value<number>(0);
        _stack.pop();
    }
    // number-complex
    else if (_stack.type(0) == cmd_complex && _stack.type(1) == cmd_number) {
        rpn_swap();
        _stack.value<ocomplex>(1) = _stack.value<number>(0) - _stack.value<ocomplex>(1);
        _stack.pop();
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief * keyword implementation
///
void program::rpn_mul() {
    MIN_ARGUMENTS(2);

    // mutiplying numbers
    if (_stack.type(0) == cmd_number && _stack.type(1) == cmd_number) {
        _stack.value<number>(1) *= _stack.value<number>(0);
        _stack.pop();
    }
    // mutiplying complexes
    else if (_stack.type(0) == cmd_complex && _stack.type(1) == cmd_complex) {
        _stack.value<ocomplex>(1) *= _stack.value<ocomplex>(0);
        _stack.pop();
    }
    // mutiplying complex*number
    else if (_stack.type(0) == cmd_number && _stack.type(1) == cmd_complex) {
        _stack.value<ocomplex>(1) *= _stack.value<number>(0);
        _stack.pop();
    }
    // mutiplying number*complex
    else if (_stack.type(0) == cmd_complex && _stack.type(1) == cmd_number) {
        rpn_swap();
        _stack.value<ocomplex>(1) *= _stack.value<number>(0);
        _stack.pop();
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief / keyword implementation
///
void program::rpn_div() {
    MIN_ARGUMENTS(2);

    // dividing numbers
    if (_stack.type(0) == cmd_number && _stack.type(1) == cmd_number) {
        _stack.value<number>(1) /= _stack.value<number>(0);
        _stack.pop();
    }
    // dividing complexes
    else if (_stack.type(0) == cmd_complex && _stack.type(1) == cmd_complex) {
        _stack.value<ocomplex>(1) /= _stack.value<ocomplex>(0);
        _stack.pop();
    }
    // dividing complex/number
    else if (_stack.type(0) == cmd_number && _stack.type(1) == cmd_complex) {
        _stack.value<ocomplex>(1) /= _stack.value<number>(0);
        _stack.pop();
    }
    // dividing number/complex
    else if (_stack.type(0) == cmd_complex && _stack.type(1) == cmd_number) {
        rpn_swap();
        _stack.value<ocomplex>(1) = _stack.value<number>(0) / _stack.value<ocomplex>(1);
        _stack.pop();
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief neg keyword implementation
///
void program::rpn_neg() {
    MIN_ARGUMENTS(1);

    if (_stack.type(0) == cmd_number)
        _stack.value<number>(0) = -_stack.value<number>(0);
    else if (_stack.type(0) == cmd_complex)
        _stack.value<ocomplex>(0) = -_stack.value<ocomplex>(0);
    else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief inv keyword implementation
///
void program::rpn_inv() {
    MIN_ARGUMENTS(1);

    if (_stack.type(0) == cmd_number)
        _stack.value<number>(0) = 1 / _stack.value<number>(0);
    else if (_stack.type(0) == cmd_complex)
        _stack.value<ocomplex>(0) = mpreal{1} / _stack.value<ocomplex>(0);
    else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief power keyword implementation
///
void program::rpn_power() {
    MIN_ARGUMENTS(2);
    if (_stack.type(0) == cmd_number && _stack.type(1) == cmd_number) {
        if (_stack.value<number>(1) >= 0) {
            _stack.value<number>(1) = pow(_stack.value<number>(1), _stack.value<number>(0));
            _stack.pop();
        } else {
            mpreal zero;
            _stack.push(new ocomplex(_stack.value<number>(1), zero, _stack.obj<number>(1).base));
            _stack.value<ocomplex>(0) = pow(_stack.value<ocomplex>(0), _stack.value<number>(1));
            _stack.erase(1, 2);
        }
    } else if (_stack.type(0) == cmd_complex && _stack.type(1) == cmd_complex) {
        _stack.value<ocomplex>(1) = pow(_stack.value<ocomplex>(1), _stack.value<ocomplex>(0));
        _stack.pop();
    } else if (_stack.type(0) == cmd_number && _stack.type(1) == cmd_complex) {
        _stack.value<ocomplex>(1) = pow(_stack.value<ocomplex>(1), _stack.value<number>(0));
        _stack.pop();
    } else if (_stack.type(0) == cmd_complex && _stack.type(1) == cmd_number) {
        rpn_swap();
        _stack.value<ocomplex>(1) = pow(_stack.value<number>(0), _stack.value<ocomplex>(1));
        _stack.pop();
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief sqrt keyword implementation
///
void program::rpn_squareroot() {
    MIN_ARGUMENTS(1);
    if (_stack.type(0) == cmd_number) {
        if (_stack.value<number>(0) >= 0) {
            _stack.value<number>(0) = sqrt(_stack.value<number>(0));
        } else {
            // negative number -> square root is complex
            mpreal zero;
            _stack.push(new ocomplex(_stack.value<number>(0), zero, _stack.obj<number>(0).base));  // TODO manage new errors
            _stack.value<ocomplex>(0) = sqrt(_stack.value<ocomplex>(0));
            _stack.erase(1);
        }
    } else if (_stack.type(0) == cmd_complex)
        _stack.value<ocomplex>(0) = sqrt(_stack.value<ocomplex>(0));
    else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief hex keyword implementation
///
void program::rpn_hex() {
    MIN_ARGUMENTS(1);
    if (_stack.type(0) == cmd_number)
        _stack.obj<number>(0).base = 16;
    else if (_stack.type(0) == cmd_complex) {
        _stack.obj<ocomplex>(0).reBase = 16;
        _stack.obj<ocomplex>(0).imBase = 16;
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief bin keyword implementation
///
void program::rpn_bin() {
    MIN_ARGUMENTS(1);
    if (_stack.type(0) == cmd_number)
        _stack.obj<number>(0).base = 2;
    else if (_stack.type(0) == cmd_complex) {
        _stack.obj<ocomplex>(0).reBase = 2;
        _stack.obj<ocomplex>(0).imBase = 2;
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief dec keyword implementation
///
void program::rpn_dec() {
    MIN_ARGUMENTS(1);
    if (_stack.type(0) == cmd_number)
        _stack.obj<number>(0).base = 10;
    else if (_stack.type(0) == cmd_complex) {
        _stack.obj<ocomplex>(0).reBase = 10;
        _stack.obj<ocomplex>(0).imBase = 10;
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief base keyword implementation
///
void program::rpn_base() {
    MIN_ARGUMENTS(2);
    if (_stack.type(1) == cmd_number || _stack.type(1) == cmd_complex) {
        int base = (int)_stack.value<number>(0).toLong();
        _stack.pop();
        if (base >= BASE_MIN && base <= BASE_MAX) {
            if (_stack.type(0) == cmd_number)
                _stack.obj<number>(0).base = base;
            else {
                _stack.obj<ocomplex>(0).reBase = base;
                _stack.obj<ocomplex>(0).imBase = base;
            }
        } else
            ERR_CONTEXT(ret_out_of_range);
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief % (purcent) keyword implementation
///
void program::rpn_purcent() {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ARG_MUST_BE_OF_TYPE(1, cmd_number);
    _stack.value<number>(1) *= _stack.value<number>(0) / 100;
    _stack.pop();
}

/// @brief %CH keyword implementation
///
void program::rpn_purcentCH() {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ARG_MUST_BE_OF_TYPE(1, cmd_number);
    _stack.value<number>(1) = (_stack.value<number>(0) * 100) / _stack.value<number>(1);
    _stack.pop();
}

/// @brief sq keyword implementation
///
void program::rpn_square() {
    MIN_ARGUMENTS(1);
    if (_stack.type(0) == cmd_number)
        _stack.value<number>(0) *= _stack.value<number>(0);
    else if (_stack.at(0)->_type == cmd_complex)
        _stack.value<ocomplex>(0) *= _stack.value<ocomplex>(0);
    else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief mod keyword implementation
///
void program::rpn_modulo() {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ARG_MUST_BE_OF_TYPE(1, cmd_number);
    _stack.value<number>(1) = fmod(_stack.value<number>(1), _stack.value<number>(0));
    _stack.pop();
}

/// @brief abs keyword implementation
///
void program::rpn_abs() {
    MIN_ARGUMENTS(1);
    if (_stack.type(0) == cmd_number)
        _stack.value<number>(0) = abs(_stack.value<number>(0));
    else if (_stack.type(0) == cmd_complex) {
        _stack.push(new number(abs(_stack.value<ocomplex>(0))));
        _stack.erase(1);
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief fact (factorial) keyword implementation
///
void program::rpn_fact() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    // fact(n) = gamma(n+1)
    _stack.value<number>(0) = gamma(_stack.value<number>(0) + 1);
}

/// @brief sign keyword implementation
///
void program::rpn_sign() {
    MIN_ARGUMENTS(1);
    if (_stack.type(0) == cmd_number)
        _stack.value<number>(0) = sgn(_stack.value<number>(0));
    else if (_stack.at(0)->_type == cmd_complex)
        _stack.value<ocomplex>(0) = _stack.value<ocomplex>(0) / abs(_stack.value<ocomplex>(0));
    else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief mant keyword implementation
///
void program::rpn_mant() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    if (!isfinite(_stack.value<number>(0))) {
        ERR_CONTEXT(ret_out_of_range);
        return;
    }
    mp_exp_t exp;
    _stack.value<number>(0) = frexp(_stack.value<number>(0), &exp);
}

/// @brief xpon keyword implementation
///
void program::rpn_xpon() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    if (!isfinite(_stack.value<number>(0))) {
        ERR_CONTEXT(ret_out_of_range);
        return;
    }
    mp_exp_t exp;
    (void)frexp(_stack.value<number>(0), &exp);
    _stack.value<number>(0) = exp;
}

/// @brief floor keyword implementation
///
void program::rpn_floor() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    _stack.value<number>(0) = floor(_stack.value<number>(0));
}

/// @brief ceil keyword implementation
///
void program::rpn_ceil() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    _stack.value<number>(0) = ceil(_stack.value<number>(0));
}

/// @brief fp keyword implementation
///
void program::rpn_fp() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    _stack.value<number>(0) = frac(_stack.value<number>(0));
}

/// @brief ip keyword implementation
///
void program::rpn_ip() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    _stack.value<number>(0) = trunc(_stack.value<number>(0));
}

/// @brief min keyword implementation
///
void program::rpn_min() {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ARG_MUST_BE_OF_TYPE(1, cmd_number);
    _stack.value<number>(0) = min(_stack.value<number>(0), _stack.value<number>(1));
    _stack.erase(1);
}

/// @brief max keyword implementation
///
void program::rpn_max() {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ARG_MUST_BE_OF_TYPE(1, cmd_number);
    _stack.value<number>(0) = max(_stack.value<number>(0), _stack.value<number>(1));
    _stack.erase(1);
}
