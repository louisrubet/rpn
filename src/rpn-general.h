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
    for(int i=0; i<sizeof(_keywords)/sizeof(_keywords[0]); i++)
		if (_keywords[i].comment.size() != 0)
		cout<<_keywords[i].name<<"\t"<<_keywords[i].comment<<endl;
	cout<<endl;
	cout<<"Current verbosity is "<<g_verbose<<endl<<endl;
	cout<<endl;
}

void test();
