// Copyright (c) 2014-2022 Louis Rubet

#include "program.h"

/// @brief if keyword (branch) implementation
///
/// @param myobj the current branch object
/// @return size_t index of the next object to run in the current program
/// @return kStepOut next object to run in the current program is current + 1
/// @return kRtError something went wrong with preprocess, abort branch
///
size_t program::RpnIf(Branch& myobj) {
    // myobj.arg1 = 'if' condition evaluation value
    MIN_ARGUMENTS_RET(1, kRtError);
    ARG_MUST_BE_OF_TYPE_RET(0, kNumber, kRtError);

    if (stack_.value<Number>(0) != 0)
        myobj.arg1 = 1;
    else
        myobj.arg1 = 0;
    stack_.pop();
    return kStepOut;
}

/// @brief then keyword (branch) implementation
///
/// @param myobj the current branch object
/// @return size_t index of the next object to run in the current program
/// @return kStepOut next object to run in the current program is current + 1
/// @return kRtError something went wrong with preprocess, abort Branch
///
size_t program::RpnThen(Branch& myobj) {
    // myobj.arg1 = index of then + 1
    // myobj.arg2 = index of else + 1 or end + 1
    // myobj.arg3 = index of if
    // if condition is true -> arg1 (= jump to then + 1)
    // else -> arg2 (= jump to else + 1 or end + 1)
    Branch* if_cmd;
    if (myobj.arg3 >= size() || at(myobj.arg3)->_type != kBranch) {
        ERROR_CONTEXT(kMissingOperand);
        return kRtError;
    }
    if_cmd = reinterpret_cast<Branch*>(at(myobj.arg3));
    if (if_cmd->arg1 == 1)
        return myobj.arg1;
    else
        return myobj.arg2;
}

/// @brief else keyword (branch) implementation
///
/// @param myobj the current branch object
/// @return size_t index of the next object to run in the current program
/// @return kStepOut next object to run in the current program is current + 1
/// @return kRtError something went wrong with preprocess, abort branch
///
size_t program::RpnElse(Branch& myobj) {
    // myobj.arg1 = index of else + 1
    // myobj.arg2 = index of end + 1
    // myobj.arg3 = index of if
    // if condition was false -> arg1 (= jump to else + 1)
    // if condition was true -> arg2 (= jump to end + 1)
    Branch* if_cmd;
    if (myobj.arg3 >= size() || at(myobj.arg3)->_type != kBranch) {
        ERROR_CONTEXT(kMissingOperand);
        return kRtError;
    }
    if_cmd = reinterpret_cast<Branch*>(at(myobj.arg3));
    if (if_cmd->arg1 == 1)
        return myobj.arg2;
    else
        return myobj.arg1;
}

/// @brief end keyword (branch) implementation
///
/// @param myobj the current branch object
/// @return size_t index of the next object to run in the current program
/// @return kStepOut next object to run in the current program is current + 1
/// @return kRtError something went wrong with preprocess, abort branch
///
size_t program::RpnEnd(Branch& myobj) {
    size_t ret = kStepOut;

    // arg1 = index of do+1 in case of do..unti..end
    if (myobj.arg1 != kStepOut) {
        // in a template do..until..end
        MIN_ARGUMENTS_RET(1, kRtError);
        ARG_MUST_BE_OF_TYPE_RET(0, kNumber, kRtError);

        // check arg
        if (stack_.value<Number>(0) == 0) ret = myobj.arg1;
        stack_.pop();
    } else if (myobj.arg2 != kStepOut) {
        // arg2 = index of while+1 in case of while..repeat..end
        ret = myobj.arg2;
    }

    return ret;
}

/// @brief do keyword (branch) implementation
///
/// @param myobj the current branch object
/// @return size_t index of the next object to run in the current program
/// @return kStepOut next object to run in the current program is current + 1
/// @return kRtError something went wrong with preprocess, abort branch
///
size_t program::RpnDo(Branch& myobj __attribute__((unused))) {
    // nothing
    return kStepOut;
}

/// @brief until keyword (branch) implementation
///
/// @param myobj the current branch object
/// @return size_t index of the next object to run in the current program
/// @return kStepOut next object to run in the current program is current + 1
/// @return kRtError something went wrong with preprocess, abort Branch
///
size_t program::RpnUntil(Branch& myobj __attribute__((unused))) {
    // nothing
    return kStepOut;
}

