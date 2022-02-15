#include "program.hpp"

/// @brief if keyword (branch) implementation
///
/// @param myobj the current branch object
/// @return int index of the next object to run in the current program
/// @return -1 the next object index to run in the current program is the current+1
///
int program::rpn_if(branch& myobj) {
    // myobj.arg1 = 'if' condition evaluation value
    MIN_ARGUMENTS_RET(1, -(int)ret_runtime_error);
    ARG_MUST_BE_OF_TYPE_RET(0, cmd_number, -(int)ret_runtime_error);

    if (((number*)_stack->at(0))->value != 0)
        myobj.arg1 = 1;
    else
        myobj.arg1 = 0;
    _stack->pop_front();
    return -1;
}

/// @brief then keyword (branch) implementation
///
/// @param myobj the current branch object
/// @return int index of the next object to run in the current program
///
int program::rpn_then(branch& myobj) {
    // myobj.arg1 = index of then + 1
    // myobj.arg2 = index of else + 1 or end + 1
    // myobj.arg3 = index of if
    // if condition is true -> arg1 (= jump to then + 1)
    // else -> arg2 (= jump to else + 1 or end + 1)
    branch* if_cmd = (branch*)(*this)[myobj.arg3];
    if (if_cmd->arg1 == 1)
        return myobj.arg1;
    else
        return myobj.arg2;
}

/// @brief else keyword (branch) implementation
///
/// @param myobj the current branch object
/// @return int index of the next object to run in the current program
/// @return -1 the next object index to run in the current program is the current+1
///
int program::rpn_else(branch& myobj) {
    // myobj.arg1 = index of else + 1
    // myobj.arg2 = index of end + 1
    // myobj.arg3 = index of if
    // if condition was false -> arg1 (= jump to else + 1)
    // if condition was true -> arg2 (= jump to end + 1)
    branch* if_cmd = (branch*)(*this)[myobj.arg3];
    if (if_cmd->arg1 == 1)
        return myobj.arg2;
    else
        return myobj.arg1;
}

/// @brief end keyword (branch) implementation
///
/// @param myobj the current branch object
/// @return int index of the next object to run in the current program
/// @return -1 the next object index to run in the current program is the current+1
///
int program::rpn_end(branch& myobj) {
    int ret = -1;

    // arg1 = index of do+1 in case of do..unti..end
    if (myobj.arg1 != -1) {
        // in a template do..unti..end
        MIN_ARGUMENTS_RET(1, -(int)ret_runtime_error);
        ARG_MUST_BE_OF_TYPE_RET(0, cmd_number, -(int)ret_runtime_error);

        // check arg
        if (_stack->value<number>(0) == 0) ret = myobj.arg1;
        _stack->pop();
    }
    // arg2 = index of while+1 in case of while..repeat..end
    else if (myobj.arg2 != -1)
        ret = myobj.arg2;

    return ret;
}

/// @brief do keyword (branch) implementation
///
/// @param myobj the current branch object
/// @return int index of the next object to run in the current program
/// @return -1 the next object index to run in the current program is the current+1
///
int program::rpn_do(branch& myobj) {
    // nothing
    return -1;
}

/// @brief until keyword (branch) implementation
///
/// @param myobj the current branch object
/// @return int index of the next object to run in the current program
/// @return -1 the next object index to run in the current program is the current+1
///
int program::rpn_until(branch& myobj) {
    // nothing
    return -1;
}

/// @brief ift keyword (branch) implementation
///
/// @param myobj the current branch object
/// @return int index of the next object to run in the current program
/// @return -1 the next object index to run in the current program is the current+1
///
void program::rpn_ift(void) {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(1, cmd_number);

    // check ift arg
    // arg is true if number != 0 or if is nan or +/-inf
    number* testee = ((number*)_stack->at(1));

    if (testee->value != 0) {
        CHECK_MPFR(rpnstack::copy_and_push_front(*_stack, _stack->size() - 1, _calc_stack));
        _stack->pop_front(2);

        CHECK_MPFR(rpnstack::copy_and_push_front(_calc_stack, _calc_stack.size() - 1, *_stack));
        _calc_stack.pop_front();
    } else
        _stack->pop_front(2);
}

/// @brief ifte keyword (branch) implementation
///
/// @param myobj the current branch object
/// @return int index of the next object to run in the current program
/// @return -1 the next object index to run in the current program is the current+1
///
void program::rpn_ifte(void) {
    MIN_ARGUMENTS(3);
    ARG_MUST_BE_OF_TYPE(2, cmd_number);

    // check ifte arg
    // arg is true if number != 0 or if is nan or +/-inf
    number* testee = ((number*)_stack->at(2));

    if (testee->value != 0)
        CHECK_MPFR(rpnstack::copy_and_push_front(*_stack, _stack->size() - 2, _calc_stack));
    else
        CHECK_MPFR(rpnstack::copy_and_push_front(*_stack, _stack->size() - 1, _calc_stack));

    (void)_stack->pop_front(3);

    CHECK_MPFR(rpnstack::copy_and_push_front(_calc_stack, _calc_stack.size() - 1, *_stack));
    _calc_stack.pop_front();
}

/// @brief while keyword (branch) implementation
///
/// @param myobj the current branch object
/// @return int index of the next object to run in the current program
/// @return -1 the next object index to run in the current program is the current+1
///
int program::rpn_while(branch& myobj) {
    // nothing
    return -1;
}

/// @brief repeat keyword (branch) implementation
///
/// @param myobj the current branch object
/// @return int index of the next object to run in the current program
/// @return -1 the next object index to run in the current program is the current+1
///
int program::rpn_repeat(branch& myobj) {
    int ret = -1;

    MIN_ARGUMENTS_RET(1, -(int)ret_runtime_error);
    ARG_MUST_BE_OF_TYPE_RET(0, cmd_number, -(int)ret_runtime_error);

    // check arg
    // myobj.arg1 is end+1
    if (_stack->value<number>(0) == 0) ret = myobj.arg1;
    _stack->pop();

    return ret;
}

