//
void rpn_pi(void)
{
    number* pi = (number*)_stack->allocate_back(number::calc_size(), cmd_number);
    CHECK_MPFR(mpfr_const_pi(pi->_value.mpfr, floating_t::s_mpfr_rnd));
}

void rpn_d2r(void)
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    // add pi on stack
    rpn_pi();

    floating_t* pi = &((number*)_stack->pop_back())->_value;
    floating_t* left = &((number*)_stack->get_obj(0))->_value;
    
    CHECK_MPFR(mpfr_mul(left->mpfr, left->mpfr, pi->mpfr, floating_t::s_mpfr_rnd));
    CHECK_MPFR(mpfr_div_si(left->mpfr, left->mpfr, 180, floating_t::s_mpfr_rnd));
}

void rpn_r2d(void)
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    // add pi on stack
    rpn_pi();

    floating_t* pi = &((number*)_stack->pop_back())->_value;
    floating_t* left = &((number*)_stack->get_obj(0))->_value;
    
    CHECK_MPFR(mpfr_div(left->mpfr, left->mpfr, pi->mpfr, floating_t::s_mpfr_rnd));
    CHECK_MPFR(mpfr_mul_si(left->mpfr, left->mpfr, 180, floating_t::s_mpfr_rnd));
}

void rpn_sin(void)
{
    MIN_ARGUMENTS(1);

    if (_stack->get_type(0) == cmd_number)
    {        
        floating_t* left = &((number*)_stack->get_obj(0))->_value;
        CHECK_MPFR(mpfr_sin(left->mpfr, left->mpfr, floating_t::s_mpfr_rnd));
    }
    else if (_stack->get_type(0) == cmd_complex)
    {
        // sin(x+iy)=sin(x)cosh(y)+icos(x)sinh(y)
        stack::copy_and_push_back(*_stack, _stack->size()-1, _calc_stack);
        
        floating_t* tmp = &((number*)_calc_stack.allocate_back(number::calc_size(), cmd_number))->_value;
        floating_t* x = ((complex*)_calc_stack.get_obj(1))->re();
        floating_t* y = ((complex*)_calc_stack.get_obj(1))->im();

        floating_t* re = ((complex*)_stack->get_obj(0))->re();
        floating_t* im = ((complex*)_stack->get_obj(0))->im();
        
        CHECK_MPFR(mpfr_sin(re->mpfr, x->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_cosh(tmp->mpfr, y->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_mul(re->mpfr, re->mpfr, tmp->mpfr, floating_t::s_mpfr_rnd));

        CHECK_MPFR(mpfr_cos(im->mpfr, x->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_sinh(tmp->mpfr, y->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_mul(im->mpfr, im->mpfr, tmp->mpfr, floating_t::s_mpfr_rnd));

        _calc_stack.pop_back(2);
    }
    else
        ERR_CONTEXT(ret_bad_operand_type);
}

void rpn_asin(void)
{
    MIN_ARGUMENTS(1);

    if (_stack->get_type(0) == cmd_number)
    {        
        floating_t* left = &((number*)_stack->get_obj(0))->_value;
        CHECK_MPFR(mpfr_asin(left->mpfr, left->mpfr, floating_t::s_mpfr_rnd));
    }
    else if (_stack->get_type(0) == cmd_complex)
    {
        number* num;
        complex* i;
        
        //asin(z)=-iln(iz+sqrt(1-z*z))
        stack::copy_and_push_back(*_stack, _stack->size()-1, _calc_stack);

        i = (complex*)_calc_stack.get_obj(0);
        CHECK_MPFR(mpfr_set_d(i->re()->mpfr, 0.0, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_set_d(i->im()->mpfr, 1.0, floating_t::s_mpfr_rnd));
        
        rpn_dup();
        rpn_square();
        num = (number*)_stack->allocate_back(number::calc_size(), cmd_number);
        CHECK_MPFR(mpfr_set_d(num->_value.mpfr, 1.0, floating_t::s_mpfr_rnd));
        rpn_minus();
        rpn_neg();
        rpn_squareroot();
        rpn_swap();
        stack::copy_and_push_back(_calc_stack, 0, *_stack);
        rpn_mul();
        rpn_plus();
        rpn_ln();
        stack::copy_and_push_back(_calc_stack, 0, *_stack);
        rpn_mul();
        rpn_neg();
        _calc_stack.pop_back();
    }
    else
        ERR_CONTEXT(ret_bad_operand_type);
}

void rpn_cos(void)
{
    MIN_ARGUMENTS(1);

    if (_stack->get_type(0) == cmd_number)
    {        
        floating_t* left = &((number*)_stack->get_obj(0))->_value;
        CHECK_MPFR(mpfr_cos(left->mpfr, left->mpfr, floating_t::s_mpfr_rnd));
    }
    else if (_stack->get_type(0) == cmd_complex)
    {
        // cos(x+iy) = cos(x)cosh(y) - isin(x)sinh(y)
        stack::copy_and_push_back(*_stack, _stack->size()-1, _calc_stack);
        
        floating_t* tmp = &((number*)_calc_stack.allocate_back(number::calc_size(), cmd_number))->_value;
        floating_t* x = ((complex*)_calc_stack.get_obj(1))->re();
        floating_t* y = ((complex*)_calc_stack.get_obj(1))->im();

        floating_t* re = ((complex*)_stack->get_obj(0))->re();
        floating_t* im = ((complex*)_stack->get_obj(0))->im();
        
        CHECK_MPFR(mpfr_cos(re->mpfr, x->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_cosh(tmp->mpfr, y->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_mul(re->mpfr, re->mpfr, tmp->mpfr, floating_t::s_mpfr_rnd));

        CHECK_MPFR(mpfr_sin(im->mpfr, x->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_sinh(tmp->mpfr, y->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_mul(im->mpfr, im->mpfr, tmp->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_neg(im->mpfr, im->mpfr, floating_t::s_mpfr_rnd));

        _calc_stack.pop_back(2);
    }
    else
        ERR_CONTEXT(ret_bad_operand_type);
}

void rpn_acos(void)
{
    MIN_ARGUMENTS(1);

    if (_stack->get_type(0) == cmd_number)
    {        
        floating_t* left = &((number*)_stack->get_obj(0))->_value;
        CHECK_MPFR(mpfr_acos(left->mpfr, left->mpfr, floating_t::s_mpfr_rnd));
    }
    else if (_stack->get_type(0) == cmd_complex)
    {
        //acos(z)=pi/2-asin(z)
        rpn_asin();
        rpn_pi();
        number* num = (number*)_stack->allocate_back(number::calc_size(), cmd_number);
        CHECK_MPFR(mpfr_set_d(num->_value.mpfr, 2.0, floating_t::s_mpfr_rnd));
        rpn_div();
        rpn_minus();
        rpn_neg();
    }
    else
        ERR_CONTEXT(ret_bad_operand_type);
}

void rpn_tan(void)
{
    MIN_ARGUMENTS(1);

    if (_stack->get_type(0) == cmd_number)
    {        
        floating_t* left = &((number*)_stack->get_obj(0))->_value;
        CHECK_MPFR(mpfr_tan(left->mpfr, left->mpfr, floating_t::s_mpfr_rnd));
    }
    else if (_stack->get_type(0) == cmd_complex)
    {
        // tan(x+iy) = (sin(2x)+isinh(2y)) / cosh(2y)+cos(2x)
        stack::copy_and_push_back(*_stack, _stack->size()-1, _calc_stack);
        
        floating_t* tmp = &((number*)_calc_stack.allocate_back(number::calc_size(), cmd_number))->_value;
        floating_t* x = ((complex*)_calc_stack.get_obj(1))->re();
        floating_t* y = ((complex*)_calc_stack.get_obj(1))->im();

        floating_t* re = ((complex*)_stack->get_obj(0))->re();
        floating_t* im = ((complex*)_stack->get_obj(0))->im();
        
        //x->2x
        CHECK_MPFR(mpfr_mul_si(x->mpfr, x->mpfr, 2, floating_t::s_mpfr_rnd));
        //y->2y
        CHECK_MPFR(mpfr_mul_si(y->mpfr, y->mpfr, 2, floating_t::s_mpfr_rnd));
        
        //sin(2x)+sinh(2y)
        CHECK_MPFR(mpfr_sin(re->mpfr, x->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_sinh(im->mpfr, y->mpfr, floating_t::s_mpfr_rnd));
        
        //cosh(2y)+cos(2x)
        CHECK_MPFR(mpfr_cosh(tmp->mpfr, y->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_cos(x->mpfr, x->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_add(tmp->mpfr, tmp->mpfr, x->mpfr, floating_t::s_mpfr_rnd));

        //sin(2x)+sinh(2y) / (cosh(2y)+cos(2x))
        CHECK_MPFR(mpfr_div(re->mpfr, re->mpfr, tmp->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_div(im->mpfr, im->mpfr, tmp->mpfr, floating_t::s_mpfr_rnd));

        _calc_stack.pop_back(2);
    }
    else
        ERR_CONTEXT(ret_bad_operand_type);
}

void rpn_atan(void)
{
    MIN_ARGUMENTS(1);

    if (_stack->get_type(0) == cmd_number)
    {        
        floating_t* left = &((number*)_stack->get_obj(0))->_value;
        CHECK_MPFR(mpfr_atan(left->mpfr, left->mpfr, floating_t::s_mpfr_rnd));
    }
    else if (_stack->get_type(0) == cmd_complex)
    {
        number* num;
        complex* i;
        
        //atan(z)=0.5i(ln((1-iz)/(1+iz))
        stack::copy_and_push_back(*_stack, _stack->size()-1, _calc_stack);

        i = (complex*)_calc_stack.get_obj(0);
        CHECK_MPFR(mpfr_set_d(i->re()->mpfr, 0.0, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_set_d(i->im()->mpfr, 1.0, floating_t::s_mpfr_rnd));
        
        stack::copy_and_push_back(_calc_stack, 0, *_stack);
        rpn_mul();
        num = (number*)_stack->allocate_back(number::calc_size(), cmd_number);
        CHECK_MPFR(mpfr_set_d(num->_value.mpfr, 1.0, floating_t::s_mpfr_rnd));
        rpn_minus();//iz-1
        rpn_neg();//1-iz
        rpn_dup();
        rpn_neg();//iz-1
        num = (number*)_stack->allocate_back(number::calc_size(), cmd_number);
        CHECK_MPFR(mpfr_set_d(num->_value.mpfr, 2.0, floating_t::s_mpfr_rnd));
        rpn_plus();//iz+1
        rpn_div();

        rpn_ln();
        CHECK_MPFR(mpfr_set_d(i->im()->mpfr, 0.5, floating_t::s_mpfr_rnd));
        stack::copy_and_push_back(_calc_stack, 0, *_stack);
        rpn_mul();

        _calc_stack.pop_back();
    }
    else
        ERR_CONTEXT(ret_bad_operand_type);
}
