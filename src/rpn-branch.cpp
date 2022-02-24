// Copyright (c) 2014-2022 Louis Rubet

#include "program.hpp"

/// @brief if keyword (branch) implementation
///
/// @param myobj the current branch object
/// @return size_t index of the next object to run in the current program
/// @return step_out next object to run in the current program is current + 1
/// @return runtime_error something went wrong with preprocess, abort branch
///
size_t program::rpn_if(branch& myobj) {
    // myobj.arg1 = 'if' condition evaluation value
    MIN_ARGUMENTS_RET(1, runtime_error);
    ARG_MUST_BE_OF_TYPE_RET(0, cmd_number, runtime_error);

    if (_stack.value<number>(0) != 0)
        myobj.arg1 = 1;
    else
        myobj.arg1 = 0;
    _stack.pop();
    return step_out;
}

/// @brief then keyword (branch) implementation
///
/// @param myobj the current branch object
/// @return size_t index of the next object to run in the current program
/// @return step_out next object to run in the current program is current + 1
/// @return runtime_error something went wrong with preprocess, abort branch
///
size_t program::rpn_then(branch& myobj) {
    // myobj.arg1 = index of then + 1
    // myobj.arg2 = index of else + 1 or end + 1
    // myobj.arg3 = index of if
    // if condition is true -> arg1 (= jump to then + 1)
    // else -> arg2 (= jump to else + 1 or end + 1)
    branch* if_cmd;
    if (myobj.arg3 >= size() || at(myobj.arg3)->_type != cmd_branch) {
        setErrorContext(ret_missing_operand);
        return runtime_error;
    }
    if_cmd = reinterpret_cast<branch*>(at(myobj.arg3));
    if (if_cmd->arg1 == 1)
        return myobj.arg1;
    else
        return myobj.arg2;
}

/// @brief else keyword (branch) implementation
///
/// @param myobj the current branch object
/// @return size_t index of the next object to run in the current program
/// @return step_out next object to run in the current program is current + 1
/// @return runtime_error something went wrong with preprocess, abort branch
///
size_t program::rpn_else(branch& myobj) {
    // myobj.arg1 = index of else + 1
    // myobj.arg2 = index of end + 1
    // myobj.arg3 = index of if
    // if condition was false -> arg1 (= jump to else + 1)
    // if condition was true -> arg2 (= jump to end + 1)
    branch* if_cmd;
    if (myobj.arg3 >= size() || at(myobj.arg3)->_type != cmd_branch) {
        setErrorContext(ret_missing_operand);
        return runtime_error;
    }
    if_cmd = reinterpret_cast<branch*>(at(myobj.arg3));
    if (if_cmd->arg1 == 1)
        return myobj.arg2;
    else
        return myobj.arg1;
}

/// @brief end keyword (branch) implementation
///
/// @param myobj the current branch object
/// @return size_t index of the next object to run in the current program
/// @return step_out next object to run in the current program is current + 1
/// @return runtime_error something went wrong with preprocess, abort branch
///
size_t program::rpn_end(branch& myobj) {
    size_t ret = step_out;

    // arg1 = index of do+1 in case of do..unti..end
    if (myobj.arg1 != -1) {
        // in a template do..until..end
        MIN_ARGUMENTS_RET(1, runtime_error);
        ARG_MUST_BE_OF_TYPE_RET(0, cmd_number, runtime_error);

        // check arg
        if (_stack.value<number>(0) == 0) ret = myobj.arg1;
        _stack.pop();
    } else if (myobj.arg2 != step_out) {
        // arg2 = index of while+1 in case of while..repeat..end
        ret = myobj.arg2;
    }

    return ret;
}

/// @brief do keyword (branch) implementation
///
/// @param myobj the current branch object
/// @return size_t index of the next object to run in the current program
/// @return step_out next object to run in the current program is current + 1
/// @return runtime_error something went wrong with preprocess, abort branch
///
size_t program::rpn_do(branch& myobj) {
    // nothing
    return step_out;
}

/// @brief until keyword (branch) implementation
///
/// @param myobj the current branch object
/// @return size_t index of the next object to run in the current program
/// @return step_out next object to run in the current program is current + 1
/// @return runtime_error something went wrong with preprocess, abort branch
///
size_t program::rpn_until(branch& myobj) {
    // nothing
    return step_out;
}