/// @brief start keyword (branch) implementation
///
/// @param myobj the current branch object
/// @return int index of the next object to run in the current program
/// @return -1 the next object index to run in the current program is the current+1
///
int program::rpn_start(branch& myobj) {
    int ret = -1;

    MIN_ARGUMENTS_RET(2, -(int)ret_runtime_error);
    ARG_MUST_BE_OF_TYPE_RET(0, cmd_number, -(int)ret_runtime_error);
    ARG_MUST_BE_OF_TYPE_RET(1, cmd_number, -(int)ret_runtime_error);

    // farg2 = last value of start command
    rpnstack::copy_and_push_front(*_stack, _stack->size() - 1, _calc_stack);
    myobj.farg2 = (number*)_calc_stack.back();
    _stack->pop_front();

    // farg1 = first value of start command
    rpnstack::copy_and_push_front(*_stack, _stack->size() - 1, _calc_stack);
    myobj.farg1 = (number*)_calc_stack.back();
    _stack->pop_front();

    // test value
    if (myobj.farg1->value > myobj.farg2->value)
        // last boundary lower than first boundary
        // -> next command shall be after 'next'
        // arg2 holds index of 'next'
        ret = myobj.arg2 + 1;

    return ret;
}

/// @brief for keyword (branch) implementation
///
/// @param myobj the current branch object
/// @return int index of the next object to run in the current program
/// @return -1 the next object index to run in the current program is the current+1
///
int program::rpn_for(branch& myobj) {
    int ret;

    MIN_ARGUMENTS_RET(2, -(int)ret_runtime_error);
    ARG_MUST_BE_OF_TYPE_RET(0, cmd_number, -(int)ret_runtime_error);
    ARG_MUST_BE_OF_TYPE_RET(1, cmd_number, -(int)ret_runtime_error);

    symbol* sym = (symbol*)(*this)[myobj.arg1];

    // farg2 = last value of for command
    // arg1 = index of symbol to increase
    rpnstack::copy_and_push_front(*_stack, _stack->size() - 1, _calc_stack);
    myobj.farg2 = (number*)_calc_stack.back();
    _stack->pop_front();

    // farg1 = first value of for command
    rpnstack::copy_and_push_front(*_stack, _stack->size() - 1, _calc_stack);
    myobj.farg1 = (number*)_calc_stack.back();
    _stack->pop_front();

    // test value
    if (myobj.farg1->value > myobj.farg2->value)
        // last boundary lower than first boundary
        // -> next command shall be after 'next'
        // arg2 holds index of 'next'
        ret = myobj.arg2 + 1;
    else {
        // store symbol with first value
        _local_heap[sym->value] = myobj.farg1;
        ret = myobj.arg1 + 1;
    }

    return ret;
}

/// @brief next keyword (branch) implementation
///
/// @param myobj the current branch object
/// @return int index of the next object to run in the current program
/// @return -1 the next object index to run in the current program is the current+1
///
int program::rpn_next(branch& myobj) {
    // arg1 = index of start or for command in program
    // farg1 = current count
    branch* start_or_for = (branch*)(*this)[myobj.arg1];
    if (!myobj.arg_bool) {
        myobj.arg_bool = true;
        myobj.farg1 = start_or_for->farg1;
    }

    // increment then test
    // carefull: round toward minus infinity to avoid missing last boundary (because growing step)
    myobj.farg1->value++;

    // for command: increment symbol too
    if (start_or_for->arg1 != -1) {
        object* obj;
        unsigned int size;
        symbol* var = (symbol*)(*this)[start_or_for->arg1];

        // increase symbol variable
        _local_heap[var->value] = myobj.farg1;
    }

    // test value
    if (myobj.farg1->value > start_or_for->farg2->value) {
        // end of loop
        myobj.arg_bool = false;  // init again next time
        _calc_stack.pop_front(2);
        return -1;
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
/// @return int index of the next object to run in the current program
/// @return -1 the next object index to run in the current program is the current+1
///
int program::rpn_step(branch& myobj) {
    int ret;
    MIN_ARGUMENTS_RET(1, -(int)ret_runtime_error);
    ARG_MUST_BE_OF_TYPE_RET(0, cmd_number, -(int)ret_runtime_error);

    mpreal step = _stack->value<number>(0);

    // end of loop if step is negative or zero
    if (step.toLong() <= 0)
        ret = -1;
    else {
        // arg1 = index of start or for command in program
        // farg1 = current count
        branch* start_or_for = (branch*)(*this)[myobj.arg1];
        if (!myobj.arg_bool) {
            myobj.arg_bool = true;
            myobj.farg1 = start_or_for->farg1;
        }

        // increment then test
        // carefull: round toward minus infinity to avoid missing last boundary (because growing step)
        myobj.farg1->value+=step.mpfr_ptr();
        mpfr_add(myobj.farg1->value.mpfr_ptr(), myobj.farg1->value.mpfr_ptr(), step.mpfr_ptr(), MPFR_RNDD);

        // for command: increment symbol too
        if (start_or_for->arg1 != -1) {
            object* obj;
            unsigned int size;
            symbol* var = (symbol*)(*this)[start_or_for->arg1];

            // increase symbol variable
            _local_heap[var->value] = myobj.farg1;
        }

        // test loop value is out of range
        if (myobj.farg1->value > start_or_for->farg2->value) {
            // end of loop
            myobj.arg_bool = false;  // init again next time
            _calc_stack.pop_front(2);
            ret = -1;
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
