#include "program.hpp"

/// @brief sto keyword implementation
///
void program::rpn_sto(void) {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_symbol);

    (*_heap)[_stack->value<ostring>(0)] = _stack->at(1);
    _stack->pop_front(2);
}

/// @brief sto+ keyword implementation
///
#if 0
void program::rpn_stoadd(void) {
    MIN_ARGUMENTS(2);

    if (_stack->at(0)->_type == cmd_symbol && _stack->at(1)->_type == cmd_number) {
        // get variable value on stack level 1, make op then modify variable
        string variable(((symbol*)_stack->back())->value);
        rpn_rcl();
        if (_err == ret_ok) {
            rpn_plus();
            (*_heap)[variable] = _stack->at(0);
            _stack->pop_front();
        }
    } else if (_stack->at(1)->_type == cmd_symbol && _stack->at(0)->_type == cmd_number) {
        // copy value, get variable value on stack level 1,
        // put back value on stack level 1, make op then modify variable
        rpnstack::copy_and_push_front(*_stack, _stack->size() - 1, _calc_stack);
        _stack->pop_front();

        string variable(((symbol*)_stack->back())->value);
        rpn_rcl();
        if (_err == ret_ok) {
            rpnstack::copy_and_push_front(_calc_stack, _calc_stack.size() - 1, *_stack);
            rpn_plus();
            (*_heap)[variable] = _stack->at(0);
            _stack->pop_front();
        }
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief sto- keyword implementation
///
void program::rpn_stosub(void) {
    MIN_ARGUMENTS(2);

    if (_stack->at(0)->_type == cmd_symbol && _stack->at(1)->_type == cmd_number) {
        // get variable value on stack level 1, make op then modify variable
        string variable(((symbol*)_stack->back())->value);
        rpn_rcl();
        if (_err == ret_ok) {
            rpn_minus();
            (*_heap)[variable] = _stack->at(0);
            _stack->pop_front();
        }
    } else if (_stack->at(1)->_type == cmd_symbol && _stack->at(0)->_type == cmd_number) {
        // copy value, get variable value on stack level 1,
        // put back value on stack level 1, make op then modify variable
        rpnstack::copy_and_push_front(*_stack, _stack->size() - 1, _calc_stack);
        _stack->pop_front();

        string variable(((symbol*)_stack->back())->value);
        rpn_rcl();
        if (_err == ret_ok) {
            rpnstack::copy_and_push_front(_calc_stack, _calc_stack.size() - 1, *_stack);
            rpn_minus();
            (*_heap)[variable] = _stack->at(0);
            _stack->pop_front();
        }
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief sto* keyword implementation
///
void program::rpn_stomul(void) {
    MIN_ARGUMENTS(2);

    if (_stack->at(0)->_type == cmd_symbol && _stack->at(1)->_type == cmd_number) {
        // get variable value on stack level 1, make op then modify variable
        string variable(((symbol*)_stack->back())->value);
        rpn_rcl();
        if (_err == ret_ok) {
            rpn_mul();
            (*_heap)[variable] = _stack->at(0);
            _stack->pop_front();
        }
    } else if (_stack->at(1)->_type == cmd_symbol && _stack->at(0)->_type == cmd_number) {
        // copy value, get variable value on stack level 1,
        // put back value on stack level 1, make op then modify variable
        rpnstack::copy_and_push_front(*_stack, _stack->size() - 1, _calc_stack);
        _stack->pop_front();

        string variable(((symbol*)_stack->back())->value);
        rpn_rcl();
        if (_err == ret_ok) {
            rpnstack::copy_and_push_front(_calc_stack, _calc_stack.size() - 1, *_stack);
            rpn_mul();
            (*_heap)[variable] = _stack->at(0);
            _stack->pop_front();
        }
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief sto/ keyword implementation
///
void program::rpn_stodiv(void) {
    MIN_ARGUMENTS(2);

    if (_stack->at(0)->_type == cmd_symbol && _stack->at(1)->_type == cmd_number) {
        // get variable value on stack level 1, make op then modify variable
        string variable(((symbol*)_stack->back())->value);
        rpn_rcl();
        if (_err == ret_ok) {
            rpn_div();
            (*_heap)[variable] = _stack->at(0);
            _stack->pop_front();
        }
    } else if (_stack->at(1)->_type == cmd_symbol && _stack->at(0)->_type == cmd_number) {
        // copy value, get variable value on stack level 1,
        // put back value on stack level 1, make op then modify variable
        rpnstack::copy_and_push_front(*_stack, _stack->size() - 1, _calc_stack);
        _stack->pop_front();

        string variable(((symbol*)_stack->back())->value);
        rpn_rcl();
        if (_err == ret_ok) {
            rpnstack::copy_and_push_front(_calc_stack, _calc_stack.size() - 1, *_stack);
            rpn_div();
            (*_heap)[variable] = _stack->at(0);
            _stack->pop_front();
        }
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief stosneg keyword implementation
///
void program::rpn_stoneg(void) {
    MIN_ARGUMENTS(1);

    if (_stack->at(0)->_type == cmd_symbol) {
        // get variable value on stack level 1, make op then modify variable
        string variable(((symbol*)_stack->back())->value);
        rpn_rcl();
        if (_err == ret_ok) {
            rpn_neg();
            (*_heap)[variable] = _stack->at(0);
            _stack->pop_front();
        }
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}

/// @brief sinv keyword implementation
///
void program::rpn_stoinv(void) {
    MIN_ARGUMENTS(1);

    if (_stack->at(0)->_type == cmd_symbol) {
        // get variable value on stack level 1, make op then modify variable
        string variable(((symbol*)_stack->back())->value);
        rpn_rcl();
        if (_err == ret_ok) {
            rpn_inv();
            (*_heap)[variable] = _stack->at(0);
            _stack->pop_front();
        }
    } else
        ERR_CONTEXT(ret_bad_operand_type);
}
#endif

/// @brief rcl keyword implementation
///
void program::rpn_rcl(void) {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_symbol);

    // recall a variable
    object* obj;
    string variable(((symbol*)_stack->back())->value);

    // mind the order of heaps
    if (find_variable(variable, obj)) {
        (void)_stack->pop_front();
        _stack->push_front(obj);
    } else
        ERR_CONTEXT(ret_unknown_variable);
}

/// @brief edit keyword implementation
///
void program::rpn_edit(void) {
    MIN_ARGUMENTS(1);

    ostringstream st;

    // re-write stack objet in a stream
    _stack->at(1)->show(st);
    _stack->pop();

    // set it as the linenoise line entry
    linenoisePreloadBuffer((const char*)st.str().c_str());
}

/// @brief recall then eval a symbol variable if it is auto-evaluable
///
/// @param symb the smlbol to recall and autoeval
///
void program::auto_rcl(symbol* symb) {
    if (symb->auto_eval) {
        object* obj;
        string variable(symb->value);

        // mind the order of heaps
        if (find_variable(variable, obj)) {
            _stack->push_front(obj);
            if (obj->_type == cmd_program) rpn_eval();
        } else
            _stack->push_front(symb);
    } else
        _stack->push_front(symb);
}

/// @brief purge keyword implementation
///
void program::rpn_purge(void) {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_symbol);

    //if (_heap->erase(_stack->value<string>(0)) == 0)
    //    ERR_CONTEXT(ret_unknown_variable);
    _stack->pop();
}

/// @brief vars keyword implementation
///
void program::rpn_vars(void) {
    object* obj;
    program* parent = _parent_prog;
    string name;

    // heap variables
    for (int i = 0; i < (int)_heap->size(); i++) {
        (void)_heap->get_by_index(i, name, obj);
        printf("var %d: name '%s', type %s, value ", i + 1, name.c_str(), obj->name());
        obj->show(cout);
        printf("\n");
    }

    // parents local variables
    while (parent != NULL) {
        for (int i = 0; i < (int)parent->_local_heap.size(); i++) {
            (void)parent->_local_heap.get_by_index(i, name, obj);
            printf("local var %d: name '%s', type %s, value ", i + 1, name.c_str(), obj->name());
            obj->show(cout);
            printf("\n");
        }
        parent = parent->_parent_prog;
    }

    // local variables
    for (int i = 0; i < (int)_local_heap.size(); i++) {
        (void)_local_heap.get_by_index(i, name, obj);
        printf("local var %d: name '%s', type %s, value ", i + 1, name.c_str(), obj->name());
        obj->show(cout);
        printf("\n");
    }
}

/// @brief clusr keyword implementation
///
void program::rpn_clusr(void) { _heap->clear(); }
