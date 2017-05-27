//
void swap(void)
{
    MIN_ARGUMENTS(2);
    stack::copy_and_push_back(*_stack, _stack->size()-1, _branch_stack);
    stack::copy_and_push_back(*_stack, _stack->size()-2, _branch_stack);
    (void)_stack->pop_back();
    (void)_stack->pop_back();
    stack::copy_and_push_back(_branch_stack, 0, *_stack);
    stack::copy_and_push_back(_branch_stack, 1, *_stack);
    _branch_stack.erase();
}

void drop(void)
{
    MIN_ARGUMENTS(1);
    (void)_stack->pop_back();
}

void drop2(void)
{
    MIN_ARGUMENTS(2);
    (void)_stack->pop_back();
    (void)_stack->pop_back();
}

void dropn(void)
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    int args = (int)mpfr_get_si(((number*)_stack->back())->_value.mpfr, floating_t::s_mpfr_rnd);
    MIN_ARGUMENTS(args+1);

    for(int i=0;i<args+1;i++)
        (void)_stack->pop_back();
}

void erase(void)
{
    while(_stack->size()>0)
        (void)_stack->pop_back();
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

    stack::copy_and_push_back(*_stack, _stack->size()-3, _branch_stack);
    stack::copy_and_push_back(*_stack, _stack->size()-2, _branch_stack);
    stack::copy_and_push_back(*_stack, _stack->size()-1, _branch_stack);
    (void)_stack->pop_back();
    (void)_stack->pop_back();
    (void)_stack->pop_back();
    stack::copy_and_push_back(_branch_stack, 1, *_stack);
    stack::copy_and_push_back(_branch_stack, 2, *_stack);
    stack::copy_and_push_back(_branch_stack, 0, *_stack);
    _branch_stack.erase();
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
        stack::copy_and_push_back(*_stack, _stack->size()-1, _branch_stack);
        (void)_stack->pop_back();
    }

    for(int i=1;i<args;i++)
        stack::copy_and_push_back(_branch_stack, args-1-i, *_stack);
    stack::copy_and_push_back(_branch_stack, args-1, *_stack);

    _branch_stack.erase();
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
        stack::copy_and_push_back(*_stack, _stack->size()-1, _branch_stack);
        (void)_stack->pop_back();
    }

    stack::copy_and_push_back(_branch_stack, 0, *_stack);
    for(int i=1;i<args;i++)
        stack::copy_and_push_back(_branch_stack, args-i, *_stack);

    _branch_stack.erase();
}

void over(void)
{
    MIN_ARGUMENTS(2);

    stack::copy_and_push_back(*_stack, _stack->size()-2, *_stack);
}
