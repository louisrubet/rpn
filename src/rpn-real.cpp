// Copyright (c) 2014-2022 Louis Rubet

#include "program.hpp"

/// @brief + keyword implementation
///
void program::rpn_plus() {
    MIN_ARGUMENTS(2);
    if (_stack.type(0) == kString && _stack.type(1) == kString) {
        _stack.value<String>(1) += _stack.value<String>(0);
        _stack.pop();
    } else if (_stack.type(0) == kNumber && _stack.type(1) == kNumber) {
        _stack.value<Number>(1) += _stack.value<Number>(0);
        _stack.pop();
    } else if (_stack.type(0) == kComplex && _stack.type(1) == kComplex) {
        _stack.value<Complex>(1) += _stack.value<Complex>(0);
        _stack.pop();
    } else if (_stack.type(0) == kNumber && _stack.type(1) == kComplex) {
        _stack.value<Complex>(1) += _stack.value<Number>(0);
        _stack.pop();
    } else if (_stack.type(0) == kComplex && _stack.type(1) == kNumber) {
        rpn_swap();
        _stack.value<Complex>(1) += _stack.value<Number>(0);
        _stack.pop();
    } else {
        setErrorContext(kBadOperandType);
    }
}

/// @brief - keyword implementation
///
void program::rpn_minus() {
    MIN_ARGUMENTS(2);
    if (_stack.type(0) == kNumber && _stack.type(1) == kNumber) {
        _stack.value<Number>(1) -= _stack.value<Number>(0);
        _stack.pop();
    } else if (_stack.type(0) == kComplex && _stack.type(1) == kComplex) {
        _stack.value<Complex>(1) -= _stack.value<Complex>(0);
        _stack.pop();
    } else if (_stack.type(0) == kNumber && _stack.type(1) == kComplex) {
        _stack.value<Complex>(1) -= _stack.value<Number>(0);
        _stack.pop();
    } else if (_stack.type(0) == kComplex && _stack.type(1) == kNumber) {
        rpn_swap();
        _stack.value<Complex>(1) = _stack.value<Number>(0) - _stack.value<Complex>(1);
        _stack.pop();
    } else {
        setErrorContext(kBadOperandType);
    }
}

/// @brief * keyword implementation
///
void program::rpn_mul() {
    MIN_ARGUMENTS(2);
    if (_stack.type(0) == kNumber && _stack.type(1) == kNumber) {
        _stack.value<Number>(1) *= _stack.value<Number>(0);
        _stack.pop();
    } else if (_stack.type(0) == kComplex && _stack.type(1) == kComplex) {
        _stack.value<Complex>(1) *= _stack.value<Complex>(0);
        _stack.pop();
    } else if (_stack.type(0) == kNumber && _stack.type(1) == kComplex) {
        _stack.value<Complex>(1) *= _stack.value<Number>(0);
        _stack.pop();
    } else if (_stack.type(0) == kComplex && _stack.type(1) == kNumber) {
        rpn_swap();
        _stack.value<Complex>(1) *= _stack.value<Number>(0);
        _stack.pop();
    } else {
        setErrorContext(kBadOperandType);
    }
}

/// @brief / keyword implementation
///
void program::rpn_div() {
    MIN_ARGUMENTS(2);
    if (_stack.type(0) == kNumber && _stack.type(1) == kNumber) {
        _stack.value<Number>(1) /= _stack.value<Number>(0);
        _stack.pop();
    } else if (_stack.type(0) == kComplex && _stack.type(1) == kComplex) {
        _stack.value<Complex>(1) /= _stack.value<Complex>(0);
        _stack.pop();
    } else if (_stack.type(0) == kNumber && _stack.type(1) == kComplex) {
        _stack.value<Complex>(1) /= _stack.value<Number>(0);
        _stack.pop();
    } else if (_stack.type(0) == kComplex && _stack.type(1) == kNumber) {
        rpn_swap();
        _stack.value<Complex>(1) = _stack.value<Number>(0) / _stack.value<Complex>(1);
        _stack.pop();
    } else {
        setErrorContext(kBadOperandType);
    }
}

/// @brief neg keyword implementation
///
void program::rpn_neg() {
    MIN_ARGUMENTS(1);
    if (_stack.type(0) == kNumber)
        _stack.value<Number>(0) = -_stack.value<Number>(0);
    else if (_stack.type(0) == kComplex)
        _stack.value<Complex>(0) = -_stack.value<Complex>(0);
    else
        setErrorContext(kBadOperandType);
}

