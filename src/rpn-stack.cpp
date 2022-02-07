#include "program.hpp"

/// @brief swap keyword implementation
///
void program::rpn_swap(void) {
    MIN_ARGUMENTS(2);
    rpnstack::copy_and_push_back(*_stack, _stack->size() - 1, _calc_stack);
    rpnstack::copy_and_push_back(*_stack, _stack->size() - 2, _calc_stack);
    (void)_stack->pop_back(2);
    rpnstack::copy_and_push_back(_calc_stack, _calc_stack.size() - 2, *_stack);
    rpnstack::copy_and_push_back(_calc_stack, _calc_stack.size() - 1, *_stack);
    _calc_stack.pop_back(2);
}

/// @brief drop keyword implementation
///
void program::rpn_drop(void) {
    MIN_ARGUMENTS(1);
    (void)_stack->pop_back();
}

/// @brief drop2 keyword implementation
///
void program::rpn_drop2(void) {
    MIN_ARGUMENTS(2);
    (void)_stack->pop_back(2);
}

/// @brief dropn keyword implementation
///
void program::rpn_dropn(void) {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    int args = (int)((number*)_stack->back())->_value.toLong();
    MIN_ARGUMENTS(args + 1);

    (void)_stack->pop_back(args + 1);
}

/// @brief erase / del keyword implementation
///
void program::rpn_erase(void) { (void)_stack->pop_back(_stack->size()); }

/// @brief dup keyword implementation
///
void program::rpn_dup(void) {
    MIN_ARGUMENTS(1);
    rpnstack::copy_and_push_back(*_stack, _stack->size() - 1, *_stack);
}

/// @brief dup2 keyword implementation
///
void program::rpn_dup2(void) {
    MIN_ARGUMENTS(2);
    rpnstack::copy_and_push_back(*_stack, _stack->size() - 2, *_stack);
    rpnstack::copy_and_push_back(*_stack, _stack->size() - 2, *_stack);
}

/// @brief dupn keyword implementation
///
void program::rpn_dupn(void) {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    int args = (int)((number*)_stack->back())->_value.toLong();
    MIN_ARGUMENTS(args + 1);
    _stack->pop_back();

    for (int i = 0; i < args; i++) rpnstack::copy_and_push_back(*_stack, _stack->size() - args, *_stack);
}

/// @brief pick keyword implementation
///
void program::rpn_pick(void) {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    unsigned int to_pick = (unsigned int)int(((number*)_stack->pop_back())->_value);

    // treat stack depth errors
    if ((to_pick == 0) || (to_pick > _stack->size())) {
        ERR_CONTEXT(ret_out_of_range);
        return;
    }

    rpnstack::copy_and_push_back(*_stack, _stack->size() - to_pick, *_stack);
}

/// @brief rot keyword implementation
///
void program::rpn_rot(void) {
    MIN_ARGUMENTS(3);

    rpnstack::copy_and_push_back(*_stack, _stack->size() - 3, _calc_stack);
    rpnstack::copy_and_push_back(*_stack, _stack->size() - 2, _calc_stack);
    rpnstack::copy_and_push_back(*_stack, _stack->size() - 1, _calc_stack);
    (void)_stack->pop_back(3);
    rpnstack::copy_and_push_back(_calc_stack, _calc_stack.size() - 2, *_stack);
    rpnstack::copy_and_push_back(_calc_stack, _calc_stack.size() - 1, *_stack);
    rpnstack::copy_and_push_back(_calc_stack, _calc_stack.size() - 3, *_stack);
    _calc_stack.pop_back(3);
}

/// @brief depth keyword implementation
///
void program::rpn_depth(void) { _stack->push_back(new number(_stack->size())); }

/// @brief roll keyword implementation
///
void program::rpn_roll(void) {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    int args = (int)((number*)_stack->back())->_value.toLong();
    MIN_ARGUMENTS(args + 1);
    _stack->pop_back();

    for (int i = 0; i < args; i++) {
        rpnstack::copy_and_push_back(*_stack, _stack->size() - 1, _calc_stack);
        (void)_stack->pop_back();
    }

    for (int i = 1; i < args; i++) rpnstack::copy_and_push_back(_calc_stack, args - 1 - i, *_stack);
    rpnstack::copy_and_push_back(_calc_stack, args - 1, *_stack);

    _calc_stack.pop_back(args);
}

/// @brief rolld keyword implementation
///
void program::rpn_rolld(void) {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    int args = (int)((number*)_stack->back())->_value.toLong();
    MIN_ARGUMENTS(args + 1);
    _stack->pop_back();

    for (int i = 0; i < args; i++) {
        rpnstack::copy_and_push_back(*_stack, _stack->size() - 1, _calc_stack);
        (void)_stack->pop_back();
    }

    rpnstack::copy_and_push_back(_calc_stack, _calc_stack.size() - args, *_stack);

    for (int i = 1; i < args; i++) rpnstack::copy_and_push_back(_calc_stack, _calc_stack.size() - i, *_stack);

    _calc_stack.pop_back(args);
}

/// @brief over keyword implementation
///
void program::rpn_over(void) {
    MIN_ARGUMENTS(2);

    rpnstack::copy_and_push_back(*_stack, _stack->size() - 2, *_stack);
}
