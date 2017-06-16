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
    else if (_stack->get_type(0) == cmd_number && _stack->get_type(1) == cmd_number)
    {
        number* right = (number*)_stack->pop_back();
        number* left = (number*)_stack->back();
        CHECK_MPFR(mpfr_add(left->_value.mpfr, left->_value.mpfr, right->_value.mpfr, floating_t::s_mpfr_rnd));
    }
    // adding complexes
    else if (_stack->get_type(0) == cmd_complex && _stack->get_type(1) == cmd_complex)
    {
        complex* right = (complex*)_stack->pop_back();
        complex* left = (complex*)_stack->back();
        CHECK_MPFR(mpfr_add(left->re()->mpfr, left->re()->mpfr, right->re()->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_add(left->im()->mpfr, left->im()->mpfr, right->im()->mpfr, floating_t::s_mpfr_rnd));
    }
    // adding complex+number
    else if (_stack->get_type(0) == cmd_number && _stack->get_type(1) == cmd_complex)
    {
        number* right = (number*)_stack->pop_back();
        complex* left = (complex*)_stack->back();
        CHECK_MPFR(mpfr_add(left->re()->mpfr, left->re()->mpfr, right->_value.mpfr, floating_t::s_mpfr_rnd));
    }
    // adding number+complex
    else if (_stack->get_type(0) == cmd_complex && _stack->get_type(1) == cmd_number)
    {
        swap();
        number* right = (number*)_stack->pop_back();
        complex* left = (complex*)_stack->back();
        CHECK_MPFR(mpfr_add(left->re()->mpfr, left->re()->mpfr, right->_value.mpfr, floating_t::s_mpfr_rnd));
    }
    else
        ERR_CONTEXT(ret_bad_operand_type);
}

