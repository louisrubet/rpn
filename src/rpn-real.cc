// Copyright (c) 2014-2022 Louis Rubet

#include "program.h"

/// @brief + keyword implementation
///
void Program::RpnPlus() {
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
        RpnSwap();
        stack_.value<Complex>(1) += stack_.value<Number>(0);
        stack_.pop();
    } else {
        ERROR_CONTEXT(kBadOperandType);
    }
}

/// @brief - keyword implementation
///
void Program::RpnMinus() {
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
        RpnSwap();
        stack_.value<Complex>(1) = stack_.value<Number>(0) - stack_.value<Complex>(1);
        stack_.pop();
    } else {
        ERROR_CONTEXT(kBadOperandType);
    }
}

/// @brief * keyword implementation
///
void Program::RpnMul() {
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
        RpnSwap();
        stack_.value<Complex>(1) *= stack_.value<Number>(0);
        stack_.pop();
    } else {
        ERROR_CONTEXT(kBadOperandType);
    }
}

/// @brief / keyword implementation
///
void Program::RpnDiv() {
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
        RpnSwap();
        stack_.value<Complex>(1) = stack_.value<Number>(0) / stack_.value<Complex>(1);
        stack_.pop();
    } else {
        ERROR_CONTEXT(kBadOperandType);
    }
}

/// @brief neg keyword implementation
///
void Program::RpnNeg() {
    MIN_ARGUMENTS(1);
    if (stack_.type(0) == kNumber)
        stack_.value<Number>(0) = -stack_.value<Number>(0);
    else if (stack_.type(0) == kComplex)
        stack_.value<Complex>(0) = -stack_.value<Complex>(0);
    else
        ERROR_CONTEXT(kBadOperandType);
}

/// @brief inv keyword implementation
///
void Program::RpnInv() {
    MIN_ARGUMENTS(1);
    if (stack_.type(0) == kNumber)
        stack_.value<Number>(0) = 1 / stack_.value<Number>(0);
    else if (stack_.type(0) == kComplex)
        stack_.value<Complex>(0) = mpreal(1) / stack_.value<Complex>(0);
    else
        ERROR_CONTEXT(kBadOperandType);
}

/// @brief power keyword implementation
///
void Program::RpnPower() {
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
        RpnSwap();
        stack_.value<Complex>(1) = pow(stack_.value<Number>(0), stack_.value<Complex>(1));
        stack_.pop();
    } else {
        ERROR_CONTEXT(kBadOperandType);
    }
}

/// @brief sqrt keyword implementation
///
void Program::RpnSquareroot() {
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
        ERROR_CONTEXT(kBadOperandType);
    }
}

/// @brief hex keyword implementation
///
void Program::RpnHex() {
    MIN_ARGUMENTS(1);
    if (stack_.type(0) == kNumber) {
        stack_.obj<Number>(0).base = 16;
    } else if (stack_.type(0) == kComplex) {
        stack_.obj<Complex>(0).re_base = 16;
        stack_.obj<Complex>(0).im_base = 16;
    } else {
        ERROR_CONTEXT(kBadOperandType);
    }
}

/// @brief bin keyword implementation
///
void Program::RpnBin() {
    MIN_ARGUMENTS(1);
    if (stack_.type(0) == kNumber) {
        stack_.obj<Number>(0).base = 2;
    } else if (stack_.type(0) == kComplex) {
        stack_.obj<Complex>(0).re_base = 2;
        stack_.obj<Complex>(0).im_base = 2;
    } else {
        ERROR_CONTEXT(kBadOperandType);
    }
}

/// @brief dec keyword implementation
///
void Program::RpnDec() {
    MIN_ARGUMENTS(1);
    if (stack_.type(0) == kNumber) {
        stack_.obj<Number>(0).base = 10;
    } else if (stack_.type(0) == kComplex) {
        stack_.obj<Complex>(0).re_base = 10;
        stack_.obj<Complex>(0).im_base = 10;
    } else {
        ERROR_CONTEXT(kBadOperandType);
    }
}

/// @brief base keyword implementation
///
void Program::RpnBase() {
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
            ERROR_CONTEXT(kOutOfRange);
        }
    } else {
        ERROR_CONTEXT(kBadOperandType);
    }
}