/// @brief ift keyword (branch) implementation
///
/// @param myobj the current branch object
/// @return size_t index of the next object to run in the current program
/// @return kStepOut next object to run in the current program is current + 1
/// @return kRtError something went wrong with preprocess, abort branch
///
void program::RpnIft(void) {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(1, kNumber);

    // check ift arg
    // arg is true if Number != 0 or if is nan or +/-inf
    if (stack_.value<Number>(1) != 0)
        stack_.erase(1);
    else
        stack_.erase(0, 2);
}

/// @brief ifte keyword (branch) implementation
///
/// @param myobj the current branch object
/// @return size_t index of the next object to run in the current program
/// @return kStepOut next object to run in the current program is current + 1
/// @return kRtError something went wrong with preprocess, abort branch
///
void program::RpnIfte(void) {
    MIN_ARGUMENTS(3);
    ARG_MUST_BE_OF_TYPE(2, kNumber);

    // check ifte arg
    if (stack_.value<Number>(2) != 0) {
        stack_.erase(2);
        stack_.pop();
    } else {
        stack_.erase(2);
        stack_.erase(1);
    }
}

/// @brief while keyword (branch) implementation
///
/// @param myobj the current branch object
/// @return size_t index of the next object to run in the current program
/// @return kStepOut next object to run in the current program is current + 1
/// @return kRtError something went wrong with preprocess, abort branch
///
size_t program::RpnWhile(Branch& myobj __attribute__((unused))) {
    // nothing
    return kStepOut;
}

/// @brief repeat keyword (branch) implementation
///
/// @param myobj the current branch object
/// @return size_t index of the next object to run in the current program
/// @return kStepOut next object to run in the current program is current + 1
/// @return kRtError something went wrong with preprocess, abort branch
///
size_t program::RpnRepeat(Branch& myobj) {
    size_t ret = kStepOut;

    MIN_ARGUMENTS_RET(1, kRtError);
    ARG_MUST_BE_OF_TYPE_RET(0, kNumber, kRtError);

    // check arg
    // myobj.arg1 is end+1
    if (stack_.value<Number>(0) == 0) ret = myobj.arg1;
    stack_.pop();

    return ret;
}

/// @brief start keyword (branch) implementation
///
/// @param myobj the current branch object
/// @return size_t index of the next object to run in the current program
/// @return kStepOut next object to run in the current program is current + 1
/// @return kRtError something went wrong with preprocess, abort branch
///
size_t program::RpnStart(Branch& myobj) {
    size_t ret = kStepOut;

    MIN_ARGUMENTS_RET(2, kRtError);
    ARG_MUST_BE_OF_TYPE_RET(0, kNumber, kRtError);
    ARG_MUST_BE_OF_TYPE_RET(1, kNumber, kRtError);

    // loop boundaries
    myobj.first_index = stack_.value<Number>(1);
    myobj.last_index = stack_.value<Number>(0);
    stack_.erase(0, 2);

    // test value
    if (myobj.first_index > myobj.last_index)
        // last boundary lower than first boundary
        // -> next command shall be after 'next'
        // arg2 holds index of 'next'
        ret = myobj.arg2 + 1;

    return ret;
}

/// @brief for keyword (branch) implementation
///
/// @param myobj the current branch object
/// @return size_t index of the next object to run in the current program
/// @return kStepOut next object to run in the current program is current + 1
/// @return kRtError something went wrong with preprocess, abort branch
///
size_t program::RpnFor(Branch& myobj) {
    size_t ret;

    MIN_ARGUMENTS_RET(2, kRtError);
    ARG_MUST_BE_OF_TYPE_RET(0, kNumber, kRtError);
    ARG_MUST_BE_OF_TYPE_RET(1, kNumber, kRtError);

    Symbol* sym;
    if (myobj.arg1 >= size() || at(myobj.arg1)->_type != kSymbol) {
        ERROR_CONTEXT(kMissingOperand);
        return kRtError;
    }
    sym = reinterpret_cast<Symbol*>(at(myobj.arg1));  // arg1 = loop variable index

    // loop boundaries
    myobj.first_index = stack_.value<Number>(1);
    myobj.last_index = stack_.value<Number>(0);

    // test value
    if (myobj.first_index > myobj.last_index) {
        // last boundary lower than first boundary
        // -> next command shall be after 'next'
        // arg2 holds index of 'next'
        ret = myobj.arg2 + 1;
    } else {
        // store symbol with first value
        auto it = local_heap_.find(sym->value);
        if (it != local_heap_.end()) {
            delete it->second;
            local_heap_.erase(it);
        }
        local_heap_[sym->value] = stack_.obj<Number>(1).Clone();
        ret = myobj.arg1 + 1;
    }

    stack_.erase(0, 2);

    return ret;
}

