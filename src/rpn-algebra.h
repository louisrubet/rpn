void plus()
{
	MIN_ARGUMENTS(2);

	// float
	if (IS_ARG_TYPE(0, cmd_number))
	{
		ARG_MUST_BE_OF_TYPE(1, cmd_number);
		//TODO really too slow
		putf(getf() + getf());
	}
	// binary
	else if (IS_ARG_TYPE(0, cmd_binary))
	{
		ARG_MUST_BE_OF_TYPE(1, cmd_binary);
		//TODO really too slow
		putb(getb() + getb());
	}
	// string
	else if (IS_ARG_TYPE(0, cmd_string))
	{
		ARG_MUST_BE_OF_TYPE(1, cmd_string);
		string& second = *((ostring*)_stack->back())->_value;
		_stack->pop_back();
		*((ostring*)_stack->back())->_value += second;
	}
	else
		ERR_CONTEXT(ret_bad_operand_type);
}

void minus()
{
	MIN_ARGUMENTS(2);

	// float
	if (IS_ARG_TYPE(0, cmd_number))
	{
		ARG_MUST_BE_OF_TYPE(1, cmd_number);
		//TODO really too slow
		floating_t first = getf();
		putf(getf() - first);
	}
	// binary
	else if (IS_ARG_TYPE(0, cmd_binary))
	{
		ARG_MUST_BE_OF_TYPE(1, cmd_binary);
		//TODO really too slow
		integer_t first = getb();
		putb(getb() - first);
	}
	else
		ERR_CONTEXT(ret_bad_operand_type);
}

void mul()
{
	// float
	if (IS_ARG_TYPE(0, cmd_number))
	{
		ARG_MUST_BE_OF_TYPE(1, cmd_number);
		//TODO really too slow
		putf(getf() * getf());
	}
	// binary
	else if (IS_ARG_TYPE(0, cmd_binary))
	{
		ARG_MUST_BE_OF_TYPE(1, cmd_binary);
		//TODO really too slow
		putb(getb() * getb());
	}
	else
		ERR_CONTEXT(ret_bad_operand_type);
}

void div()
{
	MIN_ARGUMENTS(2);

	// float
	if (IS_ARG_TYPE(0, cmd_number))
	{
		ARG_MUST_BE_OF_TYPE(1, cmd_number);
		//TODO really too slow

		floating_t first = getf();
		// arithmetic faults are managed by c++
		putf(getf() / first);
	}
	// binary
	else if (IS_ARG_TYPE(0, cmd_binary))
	{
		ARG_MUST_BE_OF_TYPE(1, cmd_binary);
		if (((binary*)_stack->get_obj(0))->_value == 0)
		{
			ERR_CONTEXT(ret_div_by_zero);
		}
		else
		{
			//TODO really too slow
			integer_t first = getb();
			putb(getb() / first);
		}
	}
	else
		ERR_CONTEXT(ret_bad_operand_type);
}

void neg()
{
	MIN_ARGUMENTS(1);

	// float
	if (IS_ARG_TYPE(0, cmd_number))
		putf(-getf());
	// binary
	else if (IS_ARG_TYPE(0, cmd_binary))
		putb(-getb());
	else
		ERR_CONTEXT(ret_bad_operand_type);
}

void inv()
{
	MIN_ARGUMENTS(1);
	ARG_MUST_BE_OF_TYPE(0, cmd_number);

	// arithmetic faults are managed by c++
	putf(1 / getf());
}

void purcent()
{
	MIN_ARGUMENTS(2);
	ARG_MUST_BE_OF_TYPE(0, cmd_number);
	ARG_MUST_BE_OF_TYPE(1, cmd_number);

	putf((getf() * getf()) / 100);
}

void purcentCH()
{
	MIN_ARGUMENTS(2);
	ARG_MUST_BE_OF_TYPE(0, cmd_number);
	ARG_MUST_BE_OF_TYPE(1, cmd_number);

	// arithmetic faults are managed by c++
	floating_t first = getf();
	putf((100 * first) / getf());
}

void power()
{
	MIN_ARGUMENTS(2);
	ARG_MUST_BE_OF_TYPE(0, cmd_number);
	ARG_MUST_BE_OF_TYPE(1, cmd_number);

	// arithmetic faults are managed by c++
	floating_t first = getf();
	putf(powl(getf(), first));
}

void squareroot()
{
	MIN_ARGUMENTS(1);
	ARG_MUST_BE_OF_TYPE(0, cmd_number);

	// arithmetic faults are managed by c++
	putf(sqrtl(getf()));
}

void square()
{
	MIN_ARGUMENTS(1);
	ARG_MUST_BE_OF_TYPE(0, cmd_number);

	floating_t first = getf();
	putf(first * first);
}
