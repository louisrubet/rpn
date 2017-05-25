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
    printf("\n" ATTR_BOLD "%s" ATTR_OFF "\n", uname);

    // description
    printf("%s\n\n", description);

    // syntax
    printf("%s\n", syntax);

    // keywords
    unsigned int i = 0;
    while(s_keywords[i].type != cmd_max)
    {
        if (s_keywords[i].comment.size() != 0)
        {
            // titles in bold
            if (s_keywords[i].type==cmd_undef)
                printf(ATTR_BOLD);
            // show title or keyword + comment
            printf("%s\t%s\n", s_keywords[i].name, s_keywords[i].comment.c_str());
            if (s_keywords[i].type==cmd_undef)
                printf(ATTR_OFF);
        }
        i++;
    }
    printf("\n");

    // show mode
    printf("Current float mode is ");
    switch(number::s_mode)
    {
        case number::std: printf("'std'"); break;
        case number::fix: printf("'fix'"); break;
        case number::sci: printf("'sci'"); break;
        default: printf("unknown"); break;
    }
    printf(" with %d digits\n", number::s_current_precision);

    // calc precision and rounding mode
    printf("Current floating point precision is %d bits\n", (int)floating_t::s_mpfr_prec);
    printf("Current rounding mode is '%s'\n", floating_t::s_mpfr_rnd_str[floating_t::s_mpfr_rnd]);
    printf("\n\n");
}

void std()
{
    int precision = -1;

    if (stack_size()>=1)
    {
        ARG_MUST_BE_OF_TYPE(0, cmd_number);

        precision = int(((number*)_stack->pop_back())->_value);
    }

    if (precision != -1)
        number::s_current_precision = precision;
    number::s_mode = number::std;

    // format for mpfr_printf 
    stringstream ss;
    ss << number::s_current_precision;
    number::s_mpfr_printf_format = string(MPFR_FORMAT_BEG) + ss.str() + string(MPFR_FORMAT_STD);
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
    number::s_mpfr_printf_format = string(MPFR_FORMAT_BEG) + ss.str() + string(MPFR_FORMAT_FIX);
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
    number::s_mpfr_printf_format = string(MPFR_FORMAT_BEG) + ss.str() + string(MPFR_FORMAT_SCI);
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

    unsigned int string_size = strlen(object::s_cmd_type_string[type]);
    unsigned int size = sizeof(symbol)+string_size+1;
    symbol* sym = (symbol*)_stack->allocate_back(size, cmd_symbol);
    sym->set(object::s_cmd_type_string[type], string_size, false);
}

void rpn_default()
{
    program::apply_default();
}

void precision()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    //set MPFR float precision
    long prec = mpfr_get_si(((number*)_stack->pop_back())->_value.mpfr, floating_t::s_mpfr_rnd);
    if (prec >= 0)
    {
        floating_t::s_mpfr_prec = (mpfr_prec_t)prec;
        floating_t::s_mpfr_prec_bytes = mpfr_custom_get_size(prec);
    }
    else
        ERR_CONTEXT(ret_out_of_range);
}

void round()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_string);
    
    ostring* str = (ostring*)_stack->pop_back();
    bool done = false;
    for(int rnd = (int)MPFR_DEFAULT_RND; rnd <= (int)MPFR_RNDA; rnd++)
    {
        if (string(floating_t::s_mpfr_rnd_str[rnd]) == str->_value)
        {
            floating_t::s_mpfr_rnd = (mpfr_rnd_t)rnd;
            done = true;
        }
    }
    if (!done)
        ERR_CONTEXT(ret_out_of_range);
}
