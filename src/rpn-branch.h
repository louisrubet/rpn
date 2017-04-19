
//
int rpn_if(branch& myobj)
{
    // myobj.arg1 = 'if' condition evaluation value
    MIN_ARGUMENTS_RET(1, -1);
    ARG_MUST_BE_OF_TYPE_RET(0, cmd_number, -1);

    if (mpfr_cmp_si(&((number*)_stack->get_obj(0))->_value.mpfr, 0UL) != 0)
        myobj.arg1 = 1;
    else
        myobj.arg1 = 0;
    _stack->pop_back();
    return -1;
}

int rpn_then(branch& myobj)
{
    // myobj.arg1 = index of then + 1
    // myobj.arg2 = index of else + 1 or end + 1
    // myobj.arg3 = index of if
    // if condition is true -> arg1 (= jump to then + 1)
    // else -> arg2 (= jump to else + 1 or end + 1)
    branch* if_cmd = (branch*)seq_obj(myobj.arg3);
    if (if_cmd->arg1 == 1)
        return myobj.arg1;
    else
        return myobj.arg2;
}

int rpn_else(branch& myobj)
{
    // myobj.arg1 = index of else + 1
    // myobj.arg2 = index of end + 1
    // myobj.arg3 = index of if
    // if condition was false -> arg1 (= jump to else + 1)
    // if condition was true -> arg2 (= jump to end + 1)
    branch* if_cmd = (branch*)seq_obj(myobj.arg3);
    if (if_cmd->arg1 == 1)
        return myobj.arg2;
    else
        return myobj.arg1;
}

void rpn_end(void)
{
    // nothing
}

int rpn_start(branch& myobj)
{
    MIN_ARGUMENTS_RET(2, 1);
    ARG_MUST_BE_OF_TYPE_RET(0, cmd_number, -1);
    ARG_MUST_BE_OF_TYPE_RET(1, cmd_number, -1);

    // farg1 = first value of start command
    // farg2 = last value of start command
    myobj.farg2 = getf();
    myobj.farg2.ensure_significand();
    myobj.farg1 = getf();
    myobj.farg1.ensure_significand();
    return -1;
}

int rpn_for(branch& myobj)
{
    MIN_ARGUMENTS_RET(2, 1);
    ARG_MUST_BE_OF_TYPE_RET(0, cmd_number, -1);
    ARG_MUST_BE_OF_TYPE_RET(1, cmd_number, -1);

    symbol* sym = ((symbol*)seq_obj(myobj.arg1));

    // farg1 = first value of for command
    // farg2 = last value of for command
    // arg1 = index of symbol to increase
    myobj.farg2 = getf();
    myobj.farg2.ensure_significand();
    myobj.farg1 = getf();
    myobj.farg1.ensure_significand();

    // store symbol with first value
    number* num = (number*)_local_heap.add(string(sym->_value), NULL, sizeof(number), cmd_number);
    num->set(myobj.farg1);
    num->ensure_significand();

    return myobj.arg1 + 1;
}

int rpn_next(branch& myobj)
{
    // arg1 = index of start or for command in program
    // farg1 = current count
    branch* start_or_for = (branch*)seq_obj(myobj.arg1);
    if (! myobj.arg_bool)
    {
        myobj.arg_bool = true;
        myobj.farg1 = start_or_for->farg1;
        myobj.farg1.ensure_significand();
    }

    // increment then test
    mpfr_add_si(&myobj.farg1.mpfr, &myobj.farg1.mpfr, 1UL, MPFR_DEF_RND);

    // for command: increment symbol too
    if (start_or_for->arg1 != -1)
    {
        void* obj;
        unsigned int size;
        int type;
        symbol* var = (symbol*)seq_obj(start_or_for->arg1);

        // check symbol variable is a number, then increase
        if (_local_heap.get(string(var->_value), obj, size, type) && (type == cmd_number))
        {
            ((number*)obj)->set(myobj.farg1);
            ((number*)obj)->ensure_significand();
        }
    }

    //test value
    if (myobj.farg1 > start_or_for->farg2)
    {
        // end of loop
        myobj.arg_bool = false;// init again next time
        return -1;
    }
    else
    {
        // for command: next instruction will be after symbol variable
        if (start_or_for->arg1 != -1)
            return start_or_for->arg1 + 1;
        // start command: next instruction will be after start command
        else
            return myobj.arg1 + 1;
    }
}

int rpn_step(branch& myobj)
{
    // arg1 = index of start or for command in program
    // farg1 = current count
    floating_t step = getf();
    step.ensure_significand();

    branch* start_or_for = (branch*)seq_obj(myobj.arg1);
    if (! myobj.arg_bool)
    {
        myobj.arg_bool = true;
        myobj.farg1 = start_or_for->farg1;
        myobj.farg1.ensure_significand();
    }

    // increment then test
    mpfr_add(&myobj.farg1.mpfr, &myobj.farg1.mpfr, &step.mpfr, MPFR_DEF_RND);

    // for command: increment symbol too
    if (start_or_for->arg1 != -1)
    {
        void* obj;
        unsigned int size;
        int type;
        symbol* var = (symbol*)seq_obj(start_or_for->arg1);

        // check symbol variable is a number, then increase
        if (_local_heap.get(string(var->_value), obj, size, type) && (type == cmd_number))
        {
            ((number*)obj)->_value = myobj.farg1;
            ((number*)obj)->ensure_significand();
        }
    }

    //test value
    if (((step>0) && (myobj.farg1 > start_or_for->farg2))
       || ((step<0) && (myobj.farg1 < start_or_for->farg2)))
    {
        // end of loop
        myobj.arg_bool = false;// init again next time
        return -1;
    }
    else
    {
        // for command: next instruction will be after symbol variable
        if (start_or_for->arg1 != -1)
            return start_or_for->arg1 + 1;
        // start command: next instruction will be after start command
        else
            return myobj.arg1 + 1;
    }
}