/// @brief inv keyword implementation
///
void program::rpn_inv() {
    MIN_ARGUMENTS(1);
    if (_stack.type(0) == kNumber)
        _stack.value<Number>(0) = 1 / _stack.value<Number>(0);
    else if (_stack.type(0) == kComplex)
        _stack.value<Complex>(0) = mpreal(1) / _stack.value<Complex>(0);
    else
        setErrorContext(kBadOperandType);
}

/// @brief power keyword implementation
///
void program::rpn_power() {
    MIN_ARGUMENTS(2);
    if (_stack.type(0) == kNumber && _stack.type(1) == kNumber) {
        if (_stack.value<Number>(1) >= 0) {
            _stack.value<Number>(1) = pow(_stack.value<Number>(1), _stack.value<Number>(0));
            _stack.pop();
        } else {
            mpreal zero;
            _stack.push(new Complex(_stack.value<Number>(1), zero, _stack.obj<Number>(1).base));
            _stack.value<Complex>(0) = pow(_stack.value<Complex>(0), _stack.value<Number>(1));
            _stack.erase(1, 2);
        }
    } else if (_stack.type(0) == kComplex && _stack.type(1) == kComplex) {
        _stack.value<Complex>(1) = pow(_stack.value<Complex>(1), _stack.value<Complex>(0));
        _stack.pop();
    } else if (_stack.type(0) == kNumber && _stack.type(1) == kComplex) {
        _stack.value<Complex>(1) = pow(_stack.value<Complex>(1), _stack.value<Number>(0));
        _stack.pop();
    } else if (_stack.type(0) == kComplex && _stack.type(1) == kNumber) {
        rpn_swap();
        _stack.value<Complex>(1) = pow(_stack.value<Number>(0), _stack.value<Complex>(1));
        _stack.pop();
    } else {
        setErrorContext(kBadOperandType);
    }
}

/// @brief sqrt keyword implementation
///
void program::rpn_squareroot() {
    MIN_ARGUMENTS(1);
    if (_stack.type(0) == kNumber) {
        if (_stack.value<Number>(0) >= 0) {
            _stack.value<Number>(0) = sqrt(_stack.value<Number>(0));
        } else {
            // negative number -> square root is complex
            mpreal zero;
            _stack.push(new Complex(_stack.value<Number>(0), zero,
                                     _stack.obj<Number>(0).base));  // TODO(louis) manage new errors
            _stack.value<Complex>(0) = sqrt(_stack.value<Complex>(0));
            _stack.erase(1);
        }
    } else if (_stack.type(0) == kComplex) {
        _stack.value<Complex>(0) = sqrt(_stack.value<Complex>(0));
    } else {
        setErrorContext(kBadOperandType);
    }
}

/// @brief hex keyword implementation
///
void program::rpn_hex() {
    MIN_ARGUMENTS(1);
    if (_stack.type(0) == kNumber) {
        _stack.obj<Number>(0).base = 16;
    } else if (_stack.type(0) == kComplex) {
        _stack.obj<Complex>(0).reBase = 16;
        _stack.obj<Complex>(0).imBase = 16;
    } else {
        setErrorContext(kBadOperandType);
    }
}

/// @brief bin keyword implementation
///
void program::rpn_bin() {
    MIN_ARGUMENTS(1);
    if (_stack.type(0) == kNumber) {
        _stack.obj<Number>(0).base = 2;
    } else if (_stack.type(0) == kComplex) {
        _stack.obj<Complex>(0).reBase = 2;
        _stack.obj<Complex>(0).imBase = 2;
    } else {
        setErrorContext(kBadOperandType);
    }
}

/// @brief dec keyword implementation
///
void program::rpn_dec() {
    MIN_ARGUMENTS(1);
    if (_stack.type(0) == kNumber) {
        _stack.obj<Number>(0).base = 10;
    } else if (_stack.type(0) == kComplex) {
        _stack.obj<Complex>(0).reBase = 10;
        _stack.obj<Complex>(0).imBase = 10;
    } else {
        setErrorContext(kBadOperandType);
    }
}

/// @brief base keyword implementation
///
void program::rpn_base() {
    MIN_ARGUMENTS(2);
    if (_stack.type(1) == kNumber || _stack.type(1) == kComplex) {
        int base = static_cast<int>(_stack.value<Number>(0).toLong());
        _stack.pop();
        if (base >= 2 && base <= 62) {
            if (_stack.type(0) == kNumber) {
                _stack.obj<Number>(0).base = base;
            } else {
                _stack.obj<Complex>(0).reBase = base;
                _stack.obj<Complex>(0).imBase = base;
            }
        } else {
            setErrorContext(kOutOfRange);
        }
    } else {
        setErrorContext(kBadOperandType);
    }
}