/// @brief % (purcent) keyword implementation
///
void Program::RpnPurcent() {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, kNumber);
    ARG_MUST_BE_OF_TYPE(1, kNumber);
    stack_.value<Number>(1) *= stack_.value<Number>(0) / 100;
    stack_.pop();
}

/// @brief %CH keyword implementation
///
void Program::RpnPurcentCH() {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, kNumber);
    ARG_MUST_BE_OF_TYPE(1, kNumber);
    stack_.value<Number>(1) = (stack_.value<Number>(0) * 100) / stack_.value<Number>(1);
    stack_.pop();
}

/// @brief sq keyword implementation
///
void Program::RpnSquare() {
    MIN_ARGUMENTS(1);
    if (stack_.type(0) == kNumber)
        stack_.value<Number>(0) *= stack_.value<Number>(0);
    else if (stack_.at(0)->type == kComplex)
        stack_.value<Complex>(0) *= stack_.value<Complex>(0);
    else
        ERROR_CONTEXT(kBadOperandType);
}

/// @brief mod keyword implementation
///
void Program::RpnModulo() {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, kNumber);
    ARG_MUST_BE_OF_TYPE(1, kNumber);
    stack_.value<Number>(1) = fmod(stack_.value<Number>(1), stack_.value<Number>(0));
    stack_.pop();
}

/// @brief abs keyword implementation
///
void Program::RpnAbs() {
    MIN_ARGUMENTS(1);
    if (stack_.type(0) == kNumber) {
        stack_.value<Number>(0) = abs(stack_.value<Number>(0));
    } else if (stack_.type(0) == kComplex) {
        stack_.push(new Number(abs(stack_.value<Complex>(0))));
        stack_.erase(1);
    } else {
        ERROR_CONTEXT(kBadOperandType);
    }
}

/// @brief fact (factorial) keyword implementation
///
void Program::RpnFact() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kNumber);
    // fact(n) = gamma(n+1)
    stack_.value<Number>(0) = gamma(stack_.value<Number>(0) + 1);
}

/// @brief sign keyword implementation
///
void Program::RpnSign() {
    MIN_ARGUMENTS(1);
    if (stack_.type(0) == kNumber)
        stack_.value<Number>(0) = sgn(stack_.value<Number>(0));
    else if (stack_.at(0)->type == kComplex)
        stack_.value<Complex>(0) = stack_.value<Complex>(0) / abs(stack_.value<Complex>(0));
    else
        ERROR_CONTEXT(kBadOperandType);
}

/// @brief mant keyword implementation
///
void Program::RpnMant() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kNumber);
    if (!isfinite(stack_.value<Number>(0))) {
        ERROR_CONTEXT(kOutOfRange);
        return;
    }
    mp_exp_t exp;
    stack_.value<Number>(0) = frexp(stack_.value<Number>(0), &exp);
}

/// @brief xpon keyword implementation
///
void Program::RpnXpon() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kNumber);
    if (!isfinite(stack_.value<Number>(0))) {
        ERROR_CONTEXT(kOutOfRange);
        return;
    }
    mp_exp_t exp;
    (void)frexp(stack_.value<Number>(0), &exp);
    stack_.value<Number>(0) = exp;
}

/// @brief floor keyword implementation
///
void Program::RpnFloor() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kNumber);
    stack_.value<Number>(0) = floor(stack_.value<Number>(0));
}

/// @brief ceil keyword implementation
///
void Program::RpnCeil() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kNumber);
    stack_.value<Number>(0) = ceil(stack_.value<Number>(0));
}

/// @brief fp keyword implementation
///
void Program::RpnFp() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kNumber);
    stack_.value<Number>(0) = frac(stack_.value<Number>(0));
}

/// @brief ip keyword implementation
///
void Program::RpnIp() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kNumber);
    stack_.value<Number>(0) = trunc(stack_.value<Number>(0));
}

/// @brief min keyword implementation
///
void Program::RpnMin() {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, kNumber);
    ARG_MUST_BE_OF_TYPE(1, kNumber);
    stack_.value<Number>(0) = min(stack_.value<Number>(0), stack_.value<Number>(1));
    stack_.erase(1);
}

/// @brief max keyword implementation
///
void Program::RpnMax() {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, kNumber);
    ARG_MUST_BE_OF_TYPE(1, kNumber);
    stack_.value<Number>(0) = max(stack_.value<Number>(0), stack_.value<Number>(1));
    stack_.erase(1);
}
