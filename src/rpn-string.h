void instr()
{
	MIN_ARGUMENTS(1);

	// stringify only if not already a string
	if (_stack->get_type(0) != cmd_string)
	{
		stringstream out;
		((object*)_stack->back())->show(out);
		_stack->pop_back();

        ostring str;
        str.set(out.str().c_str(), out.str().size());
        _stack->push_back(&str, str.size(), cmd_string);
	}
}

void strout()
{
    MIN_ARGUMENTS(1);
	ARG_MUST_BE_OF_TYPE(0, cmd_string);

    string entry(((ostring*)_stack->back())->_value);
	_stack->pop_back();

	program prog;

	// make program from string in stack level 1
    if (program::parse(entry.c_str(), prog) == ret_ok)
	{
		// run it
		prog.run(*_stack, *_heap);
	}
}
