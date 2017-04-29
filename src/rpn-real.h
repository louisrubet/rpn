void plus()
{
    MIN_ARGUMENTS(2);

    // float
    if (IS_ARG_TYPE(0, cmd_number))
    {
        ARG_MUST_BE_OF_TYPE(1, cmd_number);

        number* right = (number*)_stack->pop_back();
        number* left = (number*)_stack->back();
        CHECK_MPFR(mpfr_add(left->_value.mpfr, left->_value.mpfr, right->_value.mpfr, s_mpfr_rnd));
    }
    // binary
    else if (IS_ARG_TYPE(0, cmd_binary))
    {
        ARG_MUST_BE_OF_TYPE(1, cmd_binary);
        
        binary* right = (binary*)_stack->pop_back();
        binary* left = (binary*)_stack->back();
        left->_value += right->_value;
    }
    else
        ERR_CONTEXT(ret_bad_operand_type);
}

void minus()
{
    MIN_ARGUMENTS(2);

    // float
    if (IS_ARG_TYPE(0, cmd_number))
    {
        ARG_MUST_BE_OF_TYPE(1, cmd_number);

        number* right = (number*)_stack->pop_back();
        number* left = (number*)_stack->back();
        CHECK_MPFR(mpfr_sub(left->_value.mpfr, left->_value.mpfr, right->_value.mpfr, s_mpfr_rnd));
    }
    // binary
    else if (IS_ARG_TYPE(0, cmd_binary))
    {
        ARG_MUST_BE_OF_TYPE(1, cmd_binary);

        binary* right = (binary*)_stack->pop_back();
        binary* left = (binary*)_stack->back();
        left->_value -= right->_value;
    }
    else
        ERR_CONTEXT(ret_bad_operand_type);
}

void mul()
{
    // float
    if (IS_ARG_TYPE(0, cmd_number))
    {
        ARG_MUST_BE_OF_TYPE(1, cmd_number);

        number* right = (number*)_stack->pop_back();
        number* left = (number*)_stack->back();
        CHECK_MPFR(mpfr_mul(left->_value.mpfr, left->_value.mpfr, right->_value.mpfr, s_mpfr_rnd));
    }
    // binary
    else if (IS_ARG_TYPE(0, cmd_binary))
    {
        ARG_MUST_BE_OF_TYPE(1, cmd_binary);

        binary* right = (binary*)_stack->pop_back();
        binary* left = (binary*)_stack->back();
        left->_value *= right->_value;
    }
    else
        ERR_CONTEXT(ret_bad_operand_type);
}

void div()
{
    MIN_ARGUMENTS(2);

    // float
    if (IS_ARG_TYPE(0, cmd_number))
    {
        ARG_MUST_BE_OF_TYPE(1, cmd_number);

        number* right = (number*)_stack->pop_back();
        number* left = (number*)_stack->back();
        CHECK_MPFR(mpfr_div(left->_value.mpfr, left->_value.mpfr, right->_value.mpfr, s_mpfr_rnd));
    }
    // binary
    else if (IS_ARG_TYPE(0, cmd_binary))
    {
        ARG_MUST_BE_OF_TYPE(1, cmd_binary);
        if (((binary*)_stack->get_obj(0))->_value == 0)
        {
            ERR_CONTEXT(ret_div_by_zero);
        }
        else
        {
            binary* right = (binary*)_stack->pop_back();
            binary* left = (binary*)_stack->back();
            left->_value /= right->_value;
        }
    }
    else
        ERR_CONTEXT(ret_bad_operand_type);
}

void neg()
{
    MIN_ARGUMENTS(1);

    // float
    if (IS_ARG_TYPE(0, cmd_number))
    {
        number* left = (number*)_stack->back();
        CHECK_MPFR(mpfr_neg(left->_value.mpfr, left->_value.mpfr, s_mpfr_rnd));
    }
    // binary
    else if (IS_ARG_TYPE(0, cmd_binary))
        ((binary*)_stack->back())->_value *= -1;
    else
        ERR_CONTEXT(ret_bad_operand_type);
}

void inv()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    number* left = (number*)_stack->back();
    CHECK_MPFR(mpfr_si_div(left->_value.mpfr, 1L, left->_value.mpfr, s_mpfr_rnd));
}

void purcent()
{
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ARG_MUST_BE_OF_TYPE(1, cmd_number);

    number* right = (number*)_stack->pop_back();
    number* left = (number*)_stack->back();

    CHECK_MPFR(mpfr_mul(left->_value.mpfr, left->_value.mpfr, right->_value.mpfr, s_mpfr_rnd));
    CHECK_MPFR(mpfr_div_si(left->_value.mpfr, left->_value.mpfr, 100L, s_mpfr_rnd));
}

void purcentCH()
{
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ARG_MUST_BE_OF_TYPE(1, cmd_number);

    number* right = (number*)_stack->pop_back();
    number* left = (number*)_stack->back();

    CHECK_MPFR(mpfr_mul_si(right->_value.mpfr, right->_value.mpfr, 100L, s_mpfr_rnd));
    CHECK_MPFR(mpfr_div(left->_value.mpfr, right->_value.mpfr, left->_value.mpfr, s_mpfr_rnd));
}

void power()
{
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ARG_MUST_BE_OF_TYPE(1, cmd_number);

    number* right = (number*)_stack->pop_back();
    number* left = (number*)_stack->back();
    CHECK_MPFR(mpfr_pow(left->_value.mpfr, left->_value.mpfr, right->_value.mpfr, s_mpfr_rnd));
}

void squareroot()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    number* left = (number*)_stack->back();
    CHECK_MPFR(mpfr_sqrt(left->_value.mpfr, left->_value.mpfr, s_mpfr_rnd));
}

void square()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    number* left = (number*)_stack->back();
    CHECK_MPFR(mpfr_sqr(left->_value.mpfr, left->_value.mpfr, s_mpfr_rnd));
}

void modulo()
{
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ARG_MUST_BE_OF_TYPE(1, cmd_number);

    number* right = (number*)_stack->pop_back();
    number* left = (number*)_stack->back();

    CHECK_MPFR(mpfr_fmod(left->_value.mpfr, left->_value.mpfr, right->_value.mpfr, s_mpfr_rnd));
}