/// @brief ift keyword (branch) implementation
///
/// @param myobj the current branch object
/// @return size_t index of the next object to run in the current program
/// @return step_out next object to run in the current program is current + 1
/// @return runtime_error something went wrong with preprocess, abort branch
///
void program::rpn_ift(void) {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(1, cmd_number);

    // check ift arg
    // arg is true if number != 0 or if is nan or +/-inf
    if (_stack.value<number>(1) != 0)
        _stack.erase(1);
    else
        _stack.erase(0, 2);
}

/// @brief ifte keyword (branch) implementation
///
/// @param myobj the current branch object
/// @return size_t index of the next object to run in the current program
/// @return step_out next object to run in the current program is current + 1
/// @return runtime_error something went wrong with preprocess, abort branch
///
void program::rpn_ifte(void) {
    MIN_ARGUMENTS(3);
    ARG_MUST_BE_OF_TYPE(2, cmd_number);

    // check ifte arg
    if (_stack.value<number>(2) != 0) {
        _stack.erase(2);
        _stack.pop();
    } else {
        _stack.erase(2);
        _stack.erase(1);
    }
}

/// @brief while keyword (branch) implementation
///
/// @param myobj the current branch object
/// @return size_t index of the next object to run in the current program
/// @return step_out next object to run in the current program is current + 1
/// @return runtime_error something went wrong with preprocess, abort branch
///
size_t program::rpn_while(branch& myobj) {
    // nothing
    return step_out;
}

/// @brief repeat keyword (branch) implementation
///
/// @param myobj the current branch object
/// @return size_t index of the next object to run in the current program
/// @return step_out next object to run in the current program is current + 1
/// @return runtime_error something went wrong with preprocess, abort branch
///
size_t program::rpn_repeat(branch& myobj) {
    size_t ret = step_out;

    MIN_ARGUMENTS_RET(1, runtime_error);
    ARG_MUST_BE_OF_TYPE_RET(0, cmd_number, runtime_error);

    // check arg
    // myobj.arg1 is end+1
    if (_stack.value<number>(0) == 0) ret = myobj.arg1;
    _stack.pop();

    return ret;
}

