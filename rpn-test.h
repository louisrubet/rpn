void sup(void)
{
	MIN_ARGUMENTS(2);
	ARG_IS_OF_TYPE(0, cmd_number);
	ARG_IS_OF_TYPE(1, cmd_number);

	floating_t first = getf();
	putf(getf() > first);
}

void sup_eq(void)
{
	MIN_ARGUMENTS(2);
	ARG_IS_OF_TYPE(0, cmd_number);
	ARG_IS_OF_TYPE(1, cmd_number);

	floating_t first = getf();
	putf(getf() >= first);
}

void inf(void)
{
	MIN_ARGUMENTS(2);
	ARG_IS_OF_TYPE(0, cmd_number);
	ARG_IS_OF_TYPE(1, cmd_number);

	floating_t first = getf();
	putf(getf() < first);
}

void inf_eq(void)
{
	MIN_ARGUMENTS(2);
	ARG_IS_OF_TYPE(0, cmd_number);
	ARG_IS_OF_TYPE(1, cmd_number);

	floating_t first = getf();
	putf(getf() <= first);
}

void diff(void)
{
	MIN_ARGUMENTS(2);
	ARG_IS_OF_TYPE(0, cmd_number);
	ARG_IS_OF_TYPE(1, cmd_number);

	floating_t first = getf();
	putf(getf() != first);
}

void eq(void)
{
	MIN_ARGUMENTS(2);
	int type = _stack->get_type(0);

	if (_stack->get_type(1) == type)
	{
		switch(type)
		{
		case cmd_number:
		{
			floating_t first = getf();
			putf(getf() == first);
		}
		break;
		case cmd_symbol:
		{
			string first = getn();
			putf(getn() == first);
		}
		break;
		default:
			_stack->pop_back();
			_stack->pop_back();
			putf(0);
			break;
		}
	}
	else
	{
		_stack->pop_back();
		_stack->pop_back();
		putf(0);
	}
}
