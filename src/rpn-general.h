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
	cout<<"Current verbosity is "<<g_verbose<<endl;
	cout<<"Current float mode is ";
	switch(g_float_mode)
	{
		case mode_std: cout << "'std'"; break;
		case mode_fix: cout << "'fix'"; break;
		case mode_sci: cout << "'sci'"; break;
		default: cout << "unknown"; break;
	}
	cout<<endl<<"Current float precision is "<<g_current_precision<<endl;
	cout<<endl<<endl;
}

void test();

void std()
{
	if (stack_size()>=1)
	{
		ARG_IS_OF_TYPE(0, cmd_number);
		g_default_precision = (int)getf();
	}
	g_current_precision = g_default_precision;
	g_float_mode = mode_std;

	cout.precision(g_current_precision);
	cout.unsetf(ios_base::floatfield);
}

void fix()
{
	MIN_ARGUMENTS(1);
	ARG_IS_OF_TYPE(0, cmd_number);

	g_current_precision = (int)getf();
	g_float_mode = mode_fix;

	cout << setprecision(g_current_precision) << fixed;
}

void sci()
{
	MIN_ARGUMENTS(1);
	ARG_IS_OF_TYPE(0, cmd_number);

	g_current_precision = (int)getf();
	g_float_mode = mode_sci;

	cout << setprecision(g_current_precision) << scientific;
}
