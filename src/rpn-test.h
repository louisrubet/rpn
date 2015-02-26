void sup(void)
{
	MIN_ARGUMENTS(2);
	ARG_MUST_BE_OF_TYPE(0, cmd_number);
	ARG_MUST_BE_OF_TYPE(1, cmd_number);

	floating_t first = getf();
	putf(getf() > first);
}

void sup_eq(void)
{
	MIN_ARGUMENTS(2);
	ARG_MUST_BE_OF_TYPE(0, cmd_number);
	ARG_MUST_BE_OF_TYPE(1, cmd_number);

	floating_t first = getf();
	putf(getf() >= first);
}

void inf(void)
{
	MIN_ARGUMENTS(2);
	ARG_MUST_BE_OF_TYPE(0, cmd_number);
	ARG_MUST_BE_OF_TYPE(1, cmd_number);

	floating_t first = getf();
	putf(getf() < first);
}

void inf_eq(void)
{
	MIN_ARGUMENTS(2);
	ARG_MUST_BE_OF_TYPE(0, cmd_number);
	ARG_MUST_BE_OF_TYPE(1, cmd_number);

	floating_t first = getf();
	putf(getf() <= first);
}

void diff(void)
{
	MIN_ARGUMENTS(2);
	ARG_MUST_BE_OF_TYPE(0, cmd_number);
	ARG_MUST_BE_OF_TYPE(1, cmd_number);

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
        //TODO
#if 0
		case cmd_symbol:
		{
			string first = getn();
			putf(getn() == first);
		}
#endif
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

void test_and(void)
{
	MIN_ARGUMENTS(2);
	ARG_MUST_BE_OF_TYPE(0, cmd_number);
	ARG_MUST_BE_OF_TYPE(1, cmd_number);

	floating_t first = getf();
	floating_t second = getf();
	putf((first != 0) && (second != 0));
}

void test_or(void)
{
	MIN_ARGUMENTS(2);
	ARG_MUST_BE_OF_TYPE(0, cmd_number);
	ARG_MUST_BE_OF_TYPE(1, cmd_number);

	floating_t first = getf();
	floating_t second = getf();
	putf((first != 0) || (second != 0));
}

void test_xor(void)
{
	MIN_ARGUMENTS(2);
	ARG_MUST_BE_OF_TYPE(0, cmd_number);
	ARG_MUST_BE_OF_TYPE(1, cmd_number);

	floating_t first = getf();
	floating_t second = getf();
	putf(((first == 0) && (second != 0)) || ((first != 0) && (second == 0)));
}

void test_not(void)
{
	MIN_ARGUMENTS(1);
	ARG_MUST_BE_OF_TYPE(0, cmd_number);

	floating_t first = getf();
	putf((first == 0) ? 1 : 0);
}

void same(void)
{
	eq();
}
