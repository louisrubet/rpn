int cmp_strings_on_stack_top()
{
    // _stack sould have 2 strings at level 1 and 2
    // this function removes these 2 entries
    ostring* right =  (ostring*)_stack->pop_back();
    ostring* left = (ostring*)_stack->pop_back();
    return strncmp(left->_value, right->_value, min(left->_len, right->_len));
}

void sup(void)
{
    MIN_ARGUMENTS(2);

    // numbers
    if (_stack->get_type(0) == cmd_number && _stack->get_type(1) == cmd_number)
    {
        number* right =  (number*)_stack->pop_back();
        number* left = (number*)_stack->back();

        if (mpfr_cmp(left->_value.mpfr, right->_value.mpfr) > 0)
            mpfr_set_si(left->_value.mpfr, 1, floating_t::s_mpfr_rnd);
        else
            mpfr_set_si(left->_value.mpfr, 0, floating_t::s_mpfr_rnd);
    }
    // strings
    else if (_stack->get_type(0) == cmd_string && _stack->get_type(1) == cmd_string)
    {
        int res_cmp = cmp_strings_on_stack_top();
        number* res = (number*)_stack->allocate_back(number::calc_size(), cmd_number);
        res->_value = (res_cmp > 0) ? 1L : 0L;
    }
    else
        ERR_CONTEXT(ret_bad_operand_type);
}

void sup_eq(void)
{
    MIN_ARGUMENTS(2);

    // numbers
    if (_stack->get_type(0) == cmd_number && _stack->get_type(1) == cmd_number)
    {
        number* right =  (number*)_stack->pop_back();
        number* left = (number*)_stack->back();

        if (mpfr_cmp(left->_value.mpfr, right->_value.mpfr) >= 0)
            mpfr_set_si(left->_value.mpfr, 1, floating_t::s_mpfr_rnd);
        else
            mpfr_set_si(left->_value.mpfr, 0, floating_t::s_mpfr_rnd);
    }
    // strings
    else if (_stack->get_type(0) == cmd_string && _stack->get_type(1) == cmd_string)
    {
        int res_cmp = cmp_strings_on_stack_top();
        number* res = (number*)_stack->allocate_back(number::calc_size(), cmd_number);
        res->_value = (res_cmp >= 0) ? 1L : 0L;
    }
    else
        ERR_CONTEXT(ret_bad_operand_type);
}

void inf(void)
{
    MIN_ARGUMENTS(2);

    // numbers
    if (_stack->get_type(0) == cmd_number && _stack->get_type(1) == cmd_number)
    {
        number* right =  (number*)_stack->pop_back();
        number* left = (number*)_stack->back();

        if (mpfr_cmp(left->_value.mpfr, right->_value.mpfr) < 0)
            mpfr_set_si(left->_value.mpfr, 1, floating_t::s_mpfr_rnd);
        else
            mpfr_set_si(left->_value.mpfr, 0, floating_t::s_mpfr_rnd);
    }
    // strings
    else if (_stack->get_type(0) == cmd_string && _stack->get_type(1) == cmd_string)
    {
        int res_cmp = cmp_strings_on_stack_top();
        number* res = (number*)_stack->allocate_back(number::calc_size(), cmd_number);
        res->_value = (res_cmp < 0) ? 1L : 0L;
    }
    else
        ERR_CONTEXT(ret_bad_operand_type);
}

void inf_eq(void)
{
    MIN_ARGUMENTS(2);

    // numbers
    if (_stack->get_type(0) == cmd_number && _stack->get_type(1) == cmd_number)
    {
        number* right =  (number*)_stack->pop_back();
        number* left = (number*)_stack->back();

        if (mpfr_cmp(left->_value.mpfr, right->_value.mpfr) <= 0)
            mpfr_set_si(left->_value.mpfr, 1, floating_t::s_mpfr_rnd);
        else
            mpfr_set_si(left->_value.mpfr, 0, floating_t::s_mpfr_rnd);
    }
    // strings
    else if (_stack->get_type(0) == cmd_string && _stack->get_type(1) == cmd_string)
    {
        int res_cmp = cmp_strings_on_stack_top();
        number* res = (number*)_stack->allocate_back(number::calc_size(), cmd_number);
        res->_value = (res_cmp <= 0) ? 1L : 0L;
    }
    else
        ERR_CONTEXT(ret_bad_operand_type);
}

