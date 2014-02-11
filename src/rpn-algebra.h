void plus()
{
	MIN_ARGUMENTS(2);
	ARG_IS_OF_TYPE(0, cmd_number);
	ARG_IS_OF_TYPE(1, cmd_number);

	putf(getf() + getf());
}

void minus()
{
	MIN_ARGUMENTS(2);
	ARG_IS_OF_TYPE(0, cmd_number);
	ARG_IS_OF_TYPE(1, cmd_number);

	floating_t first = getf();
	putf(getf() - first);
}

void mul()
{
	MIN_ARGUMENTS(2);
	ARG_IS_OF_TYPE(0, cmd_number);
	ARG_IS_OF_TYPE(1, cmd_number);

	putf(getf() * getf());
}

void div()
{
	MIN_ARGUMENTS(2);
	ARG_IS_OF_TYPE(0, cmd_number);
	ARG_IS_OF_TYPE(1, cmd_number);

	// arithmetic faults are managed by c++
	floating_t first = getf();
	putf(getf() / first);
}

void neg()
{
	MIN_ARGUMENTS(1);
	ARG_IS_OF_TYPE(0, cmd_number);

	putf(-getf());
}
