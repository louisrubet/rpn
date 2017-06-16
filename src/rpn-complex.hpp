void re()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_complex);
    
    stack::copy_and_push_back(*_stack, _stack->size()-1, _branch_stack);
    _stack->pop_back();

    number* re = (number*)_stack->allocate_back(number::calc_size(), cmd_number);
    CHECK_MPFR(mpfr_set(re->_value.mpfr, ((complex*)_branch_stack.get_obj(0))->re()->mpfr, floating_t::s_mpfr_rnd));
    _branch_stack.pop_back();
}

void im()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_complex);
    
    stack::copy_and_push_back(*_stack, _stack->size()-1, _branch_stack);
    _stack->pop_back();

    number* im = (number*)_stack->allocate_back(number::calc_size(), cmd_number);
    CHECK_MPFR(mpfr_set(im->_value.mpfr, ((complex*)_branch_stack.get_obj(0))->im()->mpfr, floating_t::s_mpfr_rnd));
    _branch_stack.pop_back();
}

void arg()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_complex);

    // calc atan2(x/y)
    complex* cplx = (complex*)_stack->pop_back();
    number* num = (number*)_branch_stack.allocate_back(number::calc_size(), cmd_number);

    CHECK_MPFR(mpfr_atan2(num->_value.mpfr, cplx->im()->mpfr, cplx->re()->mpfr, floating_t::s_mpfr_rnd));

    stack::copy_and_push_back(_branch_stack, 0, *_stack);
    _branch_stack.pop_back();
}

void conj()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_complex);

    complex* cplx = (complex*)_stack->back();
    CHECK_MPFR(mpfr_neg(cplx->im()->mpfr, cplx->im()->mpfr, floating_t::s_mpfr_rnd));
}

void r2c()
{
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ARG_MUST_BE_OF_TYPE(1, cmd_number);
    
    stack::copy_and_push_back(*_stack, _stack->size()-2, _branch_stack);
    stack::copy_and_push_back(*_stack, _stack->size()-1, _branch_stack);
    _stack->pop_back();
    _stack->pop_back();

    complex* cplx = (complex*)_stack->allocate_back(complex::calc_size(), cmd_complex);
    CHECK_MPFR(mpfr_set(cplx->re()->mpfr, ((number*)_branch_stack.get_obj(1))->_value.mpfr, floating_t::s_mpfr_rnd));
    CHECK_MPFR(mpfr_set(cplx->im()->mpfr, ((number*)_branch_stack.get_obj(0))->_value.mpfr, floating_t::s_mpfr_rnd));
    _branch_stack.pop_back();
    _branch_stack.pop_back();
}

void c2r()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_complex);

    stack::copy_and_push_back(*_stack, _stack->size()-1, _branch_stack);
    _stack->pop_back();

    number* re = (number*)_stack->allocate_back(number::calc_size(), cmd_number);
    number* im = (number*)_stack->allocate_back(number::calc_size(), cmd_number);

    CHECK_MPFR(mpfr_set(re->_value.mpfr, ((complex*)_branch_stack.back())->re()->mpfr, floating_t::s_mpfr_rnd));
    CHECK_MPFR(mpfr_set(im->_value.mpfr, ((complex*)_branch_stack.back())->im()->mpfr, floating_t::s_mpfr_rnd));
    _branch_stack.pop_back();
}

void r2p()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_complex);

    dup();
    dup();
    arg();

    complex* cplx = (complex*)_stack->get_obj(1);
    CHECK_MPFR(mpfr_set(cplx->im()->mpfr, ((number*)_stack->back())->_value.mpfr, floating_t::s_mpfr_rnd));
    _stack->pop_back();
    
    swap();
    rpn_abs();
    cplx = (complex*)_stack->get_obj(1);
    CHECK_MPFR(mpfr_set(cplx->re()->mpfr, ((number*)_stack->back())->_value.mpfr, floating_t::s_mpfr_rnd));
    _stack->pop_back();
}

void p2r()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_complex);

    stack::copy_and_push_back(*_stack, _stack->size()-1, _branch_stack);
    _branch_stack.allocate_back(number::calc_size(), cmd_number);
    
    // assert complex is polar
    complex* rhotheta = (complex*)_branch_stack.get_obj(1);
    number* tmp = (number*)_branch_stack.get_obj(0);
    complex* result = (complex*)_stack->back();

    //calc cos(theta)
    CHECK_MPFR(mpfr_set(tmp->_value.mpfr, rhotheta->im()->mpfr, floating_t::s_mpfr_rnd));
    CHECK_MPFR(mpfr_cos(tmp->_value.mpfr, tmp->_value.mpfr, floating_t::s_mpfr_rnd));

    //calc rcos(theta)
    CHECK_MPFR(mpfr_mul(result->re()->mpfr, rhotheta->re()->mpfr, tmp->_value.mpfr, floating_t::s_mpfr_rnd));

    //calc sin(theta)
    CHECK_MPFR(mpfr_set(tmp->_value.mpfr, rhotheta->im()->mpfr, floating_t::s_mpfr_rnd));
    CHECK_MPFR(mpfr_sin(tmp->_value.mpfr, tmp->_value.mpfr, floating_t::s_mpfr_rnd));

    //calc rsin(theta)
    CHECK_MPFR(mpfr_mul(result->im()->mpfr, rhotheta->re()->mpfr, tmp->_value.mpfr, floating_t::s_mpfr_rnd));
}