void minus()
{
    MIN_ARGUMENTS(2);

    // substracting numbers
    if (_stack->get_type(0) == cmd_number && _stack->get_type(1) == cmd_number)
    {
        number* right = (number*)_stack->pop_back();
        number* left = (number*)_stack->back();
        CHECK_MPFR(mpfr_sub(left->_value.mpfr, left->_value.mpfr, right->_value.mpfr, floating_t::s_mpfr_rnd));
    }
    // substracting complexes
    else if (_stack->get_type(0) == cmd_complex && _stack->get_type(1) == cmd_complex)
    {
        complex* right = (complex*)_stack->pop_back();
        complex* left = (complex*)_stack->back();
        CHECK_MPFR(mpfr_sub(left->re()->mpfr, left->re()->mpfr, right->re()->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_sub(left->im()->mpfr, left->im()->mpfr, right->im()->mpfr, floating_t::s_mpfr_rnd));
    }
    // substracting complex-number
    else if (_stack->get_type(0) == cmd_number && _stack->get_type(1) == cmd_complex)
    {
        number* right = (number*)_stack->pop_back();
        complex* left = (complex*)_stack->back();
        CHECK_MPFR(mpfr_sub(left->re()->mpfr, left->re()->mpfr, right->_value.mpfr, floating_t::s_mpfr_rnd));
    }
    // substracting number-complex
    else if (_stack->get_type(0) == cmd_complex && _stack->get_type(1) == cmd_number)
    {
        swap();
        number* right = (number*)_stack->pop_back();
        complex* left = (complex*)_stack->back();
        CHECK_MPFR(mpfr_sub(left->re()->mpfr, right->_value.mpfr, left->re()->mpfr, floating_t::s_mpfr_rnd));
    }
    else
        ERR_CONTEXT(ret_bad_operand_type);
}

void mul()
{
    MIN_ARGUMENTS(2);

    // multiplying numbers
    if (_stack->get_type(0) == cmd_number && _stack->get_type(1) == cmd_number)
    {
        number* right = (number*)_stack->pop_back();
        number* left = (number*)_stack->back();
        CHECK_MPFR(mpfr_mul(left->_value.mpfr, left->_value.mpfr, right->_value.mpfr, floating_t::s_mpfr_rnd));
    }
    // multiplying complexes (a+ib)*(x+iy)=(ax-by)+i(ay+bx)=a(x+iy)+b(-y+ix)
    else if (_stack->get_type(0) == cmd_complex && _stack->get_type(1) == cmd_complex)
    {
        complex* right = (complex*)_stack->pop_back();//x+iy
        complex* left = (complex*)_stack->back();//a+ib

        stack::copy_and_push_back(*_stack, _stack->size()-1, _branch_stack);
        complex* left_sav = (complex*)_branch_stack.back();//a+ib

        // left: (a+ib)->(ax+iay)
        CHECK_MPFR(mpfr_mul(left->re()->mpfr, left->re()->mpfr, right->re()->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_mul(left->im()->mpfr, left_sav->re()->mpfr, right->im()->mpfr, floating_t::s_mpfr_rnd));
        
        // right: (x+iy)-> (bx-iby)
        CHECK_MPFR(mpfr_mul(right->im()->mpfr, left_sav->im()->mpfr, right->im()->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_neg(right->im()->mpfr, right->im()->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_mul(right->re()->mpfr, left_sav->im()->mpfr, right->re()->mpfr, floating_t::s_mpfr_rnd));

        //left=left+transpose(right)
        CHECK_MPFR(mpfr_add(left->re()->mpfr, left->re()->mpfr, right->im()->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_add(left->im()->mpfr, left->im()->mpfr, right->re()->mpfr, floating_t::s_mpfr_rnd));

        _branch_stack.pop_back();
    }
    // multiplying complex*number
    else if (_stack->get_type(0) == cmd_number && _stack->get_type(1) == cmd_complex)
    {
        number* right = (number*)_stack->pop_back();
        complex* left = (complex*)_stack->back();
        CHECK_MPFR(mpfr_mul(left->re()->mpfr, left->re()->mpfr, right->_value.mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_mul(left->im()->mpfr, left->im()->mpfr, right->_value.mpfr, floating_t::s_mpfr_rnd));
    }
    // multiplying number*complex
    else if (_stack->get_type(0) == cmd_complex && _stack->get_type(1) == cmd_number)
    {
        swap();
        number* right = (number*)_stack->pop_back();
        complex* left = (complex*)_stack->back();
        CHECK_MPFR(mpfr_mul(left->re()->mpfr, left->re()->mpfr, right->_value.mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_mul(left->im()->mpfr, left->im()->mpfr, right->_value.mpfr, floating_t::s_mpfr_rnd));
    }
    else
        ERR_CONTEXT(ret_bad_operand_type);
}

void do_divide_complexes()
{
    //(a+ib)/(x+iy)=(a+ib)(x-iy)/(x^2+y^2)=(ax+by+i(bx-ay))/(x^2+y^2)
    complex* right = (complex*)_stack->get_obj(0);//x+iy
    complex* left = (complex*)_stack->get_obj(1);//a+ib
    
    //1. calc (x^2-y^2) in _branch_stack
    number* ex = (number*)_branch_stack.allocate_back(number::calc_size(), cmd_number);
    CHECK_MPFR(mpfr_mul(ex->_value.mpfr, right->re()->mpfr, right->re()->mpfr, floating_t::s_mpfr_rnd));//x2
    number* wy = (number*)_branch_stack.allocate_back(number::calc_size(), cmd_number);
    CHECK_MPFR(mpfr_mul(wy->_value.mpfr, right->im()->mpfr, right->im()->mpfr, floating_t::s_mpfr_rnd));//y2
    CHECK_MPFR(mpfr_add(ex->_value.mpfr, ex->_value.mpfr, wy->_value.mpfr, floating_t::s_mpfr_rnd));//ex=x2+y2

    stack::copy_and_push_back(*_stack, _stack->size()-2, _branch_stack);//x+iy
    stack::copy_and_push_back(*_stack, _stack->size()-1, _branch_stack);
    complex* left_sav = (complex*)_branch_stack.get_obj(1);//a+ib
    complex* right_sav = (complex*)_branch_stack.get_obj(0);//x+iy

    //2. left.re=ax+by
    CHECK_MPFR(mpfr_mul(left->re()->mpfr, left_sav->re()->mpfr, right_sav->re()->mpfr, floating_t::s_mpfr_rnd));//left.re=ax
    CHECK_MPFR(mpfr_mul(right->re()->mpfr, left_sav->im()->mpfr, right_sav->im()->mpfr, floating_t::s_mpfr_rnd));//right.re=by
    CHECK_MPFR(mpfr_add(left->re()->mpfr, left->re()->mpfr, right->re()->mpfr, floating_t::s_mpfr_rnd));//left.re=ax+by

    //3. left.im=bx-ay
    CHECK_MPFR(mpfr_mul(left->im()->mpfr, left_sav->im()->mpfr, right_sav->re()->mpfr, floating_t::s_mpfr_rnd));//left.im=bx
    CHECK_MPFR(mpfr_mul(right->im()->mpfr, left_sav->re()->mpfr, right_sav->im()->mpfr, floating_t::s_mpfr_rnd));//right.im=ay
    CHECK_MPFR(mpfr_sub(left->im()->mpfr, left->im()->mpfr, right->im()->mpfr, floating_t::s_mpfr_rnd));//left.im=bx-ay

    //4. left.re/=(x^2-y^2), left.im/=(x^2+y^2)
    CHECK_MPFR(mpfr_div(left->re()->mpfr, left->re()->mpfr, ex->_value.mpfr, floating_t::s_mpfr_rnd));
    CHECK_MPFR(mpfr_div(left->im()->mpfr, left->im()->mpfr, ex->_value.mpfr, floating_t::s_mpfr_rnd));

    _stack->pop_back();
    _branch_stack.pop_back();
    _branch_stack.pop_back();
    _branch_stack.pop_back();
    _branch_stack.pop_back();
}

void div()
{
    MIN_ARGUMENTS(2);

    // dividing numbers
    if (_stack->get_type(0) == cmd_number && _stack->get_type(1) == cmd_number)
    {
        number* right = (number*)_stack->pop_back();
        number* left = (number*)_stack->back();
        CHECK_MPFR(mpfr_div(left->_value.mpfr, left->_value.mpfr, right->_value.mpfr, floating_t::s_mpfr_rnd));
    }
    // dividing complexes
    else if (_stack->get_type(0) == cmd_complex && _stack->get_type(1) == cmd_complex)
        do_divide_complexes();
    // dividing complex/number
    else if (_stack->get_type(0) == cmd_number && _stack->get_type(1) == cmd_complex)
    {
        number* right = (number*)_stack->pop_back();
        complex* left = (complex*)_stack->back();
        CHECK_MPFR(mpfr_div(left->re()->mpfr, left->re()->mpfr, right->_value.mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_div(left->im()->mpfr, left->im()->mpfr, right->_value.mpfr, floating_t::s_mpfr_rnd));
    }
    // dividing number/complex
    else if (_stack->get_type(0) == cmd_complex && _stack->get_type(1) == cmd_number)
    {
        //1. copy out
        stack::copy_and_push_back(*_stack, _stack->size()-1, _branch_stack);//complex
        stack::copy_and_push_back(*_stack, _stack->size()-2, _branch_stack);//number
        _stack->pop_back();
        _stack->pop_back();
        //2. copy back (2 complexes on stack)
        stack::copy_and_push_back(_branch_stack, _branch_stack.size()-2, *_stack);//complex back to stack
        stack::copy_and_push_back(_branch_stack, _branch_stack.size()-2, *_stack);//complex back to stack
        //3. set complex level 2 to (number,0)
        complex* new_cplx = (complex*)_stack->get_obj(1);
        CHECK_MPFR(mpfr_set(new_cplx->re()->mpfr, ((number*)_branch_stack.get_obj(0))->_value.mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_set_ui(new_cplx->im()->mpfr, 0UL, floating_t::s_mpfr_rnd));
        _branch_stack.pop_back();
        _branch_stack.pop_back();
        //4. divide
        do_divide_complexes();
    }
    else
        ERR_CONTEXT(ret_bad_operand_type);
}

void neg()
{
    MIN_ARGUMENTS(1);

    if (_stack->get_type(0) == cmd_number)
    {        
        number* left = (number*)_stack->back();
        CHECK_MPFR(mpfr_neg(left->_value.mpfr, left->_value.mpfr, floating_t::s_mpfr_rnd));
    }
    else if (_stack->get_type(0) == cmd_complex)
    {
        complex* left = (complex*)_stack->back();
        CHECK_MPFR(mpfr_neg(left->re()->mpfr, left->re()->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_neg(left->im()->mpfr, left->im()->mpfr, floating_t::s_mpfr_rnd));
    }
    else
        ERR_CONTEXT(ret_bad_operand_type);
}

void inv()
{
    MIN_ARGUMENTS(1);

    if (_stack->get_type(0) == cmd_number)
    {        
        number* left = (number*)_stack->back();
        CHECK_MPFR(mpfr_si_div(left->_value.mpfr, 1L, left->_value.mpfr, floating_t::s_mpfr_rnd));
    }
    else if (_stack->get_type(0) == cmd_complex)
    {
        //1. duplicate
        dup();
        //2. set complex level 2 to (1,0)
        complex* cplx = (complex*)_stack->get_obj(1);
        CHECK_MPFR(mpfr_set_ui(cplx->re()->mpfr, 1UL, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_set_ui(cplx->im()->mpfr, 0UL, floating_t::s_mpfr_rnd));
        //3. divide
        do_divide_complexes();
    }
    else
        ERR_CONTEXT(ret_bad_operand_type);
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

    if (_stack->get_type(0) == cmd_number)
    {        
        number* left = (number*)_stack->back();
        CHECK_MPFR(mpfr_abs(left->_value.mpfr, left->_value.mpfr, floating_t::s_mpfr_rnd));
    }
    else if (_stack->get_type(0) == cmd_complex)
    {
        //1. copy out -> calc x2+iy2
        stack::copy_and_push_back(*_stack, _stack->size()-1, _branch_stack);
        _stack->pop_back();

        //2. calc x2+iy2
        complex* cplx = (complex*)_branch_stack.back();
        CHECK_MPFR(mpfr_mul(cplx->re()->mpfr, cplx->re()->mpfr, cplx->re()->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_mul(cplx->im()->mpfr, cplx->im()->mpfr, cplx->im()->mpfr, floating_t::s_mpfr_rnd));

        //3. new real on stack
        _stack->allocate_back(number::calc_size(), cmd_number);
        number* module = (number*)_stack->back();

        //4. set it to |x2+y2| then take sqrt
        CHECK_MPFR(mpfr_set(module->_value.mpfr, cplx->re()->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_add(module->_value.mpfr, module->_value.mpfr, cplx->im()->mpfr, floating_t::s_mpfr_rnd));
        CHECK_MPFR(mpfr_sqrt(module->_value.mpfr, module->_value.mpfr, floating_t::s_mpfr_rnd));

        _branch_stack.pop_back();
    }
    else
        ERR_CONTEXT(ret_bad_operand_type);
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

    if (_stack->get_type(0) == cmd_number)
    {        
        // fact(n) = gamma(n+1)
        number* left = (number*)_stack->back();
        int result = mpfr_sgn(left->_value.mpfr);
        left->_value = (long)result;
    }
    else if (_stack->get_type(0) == cmd_complex)
    {
        // calc x/sqrt(x*x+y*y) +iy/sqrt(x*x+y*y)
        dup();
        rpn_abs();
        div();
    }
    else
        ERR_CONTEXT(ret_bad_operand_type);
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
