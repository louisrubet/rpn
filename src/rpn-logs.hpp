void rpn_e(void)
{
    number* euler = (number*)_stack->allocate_back(number::calc_size(), cmd_number);
    euler->_value = 1L;
    CHECK_MPFR(mpfr_exp(euler->_value.mpfr, euler->_value.mpfr, floating_t::s_mpfr_rnd));
}

void rpn_log()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    number* left = (number*)_stack->back();

    // x<0 -> log10(x) = log10(-x)+i*pi
    if (mpfr_cmp_si(left->_value.mpfr, 0) < 0)
    {
        stack::copy_and_push_back(*_stack, _stack->size()-1, _branch_stack);
        _stack->pop_back();
        left = (number*)_branch_stack.back();

        complex* cplx = (complex*)_stack->allocate_back(complex::calc_size(), cmd_complex);
        CHECK_MPFR(mpfr_neg(cplx->re()->mpfr, left->_value.mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_log10(cplx->re()->mpfr, cplx->re()->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_const_pi(cplx->im()->mpfr, floating_t::s_mpfr_rnd));
        _branch_stack.pop_back();
    }
    else
        CHECK_MPFR(mpfr_log10(left->_value.mpfr, left->_value.mpfr, floating_t::s_mpfr_rnd));
}

void rpn_alog()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    floating_t* left = &((number*)_stack->get_obj(0))->_value;
    CHECK_MPFR(mpfr_exp10(left->mpfr, left->mpfr, floating_t::s_mpfr_rnd));
}

void rpn_log2()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    number* left = (number*)_stack->back();

    // x<0 -> log2(x) = log2(-x)+i*pi
    if (mpfr_cmp_si(left->_value.mpfr, 0) < 0)
    {
        stack::copy_and_push_back(*_stack, _stack->size()-1, _branch_stack);
        _stack->pop_back();
        left = (number*)_branch_stack.back();

        complex* cplx = (complex*)_stack->allocate_back(complex::calc_size(), cmd_complex);
        CHECK_MPFR(mpfr_neg(cplx->re()->mpfr, left->_value.mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_log2(cplx->re()->mpfr, cplx->re()->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_const_pi(cplx->im()->mpfr, floating_t::s_mpfr_rnd));
        _branch_stack.pop_back();
    }
    else
        CHECK_MPFR(mpfr_log2(left->_value.mpfr, left->_value.mpfr, floating_t::s_mpfr_rnd));
}

void rpn_alog2()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    floating_t* left = &((number*)_stack->get_obj(0))->_value;
    CHECK_MPFR(mpfr_exp2(left->mpfr, left->mpfr, floating_t::s_mpfr_rnd));
}

void rpn_ln()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    number* left = (number*)_stack->back();

    // x<0 -> ln(x) = ln(-x)+i*pi
    if (mpfr_cmp_si(left->_value.mpfr, 0) < 0)
    {
        stack::copy_and_push_back(*_stack, _stack->size()-1, _branch_stack);
        _stack->pop_back();
        left = (number*)_branch_stack.back();

        complex* cplx = (complex*)_stack->allocate_back(complex::calc_size(), cmd_complex);
        CHECK_MPFR(mpfr_neg(cplx->re()->mpfr, left->_value.mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_log(cplx->re()->mpfr, cplx->re()->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_const_pi(cplx->im()->mpfr, floating_t::s_mpfr_rnd));
        _branch_stack.pop_back();
    }
    else
        CHECK_MPFR(mpfr_log(left->_value.mpfr, left->_value.mpfr, floating_t::s_mpfr_rnd));
}

void rpn_exp()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    floating_t* left = &((number*)_stack->get_obj(0))->_value;
    CHECK_MPFR(mpfr_exp(left->mpfr, left->mpfr, floating_t::s_mpfr_rnd));
}

void rpn_sinh()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    floating_t* left = &((number*)_stack->get_obj(0))->_value;
    CHECK_MPFR(mpfr_sinh(left->mpfr, left->mpfr, floating_t::s_mpfr_rnd));
}

void rpn_asinh()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    floating_t* left = &((number*)_stack->get_obj(0))->_value;
    CHECK_MPFR(mpfr_asinh(left->mpfr, left->mpfr, floating_t::s_mpfr_rnd));
}

void rpn_cosh()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    floating_t* left = &((number*)_stack->get_obj(0))->_value;
    CHECK_MPFR(mpfr_cosh(left->mpfr, left->mpfr, floating_t::s_mpfr_rnd));
}

void rpn_acosh()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    floating_t* left = &((number*)_stack->get_obj(0))->_value;
    CHECK_MPFR(mpfr_cosh(left->mpfr, left->mpfr, floating_t::s_mpfr_rnd));
}

void rpn_tanh()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    floating_t* left = &((number*)_stack->get_obj(0))->_value;
    CHECK_MPFR(mpfr_tanh(left->mpfr, left->mpfr, floating_t::s_mpfr_rnd));
}

void rpn_atanh()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    floating_t* left = &((number*)_stack->get_obj(0))->_value;
    CHECK_MPFR(mpfr_atanh(left->mpfr, left->mpfr, floating_t::s_mpfr_rnd));
}
