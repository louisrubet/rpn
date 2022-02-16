#include "program.hpp"

/// @brief swap keyword implementation
///
void program::rpn_swap(void) {
    MIN_ARGUMENTS(2);
    rpnstack::copy_and_push_front(*_stack, 1, *_stack);
    _stack->erase(2);
}

/// @brief drop keyword implementation
///
void program::rpn_drop(void) {
    MIN_ARGUMENTS(1);
    _stack->pop();
}

/// @brief drop2 keyword implementation
///
void program::rpn_drop2(void) {
    MIN_ARGUMENTS(2);
    _stack->pop_front(2);
}

/// @brief dropn keyword implementation
///
void program::rpn_dropn(void) {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    int args = (int)_stack->value<number>(0).toLong();
    MIN_ARGUMENTS(args + 1);
    _stack->erase(0, args + 1);
}

/// @brief erase / del keyword implementation
///
void program::rpn_erase(void) { _stack->erase(0, _stack->size()); }

/// @brief dup keyword implementation
///
void program::rpn_dup(void) {
    MIN_ARGUMENTS(1);
    rpnstack::copy_and_push_front(*_stack, 0, *_stack);
}

/// @brief dupn keyword implementation
///
void program::rpn_dupn(void) {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    int args = (int)((number*)_stack->front())->value.toLong();
    _stack->pop();

    MIN_ARGUMENTS(args);
    for (int i = 0; i < args; i++) rpnstack::copy_and_push_front(*_stack, args - 1, *_stack);
}

/// @brief dup2 keyword implementation
///
void program::rpn_dup2(void) {
    MIN_ARGUMENTS(2);
    rpnstack::copy_and_push_front(*_stack, 1, *_stack);
    rpnstack::copy_and_push_front(*_stack, 1, *_stack);
}

/// @brief pick keyword implementation
///
void program::rpn_pick(void) {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    unsigned int to_pick = (unsigned int)_stack->value<number>(0);
    _stack->pop();

    // treat stack depth errors
    if ((to_pick == 0) || (to_pick > _stack->size())) {
        ERR_CONTEXT(ret_out_of_range);
        return;
    }

    rpnstack::copy_and_push_front(*_stack, to_pick - 1, *_stack);
}

/// @brief rot keyword implementation
///
void program::rpn_rot(void) {
    MIN_ARGUMENTS(3);

    rpnstack::copy_and_push_front(*_stack, 2, *_stack);
    _stack->erase(3);
}

/// @brief depth keyword implementation
///
void program::rpn_depth(void) { _stack->push_front(new number(_stack->size())); }

/// @brief roll keyword implementation
///
void program::rpn_roll(void) {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    size_t args = (int)((number*)_stack->front())->value;
    _stack->pop();
    MIN_ARGUMENTS(args);

    rpnstack::copy_and_push_front(*_stack, args - 1, *_stack);
    _stack->erase(args);
}

/// @brief rolld keyword implementation
///
void program::rpn_rolld(void) {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    int args = (int)((number*)_stack->front())->value.toLong();
    _stack->pop();
    MIN_ARGUMENTS(args);

    _stack->insert(_stack->begin()+args, _stack->at(0)->clone());
    _stack->pop();
}

/// @brief over keyword implementation
///
void program::rpn_over(void) {
    MIN_ARGUMENTS(2);

    rpnstack::copy_and_push_front(*_stack, 1, *_stack);
}