/// @brief next keyword (branch) implementation
///
/// @param myobj the current branch object
/// @return size_t index of the next object to run in the current program
/// @return kStepOut next object to run in the current program is current + 1
/// @return kRtError something went wrong with preprocess, abort branch
///
size_t program::RpnNext(Branch& myobj) {
    // arg1 = loop variable index
    // first_index = current point in the loop
    Branch* start_or_for;
    if (myobj.arg1 >= size() || at(myobj.arg1)->_type != kBranch) {
        ERROR_CONTEXT(kMissingOperand);
        return kRtError;
    }
    start_or_for = reinterpret_cast<Branch*>(at(myobj.arg1));
    if (!myobj.arg_bool) {
        myobj.arg_bool = true;
        myobj.first_index = start_or_for->first_index;
    }

    // increment then test
    // carefull: round toward minus infinity to avoid missing last boundary (because growing step)
    mpfr_add(myobj.first_index.mpfr_ptr(), myobj.first_index.mpfr_srcptr(), mpreal(1).mpfr_srcptr(), MPFR_RNDD);

    // for command: increment symbol too
    if (start_or_for->arg1 != kStepOut) {
        Symbol* var;
        if (start_or_for->arg1 >= size() || at(start_or_for->arg1)->_type != kSymbol) {
            ERROR_CONTEXT(kMissingOperand);
            return kRtError;
        }
        var = reinterpret_cast<Symbol*>(at(start_or_for->arg1));

        // store symbol variable (asserted existing in the local heap)
        reinterpret_cast<Number*>(local_heap_[var->value])->value = myobj.first_index;
    }

    // test value
    if (myobj.first_index > start_or_for->last_index) {
        // end of loop
        myobj.arg_bool = false;  // init again next time
        return kStepOut;
    } else {
        // for command: next instruction will be after symbol variable
        if (start_or_for->arg1 != kStepOut) return start_or_for->arg1 + 1;
        // start command: next instruction will be after start command
        else
            return myobj.arg1 + 1;
    }
}

/// @brief step keyword (branch) implementation
///
/// @param myobj the current branch object
/// @return size_t index of the next object to run in the current program
/// @return kStepOut next object to run in the current program is current + 1
/// @return kRtError something went wrong with preprocess, abort branch
///
size_t program::RpnStep(Branch& myobj) {
    size_t ret;
    MIN_ARGUMENTS_RET(1, kRtError);
    ARG_MUST_BE_OF_TYPE_RET(0, kNumber, kRtError);

    mpreal step = stack_.value<Number>(0);
    stack_.pop();

    // end of loop if step is negative or zero
    if (step <= 0) {
        ret = kStepOut;
    } else {
        // arg1 = loop variable index
        // first_index = current count
        Branch* start_or_for;
        if (myobj.arg1 >= size() || at(myobj.arg1)->_type != kBranch) {
            ERROR_CONTEXT(kMissingOperand);
            return kRtError;
        }
        start_or_for = reinterpret_cast<Branch*>(at(myobj.arg1));
        if (!myobj.arg_bool) {
            myobj.arg_bool = true;
            myobj.first_index = start_or_for->first_index;
        }

        // increment then test
        // carefull: round toward minus infinity to avoid missing last boundary (because growing step)
        mpfr_add(myobj.first_index.mpfr_ptr(), myobj.first_index.mpfr_srcptr(), step.mpfr_srcptr(), MPFR_RNDD);

        if (start_or_for->arg1 != kStepOut) {
            Symbol* var;

            // for command: increment symbol too
            if (start_or_for->arg1 >= size() || at(start_or_for->arg1)->_type != kSymbol) {
                ERROR_CONTEXT(kMissingOperand);
                return kRtError;
            }
            var = reinterpret_cast<Symbol*>(at(start_or_for->arg1));
            // increase symbol variable
            reinterpret_cast<Number*>(local_heap_[var->value])->value = myobj.first_index;
        }

        // test loop value is out of range
        if (myobj.first_index > start_or_for->last_index) {
            // end of loop
            myobj.arg_bool = false;  // init again next time
            ret = kStepOut;
        } else {
            // for command: next instruction will be after symbol variable
            if (start_or_for->arg1 != kStepOut) ret = start_or_for->arg1 + 1;
            // start command: next instruction will be after start command
            else
                ret = myobj.arg1 + 1;
        }
    }

    return ret;
}
