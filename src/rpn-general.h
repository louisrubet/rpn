void test();

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
	cout<<endl;
	rpn_uname();
	cout<<endl;

	cout<<"HP28S Reverse Polish Notation language simulator"<<endl;
	cout<<"syntax: rpn [command]"<<endl;
	cout<<"with optional command = list of commands"<<endl;
    for(unsigned int i=0; i<sizeof(_keywords)/sizeof(_keywords[0]); i++)
		if (_keywords[i].comment.size() != 0)
		cout<<_keywords[i].name<<"\t"<<_keywords[i].comment<<endl;
	cout<<endl;
	cout<<"Current verbosity is "<<g_verbose<<endl;
	cout<<"Current float mode is ";
	switch(number::s_mode)
	{
		case number::std: cout << "'std'"; break;
		case number::fix: cout << "'fix'"; break;
		case number::sci: cout << "'sci'"; break;
		default: cout << "unknown"; break;
	}
	cout<<endl<<"Current float precision is "<<number::s_current_precision<<endl;
	cout<<"Current binary mode is ";
	switch(binary::s_mode)
	{
		case binary::dec: cout << "'dec'"; break;
		case binary::hex: cout << "'hex'"; break;
		case binary::oct: cout << "'oct'"; break;
		case binary::bin: cout << "'bin'"; break;
		default: cout << "unknown"; break;
	}
	cout<<endl<<endl;
}

void std()
{
	if (stack_size()>=1)
	{
		ARG_IS_OF_TYPE(0, cmd_number);
		number::s_default_precision = (int)getf();
	}
	number::s_current_precision = number::s_default_precision;
	number::s_mode = number::std;

	cout.precision(number::s_current_precision);
	cout.unsetf(ios_base::floatfield);
}

void fix()
{
	MIN_ARGUMENTS(1);
	ARG_IS_OF_TYPE(0, cmd_number);

	number::s_current_precision = (int)getf();
	number::s_mode = number::fix;

	cout << setprecision(number::s_current_precision) << fixed;
}

void sci()
{
	MIN_ARGUMENTS(1);
	ARG_IS_OF_TYPE(0, cmd_number);

	number::s_current_precision = (int)getf();
	number::s_mode = number::sci;

	cout << setprecision(number::s_current_precision) << scientific;
}

void rpn_version()
{
	cout << version << endl;
}

void rpn_uname()
{
	cout << uname << endl;
}
