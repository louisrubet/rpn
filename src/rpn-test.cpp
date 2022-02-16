#include "program.hpp"

/// @brief compared 2 strings on top of the stack
///
/// @return 0 strings are equal
/// @return !0 strings are not equal (see strcmp output)
///
long program::cmp_strings_on_stack_top() {
    // _stack should have 2 strings at level 1 and 2
    // this function removes these 2 entries
    long res = (long)static_cast<ostring*>(_stack->at(1))->value.compare(static_cast<ostring*>(_stack->at(0))->value);
    (void)_stack->pop_front(2);
    return res;
}

/// @brief > keyword implementation
///
void program::rpn_sup(void) {
    MIN_ARGUMENTS(2);

    // numbers
    if (_stack->type(0) == cmd_number && _stack->type(1) == cmd_number) {
        _stack->push_front(new number(_stack->value<number>(1) > _stack->value<number>(0)));
        _stack->erase(1, 2);
    }
    // strings
    else if (_stack->type(0) == cmd_string && _stack->type(1) == cmd_string) {
        _stack->push_front(new number(cmp_strings_on_stack_top()));
        _stack->erase(1, 2);
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief >= keyword implementation
///
void program::rpn_sup_eq(void) {
    MIN_ARGUMENTS(2);

    // numbers
    if (_stack->type(0) == cmd_number && _stack->type(1) == cmd_number) {
        _stack->push_front(new number(_stack->value<number>(1) >= _stack->value<number>(0)));
        _stack->erase(1, 2);
    }
    // strings
    else if (_stack->type(0) == cmd_string && _stack->type(1) == cmd_string) {
        _stack->push_front(new number(cmp_strings_on_stack_top()));
        _stack->erase(1, 2);
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief < keyword implementation
///
void program::rpn_inf(void) {
    MIN_ARGUMENTS(2);

    // numbers
    if (_stack->type(0) == cmd_number && _stack->type(1) == cmd_number) {
        _stack->push_front(new number(_stack->value<number>(1) < _stack->value<number>(0)));
        _stack->erase(1, 2);
    }
    // strings
    else if (_stack->type(0) == cmd_string && _stack->type(1) == cmd_string) {
        _stack->push_front(new number(cmp_strings_on_stack_top()));
        _stack->erase(1, 2);
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief <= keyword implementation
///
void program::rpn_inf_eq(void) {
    // numbers
    if (_stack->type(0) == cmd_number && _stack->type(1) == cmd_number) {
        _stack->push_front(new number(_stack->value<number>(1) <= _stack->value<number>(0)));
        _stack->erase(1, 2);
    }
    // strings
    else if (_stack->type(0) == cmd_string && _stack->type(1) == cmd_string) {
        _stack->push_front(new number(cmp_strings_on_stack_top()));
        _stack->erase(1, 2);
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief != keyword implementation
///
void program::rpn_diff(void) {
    MIN_ARGUMENTS(2);

    // numbers
    if (_stack->type(0) == cmd_number && _stack->type(1) == cmd_number) {
        _stack->push_front(new number(_stack->value<number>(1) != _stack->value<number>(0)));
        _stack->erase(1, 2);
    }
    // complexes
    else if (_stack->type(0) == cmd_complex && _stack->type(1) == cmd_complex) {
        _stack->push_front(new number(_stack->value<ocomplex>(1) != _stack->value<ocomplex>(0)));
        _stack->erase(1, 2);
    }
    // strings
    else if (_stack->type(0) == cmd_string && _stack->type(1) == cmd_string) {
        _stack->push_front(new number(cmp_strings_on_stack_top()));
        _stack->erase(1, 2);
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief == keyword implementation
///
void program::rpn_eq(void) {
    MIN_ARGUMENTS(2);

    // numbers
    if (_stack->type(0) == cmd_number && _stack->type(1) == cmd_number) {
        _stack->push_front(new number(_stack->value<number>(1) == _stack->value<number>(0)));
        _stack->erase(1, 2);
    }
    // complexes
    else if (_stack->type(0) == cmd_complex && _stack->type(1) == cmd_complex) {
        _stack->push_front(new number(_stack->value<ocomplex>(1) == _stack->value<ocomplex>(0)));
        _stack->erase(1, 2);
    }
    // strings
    else if (_stack->type(0) == cmd_string && _stack->type(1) == cmd_string) {
        _stack->push_front(new number(cmp_strings_on_stack_top()));
        _stack->erase(1, 2);
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief and keyword implementation
///
void program::rpn_test_and(void) {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ARG_MUST_BE_OF_TYPE(1, cmd_number);

    if (_stack->value<number>(0) != 0 && _stack->value<number>(1) != 0)
        _stack->push(new number(1));
    else
        _stack->push(new number(0));
    _stack->erase(1, 2);
}

/// @brief or keyword implementation
///
void program::rpn_test_or(void) {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ARG_MUST_BE_OF_TYPE(1, cmd_number);

    if (_stack->value<number>(0) != 0 || _stack->value<number>(1) != 0)
        _stack->push(new number(1));
    else
        _stack->push(new number(0));
    _stack->erase(1, 2);
}

/// @brief xor keyword implementation
///
void program::rpn_test_xor(void) {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ARG_MUST_BE_OF_TYPE(1, cmd_number);

    if (_stack->value<number>(0) != 0 ^ _stack->value<number>(1) != 0)
        _stack->push(new number(1));
    else
        _stack->push(new number(0));
    _stack->erase(1, 2);
}

/// @brief not keyword implementation
///
void program::rpn_test_not(void) {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    _stack->push(new number(_stack->value<number>(0) == 0?1:0));
    _stack->erase(1, 1);
}

/// @brief test same implementation
///
void program::rpn_same(void) { rpn_eq(); }
