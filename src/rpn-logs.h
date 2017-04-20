void rpn_e(void)
{
    number num;
    CHECK_MPFR(mpfr_const_euler(&num._value.mpfr, s_mpfr_rnd));
    _stack->push_back(&num, num.size(), cmd_number);
    ((number*)_stack->back())->ensure_significand();
}

void rpn_log()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    floating_t* left = &((number*)_stack->get_obj(0))->_value;
    CHECK_MPFR(mpfr_log10(&left->mpfr, &left->mpfr, s_mpfr_rnd));
}

void rpn_alog()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    floating_t* left = &((number*)_stack->get_obj(0))->_value;
    CHECK_MPFR(mpfr_exp10(&left->mpfr, &left->mpfr, s_mpfr_rnd));
}

void rpn_ln()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    floating_t* left = &((number*)_stack->get_obj(0))->_value;
    CHECK_MPFR(mpfr_log(&left->mpfr, &left->mpfr, s_mpfr_rnd));
}

void rpn_exp()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    floating_t* left = &((number*)_stack->get_obj(0))->_value;
    CHECK_MPFR(mpfr_exp(&left->mpfr, &left->mpfr, s_mpfr_rnd));
}

void rpn_sinh()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    floating_t* left = &((number*)_stack->get_obj(0))->_value;
    CHECK_MPFR(mpfr_sinh(&left->mpfr, &left->mpfr, s_mpfr_rnd));
}

void rpn_asinh()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    floating_t* left = &((number*)_stack->get_obj(0))->_value;
    CHECK_MPFR(mpfr_asinh(&left->mpfr, &left->mpfr, s_mpfr_rnd));
}

void rpn_cosh()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    floating_t* left = &((number*)_stack->get_obj(0))->_value;
    CHECK_MPFR(mpfr_cosh(&left->mpfr, &left->mpfr, s_mpfr_rnd));
}

void rpn_acosh()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    floating_t* left = &((number*)_stack->get_obj(0))->_value;
    CHECK_MPFR(mpfr_cosh(&left->mpfr, &left->mpfr, s_mpfr_rnd));
}

void rpn_tanh()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    floating_t* left = &((number*)_stack->get_obj(0))->_value;
    CHECK_MPFR(mpfr_tanh(&left->mpfr, &left->mpfr, s_mpfr_rnd));
}

void rpn_atanh()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    floating_t* left = &((number*)_stack->get_obj(0))->_value;
    CHECK_MPFR(mpfr_atanh(&left->mpfr, &left->mpfr, s_mpfr_rnd));
}