/// @brief % (purcent) keyword implementation
///
void program::rpn_purcent() {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, kNumber);
    ARG_MUST_BE_OF_TYPE(1, kNumber);
    _stack.value<Number>(1) *= _stack.value<Number>(0) / 100;
    _stack.pop();
}

/// @brief %CH keyword implementation
///
void program::rpn_purcentCH() {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, kNumber);
    ARG_MUST_BE_OF_TYPE(1, kNumber);
    _stack.value<Number>(1) = (_stack.value<Number>(0) * 100) / _stack.value<Number>(1);
    _stack.pop();
}

/// @brief sq keyword implementation
///
void program::rpn_square() {
    MIN_ARGUMENTS(1);
    if (_stack.type(0) == kNumber)
        _stack.value<Number>(0) *= _stack.value<Number>(0);
    else if (_stack.at(0)->_type == kComplex)
        _stack.value<Complex>(0) *= _stack.value<Complex>(0);
    else
        setErrorContext(kBadOperandType);
}

/// @brief mod keyword implementation
///
void program::rpn_modulo() {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, kNumber);
    ARG_MUST_BE_OF_TYPE(1, kNumber);
    _stack.value<Number>(1) = fmod(_stack.value<Number>(1), _stack.value<Number>(0));
    _stack.pop();
}

/// @brief abs keyword implementation
///
void program::rpn_abs() {
    MIN_ARGUMENTS(1);
    if (_stack.type(0) == kNumber) {
        _stack.value<Number>(0) = abs(_stack.value<Number>(0));
    } else if (_stack.type(0) == kComplex) {
        _stack.push(new Number(abs(_stack.value<Complex>(0))));
        _stack.erase(1);
    } else {
        setErrorContext(kBadOperandType);
    }
}

/// @brief fact (factorial) keyword implementation
///
void program::rpn_fact() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kNumber);
    // fact(n) = gamma(n+1)
    _stack.value<Number>(0) = gamma(_stack.value<Number>(0) + 1);
}

/// @brief sign keyword implementation
///
void program::rpn_sign() {
    MIN_ARGUMENTS(1);
    if (_stack.type(0) == kNumber)
        _stack.value<Number>(0) = sgn(_stack.value<Number>(0));
    else if (_stack.at(0)->_type == kComplex)
        _stack.value<Complex>(0) = _stack.value<Complex>(0) / abs(_stack.value<Complex>(0));
    else
        setErrorContext(kBadOperandType);
}

/// @brief mant keyword implementation
///
void program::rpn_mant() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kNumber);
    if (!isfinite(_stack.value<Number>(0))) {
        setErrorContext(kOutOfRange);
        return;
    }
    mp_exp_t exp;
    _stack.value<Number>(0) = frexp(_stack.value<Number>(0), &exp);
}

/// @brief xpon keyword implementation
///
void program::rpn_xpon() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kNumber);
    if (!isfinite(_stack.value<Number>(0))) {
        setErrorContext(kOutOfRange);
        return;
    }
    mp_exp_t exp;
    (void)frexp(_stack.value<Number>(0), &exp);
    _stack.value<Number>(0) = exp;
}

/// @brief floor keyword implementation
///
void program::rpn_floor() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kNumber);
    _stack.value<Number>(0) = floor(_stack.value<Number>(0));
}

/// @brief ceil keyword implementation
///
void program::rpn_ceil() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kNumber);
    _stack.value<Number>(0) = ceil(_stack.value<Number>(0));
}

/// @brief fp keyword implementation
///
void program::rpn_fp() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kNumber);
    _stack.value<Number>(0) = frac(_stack.value<Number>(0));
}

/// @brief ip keyword implementation
///
void program::rpn_ip() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kNumber);
    _stack.value<Number>(0) = trunc(_stack.value<Number>(0));
}

/// @brief min keyword implementation
///
void program::rpn_min() {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, kNumber);
    ARG_MUST_BE_OF_TYPE(1, kNumber);
    _stack.value<Number>(0) = min(_stack.value<Number>(0), _stack.value<Number>(1));
    _stack.erase(1);
}

/// @brief max keyword implementation
///
void program::rpn_max() {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, kNumber);
    ARG_MUST_BE_OF_TYPE(1, kNumber);
    _stack.value<Number>(0) = max(_stack.value<Number>(0), _stack.value<Number>(1));
    _stack.erase(1);
}
