void std()
{
	cout << setprecision(FLOATING_DEFAULT_PRECISION);
	cout.unsetf(ios_base::floatfield);
}

void fix()
{
	MIN_ARGUMENTS(1);
	ARG_IS_OF_TYPE(0, cmd_number);

	cout << setprecision((int)getf()) << fixed;
}

void sci()
{
	MIN_ARGUMENTS(1);
	ARG_IS_OF_TYPE(0, cmd_number);

	cout << setprecision((int)getf()) << scientific;
}
