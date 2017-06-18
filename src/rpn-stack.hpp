//
void swap(void)
{
    MIN_ARGUMENTS(2);
    stack::copy_and_push_back(*_stack, _stack->size()-1, _calc_stack);
    stack::copy_and_push_back(*_stack, _stack->size()-2, _calc_stack);
    (void)_stack->pop_back(2);
    stack::copy_and_push_back(_calc_stack, _calc_stack.size()-2, *_stack);
    stack::copy_and_push_back(_calc_stack, _calc_stack.size()-1, *_stack);
    _calc_stack.pop_back(2);

}
void drop(void)
{
    MIN_ARGUMENTS(1);
    (void)_stack->pop_back();
}

void drop2(void)
{
    MIN_ARGUMENTS(2);
    (void)_stack->pop_back(2);
}

void dropn(void)
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    int args = (int)mpfr_get_si(((number*)_stack->back())->_value.mpfr, floating_t::s_mpfr_rnd);
    MIN_ARGUMENTS(args+1);

    (void)_stack->pop_back(args+1);
}

void erase(void)
{
    (void)_stack->pop_back(_stack->size());
}

void dup(void)
{
    MIN_ARGUMENTS(1);
    stack::copy_and_push_back(*_stack, _stack->size()-1, *_stack);
}

void dup2(void)
{
    MIN_ARGUMENTS(2);
    stack::copy_and_push_back(*_stack, _stack->size()-2, *_stack);
    stack::copy_and_push_back(*_stack, _stack->size()-2, *_stack);
}

void dupn(void)
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    int args = (int)mpfr_get_si(((number*)_stack->back())->_value.mpfr, floating_t::s_mpfr_rnd);
    MIN_ARGUMENTS(args+1);
    _stack->pop_back();
    
    for (int i=0;i<args;i++)
        stack::copy_and_push_back(*_stack, _stack->size()-args, *_stack);
}

void pick(void)
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    unsigned int to_pick = (unsigned int)int(((number*)_stack->pop_back())->_value);

    // treat stack depth errors
    if ((to_pick == 0) || (to_pick > _stack->size()))
    {
        ERR_CONTEXT(ret_out_of_range);
        return;
    }

    stack::copy_and_push_back(*_stack, _stack->size()-to_pick, *_stack);
}

void rot(void)
{
    MIN_ARGUMENTS(3);

    stack::copy_and_push_back(*_stack, _stack->size()-3, _calc_stack);
    stack::copy_and_push_back(*_stack, _stack->size()-2, _calc_stack);
    stack::copy_and_push_back(*_stack, _stack->size()-1, _calc_stack);
    (void)_stack->pop_back(3);
    stack::copy_and_push_back(_calc_stack, _calc_stack.size()-2, *_stack);
    stack::copy_and_push_back(_calc_stack, _calc_stack.size()-1, *_stack);
    stack::copy_and_push_back(_calc_stack, _calc_stack.size()-3, *_stack);
    _calc_stack.pop_back(3);
}

void depth(void)
{
    unsigned long depth = (unsigned long)_stack->size();
    number* num = (number*)_stack->allocate_back(number::calc_size(), cmd_number);
    num->set(depth);
}

void roll(void)
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    int args = (int)mpfr_get_si(((number*)_stack->back())->_value.mpfr, floating_t::s_mpfr_rnd);
    MIN_ARGUMENTS(args+1);
    _stack->pop_back();

    for(int i=0;i<args;i++)
    {
        stack::copy_and_push_back(*_stack, _stack->size()-1, _calc_stack);
        (void)_stack->pop_back();
    }

    for(int i=1;i<args;i++)
        stack::copy_and_push_back(_calc_stack, args-1-i, *_stack);
    stack::copy_and_push_back(_calc_stack, args-1, *_stack);

    _calc_stack.pop_back(args);
}

void rolld(void)
{
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    int args = (int)mpfr_get_si(((number*)_stack->back())->_value.mpfr, floating_t::s_mpfr_rnd);
    MIN_ARGUMENTS(args+1);
    _stack->pop_back();

    for(int i=0;i<args;i++)
    {
        stack::copy_and_push_back(*_stack, _stack->size()-1, _calc_stack);
        (void)_stack->pop_back();
    }

    stack::copy_and_push_back(_calc_stack, _calc_stack.size()-args, *_stack);

    for(int i=1;i<args;i++)
        stack::copy_and_push_back(_calc_stack, _calc_stack.size()-i, *_stack);

    _calc_stack.pop_back(args);
}

void over(void)
{
    MIN_ARGUMENTS(2);

    stack::copy_and_push_back(*_stack, _stack->size()-2, *_stack);
}
