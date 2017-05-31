//
void sto(void)
{
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_symbol);

    string name(((symbol*)_stack->pop_back())->_value);
    _heap->add(name, _stack->get_obj(0), _stack->get_len(0));
    (void)_stack->pop_back();
}

//
void stoadd(void)
{
    MIN_ARGUMENTS(2);

    if (_stack->get_type(0) == cmd_symbol && _stack->get_type(1) == cmd_number)
    {
        // get variable value on stack level 1, make op then modify variable
        string variable(((symbol*)_stack->back())->_value);
        rcl();
        if (_err == ret_ok)
        {
            plus();
            _heap->add(variable, _stack->get_obj(0), _stack->get_len(0));
            _stack->pop_back();
        }
    }
    else if (_stack->get_type(1) == cmd_symbol && _stack->get_type(0) == cmd_number)
    {
        // copy value, get variable value on stack level 1, 
        // put back value on stack level 1, make op then modify variable
        stack::copy_and_push_back(*_stack, _stack->size()-1, _branch_stack);
        _stack->pop_back();

        string variable(((symbol*)_stack->back())->_value);
        rcl();
        if (_err == ret_ok)
        {
            stack::copy_and_push_back(_branch_stack, _branch_stack.size()-1, *_stack);
            plus();
            _heap->add(variable, _stack->get_obj(0), _stack->get_len(0));
            _stack->pop_back();
        }
    }
    else
        ERR_CONTEXT(ret_bad_operand_type);
}

void stosub(void)
{
    MIN_ARGUMENTS(2);

    if (_stack->get_type(0) == cmd_symbol && _stack->get_type(1) == cmd_number)
    {
        // get variable value on stack level 1, make op then modify variable
        string variable(((symbol*)_stack->back())->_value);
        rcl();
        if (_err == ret_ok)
        {
            minus();
            _heap->add(variable, _stack->get_obj(0), _stack->get_len(0));
            _stack->pop_back();
        }
    }
    else if (_stack->get_type(1) == cmd_symbol && _stack->get_type(0) == cmd_number)
    {
        // copy value, get variable value on stack level 1, 
        // put back value on stack level 1, make op then modify variable
        stack::copy_and_push_back(*_stack, _stack->size()-1, _branch_stack);
        _stack->pop_back();

        string variable(((symbol*)_stack->back())->_value);
        rcl();
        if (_err == ret_ok)
        {
            stack::copy_and_push_back(_branch_stack, _branch_stack.size()-1, *_stack);
            minus();
            _heap->add(variable, _stack->get_obj(0), _stack->get_len(0));
            _stack->pop_back();
        }
    }
    else
        ERR_CONTEXT(ret_bad_operand_type);
}

void stomul(void)
{
    MIN_ARGUMENTS(2);

    if (_stack->get_type(0) == cmd_symbol && _stack->get_type(1) == cmd_number)
    {
        // get variable value on stack level 1, make op then modify variable
        string variable(((symbol*)_stack->back())->_value);
        rcl();
        if (_err == ret_ok)
        {
            mul();
            _heap->add(variable, _stack->get_obj(0), _stack->get_len(0));
            _stack->pop_back();
        }
    }
    else if (_stack->get_type(1) == cmd_symbol && _stack->get_type(0) == cmd_number)
    {
        // copy value, get variable value on stack level 1, 
        // put back value on stack level 1, make op then modify variable
        stack::copy_and_push_back(*_stack, _stack->size()-1, _branch_stack);
        _stack->pop_back();

        string variable(((symbol*)_stack->back())->_value);
        rcl();
        if (_err == ret_ok)
        {
            stack::copy_and_push_back(_branch_stack, _branch_stack.size()-1, *_stack);
            mul();
            _heap->add(variable, _stack->get_obj(0), _stack->get_len(0));
            _stack->pop_back();
        }
    }
    else
        ERR_CONTEXT(ret_bad_operand_type);
}

void stodiv(void)
{
    MIN_ARGUMENTS(2);

    if (_stack->get_type(0) == cmd_symbol && _stack->get_type(1) == cmd_number)
    {
        // get variable value on stack level 1, make op then modify variable
        string variable(((symbol*)_stack->back())->_value);
        rcl();
        if (_err == ret_ok)
        {
            div();
            _heap->add(variable, _stack->get_obj(0), _stack->get_len(0));
            _stack->pop_back();
        }
    }
    else if (_stack->get_type(1) == cmd_symbol && _stack->get_type(0) == cmd_number)
    {
        // copy value, get variable value on stack level 1, 
        // put back value on stack level 1, make op then modify variable
        stack::copy_and_push_back(*_stack, _stack->size()-1, _branch_stack);
        _stack->pop_back();

        string variable(((symbol*)_stack->back())->_value);
        rcl();
        if (_err == ret_ok)
        {
            stack::copy_and_push_back(_branch_stack, _branch_stack.size()-1, *_stack);
            div();
            _heap->add(variable, _stack->get_obj(0), _stack->get_len(0));
            _stack->pop_back();
        }
    }
    else
        ERR_CONTEXT(ret_bad_operand_type);
}

