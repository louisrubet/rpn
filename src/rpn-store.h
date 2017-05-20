//
void sto(void)
{
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_symbol);

    string name(((symbol*)_stack->pop_back())->_value);
    _global_heap->add(name, _stack->get_obj(0), _stack->get_len(0));
    (void)_stack->pop_back();
}

void rcl(void)
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_symbol);

    // recall a variable
    object* obj;
    unsigned int size;
    int type;
    string variable(((symbol*)_stack->back())->_value);

    // mind the order of heaps
    if (_local_heap.get(variable, obj, size)
            || ((_parent_local_heap != NULL) && (_parent_local_heap->get(variable, obj, size)))
            || _global_heap->get(variable, obj, size))
    {
        (void)_stack->pop_back();
        object* new_obj = _stack->allocate_back(size, obj->_type);
        stack::copy_and_push_back(obj, *_stack, size);
    }
    else
        ERR_CONTEXT(ret_unknown_variable);
}

void edit(void)
{
    MIN_ARGUMENTS(1);

    stringstream out;

    if (((object*)_stack->back())->_type == cmd_number)
    {
        switch(number::s_mode)
        {
        case number::std:
            out.precision(number::s_current_precision);
            out.unsetf(ios_base::floatfield);
            break;
        case number::fix:
            out << setprecision(number::s_current_precision) << fixed;
            break;
        case number::sci:
            out << setprecision(number::s_current_precision) << scientific;
            break;
        }
    }

    // re-write stack objet in a stream
    ((object*)_stack->pop_back())->show(out);

    // edit: stuff chars using readline facility
    string str = out.str();

    for(int i=0;i<str.size();i++)
        rl_stuff_char(str[i]);
}

// carefull : this is not a langage command
void auto_rcl(symbol* symb)
{
    if (symb->_auto_eval)
    {
        object* obj;
        unsigned int size;
        int type;
        string variable(symb->_value);

        // mind the order of heaps
        if (_local_heap.get(variable, obj, size)
                || ((_parent_local_heap != NULL) && (_parent_local_heap->get(variable, obj, size)))
                || _global_heap->get(variable, obj, size))
        {
            stack::copy_and_push_back(obj, *_stack, size);
            if (type == cmd_program)
                eval();
        }
        else
        {
            stack::copy_and_push_back(symb, *_stack, symb->size());
        }
    }
    else
    {
        stack::copy_and_push_back(symb, *_stack, symb->size());
    }
}

void purge(void)
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_symbol);

    string name(((symbol*)_stack->pop_back())->_value);

    // mind the order of heaps
    bool done = false;
    if (!_local_heap.erase(name))
    {
        if ((_parent_local_heap != NULL) && (_parent_local_heap->erase(name)))
            done = true;
        else if (_global_heap->erase(name))
            done = true;
    }
    else
        done = true;

    if (!done)
        ERR_CONTEXT(ret_unknown_variable);
}

void vars(void)
{
    object* obj;
    unsigned int size;
    int type;
    string name;

    for (int i=0; i<(int)_global_heap->size(); i++)
    {
        (void)_global_heap->get_by_index(i, name, obj, size);
        cout<<"var "<<i+1<<": name '"<<name<<"', type "<<cmd_type_string[obj->_type]<<", value ";
        obj->show();
        cout<<endl;
    }
    if (_parent_local_heap != NULL)
    {
        for (int i=0; i<(int)_parent_local_heap->size(); i++)
        {
            (void)_parent_local_heap->get_by_index(i, name, obj, size);
            cout<<"local var "<<i+1<<": name '"<<name<<"', type "<<cmd_type_string[obj->_type]<<", value ";
            obj->show();
            cout<<endl;
        }
    }
    for (int i=0; i<(int)_local_heap.size(); i++)
    {
        (void)_local_heap.get_by_index(i, name, obj, size);
        cout<<"local var "<<i+1<<": name '"<<name<<"', type "<<cmd_type_string[obj->_type]<<", value ";
        obj->show();
        cout<<endl;
    }
}
