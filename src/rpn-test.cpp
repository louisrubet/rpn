// Copyright (c) 2014-2022 Louis Rubet

#include "program.hpp"

/// @brief compared 2 strings on top of the stack
///
/// @return 0 strings are equal
/// @return !0 strings are not equal (see strcmp output)
///
static int CmpStringOnStackTop(rpnstack& stk) {
    // _stack should have 2 strings at level 1 and 2
    // this function removes these 2 entries
    int res = stk.value<String>(1).compare(stk.value<String>(0));
    stk.erase(0, 2);
    return res;
}

/// @brief > keyword implementation
///
void program::rpn_sup(void) {
    MIN_ARGUMENTS(2);
    if (_stack.type(0) == cmd_number && _stack.type(1) == cmd_number) {
        _stack.push_front(new Number(_stack.value<Number>(1) > _stack.value<Number>(0)));
        _stack.erase(1, 2);
    } else if (_stack.type(0) == cmd_string && _stack.type(1) == cmd_string) {
        _stack.push_front(new Number(CmpStringOnStackTop(_stack) == 1));
        _stack.erase(1, 2);
    } else {
        setErrorContext(ret_bad_operand_type);
    }
}

/// @brief >= keyword implementation
///
void program::rpn_sup_eq(void) {
    MIN_ARGUMENTS(2);
    if (_stack.type(0) == cmd_number && _stack.type(1) == cmd_number) {
        _stack.push_front(new Number(_stack.value<Number>(1) >= _stack.value<Number>(0)));
        _stack.erase(1, 2);
    } else if (_stack.type(0) == cmd_string && _stack.type(1) == cmd_string) {
        _stack.push_front(new Number(CmpStringOnStackTop(_stack) != -1));
        _stack.erase(1, 2);
    } else {
        setErrorContext(ret_bad_operand_type);
    }
}

/// @brief < keyword implementation
///
void program::rpn_inf(void) {
    MIN_ARGUMENTS(2);

    if (_stack.type(0) == cmd_number && _stack.type(1) == cmd_number) {
        _stack.push_front(new Number(_stack.value<Number>(1) < _stack.value<Number>(0)));
        _stack.erase(1, 2);
    } else if (_stack.type(0) == cmd_string && _stack.type(1) == cmd_string) {
        _stack.push_front(new Number(CmpStringOnStackTop(_stack) == -1));
        _stack.erase(1, 2);
    } else {
        setErrorContext(ret_bad_operand_type);
    }
}

/// @brief <= keyword implementation
///
void program::rpn_inf_eq(void) {
    MIN_ARGUMENTS(2);
    if (_stack.type(0) == cmd_number && _stack.type(1) == cmd_number) {
        _stack.push_front(new Number(_stack.value<Number>(1) <= _stack.value<Number>(0)));
        _stack.erase(1, 2);
    } else if (_stack.type(0) == cmd_string && _stack.type(1) == cmd_string) {
        _stack.push_front(new Number(CmpStringOnStackTop(_stack) != 1));
        _stack.erase(1, 2);
    } else {
        setErrorContext(ret_bad_operand_type);
    }
}

/// @brief != keyword implementation
///
void program::rpn_diff(void) {
    MIN_ARGUMENTS(2);
    if (_stack.type(0) == cmd_number && _stack.type(1) == cmd_number) {
        _stack.push_front(new Number(_stack.value<Number>(1) != _stack.value<Number>(0)));
        _stack.erase(1, 2);
    } else if (_stack.type(0) == cmd_complex && _stack.type(1) == cmd_complex) {
        _stack.push_front(new Number(_stack.value<Complex>(1) != _stack.value<Complex>(0)));
        _stack.erase(1, 2);
    } else if (_stack.type(0) == cmd_string && _stack.type(1) == cmd_string) {
        _stack.push_front(new Number(CmpStringOnStackTop(_stack) != 0));
        _stack.erase(1, 2);
    } else {
        setErrorContext(ret_bad_operand_type);
    }
}

/// @brief == keyword implementation
///
void program::rpn_eq(void) {
    MIN_ARGUMENTS(2);
    if (_stack.type(0) == cmd_number && _stack.type(1) == cmd_number) {
        _stack.push_front(new Number(_stack.value<Number>(1) == _stack.value<Number>(0)));
        _stack.erase(1, 2);
    } else if (_stack.type(0) == cmd_complex && _stack.type(1) == cmd_complex) {
        _stack.push_front(new Number(_stack.value<Complex>(1) == _stack.value<Complex>(0)));
        _stack.erase(1, 2);
    } else if (_stack.type(0) == cmd_string && _stack.type(1) == cmd_string) {
        _stack.push_front(new Number(CmpStringOnStackTop(_stack) == 0));
        _stack.erase(1, 2);
    } else {
        setErrorContext(ret_bad_operand_type);
    }
}

/// @brief and keyword implementation
///
void program::rpn_test_and(void) {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ARG_MUST_BE_OF_TYPE(1, cmd_number);
    if (_stack.value<Number>(0) != 0 && _stack.value<Number>(1) != 0)
        _stack.push(new Number(1));
    else
        _stack.push(new Number(0));
    _stack.erase(1, 2);
}

/// @brief or keyword implementation
///
void program::rpn_test_or(void) {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ARG_MUST_BE_OF_TYPE(1, cmd_number);
    if (_stack.value<Number>(0) != 0 || _stack.value<Number>(1) != 0)
        _stack.push(new Number(1));
    else
        _stack.push(new Number(0));
    _stack.erase(1, 2);
}

/// @brief xor keyword implementation
///
void program::rpn_test_xor(void) {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ARG_MUST_BE_OF_TYPE(1, cmd_number);
    if (_stack.value<Number>(0) != 0 ^ _stack.value<Number>(1) != 0)
        _stack.push(new Number(1));
    else
        _stack.push(new Number(0));
    _stack.erase(1, 2);
}

/// @brief not keyword implementation
///
void program::rpn_test_not(void) {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    _stack.push(new Number(_stack.value<Number>(0) == 0 ? 1 : 0));
    _stack.erase(1, 1);
}

/// @brief test same implementation
///
void program::rpn_same(void) { rpn_eq(); }
