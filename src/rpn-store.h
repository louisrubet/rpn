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
    if (_local_heap->get(variable, obj, size, type) || _global_heap->get(variable, obj, size, type))
	{
		_stack->pop_back();
		_stack->push_back(obj, size, type);
	}
	else
		ERR_CONTEXT(ret_unknown_variable);
}

// carefull : this is not a langage command
void auto_rcl(symbol* symb)
{
	if (symb->_auto_eval)
	{
		void* obj;
		unsigned int size;
		int type;
        if (_local_heap->get(string(symb->_value), obj, size, type) || _global_heap->get(string(symb->_value), obj, size, type))
			_stack->push_back(obj, size, type);
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
    if (_local_heap->erase(name))
    {
        //TODO another error
        ERR_CONTEXT(ret_bad_operand_type);
    }
    else
    {
        if (!_global_heap->erase(name))
            _stack->pop_back();
        else
            ERR_CONTEXT(ret_unknown_variable);
    }
}

void vars(void)
{
	object* obj;
	unsigned int size;
	int type;
	string name;

    for (int i=0; i<(int)_local_heap->size(); i++)
    {
        (void)_local_heap->get_by_index(i, name, (void*&)obj, size, type);
        cout<<"local var "<<i+1<<": name '"<<name<<"', type "<<cmd_type_string[type]<<", value ";
        obj->show();
        cout<<endl;
    }

    for (int i=0; i<(int)_global_heap->size(); i++)
	{
        (void)_global_heap->get_by_index(i, name, (void*&)obj, size, type);
		cout<<"var "<<i+1<<": name '"<<name<<"', type "<<cmd_type_string[type]<<", value ";
		obj->show();
		cout<<endl;
	}
}
