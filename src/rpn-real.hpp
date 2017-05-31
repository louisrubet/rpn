void plus()
{
    MIN_ARGUMENTS(2);

    // adding strings
    if (_stack->get_type(0) == cmd_string && _stack->get_type(1) == cmd_string)
    {
        unsigned int left_str_size = ((ostring*)_stack->get_obj(1))->_len;
        unsigned int right_str_size = ((ostring*)_stack->get_obj(0))->_len;

        stack::copy_and_push_back(*_stack, _stack->size()-2, _branch_stack);
        stack::copy_and_push_back(*_stack, _stack->size()-1, _branch_stack);
        (void)_stack->pop_back();
        (void)_stack->pop_back();

        ostring* str = (ostring*)_stack->allocate_back(left_str_size+right_str_size+1+sizeof(ostring), cmd_string);
        str->_len = left_str_size+right_str_size;

        strncpy(str->_value, ((ostring*)_branch_stack.get_obj(1))->_value, left_str_size);
        strncat(str->_value, ((ostring*)_branch_stack.get_obj(0))->_value, right_str_size);
        _branch_stack.pop_back();
        _branch_stack.pop_back();
    }
    // adding numbers
    else
    {
        ARG_MUST_BE_OF_TYPE(0, cmd_number);
        ARG_MUST_BE_OF_TYPE(1, cmd_number);
        number* right = (number*)_stack->pop_back();
        number* left = (number*)_stack->back();
        CHECK_MPFR(mpfr_add(left->_value.mpfr, left->_value.mpfr, right->_value.mpfr, floating_t::s_mpfr_rnd));
    }
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

void mant()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    number* left = (number*)_stack->back();

    if (mpfr_number_p(left->_value.mpfr))
    {
        if (mpfr_zero_p(left->_value.mpfr))
            left->_value = 0.0;
        else
        {
            mpfr_abs(left->_value.mpfr, left->_value.mpfr, floating_t::s_mpfr_rnd);

            number* one = (number*)_branch_stack.allocate_back(number::calc_size(), cmd_number);
            number* ten = (number*)_branch_stack.allocate_back(number::calc_size(), cmd_number);
            ten->_value = 10L;

            one->_value = 1L;
            while (mpfr_greaterequal_p(left->_value.mpfr, one->_value.mpfr))
                mpfr_div(left->_value.mpfr, left->_value.mpfr, ten->_value.mpfr, floating_t::s_mpfr_rnd);

            one->_value = 0.1;
            while (mpfr_less_p(left->_value.mpfr, one->_value.mpfr))
                mpfr_mul(left->_value.mpfr, left->_value.mpfr, ten->_value.mpfr, floating_t::s_mpfr_rnd);

            _branch_stack.pop_back();
            _branch_stack.pop_back();
        }
    }
    else
        ERR_CONTEXT(ret_out_of_range);
}

void xpon()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    number* left = (number*)_stack->back();

    if (mpfr_number_p(left->_value.mpfr))
    {
        if (mpfr_zero_p(left->_value.mpfr))
            left->_value = 0.0;
        else
        {
            double exponant = 0.0;
            mpfr_abs(left->_value.mpfr, left->_value.mpfr, floating_t::s_mpfr_rnd);

            number* one = (number*)_branch_stack.allocate_back(number::calc_size(), cmd_number);
            number* ten = (number*)_branch_stack.allocate_back(number::calc_size(), cmd_number);
            ten->_value = 10L;

            one->_value = 1L;
            while (mpfr_greaterequal_p(left->_value.mpfr, one->_value.mpfr))
            {
                mpfr_div(left->_value.mpfr, left->_value.mpfr, ten->_value.mpfr, floating_t::s_mpfr_rnd);
                exponant+=1.0;
            }

            one->_value = 0.1;
            while (mpfr_less_p(left->_value.mpfr, one->_value.mpfr))
            {
                mpfr_mul(left->_value.mpfr, left->_value.mpfr, ten->_value.mpfr, floating_t::s_mpfr_rnd);
                exponant-=1.0;
            }

            left->_value = exponant;

            _branch_stack.pop_back();
            _branch_stack.pop_back();
        }
    }
    else
        ERR_CONTEXT(ret_out_of_range);
}

void rpn_min()
{
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ARG_MUST_BE_OF_TYPE(1, cmd_number);

    number* right = (number*)_stack->pop_back();
    number* left = (number*)_stack->back();

    CHECK_MPFR(mpfr_min(left->_value.mpfr, left->_value.mpfr, right->_value.mpfr, floating_t::s_mpfr_rnd));
}

void rpn_max()
{
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ARG_MUST_BE_OF_TYPE(1, cmd_number);

    number* right = (number*)_stack->pop_back();
    number* left = (number*)_stack->back();

    CHECK_MPFR(mpfr_max(left->_value.mpfr, left->_value.mpfr, right->_value.mpfr, floating_t::s_mpfr_rnd));
}
