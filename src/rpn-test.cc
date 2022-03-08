// Copyright (c) 2014-2022 Louis Rubet

#include "program.h"

/// @brief compared 2 strings on top of the stack_
///
/// @return 0 strings are equal
/// @return !0 strings are not equal (see strcmp output)
///
static int CmpStringOnStackTop(rpnstack& stk) {
    // stack_ should have 2 strings at level 1 and 2
    // this function removes these 2 entries
    int res = stk.value<String>(1).compare(stk.value<String>(0));
    stk.erase(0, 2);
    return res;
}

/// @brief > keyword implementation
///
void Program::RpnSup(void) {
    MIN_ARGUMENTS(2);
    if (stack_.type(0) == kNumber && stack_.type(1) == kNumber) {
        stack_.push_front(new Number(stack_.value<Number>(1) > stack_.value<Number>(0)));
        stack_.erase(1, 2);
    } else if (stack_.type(0) == kString && stack_.type(1) == kString) {
        stack_.push_front(new Number(CmpStringOnStackTop(stack_) == 1));
        stack_.erase(1, 2);
    } else {
        ERROR_CONTEXT(kBadOperandType);
    }
}

/// @brief >= keyword implementation
///
void Program::RpnSupEq(void) {
    MIN_ARGUMENTS(2);
    if (stack_.type(0) == kNumber && stack_.type(1) == kNumber) {
        stack_.push_front(new Number(stack_.value<Number>(1) >= stack_.value<Number>(0)));
        stack_.erase(1, 2);
    } else if (stack_.type(0) == kString && stack_.type(1) == kString) {
        stack_.push_front(new Number(CmpStringOnStackTop(stack_) != -1));
        stack_.erase(1, 2);
    } else {
        ERROR_CONTEXT(kBadOperandType);
    }
}

/// @brief < keyword implementation
///
void Program::RpnInf(void) {
    MIN_ARGUMENTS(2);

    if (stack_.type(0) == kNumber && stack_.type(1) == kNumber) {
        stack_.push_front(new Number(stack_.value<Number>(1) < stack_.value<Number>(0)));
        stack_.erase(1, 2);
    } else if (stack_.type(0) == kString && stack_.type(1) == kString) {
        stack_.push_front(new Number(CmpStringOnStackTop(stack_) == -1));
        stack_.erase(1, 2);
    } else {
        ERROR_CONTEXT(kBadOperandType);
    }
}

/// @brief <= keyword implementation
///
void Program::RpnInfEq(void) {
    MIN_ARGUMENTS(2);
    if (stack_.type(0) == kNumber && stack_.type(1) == kNumber) {
        stack_.push_front(new Number(stack_.value<Number>(1) <= stack_.value<Number>(0)));
        stack_.erase(1, 2);
    } else if (stack_.type(0) == kString && stack_.type(1) == kString) {
        stack_.push_front(new Number(CmpStringOnStackTop(stack_) != 1));
        stack_.erase(1, 2);
    } else {
        ERROR_CONTEXT(kBadOperandType);
    }
}

/// @brief != keyword implementation
///
void Program::RpnDiff(void) {
    MIN_ARGUMENTS(2);
    if (stack_.type(0) == kNumber && stack_.type(1) == kNumber) {
        stack_.push_front(new Number(stack_.value<Number>(1) != stack_.value<Number>(0)));
        stack_.erase(1, 2);
    } else if (stack_.type(0) == kComplex && stack_.type(1) == kComplex) {
        stack_.push_front(new Number(stack_.value<Complex>(1) != stack_.value<Complex>(0)));
        stack_.erase(1, 2);
    } else if (stack_.type(0) == kString && stack_.type(1) == kString) {
        stack_.push_front(new Number(CmpStringOnStackTop(stack_) != 0));
        stack_.erase(1, 2);
    } else {
        ERROR_CONTEXT(kBadOperandType);
    }
}

/// @brief == keyword implementation
///
void Program::RpnEq(void) {
    MIN_ARGUMENTS(2);
    if (stack_.type(0) == kNumber && stack_.type(1) == kNumber) {
        stack_.push_front(new Number(stack_.value<Number>(1) == stack_.value<Number>(0)));
        stack_.erase(1, 2);
    } else if (stack_.type(0) == kComplex && stack_.type(1) == kComplex) {
        stack_.push_front(new Number(stack_.value<Complex>(1) == stack_.value<Complex>(0)));
        stack_.erase(1, 2);
    } else if (stack_.type(0) == kString && stack_.type(1) == kString) {
        stack_.push_front(new Number(CmpStringOnStackTop(stack_) == 0));
        stack_.erase(1, 2);
    } else {
        ERROR_CONTEXT(kBadOperandType);
    }
}

/// @brief and keyword implementation
///
void Program::RpnTestAnd(void) {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, kNumber);
    ARG_MUST_BE_OF_TYPE(1, kNumber);
    if (stack_.value<Number>(0) != 0 && stack_.value<Number>(1) != 0)
        stack_.push(new Number(1));
    else
        stack_.push(new Number(0));
    stack_.erase(1, 2);
}

/// @brief or keyword implementation
///
void Program::RpnTestOr(void) {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, kNumber);
    ARG_MUST_BE_OF_TYPE(1, kNumber);
    if (stack_.value<Number>(0) != 0 || stack_.value<Number>(1) != 0)
        stack_.push(new Number(1));
    else
        stack_.push(new Number(0));
    stack_.erase(1, 2);
}

/// @brief xor keyword implementation
///
void Program::RpnTestXor(void) {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, kNumber);
    ARG_MUST_BE_OF_TYPE(1, kNumber);
    if ((stack_.value<Number>(0) != 0) ^ (stack_.value<Number>(1) != 0))
        stack_.push(new Number(1));
    else
        stack_.push(new Number(0));
    stack_.erase(1, 2);
}

/// @brief not keyword implementation
///
void Program::RpnTestNot(void) {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kNumber);

    stack_.push(new Number(stack_.value<Number>(0) == 0 ? 1 : 0));
    stack_.erase(1, 1);
}

/// @brief test same implementation
///
void Program::RpnSame(void) { RpnEq(); }
