stack _stack_manip;

//
void swap(void)
{
    MIN_ARGUMENTS(2);
    
    stack::copy_and_push_back(*_stack, _stack->size()-1, _stack_manip);
    stack::copy_and_push_back(*_stack, _stack->size()-2, _stack_manip);
    (void)_stack->pop_back();
    (void)_stack->pop_back();
    stack::copy_and_push_back(_stack_manip, 0, *_stack);
    stack::copy_and_push_back(_stack_manip, 1, *_stack);
    _stack_manip.erase();
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

    //TODO could be optimized
    stack::copy_and_push_back(*_stack, _stack->size()-3, _stack_manip);
    stack::copy_and_push_back(*_stack, _stack->size()-2, _stack_manip);
    stack::copy_and_push_back(*_stack, _stack->size()-1, _stack_manip);
    (void)_stack->pop_back();
    (void)_stack->pop_back();
    (void)_stack->pop_back();
    stack::copy_and_push_back(_stack_manip, 2, *_stack);
    stack::copy_and_push_back(_stack_manip, 1, *_stack);
    stack::copy_and_push_back(_stack_manip, 0, *_stack);
    _stack_manip.erase();
}

void depth(void)
{
    unsigned long depth = (unsigned long)_stack->size();
    void* significand;
    number* num = (number*)_stack->allocate_back(sizeof(number), cmd_number, MPFR_128BITS_STORING_LENGTH, &significand);
    num->init(significand);
    num->set(depth);
}