void diff(void)
{
    MIN_ARGUMENTS(2);

    // numbers
    if (_stack->get_type(0) == cmd_number && _stack->get_type(1) == cmd_number)
    {
        number* right =  (number*)_stack->pop_back();
        number* left = (number*)_stack->back();

        if (mpfr_cmp(left->_value.mpfr, right->_value.mpfr) != 0)
            mpfr_set_si(left->_value.mpfr, 1, floating_t::s_mpfr_rnd);
        else
            mpfr_set_si(left->_value.mpfr, 0, floating_t::s_mpfr_rnd);
    }
    // strings
    else if (_stack->get_type(0) == cmd_string && _stack->get_type(1) == cmd_string)
    {
        int res_cmp = cmp_strings_on_stack_top();
        number* res = (number*)_stack->allocate_back(number::calc_size(), cmd_number);
        res->_value = (res_cmp != 0) ? 1L : 0L;
    }
    else
        ERR_CONTEXT(ret_bad_operand_type);
}

void eq(void)
{
    MIN_ARGUMENTS(2);

    // numbers
    if (_stack->get_type(0) == cmd_number && _stack->get_type(1) == cmd_number)
    {
        number* right =  (number*)_stack->pop_back();
        number* left = (number*)_stack->back();

        if (mpfr_cmp(left->_value.mpfr, right->_value.mpfr) == 0)
            mpfr_set_si(left->_value.mpfr, 1, floating_t::s_mpfr_rnd);
        else
            mpfr_set_si(left->_value.mpfr, 0, floating_t::s_mpfr_rnd);
    }
    // strings
    else if (_stack->get_type(0) == cmd_string && _stack->get_type(1) == cmd_string)
    {
        int res_cmp = cmp_strings_on_stack_top();
        number* res = (number*)_stack->allocate_back(number::calc_size(), cmd_number);
        res->_value = (res_cmp == 0) ? 1L : 0L;
    }
    else
        ERR_CONTEXT(ret_bad_operand_type);
}

void test_and(void)
{
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ARG_MUST_BE_OF_TYPE(1, cmd_number);

    number* right =  (number*)_stack->pop_back();
    number* left = (number*)_stack->back();

    if ((mpfr_cmp_si(left->_value.mpfr, 0) != 0)
        && (mpfr_cmp_si(right->_value.mpfr, 0) != 0))
        mpfr_set_si(left->_value.mpfr, 1, floating_t::s_mpfr_rnd);
    else
        mpfr_set_si(left->_value.mpfr, 0, floating_t::s_mpfr_rnd);
}

void test_or(void)
{
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ARG_MUST_BE_OF_TYPE(1, cmd_number);

    number* right =  (number*)_stack->pop_back();
    number* left = (number*)_stack->back();

    if ((mpfr_cmp_si(left->_value.mpfr, 0) != 0)
        || (mpfr_cmp_si(right->_value.mpfr, 0) != 0))
        mpfr_set_si(left->_value.mpfr, 1, floating_t::s_mpfr_rnd);
    else
        mpfr_set_si(left->_value.mpfr, 0, floating_t::s_mpfr_rnd);
}

void test_xor(void)
{
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ARG_MUST_BE_OF_TYPE(1, cmd_number);

    number* right =  (number*)_stack->pop_back();
    number* left = (number*)_stack->back();

    if (mpfr_cmp_si(left->_value.mpfr, 0) == 0)
    {
        if (mpfr_cmp_si(right->_value.mpfr, 0) != 0)
            mpfr_set_si(left->_value.mpfr, 1, floating_t::s_mpfr_rnd);
        else
            mpfr_set_si(left->_value.mpfr, 0, floating_t::s_mpfr_rnd);            
    }
    else
    {
        if (mpfr_cmp_si(right->_value.mpfr, 0) == 0)
            mpfr_set_si(left->_value.mpfr, 1, floating_t::s_mpfr_rnd);
        else
            mpfr_set_si(left->_value.mpfr, 0, floating_t::s_mpfr_rnd);            
    }
}

void test_not(void)
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    number* left = (number*)_stack->back();
    if (mpfr_cmp_si(left->_value.mpfr, 0) == 0)
        mpfr_set_si(left->_value.mpfr, 1, floating_t::s_mpfr_rnd);
    else
        mpfr_set_si(left->_value.mpfr, 0, floating_t::s_mpfr_rnd);            
}

void same(void)
{
    eq();
}
