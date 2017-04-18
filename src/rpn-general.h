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
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    g_verbose = int(getf());
}

void help()
{
    cout<<endl;
    cout<<ATTR_BOLD<<uname<<ATTR_OFF<<endl;
    cout<<endl;

    // syntax
    for (int i = 0; syntax[i] != NULL; i++)
        cout<<syntax[i]<<endl;

    // keywords
    for(unsigned int i=0; i<sizeof(_keywords)/sizeof(_keywords[0]); i++)
    {
        if (_keywords[i].comment.size() != 0)
        {
            // titles in bold
            if (_keywords[i].type==cmd_undef)
                cout<<ATTR_BOLD;
            // show title or keyword + comment
            cout<<_keywords[i].name<<"\t"<<_keywords[i].comment<<endl;
            if (_keywords[i].type==cmd_undef)
                cout<<ATTR_OFF;
        }
    }
    cout<<endl;

    // different modes
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
        ARG_MUST_BE_OF_TYPE(0, cmd_number);
        number::s_default_precision = int(getf());
    }

    number::s_current_precision = number::s_default_precision;
    number::s_mode = number::std;

    // format for mpfr_printf 
    stringstream ss;
    ss << number::s_current_precision;
    s_mpfr_printf_format = s_mpfr_printf_format_beg + ss.str() + s_mpfr_printf_format_std;
}

void fix()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    number::s_current_precision = int(getf());
    number::s_mode = number::fix;

    // format for mpfr_printf 
    stringstream ss;
    ss << number::s_current_precision;
    s_mpfr_printf_format = s_mpfr_printf_format_beg + ss.str() + s_mpfr_printf_format_fix;
}

void sci()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    number::s_current_precision = (int)getf();
    number::s_mode = number::sci;

    // format for mpfr_printf 
    stringstream ss;
    ss << number::s_current_precision;
    s_mpfr_printf_format = s_mpfr_printf_format_beg + ss.str() + s_mpfr_printf_format_sci;
}

void rpn_version()
{
    int naked_entry_len;
    int obj_len;
    ostring* str;

    // entry length without prefix / postfix
    naked_entry_len = (int)strlen(version);

    // total object length
    obj_len = sizeof(ostring)+naked_entry_len+1;

    // allocate object
    str = (ostring*)malloc(obj_len);

    // set it
    str->set(version, naked_entry_len);

    // push in stack
    _stack->push_back(str, str->size(), cmd_string);
    free(str);
}

void rpn_uname()
{
    int naked_entry_len;
    int obj_len;
    ostring* str;

    // entry length without prefix / postfix
    naked_entry_len = (int)strlen(uname);

    // total object length
    obj_len = sizeof(ostring)+naked_entry_len+1;

    // allocate object
    str = (ostring*)malloc(obj_len);

    // set it
    str->set(uname, naked_entry_len);

    // push in stack
    _stack->push_back(str, str->size(), cmd_string);
    free(str);
}
