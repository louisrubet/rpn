#include "program.hpp"

/// @brief compared 2 strings on top of the stack
/// 
/// @return 0 strings are equal
/// @return !0 strings are not equal (see strcmp output)
///
long program::cmp_strings_on_stack_top() {
    // _stack sould have 2 strings at level 1 and 2
    // this function removes these 2 entries
    long res = (long)static_cast<ostring*>(_stack->at(0))->value.compare(static_cast<ostring*>(_stack->at(1))->value);
    (void)_stack->pop_front();
    (void)_stack->pop_front();
    return res;
}

/// @brief > keyword implementation
///
void program::rpn_sup(void) {
    MIN_ARGUMENTS(2);

    // numbers
    if (_stack->at(0)->_type == cmd_number && _stack->at(1)->_type == cmd_number) {
        number* right = (number*)_stack->pop_front();
        number* left = (number*)_stack->back();

        if (mpfr_cmp(left->value.mpfr_ptr(), right->value.mpfr_ptr()) > 0)
            mpfr_set_si(left->value.mpfr_ptr(), 1, mpreal::get_default_rnd());
        else
            mpfr_set_si(left->value.mpfr_ptr(), 0, mpreal::get_default_rnd());
    }
    // strings
    else if (_stack->at(0)->_type == cmd_string && _stack->at(1)->_type == cmd_string)
        _stack->push_front(new number(cmp_strings_on_stack_top()));
    else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief >= keyword implementation
///
void program::rpn_sup_eq(void) {
    MIN_ARGUMENTS(2);

    // numbers
    if (_stack->at(0)->_type == cmd_number && _stack->at(1)->_type == cmd_number) {
        number* right = (number*)_stack->pop_front();
        number* left = (number*)_stack->back();

        if (mpfr_cmp(left->value.mpfr_ptr(), right->value.mpfr_ptr()) >= 0)
            mpfr_set_si(left->value.mpfr_ptr(), 1, mpreal::get_default_rnd());
        else
            mpfr_set_si(left->value.mpfr_ptr(), 0, mpreal::get_default_rnd());
    }
    // strings
    else if (_stack->at(0)->_type == cmd_string && _stack->at(1)->_type == cmd_string)
        _stack->push_front(new number(cmp_strings_on_stack_top()));
    else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief < keyword implementation
///
void program::rpn_inf(void) {
    MIN_ARGUMENTS(2);

    // numbers
    if (_stack->at(0)->_type == cmd_number && _stack->at(1)->_type == cmd_number) {
        number* right = (number*)_stack->pop_front();
        number* left = (number*)_stack->back();

        if (mpfr_cmp(left->value.mpfr_ptr(), right->value.mpfr_ptr()) < 0)
            mpfr_set_si(left->value.mpfr_ptr(), 1, mpreal::get_default_rnd());
        else
            mpfr_set_si(left->value.mpfr_ptr(), 0, mpreal::get_default_rnd());
    }
    // strings
    else if (_stack->at(0)->_type == cmd_string && _stack->at(1)->_type == cmd_string)
        _stack->push_front(new number(cmp_strings_on_stack_top()));
    else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief <= keyword implementation
///
void program::rpn_inf_eq(void) {
    MIN_ARGUMENTS(2);

    // numbers
    if (_stack->at(0)->_type == cmd_number && _stack->at(1)->_type == cmd_number) {
        number* right = (number*)_stack->pop_front();
        number* left = (number*)_stack->back();

        if (mpfr_cmp(left->value.mpfr_ptr(), right->value.mpfr_ptr()) <= 0)
            mpfr_set_si(left->value.mpfr_ptr(), 1, mpreal::get_default_rnd());
        else
            mpfr_set_si(left->value.mpfr_ptr(), 0, mpreal::get_default_rnd());
    }
    // strings
    else if (_stack->at(0)->_type == cmd_string && _stack->at(1)->_type == cmd_string)
        _stack->push_front(new number(cmp_strings_on_stack_top()));
    else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief != keyword implementation
///
void program::rpn_diff(void) {
    MIN_ARGUMENTS(2);

    // numbers
    if (_stack->at(0)->_type == cmd_number && _stack->at(1)->_type == cmd_number) {
        number* right = (number*)_stack->pop_front();
        number* left = (number*)_stack->back();

        if (mpfr_cmp(left->value.mpfr_ptr(), right->value.mpfr_ptr()) != 0)
            mpfr_set_si(left->value.mpfr_ptr(), 1, mpreal::get_default_rnd());
        else
            mpfr_set_si(left->value.mpfr_ptr(), 0, mpreal::get_default_rnd());
    }
    // complexes
    else if (_stack->at(0)->_type == cmd_complex && _stack->at(1)->_type == cmd_complex) {
        ocomplex* right = (ocomplex*)_stack->pop_front();
        ocomplex* left = (ocomplex*)_stack->pop_front();
        _stack->push_front(new number(left->value != right->value?1L:0L));
    }
    // strings
    else if (_stack->at(0)->_type == cmd_string && _stack->at(1)->_type == cmd_string)
        _stack->push_front(new number(cmp_strings_on_stack_top()));
    else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief == keyword implementation
///
void program::rpn_eq(void) {
    MIN_ARGUMENTS(2);

    // numbers
    if (_stack->at(0)->_type == cmd_number && _stack->at(1)->_type == cmd_number) {
        number* right = (number*)_stack->pop_front();
        number* left = (number*)_stack->back();

        if (mpfr_cmp(left->value.mpfr_ptr(), right->value.mpfr_ptr()) == 0)
            mpfr_set_si(left->value.mpfr_ptr(), 1, mpreal::get_default_rnd());
        else
            mpfr_set_si(left->value.mpfr_ptr(), 0, mpreal::get_default_rnd());
    }
    // complexes
    else if (_stack->at(0)->_type == cmd_complex && _stack->at(1)->_type == cmd_complex) {
        ocomplex* right = (ocomplex*)_stack->pop_front();
        ocomplex* left = (ocomplex*)_stack->pop_front();
        _stack->push_front(new number(left->value == right->value?1L:0L));
    }
    // strings
    else if (_stack->at(0)->_type == cmd_string && _stack->at(1)->_type == cmd_string)
        _stack->push_front(new number(cmp_strings_on_stack_top()));
    else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief and keyword implementation
///
void program::rpn_test_and(void) {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ARG_MUST_BE_OF_TYPE(1, cmd_number);

    number* right = (number*)_stack->pop_front();
    number* left = (number*)_stack->back();

    if ((mpfr_cmp_si(left->value.mpfr_ptr(), 0) != 0) && (mpfr_cmp_si(right->value.mpfr_ptr(), 0) != 0))
        mpfr_set_si(left->value.mpfr_ptr(), 1, mpreal::get_default_rnd());
    else
        mpfr_set_si(left->value.mpfr_ptr(), 0, mpreal::get_default_rnd());
}

/// @brief or keyword implementation
///
void program::rpn_test_or(void) {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ARG_MUST_BE_OF_TYPE(1, cmd_number);

    number* right = (number*)_stack->pop_front();
    number* left = (number*)_stack->back();

    if ((mpfr_cmp_si(left->value.mpfr_ptr(), 0) != 0) || (mpfr_cmp_si(right->value.mpfr_ptr(), 0) != 0))
        mpfr_set_si(left->value.mpfr_ptr(), 1, mpreal::get_default_rnd());
    else
        mpfr_set_si(left->value.mpfr_ptr(), 0, mpreal::get_default_rnd());
}

/// @brief xor keyword implementation
///
void program::rpn_test_xor(void) {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ARG_MUST_BE_OF_TYPE(1, cmd_number);

    number* right = (number*)_stack->pop_front();
    number* left = (number*)_stack->back();

    if (mpfr_cmp_si(left->value.mpfr_ptr(), 0) == 0) {
        if (mpfr_cmp_si(right->value.mpfr_ptr(), 0) != 0)
            mpfr_set_si(left->value.mpfr_ptr(), 1, mpreal::get_default_rnd());
        else
            mpfr_set_si(left->value.mpfr_ptr(), 0, mpreal::get_default_rnd());
    } else {
        if (mpfr_cmp_si(right->value.mpfr_ptr(), 0) == 0)
            mpfr_set_si(left->value.mpfr_ptr(), 1, mpreal::get_default_rnd());
        else
            mpfr_set_si(left->value.mpfr_ptr(), 0, mpreal::get_default_rnd());
    }
}

/// @brief not keyword implementation
///
void program::rpn_test_not(void) {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    number* left = (number*)_stack->back();
    if (mpfr_cmp_si(left->value.mpfr_ptr(), 0) == 0)
        mpfr_set_si(left->value.mpfr_ptr(), 1, mpreal::get_default_rnd());
    else
        mpfr_set_si(left->value.mpfr_ptr(), 0, mpreal::get_default_rnd());
}

/// @brief test same implementation
///
void program::rpn_same(void) { rpn_eq(); }
