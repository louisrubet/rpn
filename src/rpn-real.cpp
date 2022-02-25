// Copyright (c) 2014-2022 Louis Rubet

#include "program.hpp"

/// @brief + keyword implementation
///
void program::rpn_plus() {
    MIN_ARGUMENTS(2);
    if (stack_.type(0) == kString && stack_.type(1) == kString) {
        stack_.value<String>(1) += stack_.value<String>(0);
        stack_.pop();
    } else if (stack_.type(0) == kNumber && stack_.type(1) == kNumber) {
        stack_.value<Number>(1) += stack_.value<Number>(0);
        stack_.pop();
    } else if (stack_.type(0) == kComplex && stack_.type(1) == kComplex) {
        stack_.value<Complex>(1) += stack_.value<Complex>(0);
        stack_.pop();
    } else if (stack_.type(0) == kNumber && stack_.type(1) == kComplex) {
        stack_.value<Complex>(1) += stack_.value<Number>(0);
        stack_.pop();
    } else if (stack_.type(0) == kComplex && stack_.type(1) == kNumber) {
        rpn_swap();
        stack_.value<Complex>(1) += stack_.value<Number>(0);
        stack_.pop();
    } else {
        setErrorContext(kBadOperandType);
    }
}

/// @brief - keyword implementation
///
void program::rpn_minus() {
    MIN_ARGUMENTS(2);
    if (stack_.type(0) == kNumber && stack_.type(1) == kNumber) {
        stack_.value<Number>(1) -= stack_.value<Number>(0);
        stack_.pop();
    } else if (stack_.type(0) == kComplex && stack_.type(1) == kComplex) {
        stack_.value<Complex>(1) -= stack_.value<Complex>(0);
        stack_.pop();
    } else if (stack_.type(0) == kNumber && stack_.type(1) == kComplex) {
        stack_.value<Complex>(1) -= stack_.value<Number>(0);
        stack_.pop();
    } else if (stack_.type(0) == kComplex && stack_.type(1) == kNumber) {
        rpn_swap();
        stack_.value<Complex>(1) = stack_.value<Number>(0) - stack_.value<Complex>(1);
        stack_.pop();
    } else {
        setErrorContext(kBadOperandType);
    }
}

/// @brief * keyword implementation
///
void program::rpn_mul() {
    MIN_ARGUMENTS(2);
    if (stack_.type(0) == kNumber && stack_.type(1) == kNumber) {
        stack_.value<Number>(1) *= stack_.value<Number>(0);
        stack_.pop();
    } else if (stack_.type(0) == kComplex && stack_.type(1) == kComplex) {
        stack_.value<Complex>(1) *= stack_.value<Complex>(0);
        stack_.pop();
    } else if (stack_.type(0) == kNumber && stack_.type(1) == kComplex) {
        stack_.value<Complex>(1) *= stack_.value<Number>(0);
        stack_.pop();
    } else if (stack_.type(0) == kComplex && stack_.type(1) == kNumber) {
        rpn_swap();
        stack_.value<Complex>(1) *= stack_.value<Number>(0);
        stack_.pop();
    } else {
        setErrorContext(kBadOperandType);
    }
}

/// @brief / keyword implementation
///
void program::rpn_div() {
    MIN_ARGUMENTS(2);
    if (stack_.type(0) == kNumber && stack_.type(1) == kNumber) {
        stack_.value<Number>(1) /= stack_.value<Number>(0);
        stack_.pop();
    } else if (stack_.type(0) == kComplex && stack_.type(1) == kComplex) {
        stack_.value<Complex>(1) /= stack_.value<Complex>(0);
        stack_.pop();
    } else if (stack_.type(0) == kNumber && stack_.type(1) == kComplex) {
        stack_.value<Complex>(1) /= stack_.value<Number>(0);
        stack_.pop();
    } else if (stack_.type(0) == kComplex && stack_.type(1) == kNumber) {
        rpn_swap();
        stack_.value<Complex>(1) = stack_.value<Number>(0) / stack_.value<Complex>(1);
        stack_.pop();
    } else {
        setErrorContext(kBadOperandType);
    }
}

/// @brief neg keyword implementation
///
void program::rpn_neg() {
    MIN_ARGUMENTS(1);
    if (stack_.type(0) == kNumber)
        stack_.value<Number>(0) = -stack_.value<Number>(0);
    else if (stack_.type(0) == kComplex)
        stack_.value<Complex>(0) = -stack_.value<Complex>(0);
    else
        setErrorContext(kBadOperandType);
}

