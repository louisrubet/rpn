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
		if (_heap->get(getn(), obj, size, type))
			_stack->push_back(obj, size, type);
		else
			ERR_CONTEXT(ret_unknown_variable);
	}
	else if (IS_ARG_TYPE(0, cmd_program))
	{
		string& entry = *((oprogram*)_stack->back())->_value;
		_stack->pop_back();

		program prog;

		// make program from string in stack level 1
		if (program::parse(entry, prog) == ret_ok)
		{
			// run it
			prog.run(*_stack, *_heap);
		}
	}
	else
		ERR_CONTEXT(ret_bad_operand_type);
}