/// @brief start keyword (branch) implementation
///
/// @param myobj the current branch object
/// @return size_t index of the next object to run in the current program
/// @return step_out next object to run in the current program is current + 1
/// @return runtime_error something went wrong with preprocess, abort branch
///
size_t program::rpn_start(branch& myobj) {
    size_t ret = step_out;

    MIN_ARGUMENTS_RET(2, runtime_error);
    ARG_MUST_BE_OF_TYPE_RET(0, cmd_number, runtime_error);
    ARG_MUST_BE_OF_TYPE_RET(1, cmd_number, runtime_error);

    // loop boundaries
    myobj.firstIndex = _stack.value<number>(1);
    myobj.lastIndex = _stack.value<number>(0);
    _stack.erase(0, 2);

    // test value
    if (myobj.firstIndex > myobj.lastIndex)
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
/// @return step_out next object to run in the current program is current + 1
/// @return runtime_error something went wrong with preprocess, abort branch
///
size_t program::rpn_for(branch& myobj) {
    size_t ret;

    MIN_ARGUMENTS_RET(2, runtime_error);
    ARG_MUST_BE_OF_TYPE_RET(0, cmd_number, runtime_error);
    ARG_MUST_BE_OF_TYPE_RET(1, cmd_number, runtime_error);

    symbol* sym;
    if (myobj.arg1 >= size() || at(myobj.arg1)->_type != cmd_symbol) {
        setErrorContext(ret_missing_operand);
        return runtime_error;
    }
    sym = reinterpret_cast<symbol*>(at(myobj.arg1));  // arg1 = loop variable index

    // loop boundaries
    myobj.firstIndex = _stack.value<number>(1);
    myobj.lastIndex = _stack.value<number>(0);

    // test value
    if (myobj.firstIndex > myobj.lastIndex) {
        // last boundary lower than first boundary
        // -> next command shall be after 'next'
        // arg2 holds index of 'next'
        ret = myobj.arg2 + 1;
    } else {
        // store symbol with first value
        auto it = _local_heap.find(sym->value);
        if (it != _local_heap.end()) {
            delete it->second;
            _local_heap.erase(it);
        }
        _local_heap[sym->value] = _stack.obj<number>(1).clone();
        ret = myobj.arg1 + 1;
    }

    _stack.erase(0, 2);

    return ret;
}

/// @brief next keyword (branch) implementation
///
/// @param myobj the current branch object
/// @return size_t index of the next object to run in the current program
/// @return step_out next object to run in the current program is current + 1
/// @return runtime_error something went wrong with preprocess, abort branch
///
size_t program::rpn_next(branch& myobj) {
    // arg1 = loop variable index
    // firstIndex = current point in the loop
    branch* start_or_for;
    if (myobj.arg1 >= size() || at(myobj.arg1)->_type != cmd_branch) {
        setErrorContext(ret_missing_operand);
        return runtime_error;
    }
    start_or_for = reinterpret_cast<branch*>(at(myobj.arg1));
    if (!myobj.arg_bool) {
        myobj.arg_bool = true;
        myobj.firstIndex = start_or_for->firstIndex;
    }

    // increment then test
    // carefull: round toward minus infinity to avoid missing last boundary (because growing step)
    mpfr_add(myobj.firstIndex.mpfr_ptr(), myobj.firstIndex.mpfr_srcptr(), mpreal(1).mpfr_srcptr(), MPFR_RNDD);

    // for command: increment symbol too
    if (start_or_for->arg1 != -1) {
        object* obj;
        symbol* var;
        if (start_or_for->arg1 >= size() || at(start_or_for->arg1)->_type != cmd_symbol) {
            setErrorContext(ret_missing_operand);
            return runtime_error;
        }
        var = reinterpret_cast<symbol*>(at(start_or_for->arg1));

        // store symbol variable (asserted existing in the local heap)
        reinterpret_cast<number*>(_local_heap[var->value])->value = myobj.firstIndex;
    }

    // test value
    if (myobj.firstIndex > start_or_for->lastIndex) {
        // end of loop
        myobj.arg_bool = false;  // init again next time
        return step_out;
    } else {
        // for command: next instruction will be after symbol variable
        if (start_or_for->arg1 != -1) return start_or_for->arg1 + 1;
        // start command: next instruction will be after start command
        else
            return myobj.arg1 + 1;
    }
}

/// @brief step keyword (branch) implementation
///
/// @param myobj the current branch object
/// @return size_t index of the next object to run in the current program
/// @return step_out next object to run in the current program is current + 1
/// @return runtime_error something went wrong with preprocess, abort branch
///
size_t program::rpn_step(branch& myobj) {
    size_t ret;
    MIN_ARGUMENTS_RET(1, runtime_error);
    ARG_MUST_BE_OF_TYPE_RET(0, cmd_number, runtime_error);

    mpreal step = _stack.value<number>(0);
    _stack.pop();

    // end of loop if step is negative or zero
    if (step <= 0) {
        ret = step_out;
    } else {
        // arg1 = loop variable index
        // firstIndex = current count
        branch* start_or_for;
        if (myobj.arg1 >= size() || at(myobj.arg1)->_type != cmd_branch) {
            setErrorContext(ret_missing_operand);
            return runtime_error;
        }
        start_or_for = reinterpret_cast<branch*>(at(myobj.arg1));
        if (!myobj.arg_bool) {
            myobj.arg_bool = true;
            myobj.firstIndex = start_or_for->firstIndex;
        }

        // increment then test
        // carefull: round toward minus infinity to avoid missing last boundary (because growing step)
        mpfr_add(myobj.firstIndex.mpfr_ptr(), myobj.firstIndex.mpfr_srcptr(), step.mpfr_srcptr(), MPFR_RNDD);

        if (start_or_for->arg1 != -1) {
            object* obj;
            symbol* var;

            // for command: increment symbol too
            if (start_or_for->arg1 >= size() || at(start_or_for->arg1)->_type != cmd_symbol) {
                setErrorContext(ret_missing_operand);
                return runtime_error;
            }
            var = reinterpret_cast<symbol*>(at(start_or_for->arg1));
            // increase symbol variable
            reinterpret_cast<number*>(_local_heap[var->value])->value = myobj.firstIndex;
        }

        // test loop value is out of range
        if (myobj.firstIndex > start_or_for->lastIndex) {
            // end of loop
            myobj.arg_bool = false;  // init again next time
            ret = step_out;
        } else {
            // for command: next instruction will be after symbol variable
            if (start_or_for->arg1 != -1) ret = start_or_for->arg1 + 1;
            // start command: next instruction will be after start command
            else
                ret = myobj.arg1 + 1;
        }
    }

    return ret;
}
