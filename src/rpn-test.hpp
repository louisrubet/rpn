void sup(void)
{
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ARG_MUST_BE_OF_TYPE(1, cmd_number);

    number* right =  (number*)_stack->pop_back();
    number* left = (number*)_stack->back();

    if (mpfr_cmp(left->_value.mpfr, right->_value.mpfr) > 0)
        mpfr_set_si(left->_value.mpfr, 1, floating_t::s_mpfr_rnd);
    else
        mpfr_set_si(left->_value.mpfr, 0, floating_t::s_mpfr_rnd);
}

void sup_eq(void)
{
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ARG_MUST_BE_OF_TYPE(1, cmd_number);

    number* right =  (number*)_stack->pop_back();
    number* left = (number*)_stack->back();

    if (mpfr_cmp(left->_value.mpfr, right->_value.mpfr) >= 0)
        mpfr_set_si(left->_value.mpfr, 1, floating_t::s_mpfr_rnd);
    else
        mpfr_set_si(left->_value.mpfr, 0, floating_t::s_mpfr_rnd);
}

void inf(void)
{
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ARG_MUST_BE_OF_TYPE(1, cmd_number);

    number* right =  (number*)_stack->pop_back();
    number* left = (number*)_stack->back();

    if (mpfr_cmp(left->_value.mpfr, right->_value.mpfr) < 0)
        mpfr_set_si(left->_value.mpfr, 1, floating_t::s_mpfr_rnd);
    else
        mpfr_set_si(left->_value.mpfr, 0, floating_t::s_mpfr_rnd);
}

void inf_eq(void)
{
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ARG_MUST_BE_OF_TYPE(1, cmd_number);

    number* right =  (number*)_stack->pop_back();
    number* left = (number*)_stack->back();

    if (mpfr_cmp(left->_value.mpfr, right->_value.mpfr) <= 0)
        mpfr_set_si(left->_value.mpfr, 1, floating_t::s_mpfr_rnd);
    else
        mpfr_set_si(left->_value.mpfr, 0, floating_t::s_mpfr_rnd);
}

void diff(void)
{
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ARG_MUST_BE_OF_TYPE(1, cmd_number);

    number* right =  (number*)_stack->pop_back();
    number* left = (number*)_stack->back();

    if (mpfr_cmp(left->_value.mpfr, right->_value.mpfr) != 0)
        mpfr_set_si(left->_value.mpfr, 1, floating_t::s_mpfr_rnd);
    else
        mpfr_set_si(left->_value.mpfr, 0, floating_t::s_mpfr_rnd);
}

void eq(void)
{
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ARG_MUST_BE_OF_TYPE(1, cmd_number);

    number* right =  (number*)_stack->pop_back();
    number* left = (number*)_stack->back();

    if (mpfr_cmp(left->_value.mpfr, right->_value.mpfr) == 0)
        mpfr_set_si(left->_value.mpfr, 1, floating_t::s_mpfr_rnd);
    else
        mpfr_set_si(left->_value.mpfr, 0, floating_t::s_mpfr_rnd);
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