/// @brief inv keyword implementation
///
void program::rpn_inv() {
    MIN_ARGUMENTS(1);
    if (stack_.type(0) == kNumber)
        stack_.value<Number>(0) = 1 / stack_.value<Number>(0);
    else if (stack_.type(0) == kComplex)
        stack_.value<Complex>(0) = mpreal(1) / stack_.value<Complex>(0);
    else
        setErrorContext(kBadOperandType);
}

/// @brief power keyword implementation
///
void program::rpn_power() {
    MIN_ARGUMENTS(2);
    if (stack_.type(0) == kNumber && stack_.type(1) == kNumber) {
        if (stack_.value<Number>(1) >= 0) {
            stack_.value<Number>(1) = pow(stack_.value<Number>(1), stack_.value<Number>(0));
            stack_.pop();
        } else {
            mpreal zero;
            stack_.push(new Complex(stack_.value<Number>(1), zero, stack_.obj<Number>(1).base));
            stack_.value<Complex>(0) = pow(stack_.value<Complex>(0), stack_.value<Number>(1));
            stack_.erase(1, 2);
        }
    } else if (stack_.type(0) == kComplex && stack_.type(1) == kComplex) {
        stack_.value<Complex>(1) = pow(stack_.value<Complex>(1), stack_.value<Complex>(0));
        stack_.pop();
    } else if (stack_.type(0) == kNumber && stack_.type(1) == kComplex) {
        stack_.value<Complex>(1) = pow(stack_.value<Complex>(1), stack_.value<Number>(0));
        stack_.pop();
    } else if (stack_.type(0) == kComplex && stack_.type(1) == kNumber) {
        rpn_swap();
        stack_.value<Complex>(1) = pow(stack_.value<Number>(0), stack_.value<Complex>(1));
        stack_.pop();
    } else {
        setErrorContext(kBadOperandType);
    }
}

/// @brief sqrt keyword implementation
///
void program::rpn_squareroot() {
    MIN_ARGUMENTS(1);
    if (stack_.type(0) == kNumber) {
        if (stack_.value<Number>(0) >= 0) {
            stack_.value<Number>(0) = sqrt(stack_.value<Number>(0));
        } else {
            // negative number -> square root is complex
            mpreal zero;
            stack_.push(new Complex(stack_.value<Number>(0), zero,
                                     stack_.obj<Number>(0).base));  // TODO(louis) manage new errors
            stack_.value<Complex>(0) = sqrt(stack_.value<Complex>(0));
            stack_.erase(1);
        }
    } else if (stack_.type(0) == kComplex) {
        stack_.value<Complex>(0) = sqrt(stack_.value<Complex>(0));
    } else {
        setErrorContext(kBadOperandType);
    }
}

/// @brief hex keyword implementation
///
void program::rpn_hex() {
    MIN_ARGUMENTS(1);
    if (stack_.type(0) == kNumber) {
        stack_.obj<Number>(0).base = 16;
    } else if (stack_.type(0) == kComplex) {
        stack_.obj<Complex>(0).re_base = 16;
        stack_.obj<Complex>(0).im_base = 16;
    } else {
        setErrorContext(kBadOperandType);
    }
}

/// @brief bin keyword implementation
///
void program::rpn_bin() {
    MIN_ARGUMENTS(1);
    if (stack_.type(0) == kNumber) {
        stack_.obj<Number>(0).base = 2;
    } else if (stack_.type(0) == kComplex) {
        stack_.obj<Complex>(0).re_base = 2;
        stack_.obj<Complex>(0).im_base = 2;
    } else {
        setErrorContext(kBadOperandType);
    }
}

/// @brief dec keyword implementation
///
void program::rpn_dec() {
    MIN_ARGUMENTS(1);
    if (stack_.type(0) == kNumber) {
        stack_.obj<Number>(0).base = 10;
    } else if (stack_.type(0) == kComplex) {
        stack_.obj<Complex>(0).re_base = 10;
        stack_.obj<Complex>(0).im_base = 10;
    } else {
        setErrorContext(kBadOperandType);
    }
}

