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
