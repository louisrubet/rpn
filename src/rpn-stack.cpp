// Copyright (c) 2014-2022 Louis Rubet

#include "program.hpp"

/// @brief swap keyword implementation
///
void program::rpn_swap(void) {
    MIN_ARGUMENTS(2);
    Object* tmp = stack_.front();
    stack_.erase(0, 1, false);
    stack_.insert(stack_.begin() + 1, tmp);
}

/// @brief drop keyword implementation
///
void program::rpn_drop(void) {
    MIN_ARGUMENTS(1);
    stack_.pop();
}

/// @brief drop2 keyword implementation
///
void program::rpn_drop2(void) {
    MIN_ARGUMENTS(2);
    stack_.erase(0, 2);
}

/// @brief dropn keyword implementation
///
void program::rpn_dropn(void) {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kNumber);

    int args = static_cast<int>(stack_.value<Number>(0).toLong());
    MIN_ARGUMENTS(args + 1);
    stack_.erase(0, args + 1);
}

/// @brief erase / del keyword implementation
///
void program::rpn_erase(void) { stack_.erase(0, stack_.size()); }

/// @brief dup keyword implementation
///
void program::rpn_dup(void) {
    MIN_ARGUMENTS(1);
    stack_.push_front(stack_.at(0)->clone());
}

/// @brief dupn keyword implementation
///
void program::rpn_dupn(void) {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kNumber);

    int args = static_cast<int>(stack_.value<Number>(0).toLong());
    stack_.pop();

    MIN_ARGUMENTS(args);
    for (int i = 0; i < args; i++) stack_.push_front(stack_.at(args - 1)->clone());
}

/// @brief dup2 keyword implementation
///
void program::rpn_dup2(void) {
    MIN_ARGUMENTS(2);
    stack_.push_front(stack_.at(1)->clone());
    stack_.push_front(stack_.at(1)->clone());
}

/// @brief pick keyword implementation
///
void program::rpn_pick(void) {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kNumber);

    int to_pick = static_cast<int>(stack_.value<Number>(0).toLong());
    stack_.pop();

    // treat stack_ depth errors
    if ((to_pick == 0) || (to_pick > stack_.size())) {
        setErrorContext(kOutOfRange);
        return;
    }

    stack_.push_front(stack_.at(to_pick - 1)->clone());
}

/// @brief rot keyword implementation
///
void program::rpn_rot(void) {
    MIN_ARGUMENTS(3);
    Object* tmp = stack_.at(2);
    stack_.erase(2, 1, false);
    stack_.insert(stack_.begin(), tmp);
}

/// @brief depth keyword implementation
///
void program::rpn_depth(void) { stack_.push_front(new Number(stack_.size())); }

/// @brief roll keyword implementation
///
void program::rpn_roll(void) {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kNumber);

    int args = static_cast<int>(stack_.value<Number>(0).toLong());
    stack_.pop();
    MIN_ARGUMENTS(args);

    Object* tmp = stack_.at(args - 1);
    stack_.erase(args - 1, 1, false);
    stack_.insert(stack_.begin(), tmp);
}

/// @brief rolld keyword implementation
///
void program::rpn_rolld(void) {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, kNumber);

    int args = static_cast<int>(stack_.value<Number>(0).toLong());
    stack_.pop();
    MIN_ARGUMENTS(args);

    Object* tmp = stack_.at(0);
    stack_.erase(0, 1, false);
    stack_.insert(stack_.begin() + args - 1, tmp);
}

/// @brief over keyword implementation
///
void program::rpn_over(void) {
    MIN_ARGUMENTS(2);
    stack_.push_front(stack_.at(1)->clone());
}
