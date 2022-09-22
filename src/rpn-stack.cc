// Copyright (c) 2014-2022 Louis Rubet

#include "program.h"

/// @brief swap keyword implementation
///
void Program::RpnSwap(void) {
    MIN_ARGUMENTS(2);
    Object* tmp = stack_.front();
    stack_.erase(0, 1, false);
    stack_.insert(stack_.begin() + 1, tmp);
}

/// @brief drop keyword implementation
///
void Program::RpnDrop(void) {
    MIN_ARGUMENTS(1);
    stack_.pop();
}

/// @brief drop2 keyword implementation
///
void Program::RpnDrop2(void) {
    MIN_ARGUMENTS(2);
    stack_.erase(0, 2);
}

/// @brief dropn keyword implementation
///
void Program::RpnDropn(void) {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kNumber);

    size_t args = stack_.value<Number>(0).toULong();
    MIN_ARGUMENTS(args + 1);
    stack_.erase(0, args + 1);
}

/// @brief erase / del keyword implementation
///
void Program::RpnErase(void) { stack_.erase(0, stack_.size()); }

/// @brief dup keyword implementation
///
void Program::RpnDup(void) {
    MIN_ARGUMENTS(1);
    stack_.push_front(stack_.at(0)->Clone());
}

/// @brief dupn keyword implementation
///
void Program::RpnDupn(void) {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kNumber);

    size_t args = stack_.value<Number>(0).toULong();
    stack_.pop();
    if (args == 0) {
        ERROR_CONTEXT(kOutOfRange);
        return;
    }

    MIN_ARGUMENTS(args);
    for (size_t i = 0; i < args; i++) stack_.push_front(stack_.at(args - 1)->Clone());
}

/// @brief dup2 keyword implementation
///
void Program::RpnDup2(void) {
    MIN_ARGUMENTS(2);
    stack_.push_front(stack_.at(1)->Clone());
    stack_.push_front(stack_.at(1)->Clone());
}

/// @brief pick keyword implementation
///
void Program::RpnPick(void) {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kNumber);

    size_t to_pick = static_cast<int>(stack_.value<Number>(0).toLong());
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
void Program::RpnRot(void) {
    MIN_ARGUMENTS(3);
    Object* tmp = stack_.at(2);
    stack_.erase(2, 1, false);
    stack_.insert(stack_.begin(), tmp);
}

/// @brief depth keyword implementation
///
void Program::RpnDepth(void) { stack_.push_front(new Number((uint64_t)stack_.size())); }

/// @brief roll keyword implementation
///
void Program::RpnRoll(void) {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kNumber);

    size_t args = static_cast<int>(stack_.value<Number>(0).toLong());
    stack_.pop();
    MIN_ARGUMENTS(args);
    if (args == 0) {
        ERROR_CONTEXT(kOutOfRange);
        return;
    }

    Object* tmp = stack_.at(args - 1);
    stack_.erase(args - 1, 1, false);
    stack_.insert(stack_.begin(), tmp);
}

/// @brief rolld keyword implementation
///
void Program::RpnRolld(void) {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, kNumber);

    size_t args = static_cast<int>(stack_.value<Number>(0).toLong());
    stack_.pop();
    MIN_ARGUMENTS(args);
    if (args == 0) {
        ERROR_CONTEXT(kOutOfRange);
        return;
    }

    Object* tmp = stack_.at(0);
    stack_.erase(0, 1, false);
    stack_.insert(stack_.begin() + args - 1, tmp);
}

/// @brief over keyword implementation
///
void Program::RpnOver(void) {
    MIN_ARGUMENTS(2);
    stack_.push_front(stack_.at(1)->Clone());
}
