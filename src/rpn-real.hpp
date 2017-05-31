void plus()
{
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ARG_MUST_BE_OF_TYPE(1, cmd_number);

    number* right = (number*)_stack->pop_back();
    number* left = (number*)_stack->back();
    CHECK_MPFR(mpfr_add(left->_value.mpfr, left->_value.mpfr, right->_value.mpfr, floating_t::s_mpfr_rnd));
}

void minus()
{
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ARG_MUST_BE_OF_TYPE(1, cmd_number);

    number* right = (number*)_stack->pop_back();
    number* left = (number*)_stack->back();
    CHECK_MPFR(mpfr_sub(left->_value.mpfr, left->_value.mpfr, right->_value.mpfr, floating_t::s_mpfr_rnd));
}

void mul()
{
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ARG_MUST_BE_OF_TYPE(1, cmd_number);

    number* right = (number*)_stack->pop_back();
    number* left = (number*)_stack->back();
    CHECK_MPFR(mpfr_mul(left->_value.mpfr, left->_value.mpfr, right->_value.mpfr, floating_t::s_mpfr_rnd));
}

void div()
{
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ARG_MUST_BE_OF_TYPE(1, cmd_number);

    number* right = (number*)_stack->pop_back();
    number* left = (number*)_stack->back();
    CHECK_MPFR(mpfr_div(left->_value.mpfr, left->_value.mpfr, right->_value.mpfr, floating_t::s_mpfr_rnd));
}

void neg()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    number* left = (number*)_stack->back();
    CHECK_MPFR(mpfr_neg(left->_value.mpfr, left->_value.mpfr, floating_t::s_mpfr_rnd));
}

void inv()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    number* left = (number*)_stack->back();
    CHECK_MPFR(mpfr_si_div(left->_value.mpfr, 1L, left->_value.mpfr, floating_t::s_mpfr_rnd));
}

void purcent()
{
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ARG_MUST_BE_OF_TYPE(1, cmd_number);

    number* right = (number*)_stack->pop_back();
    number* left = (number*)_stack->back();

    CHECK_MPFR(mpfr_mul(left->_value.mpfr, left->_value.mpfr, right->_value.mpfr, floating_t::s_mpfr_rnd));
    CHECK_MPFR(mpfr_div_si(left->_value.mpfr, left->_value.mpfr, 100L, floating_t::s_mpfr_rnd));
}

void purcentCH()
{
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ARG_MUST_BE_OF_TYPE(1, cmd_number);

    number* right = (number*)_stack->pop_back();
    number* left = (number*)_stack->back();

    CHECK_MPFR(mpfr_mul_si(right->_value.mpfr, right->_value.mpfr, 100L, floating_t::s_mpfr_rnd));
    CHECK_MPFR(mpfr_div(left->_value.mpfr, right->_value.mpfr, left->_value.mpfr, floating_t::s_mpfr_rnd));
}

void power()
{
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ARG_MUST_BE_OF_TYPE(1, cmd_number);

    number* right = (number*)_stack->pop_back();
    number* left = (number*)_stack->back();
    CHECK_MPFR(mpfr_pow(left->_value.mpfr, left->_value.mpfr, right->_value.mpfr, floating_t::s_mpfr_rnd));
}

void squareroot()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    number* left = (number*)_stack->back();
    CHECK_MPFR(mpfr_sqrt(left->_value.mpfr, left->_value.mpfr, floating_t::s_mpfr_rnd));
}

void square()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    number* left = (number*)_stack->back();
    CHECK_MPFR(mpfr_sqr(left->_value.mpfr, left->_value.mpfr, floating_t::s_mpfr_rnd));
}

void modulo()
{
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ARG_MUST_BE_OF_TYPE(1, cmd_number);

    number* right = (number*)_stack->pop_back();
    number* left = (number*)_stack->back();

    CHECK_MPFR(mpfr_fmod(left->_value.mpfr, left->_value.mpfr, right->_value.mpfr, floating_t::s_mpfr_rnd));
}

void rpn_abs()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    number* left = (number*)_stack->back();

    CHECK_MPFR(mpfr_abs(left->_value.mpfr, left->_value.mpfr, floating_t::s_mpfr_rnd));
}

void hex()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ((number*)_stack->back())->_representation = number::hex;
}

void dec()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ((number*)_stack->back())->_representation = number::dec;
}

void fact()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    // fact(n) = gamma(n+1)
    number* left = (number*)_stack->back();
    number* right = (number*)_stack->allocate_back(number::calc_size(), cmd_number);
    right->_value = 1L;
    plus();

    CHECK_MPFR(mpfr_gamma(left->_value.mpfr, left->_value.mpfr, floating_t::s_mpfr_rnd));
}

void sign()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    // fact(n) = gamma(n+1)
    number* left = (number*)_stack->back();
    int result = mpfr_sgn(left->_value.mpfr);
    left->_value = (long)result;
}
