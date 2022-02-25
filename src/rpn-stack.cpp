// Copyright (c) 2014-2022 Louis Rubet

#include "program.hpp"

/// @brief swap keyword implementation
///
void program::RpnSwap(void) {
    MIN_ARGUMENTS(2);
    Object* tmp = stack_.front();
    stack_.erase(0, 1, false);
    stack_.insert(stack_.begin() + 1, tmp);
}

/// @brief drop keyword implementation
///
void program::RpnDrop(void) {
    MIN_ARGUMENTS(1);
    stack_.pop();
}

/// @brief drop2 keyword implementation
///
void program::RpnDrop2(void) {
    MIN_ARGUMENTS(2);
    stack_.erase(0, 2);
}

/// @brief dropn keyword implementation
///
void program::RpnDropn(void) {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kNumber);

    int args = static_cast<int>(stack_.value<Number>(0).toLong());
    MIN_ARGUMENTS(args + 1);
    stack_.erase(0, args + 1);
}

/// @brief erase / del keyword implementation
///
void program::RpnErase(void) { stack_.erase(0, stack_.size()); }

/// @brief dup keyword implementation
///
void program::RpnDup(void) {
    MIN_ARGUMENTS(1);
    stack_.push_front(stack_.at(0)->Clone());
}

/// @brief dupn keyword implementation
///
void program::RpnDupn(void) {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kNumber);

    int args = static_cast<int>(stack_.value<Number>(0).toLong());
    stack_.pop();

    MIN_ARGUMENTS(args);
    for (int i = 0; i < args; i++) stack_.push_front(stack_.at(args - 1)->Clone());
}

/// @brief dup2 keyword implementation
///
void program::RpnDup2(void) {
    MIN_ARGUMENTS(2);
    stack_.push_front(stack_.at(1)->Clone());
    stack_.push_front(stack_.at(1)->Clone());
}

/// @brief pick keyword implementation
///
void program::RpnPick(void) {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kNumber);

    int to_pick = static_cast<int>(stack_.value<Number>(0).toLong());
    stack_.pop();

    // treat stack_ depth errors
    if ((to_pick == 0) || (to_pick > stack_.size())) {
        ERROR_CONTEXT(kOutOfRange);
        return;
    }

    stack_.push_front(stack_.at(to_pick - 1)->Clone());
}

/// @brief rot keyword implementation
///
void program::RpnRot(void) {
    MIN_ARGUMENTS(3);
    Object* tmp = stack_.at(2);
    stack_.erase(2, 1, false);
    stack_.insert(stack_.begin(), tmp);
}

/// @brief depth keyword implementation
///
void program::RpnDepth(void) { stack_.push_front(new Number(stack_.size())); }

/// @brief roll keyword implementation
///
void program::RpnRoll(void) {
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
void program::RpnRolld(void) {
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
void program::RpnOver(void) {
    MIN_ARGUMENTS(2);
    stack_.push_front(stack_.at(1)->Clone());
}
