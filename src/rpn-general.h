//
void nop()
{
	// nop
}

void good_bye()
{
	ERR_CONTEXT(ret_good_bye);
}

void verbose()
{
	MIN_ARGUMENTS(1);
	ARG_IS_OF_TYPE(0, cmd_number);
	g_verbose = (int)getf();
}

void help()
{
	cout<<"rpn - HP28S reverse polish notation language simulator"<<endl;
	cout<<"syntax: rpn [command]"<<endl;
	cout<<"with optional command = list of commands"<<endl;
    for(unsigned int i=0; i<sizeof(_keywords)/sizeof(_keywords[0]); i++)
		if (_keywords[i].comment.size() != 0)
		cout<<_keywords[i].name<<"\t"<<_keywords[i].comment<<endl;
	cout<<endl;
	cout<<"Current verbosity is "<<g_verbose<<endl<<endl;
	cout<<endl;
}

void test();

void std()
{
	if (stack_size()>=1)
	{
		ARG_IS_OF_TYPE(0, cmd_number);
		g_default_precision = (int)getf();
	}
	cout.precision(g_default_precision);
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
