//
void rpn_sto(void)
{
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_symbol);

    string name(((symbol*)_stack->pop_back())->_value);
    _heap->add(name, _stack->get_obj(0), _stack->get_len(0));
    (void)_stack->pop_back();
}

//
void rpn_stoadd(void)
{
    MIN_ARGUMENTS(2);

    if (_stack->get_type(0) == cmd_symbol && _stack->get_type(1) == cmd_number)
    {
        // get variable value on stack level 1, make op then modify variable
        string variable(((symbol*)_stack->back())->_value);
        rpn_rcl();
        if (_err == ret_ok)
        {
            rpn_plus();
            _heap->add(variable, _stack->get_obj(0), _stack->get_len(0));
            _stack->pop_back();
        }
    }
    else if (_stack->get_type(1) == cmd_symbol && _stack->get_type(0) == cmd_number)
    {
        // copy value, get variable value on stack level 1, 
        // put back value on stack level 1, make op then modify variable
        stack::copy_and_push_back(*_stack, _stack->size()-1, _calc_stack);
        _stack->pop_back();

        string variable(((symbol*)_stack->back())->_value);
        rpn_rcl();
        if (_err == ret_ok)
        {
            stack::copy_and_push_back(_calc_stack, _calc_stack.size()-1, *_stack);
            rpn_plus();
            _heap->add(variable, _stack->get_obj(0), _stack->get_len(0));
            _stack->pop_back();
        }
    }
    else
        ERR_CONTEXT(ret_bad_operand_type);
}

void rpn_stosub(void)
{
    MIN_ARGUMENTS(2);

    if (_stack->get_type(0) == cmd_symbol && _stack->get_type(1) == cmd_number)
    {
        // get variable value on stack level 1, make op then modify variable
        string variable(((symbol*)_stack->back())->_value);
        rpn_rcl();
        if (_err == ret_ok)
        {
            rpn_minus();
            _heap->add(variable, _stack->get_obj(0), _stack->get_len(0));
            _stack->pop_back();
        }
    }
    else if (_stack->get_type(1) == cmd_symbol && _stack->get_type(0) == cmd_number)
    {
        // copy value, get variable value on stack level 1, 
        // put back value on stack level 1, make op then modify variable
        stack::copy_and_push_back(*_stack, _stack->size()-1, _calc_stack);
        _stack->pop_back();

        string variable(((symbol*)_stack->back())->_value);
        rpn_rcl();
        if (_err == ret_ok)
        {
            stack::copy_and_push_back(_calc_stack, _calc_stack.size()-1, *_stack);
            rpn_minus();
            _heap->add(variable, _stack->get_obj(0), _stack->get_len(0));
            _stack->pop_back();
        }
    }
    else
        ERR_CONTEXT(ret_bad_operand_type);
}

void rpn_stomul(void)
{
    MIN_ARGUMENTS(2);

    if (_stack->get_type(0) == cmd_symbol && _stack->get_type(1) == cmd_number)
    {
        // get variable value on stack level 1, make op then modify variable
        string variable(((symbol*)_stack->back())->_value);
        rpn_rcl();
        if (_err == ret_ok)
        {
            rpn_mul();
            _heap->add(variable, _stack->get_obj(0), _stack->get_len(0));
            _stack->pop_back();
        }
    }
    else if (_stack->get_type(1) == cmd_symbol && _stack->get_type(0) == cmd_number)
    {
        // copy value, get variable value on stack level 1, 
        // put back value on stack level 1, make op then modify variable
        stack::copy_and_push_back(*_stack, _stack->size()-1, _calc_stack);
        _stack->pop_back();

        string variable(((symbol*)_stack->back())->_value);
        rpn_rcl();
        if (_err == ret_ok)
        {
            stack::copy_and_push_back(_calc_stack, _calc_stack.size()-1, *_stack);
            rpn_mul();
            _heap->add(variable, _stack->get_obj(0), _stack->get_len(0));
            _stack->pop_back();
        }
    }
    else
        ERR_CONTEXT(ret_bad_operand_type);
}

void rpn_stodiv(void)
{
    MIN_ARGUMENTS(2);

    if (_stack->get_type(0) == cmd_symbol && _stack->get_type(1) == cmd_number)
    {
        // get variable value on stack level 1, make op then modify variable
        string variable(((symbol*)_stack->back())->_value);
        rpn_rcl();
        if (_err == ret_ok)
        {
            rpn_div();
            _heap->add(variable, _stack->get_obj(0), _stack->get_len(0));
            _stack->pop_back();
        }
    }
    else if (_stack->get_type(1) == cmd_symbol && _stack->get_type(0) == cmd_number)
    {
        // copy value, get variable value on stack level 1, 
        // put back value on stack level 1, make op then modify variable
        stack::copy_and_push_back(*_stack, _stack->size()-1, _calc_stack);
        _stack->pop_back();

        string variable(((symbol*)_stack->back())->_value);
        rpn_rcl();
        if (_err == ret_ok)
        {
            stack::copy_and_push_back(_calc_stack, _calc_stack.size()-1, *_stack);
            rpn_div();
            _heap->add(variable, _stack->get_obj(0), _stack->get_len(0));
            _stack->pop_back();
        }
    }
    else
        ERR_CONTEXT(ret_bad_operand_type);
}

void rpn_stoneg(void)
{
    MIN_ARGUMENTS(1);

    if (_stack->get_type(0) == cmd_symbol)
    {
        // get variable value on stack level 1, make op then modify variable
        string variable(((symbol*)_stack->back())->_value);
        rpn_rcl();
        if (_err == ret_ok)
        {
            rpn_neg();
            _heap->add(variable, _stack->get_obj(0), _stack->get_len(0));
            _stack->pop_back();
        }
    }
    else
        ERR_CONTEXT(ret_bad_operand_type);
}

void rpn_stoinv(void)
{
    MIN_ARGUMENTS(1);

    if (_stack->get_type(0) == cmd_symbol)
    {
        // get variable value on stack level 1, make op then modify variable
        string variable(((symbol*)_stack->back())->_value);
        rpn_rcl();
        if (_err == ret_ok)
        {
            rpn_inv();
            _heap->add(variable, _stack->get_obj(0), _stack->get_len(0));
            _stack->pop_back();
        }
    }
    else
        ERR_CONTEXT(ret_bad_operand_type);
}

void rpn_rcl(void)
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

void rpn_edit(void)
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

        // get stream data
        void* file_data = mmap(NULL, len, PROT_READ, MAP_PRIVATE, fileno(tmp), 0);
        if (file_data != MAP_FAILED)
        {
            // set it as the linenoise line entry
            linenoisePreloadBuffer((const char*)file_data);
            munmap(file_data, len);
        }
        else
            ERR_CONTEXT(ret_runtime_error);

        fclose(tmp);
    }
    else
        ERR_CONTEXT(ret_runtime_error);
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
                rpn_eval();
        }
        else
            stack::copy_and_push_back(symb, *_stack, symb->size());
    }
    else
        stack::copy_and_push_back(symb, *_stack, symb->size());
}

void rpn_purge(void)
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_symbol);

    string name(((symbol*)_stack->pop_back())->_value);
    if (!_heap->erase(name))
        ERR_CONTEXT(ret_unknown_variable);
}

void rpn_vars(void)
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

void rpn_clusr(void)
{
    _heap->erase_all();
}
