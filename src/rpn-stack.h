//
void swap(void)
{
    MIN_ARGUMENTS(2);
    _stack->copy_obj_to_local(0, 0);
    _stack->copy_obj_to_local(1, 1);
    (void)_stack->pop_back();
    (void)_stack->pop_back();
    _stack->push_obj_from_local(0);
    _stack->push_obj_from_local(1);
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
    _stack->copy_obj_to_local(0, 0);
    _stack->push_obj_from_local(0);
}

void dup2(void)
{
    MIN_ARGUMENTS(2);
    _stack->copy_obj_to_local(0, 0);
    _stack->copy_obj_to_local(1, 1);
    _stack->push_obj_from_local(1);
    _stack->push_obj_from_local(0);
}

void pick(void)
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    unsigned int to_pick = (unsigned int)int(((number*)_stack->pop_back())->_value);

    // treat stack depth errors
    if ((to_pick == 0) || (to_pick > _stack->size()))
    {
        ERR_CONTEXT(ret_missing_operand);
        return;
    }
    _stack->copy_obj_to_local(to_pick - 1, 0);
    _stack->push_obj_from_local(0);
}

void rot(void)
{
    MIN_ARGUMENTS(3);
    _stack->copy_obj_to_local(0, 0);
    _stack->copy_obj_to_local(1, 1);
    _stack->copy_obj_to_local(2, 2);
    (void)_stack->pop_back();
    (void)_stack->pop_back();
    (void)_stack->pop_back();
    _stack->push_obj_from_local(1);
    _stack->push_obj_from_local(0);
    _stack->push_obj_from_local(2);
}

void depth(void)
{
    number num;
    num.set((long)_stack->size());
    _stack->push_back(&num, num.size(), cmd_number);
}
