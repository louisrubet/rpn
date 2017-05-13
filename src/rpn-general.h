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

void help()
{
    // software name
    cout<<endl;
    cout<<ATTR_BOLD<<uname<<ATTR_OFF<<endl;
    cout<<endl;

    // description
    cout<<description<<endl<<endl;

    // syntax
    cout<<syntax<<endl;

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
    cout<<"Current float mode is ";
    switch(number::s_mode)
    {
        case number::std: cout << "'std'"; break;
        case number::fix: cout << "'fix'"; break;
        case number::sci: cout << "'sci'"; break;
        default: cout << "unknown"; break;
    }

    cout<<endl<<"Current float precision is "<<number::s_current_precision<<endl;
    cout<<endl<<endl;
}

void std()
{
    if (stack_size()>=1)
    {
        ARG_MUST_BE_OF_TYPE(0, cmd_number);

        int precision = int(((number*)_stack->pop_back())->_value);
        number::s_default_precision = int(precision);
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

    int precision = int(((number*)_stack->pop_back())->_value);
    number::s_current_precision = int(precision);

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

    int precision = int(((number*)_stack->pop_back())->_value);
    number::s_current_precision = int(precision);

    number::s_mode = number::sci;

    // format for mpfr_printf 
    stringstream ss;
    ss << number::s_current_precision;
    s_mpfr_printf_format = s_mpfr_printf_format_beg + ss.str() + s_mpfr_printf_format_sci;
}

void rpn_version()
{
    // allocate and set object
    unsigned int naked_entry_len = strlen(version);
    ostring* str = (ostring*)_stack->allocate_back(sizeof(ostring)+naked_entry_len+1, cmd_string);
    str->set(version, naked_entry_len);
}

void rpn_uname()
{
    // allocate and set object
    unsigned int naked_entry_len = strlen(uname);
    ostring* str = (ostring*)_stack->allocate_back(sizeof(ostring)+naked_entry_len+1, cmd_string);
    str->set(uname, naked_entry_len);
}

void type()
{
    MIN_ARGUMENTS(1);

    int type = _stack->back()->_type;
    if (type < 0 || type >= (int)cmd_max)
        type = (int)cmd_undef;

    unsigned int string_size = strlen(cmd_type_string[type]);
    unsigned int size = sizeof(symbol)+string_size+1;
    symbol* sym = (symbol*)allocate_back(size, cmd_symbol);
    sym->set(cmd_type_string[type], string_size, false);
}

void rpn_default()
{
    program::apply_default();
}
