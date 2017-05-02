//
void eval(void)
{
    bool run_prog = false;
    string prog_text;

    MIN_ARGUMENTS(1);
    if (IS_ARG_TYPE(0, cmd_symbol))
    {
        // recall a variable
        object* obj;
        unsigned int size;
        int type;
        string variable(((symbol*)_stack->back())->_value);

        // mind the order of heaps
        if (_local_heap.get(variable, obj, size)
                || ((_parent_local_heap != NULL) && _parent_local_heap->get(variable, obj, size))
                || _global_heap->get(variable, obj, size))
        {
            // if variable holds a program, run this program
            if (type == cmd_program)
            {
                prog_text = ((oprogram*)obj)->_value;
                (void)_stack->pop_back();
                run_prog = true;
            }
            else
            {
                // else recall this variable (i.e. stack its content)
                (void)_stack->pop_back();
                stack::copy_and_push_back(obj, *_stack, size);
            }
        }
        else
            ERR_CONTEXT(ret_unknown_variable);
    }
    else if (IS_ARG_TYPE(0, cmd_program))
    {
        // eval a program
        prog_text = ((oprogram*)_stack->pop_back())->_value;
        run_prog = true;
    }
    else
        ERR_CONTEXT(ret_bad_operand_type);

    // run prog if any
    if (run_prog)
    {
        program prog;

        // make program from entry
        if (program::parse(prog_text.c_str(), prog) == ret_ok)
        {
            // run it
            prog.run(*_stack, *_global_heap, &_local_heap);
        }
    }
}

// carefull, this not a command but a branch
int inprog(branch& myobj)
{
    string context("->");// for showing errors
    int count_symbols = 0;
    bool prog_found = false;

    if (myobj.arg1 == -1)
    {
        ERR_CONTEXT(ret_unknown_err);
        return -1;
    }

    // syntax must be
    // -> <auto evaluated symbol #1> ... <auto evaluated symbol #n> <oprogram>

    // find next oprogram object
    for (unsigned int i = myobj.arg1 + 1; i < size(); i++)
    {
        // count symbol
        if (seq_type(i) == cmd_symbol)
            count_symbols++;
        // stop if prog
        else if (seq_type(i) == cmd_program)
        {
            prog_found = true;
            break;
        }
        // found something other than symbol
        else
        {
            ERR_CONTEXT(ret_bad_operand_type);
            show_error(_err, context);
            return -1;
        }
    }

    // found 0 symbols
    if (count_symbols == 0)
    {
        ERR_CONTEXT(ret_syntax);
        show_error(_err, context);
        return -1;
    }

    // <oprogram> is missing
    if (! prog_found)
    {
        ERR_CONTEXT(ret_syntax);
        show_error(_err, context);
        return -1;
    }

    // check symbols number vs stack size
    if (stack_size() < count_symbols)
    {
        ERR_CONTEXT(ret_missing_operand);
        show_error(_err, context);
        return -1;
    }

    // load variables
    heap local_heap;
    for (unsigned int i = myobj.arg1 + count_symbols; i > myobj.arg1; i--)
    {
        local_heap.add(string(((symbol*)seq_obj(i))->_value), _stack->get_obj(0), _stack->get_len(0), _stack->get_type(0));
        (void)_stack->pop_back();
    }

    // run the program
    string entry(((oprogram*)seq_obj(myobj.arg1 + count_symbols + 1))->_value);
    program prog;

    // make the program from entry
    if (program::parse(entry.c_str(), prog) == ret_ok)
    {
        // run it
        prog.run(*_stack, *_global_heap, &local_heap);
    }

    // point on next command
    return myobj.arg1 + count_symbols + 2;
}
