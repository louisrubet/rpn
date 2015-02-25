//
void eval(void)
{
	MIN_ARGUMENTS(1);
	if (IS_ARG_TYPE(0, cmd_symbol))
	{
		// recall a variable
		void* obj;
		unsigned int size;
		int type;
		string& variable = *((symbol*)_stack->back())->_value;
		if (_heap->get(variable, obj, size, type))
		{
			_stack->pop_back();
			_stack->push_back(obj, size, type);
		}
		else
			ERR_CONTEXT(ret_unknown_variable);
	}
	else if (IS_ARG_TYPE(0, cmd_program))
	{
		// eval a program
		string& entry = *((oprogram*)_stack->back())->_value;
		_stack->pop_back();

		program prog;

        // make program from entry
		if (program::parse(entry, prog) == ret_ok)
		{
			// run it
			prog.run(*_stack, *_heap);
		}
	}
	else
		ERR_CONTEXT(ret_bad_operand_type);
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
    for (unsigned int i = myobj.arg1 + count_symbols; i > myobj.arg1; i--)
    {
        _heap->add(*((symbol*)seq_obj(i))->_value, _stack->get_obj(0), _stack->get_len(0), _stack->get_type(0));
        _stack->pop_back();
    }

    // run the program
    string& entry = *((oprogram*)seq_obj(myobj.arg1 + count_symbols + 1))->_value;
    program prog;

    // make the program from entry
    if (program::parse(entry, prog) == ret_ok)
    {
        // run it
        prog.run(*_stack, *_heap);
    }

    // point on next command
    return myobj.arg1 + count_symbols + 2;
}
