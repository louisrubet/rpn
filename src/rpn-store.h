//
void sto(void)
{
	MIN_ARGUMENTS(2);
	ARG_MUST_BE_OF_TYPE(0, cmd_symbol);

    string name(((symbol*)_stack->get_obj(0))->_value);
    _stack->pop_back();
    _global_heap->add(name, _stack->get_obj(0), _stack->get_len(0), _stack->get_type(0));
	_stack->pop_back();
}

void rcl(void)
{
	MIN_ARGUMENTS(1);
	ARG_MUST_BE_OF_TYPE(0, cmd_symbol);

	// recall a variable
	void* obj;
	unsigned int size;
	int type;
    string variable(((symbol*)_stack->back())->_value);

    // mind the order of heaps
    if (_local_heap.get(variable, obj, size, type)
            || ((_parent_local_heap != NULL) && (_parent_local_heap->get(variable, obj, size, type)))
            || _global_heap->get(variable, obj, size, type))
	{
		_stack->pop_back();
		_stack->push_back(obj, size, type);
	}
	else
		ERR_CONTEXT(ret_unknown_variable);
}

void edit(void)
{
    MIN_ARGUMENTS(1);

    // re-write stack objet in a stream
    stringstream out;

    ((object*)_stack->back())->show(out);
    _stack->pop_back();

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
		void* obj;
		unsigned int size;
		int type;
        string variable(symb->_value);

        // mind the order of heaps
        if (_local_heap.get(variable, obj, size, type)
                || ((_parent_local_heap != NULL) && (_parent_local_heap->get(variable, obj, size, type)))
                || _global_heap->get(variable, obj, size, type))
        {
            _stack->push_back(obj, size, type);
            if (type == cmd_program)
            {
                eval();
            }
        }
		else
            _stack->push_back(symb, symb->size(), cmd_symbol);
	}
	else
        _stack->push_back(symb, symb->size(), cmd_symbol);
}

void purge(void)
{
	MIN_ARGUMENTS(1);
	ARG_MUST_BE_OF_TYPE(0, cmd_symbol);

    string name(((symbol*)_stack->back())->_value);

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

    _stack->pop_back();
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
        (void)_global_heap->get_by_index(i, name, (void*&)obj, size, type);
        cout<<"var "<<i+1<<": name '"<<name<<"', type "<<cmd_type_string[type]<<", value ";
        obj->show();
        cout<<endl;
    }
    if (_parent_local_heap != NULL)
    {
        for (int i=0; i<(int)_parent_local_heap->size(); i++)
        {
            (void)_parent_local_heap->get_by_index(i, name, (void*&)obj, size, type);
            cout<<"local var "<<i+1<<": name '"<<name<<"', type "<<cmd_type_string[type]<<", value ";
            obj->show();
            cout<<endl;
        }
    }
    for (int i=0; i<(int)_local_heap.size(); i++)
    {
        (void)_local_heap.get_by_index(i, name, (void*&)obj, size, type);
        cout<<"local var "<<i+1<<": name '"<<name<<"', type "<<cmd_type_string[type]<<", value ";
        obj->show();
        cout<<endl;
    }
}