void stoneg(void)
{
    MIN_ARGUMENTS(1);

    if (_stack->get_type(0) == cmd_symbol)
    {
        // get variable value on stack level 1, make op then modify variable
        string variable(((symbol*)_stack->back())->_value);
        rcl();
        if (_err == ret_ok)
        {
            neg();
            _heap->add(variable, _stack->get_obj(0), _stack->get_len(0));
            _stack->pop_back();
        }
    }
    else
        ERR_CONTEXT(ret_bad_operand_type);
}

void stoinv(void)
{
    MIN_ARGUMENTS(1);

    if (_stack->get_type(0) == cmd_symbol)
    {
        // get variable value on stack level 1, make op then modify variable
        string variable(((symbol*)_stack->back())->_value);
        rcl();
        if (_err == ret_ok)
        {
            inv();
            _heap->add(variable, _stack->get_obj(0), _stack->get_len(0));
            _stack->pop_back();
        }
    }
    else
        ERR_CONTEXT(ret_bad_operand_type);
}

void rcl(void)
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_symbol);

    // recall a variable
    object* obj;
    unsigned int size;
    string variable(((symbol*)_stack->back())->_value);

    // mind the order of heaps
    if (find_variable(variable, obj, size))
    {
        (void)_stack->pop_back();
        stack::copy_and_push_back(obj, *_stack, size);
    }
    else
        ERR_CONTEXT(ret_unknown_variable);
}

void edit(void)
{
    MIN_ARGUMENTS(1);

    FILE* tmp = tmpfile();
    if (tmp != NULL)
    {
        // re-write stack objet in a stream
        ((object*)_stack->pop_back())->show(tmp);

        // edit: stuff chars using readline facility
        int len = (int)ftell(tmp);
        rewind(tmp);
        for(int i=0;i<len;i++)
        {
            char readc;
            if (fread(&readc, 1, 1, tmp) == 1)
                rl_stuff_char(readc);
            else
            {
                ERR_CONTEXT(ret_runtime_error);
                break;
            }
        }
        fclose(tmp);
    }
}

// carefull : this is not a langage command
void auto_rcl(symbol* symb)
{
    if (symb->_auto_eval)
    {
        object* obj;
        unsigned int size;
        string variable(symb->_value);

        // mind the order of heaps
        if (find_variable(variable, obj, size))
        {
            stack::copy_and_push_back(obj, *_stack, size);
            if (obj->_type == cmd_program)
                eval();
        }
        else
            stack::copy_and_push_back(symb, *_stack, symb->size());
    }
    else
        stack::copy_and_push_back(symb, *_stack, symb->size());
}

void purge(void)
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_symbol);

    string name(((symbol*)_stack->pop_back())->_value);
    if (!_heap->erase(name))
        ERR_CONTEXT(ret_unknown_variable);
}

void vars(void)
{
    object* obj;
    unsigned int size;
    program* parent = _parent_prog;
    string name;

    // heap variables
    for (int i=0; i<(int)_heap->count_vars(); i++)
    {
        (void)_heap->get_by_index(i, name, obj, size);
        printf("var %d: name '%s', type %s, value ", i+1, name.c_str(), object::s_cmd_type_string[obj->_type]);
        obj->show();
        printf("\n");
    }

    // parents local variables
    while (parent != NULL)
    {
        for (int i=0; i<(int)parent->_local_heap.size(); i++)
        {
            (void)parent->_local_heap.get_by_index(i, name, obj, size);
            printf("local var %d: name '%s', type %s, value ", i+1, name.c_str(), object::s_cmd_type_string[obj->_type]);
            obj->show();
            printf("\n");
        }
        parent = parent->_parent_prog;
    }

    // local variables
    for (int i=0; i<(int)_local_heap.size(); i++)
    {
        (void)_local_heap.get_by_index(i, name, obj, size);
        printf("local var %d: name '%s', type %s, value ", i+1, name.c_str(), object::s_cmd_type_string[obj->_type]);
        obj->show();
        printf("\n");
    }
}

void clusr(void)
{
    _heap->erase_all();
}