/// @brief base keyword implementation
///
void program::rpn_base() {
    MIN_ARGUMENTS(2);
    if (stack_.type(1) == kNumber || stack_.type(1) == kComplex) {
        int base = static_cast<int>(stack_.value<Number>(0).toLong());
        stack_.pop();
        if (base >= 2 && base <= 62) {
            if (stack_.type(0) == kNumber) {
                stack_.obj<Number>(0).base = base;
            } else {
                stack_.obj<Complex>(0).re_base = base;
                stack_.obj<Complex>(0).im_base = base;
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
    stack_.value<Number>(1) *= stack_.value<Number>(0) / 100;
    stack_.pop();
}

/// @brief %CH keyword implementation
///
void program::rpn_purcentCH() {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, kNumber);
    ARG_MUST_BE_OF_TYPE(1, kNumber);
    stack_.value<Number>(1) = (stack_.value<Number>(0) * 100) / stack_.value<Number>(1);
    stack_.pop();
}

/// @brief sq keyword implementation
///
void program::rpn_square() {
    MIN_ARGUMENTS(1);
    if (stack_.type(0) == kNumber)
        stack_.value<Number>(0) *= stack_.value<Number>(0);
    else if (stack_.at(0)->_type == kComplex)
        stack_.value<Complex>(0) *= stack_.value<Complex>(0);
    else
        setErrorContext(kBadOperandType);
}

/// @brief mod keyword implementation
///
void program::rpn_modulo() {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, kNumber);
    ARG_MUST_BE_OF_TYPE(1, kNumber);
    stack_.value<Number>(1) = fmod(stack_.value<Number>(1), stack_.value<Number>(0));
    stack_.pop();
}

/// @brief abs keyword implementation
///
void program::rpn_abs() {
    MIN_ARGUMENTS(1);
    if (stack_.type(0) == kNumber) {
        stack_.value<Number>(0) = abs(stack_.value<Number>(0));
    } else if (stack_.type(0) == kComplex) {
        stack_.push(new Number(abs(stack_.value<Complex>(0))));
        stack_.erase(1);
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
    stack_.value<Number>(0) = gamma(stack_.value<Number>(0) + 1);
}

/// @brief sign keyword implementation
///
void program::rpn_sign() {
    MIN_ARGUMENTS(1);
    if (stack_.type(0) == kNumber)
        stack_.value<Number>(0) = sgn(stack_.value<Number>(0));
    else if (stack_.at(0)->_type == kComplex)
        stack_.value<Complex>(0) = stack_.value<Complex>(0) / abs(stack_.value<Complex>(0));
    else
        setErrorContext(kBadOperandType);
}

/// @brief mant keyword implementation
///
void program::rpn_mant() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kNumber);
    if (!isfinite(stack_.value<Number>(0))) {
        setErrorContext(kOutOfRange);
        return;
    }
    mp_exp_t exp;
    stack_.value<Number>(0) = frexp(stack_.value<Number>(0), &exp);
}

/// @brief xpon keyword implementation
///
void program::rpn_xpon() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kNumber);
    if (!isfinite(stack_.value<Number>(0))) {
        setErrorContext(kOutOfRange);
        return;
    }
    mp_exp_t exp;
    (void)frexp(stack_.value<Number>(0), &exp);
    stack_.value<Number>(0) = exp;
}

/// @brief floor keyword implementation
///
void program::rpn_floor() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kNumber);
    stack_.value<Number>(0) = floor(stack_.value<Number>(0));
}

/// @brief ceil keyword implementation
///
void program::rpn_ceil() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kNumber);
    stack_.value<Number>(0) = ceil(stack_.value<Number>(0));
}

/// @brief fp keyword implementation
///
void program::rpn_fp() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kNumber);
    stack_.value<Number>(0) = frac(stack_.value<Number>(0));
}

/// @brief ip keyword implementation
///
void program::rpn_ip() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kNumber);
    stack_.value<Number>(0) = trunc(stack_.value<Number>(0));
}

/// @brief min keyword implementation
///
void program::rpn_min() {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, kNumber);
    ARG_MUST_BE_OF_TYPE(1, kNumber);
    stack_.value<Number>(0) = min(stack_.value<Number>(0), stack_.value<Number>(1));
    stack_.erase(1);
}

/// @brief max keyword implementation
///
void program::rpn_max() {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, kNumber);
    ARG_MUST_BE_OF_TYPE(1, kNumber);
    stack_.value<Number>(0) = max(stack_.value<Number>(0), stack_.value<Number>(1));
    stack_.erase(1);
}
