void rpn_e(void)
{
    number* euler = (number*)_stack->allocate_back(number::calc_size(), cmd_number);
    euler->_value = 1L;
    CHECK_MPFR(mpfr_exp(euler->_value.mpfr, euler->_value.mpfr, floating_t::s_mpfr_rnd));
}

void rpn_log10()
{
    MIN_ARGUMENTS(1);

    if (_stack->get_type(0) == cmd_number || _stack->get_type(0) == cmd_complex)
    {       
        //log10(z)=ln(z)/ln(10)
        rpn_ln();

        number* ten = (number*)_stack->allocate_back(number::calc_size(), cmd_number);
        CHECK_MPFR(mpfr_set_d(ten->_value.mpfr, 10.0, floating_t::s_mpfr_rnd));
        rpn_ln();
        div();
    }
    else
        ERR_CONTEXT(ret_bad_operand_type);
}

void rpn_alog10()
{
    MIN_ARGUMENTS(1);

    if (_stack->get_type(0) == cmd_number || _stack->get_type(0) == cmd_complex)
    {       
        floating_t* left = &((number*)_stack->get_obj(0))->_value;
        number* ten = (number*)_stack->allocate_back(number::calc_size(), cmd_number);
        CHECK_MPFR(mpfr_set_d(ten->_value.mpfr, 10.0, floating_t::s_mpfr_rnd));
        rpn_ln();
        mul();
        rpn_exp();
    }
}

void rpn_log2()
{
    MIN_ARGUMENTS(1);

    if (_stack->get_type(0) == cmd_number || _stack->get_type(0) == cmd_complex)
    {
        //log2(z)=ln(z)/ln(2)
        rpn_ln();

        number* two = (number*)_stack->allocate_back(number::calc_size(), cmd_number);
        CHECK_MPFR(mpfr_set_d(two->_value.mpfr, 2.0, floating_t::s_mpfr_rnd));
        rpn_ln();
        div();
    }
    else
        ERR_CONTEXT(ret_bad_operand_type);
}

void rpn_alog2()
{
    MIN_ARGUMENTS(1);

    if (_stack->get_type(0) == cmd_number || _stack->get_type(0) == cmd_complex)
    {       
        floating_t* left = &((number*)_stack->get_obj(0))->_value;
        number* two = (number*)_stack->allocate_back(number::calc_size(), cmd_number);
        CHECK_MPFR(mpfr_set_d(two->_value.mpfr, 2.0, floating_t::s_mpfr_rnd));
        rpn_ln();
        mul();
        rpn_exp();
    }
}

void rpn_ln()
{
    MIN_ARGUMENTS(1);
    
    if (_stack->get_type(0) == cmd_number)
    {       
        number* left = (number*)_stack->back();

        // x<0 -> ln(x) = ln(-x)+i*pi
        if (mpfr_cmp_si(left->_value.mpfr, 0) < 0)
        {
            stack::copy_and_push_back(*_stack, _stack->size()-1, _calc_stack);
            _stack->pop_back();
            left = (number*)_calc_stack.back();

            complex* cplx = (complex*)_stack->allocate_back(complex::calc_size(), cmd_complex);
            CHECK_MPFR(mpfr_neg(cplx->re()->mpfr, left->_value.mpfr, floating_t::s_mpfr_rnd));
            CHECK_MPFR(mpfr_log(cplx->re()->mpfr, cplx->re()->mpfr, floating_t::s_mpfr_rnd));
            CHECK_MPFR(mpfr_const_pi(cplx->im()->mpfr, floating_t::s_mpfr_rnd));
            _calc_stack.pop_back();
        }
        else
            CHECK_MPFR(mpfr_log(left->_value.mpfr, left->_value.mpfr, floating_t::s_mpfr_rnd));
    }
    else if (_stack->get_type(0) == cmd_complex)
    {
        // ln(x+iy) = 0.5*ln(x*x+y*y) + i atan(x/y)
        stack::copy_and_push_back(*_stack, _stack->size()-1, _calc_stack);

        floating_t* x = ((complex*)_calc_stack.get_obj(0))->re();
        floating_t* y = ((complex*)_calc_stack.get_obj(0))->im();

        floating_t* re = ((complex*)_stack->get_obj(0))->re();
        floating_t* im = ((complex*)_stack->get_obj(0))->im();

        // 1. atan(x/y)
        CHECK_MPFR(mpfr_atan2(im->mpfr, y->mpfr, x->mpfr, floating_t::s_mpfr_rnd));

        // 2. 0.5*ln(x*x+y*y)
        CHECK_MPFR(mpfr_mul(x->mpfr, x->mpfr, x->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_mul(y->mpfr, y->mpfr, y->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_add(re->mpfr, x->mpfr, y->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_log(re->mpfr, re->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_mul_d(re->mpfr, re->mpfr, 0.5, floating_t::s_mpfr_rnd));
        
        _calc_stack.pop_back();
    }
    else
        ERR_CONTEXT(ret_bad_operand_type);
}

void rpn_exp()
{
    MIN_ARGUMENTS(1);
    
    if (_stack->get_type(0) == cmd_number)
    {       
        floating_t* left = &((number*)_stack->get_obj(0))->_value;
        CHECK_MPFR(mpfr_exp(left->mpfr, left->mpfr, floating_t::s_mpfr_rnd));
    }
    else if (_stack->get_type(0) == cmd_complex)
    {
        // exp(x)*(cos(y)+i sin(y))
        stack::copy_and_push_back(*_stack, _stack->size()-1, _calc_stack);

        floating_t* x = ((complex*)_calc_stack.get_obj(0))->re();
        floating_t* y = ((complex*)_calc_stack.get_obj(0))->im();

        floating_t* re = ((complex*)_stack->get_obj(0))->re();
        floating_t* im = ((complex*)_stack->get_obj(0))->im();

        CHECK_MPFR(mpfr_cos(re->mpfr, y->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_sin(im->mpfr, y->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_exp(x->mpfr, x->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_mul(re->mpfr, re->mpfr, x->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_mul(im->mpfr, im->mpfr, x->mpfr, floating_t::s_mpfr_rnd));

        _calc_stack.pop_back();
    }
    else
        ERR_CONTEXT(ret_bad_operand_type);    
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
