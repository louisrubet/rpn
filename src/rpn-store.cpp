// Copyright (c) 2014-2022 Louis Rubet

#include "input.hpp"
#include "program.hpp"

/// @brief sto keyword implementation
///
void program::rpn_sto(void) {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_symbol);

    // store symbol with first value
    const auto it = _heap.find(_stack.value<ostring>(0));
    if (it != _heap.end()) {
        delete it->second;
        _heap.erase(it);
    }
    _heap[_stack.value<ostring>(0)] = _stack.at(1)->clone();
    _stack.erase(0, 2);
}

/// @brief sto+ keyword implementation
///
void program::rpn_stoadd(void) {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_symbol);
    if (_heap.find(_stack.value<ostring>(0)) == _heap.end()) {
        setErrorContext(ret_unknown_variable);
        return;
    }
    rpn_dup();
    rpn_rcl();  // TODO(louis) is rcl the good one? it will recall local variables too
    rpn_rot();
    rpn_plus();
    rpn_swap();
    rpn_sto();
}

/// @brief sto- keyword implementation
///
void program::rpn_stosub(void) {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_symbol);
    if (_heap.find(_stack.value<ostring>(0)) == _heap.end()) {
        setErrorContext(ret_unknown_variable);
        return;
    }
    rpn_dup();
    rpn_rcl();
    rpn_rot();
    rpn_minus();
    rpn_swap();
    rpn_sto();
}

/// @brief sto* keyword implementation
///
void program::rpn_stomul(void) {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_symbol);
    if (_heap.find(_stack.value<ostring>(0)) == _heap.end()) {
        setErrorContext(ret_unknown_variable);
        return;
    }
    rpn_dup();
    rpn_rcl();
    rpn_rot();
    rpn_mul();
    rpn_swap();
    rpn_sto();
}

/// @brief sto/ keyword implementation
///
void program::rpn_stodiv(void) {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_symbol);
    if (_heap.find(_stack.value<ostring>(0)) == _heap.end()) {
        setErrorContext(ret_unknown_variable);
        return;
    }
    rpn_dup();
    rpn_rcl();
    rpn_rot();
    rpn_div();
    rpn_swap();
    rpn_sto();
}

/// @brief stosneg keyword implementation
///
void program::rpn_stoneg(void) {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_symbol);
    if (_heap.find(_stack.value<ostring>(0)) == _heap.end()) {
        setErrorContext(ret_unknown_variable);
        return;
    }
    rpn_dup();
    rpn_rcl();
    rpn_neg();
    rpn_swap();
    rpn_sto();
}

/// @brief sinv keyword implementation
///
void program::rpn_stoinv(void) {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_symbol);
    if (_heap.find(_stack.value<ostring>(0)) == _heap.end()) {
        setErrorContext(ret_unknown_variable);
        return;
    }
    rpn_dup();
    rpn_rcl();
    rpn_inv();
    rpn_swap();
    rpn_sto();
}

/// @brief rcl keyword implementation
///
void program::rpn_rcl(void) {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_symbol);

    // recall a variable
    object* obj;
    string variable(_stack.value<symbol>(0));

    // mind the order of heaps
    if (find_variable(variable, obj)) {
        (void)_stack.pop();
        _stack.push_front(obj->clone());
    } else {
        setErrorContext(ret_unknown_variable);
    }
}

/// @brief edit keyword implementation
///
void program::rpn_edit(void) {
    MIN_ARGUMENTS(1);

    ostringstream st;

    // re-write stack objet in a stream
    _stack.at(0)->show(st);
    _stack.pop();

    // set it as the linenoise line entry
    Input::preload(st.str().c_str());
}

/// @brief recall then eval a symbol variable if it is auto-evaluable
///
/// @param symb the smlbol to recall and autoeval
///
void program::auto_rcl(symbol* symb) {
    if (symb->autoEval) {
        object* obj;
        string variable(symb->value);

        // mind the order of heaps
        if (find_variable(variable, obj)) {
            _stack.push_front(obj->clone());
            if (obj->_type == cmd_program) rpn_eval();
        } else {
            _stack.push_front(symb->clone());
        }
    } else {
        _stack.push_front(symb->clone());
    }
}

/// @brief purge keyword implementation
///
void program::rpn_purge(void) {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_symbol);

    const auto i = _heap.find(_stack.value<symbol>(0));
    if (i != _heap.end()) {
        delete i->second;
        _heap.erase(i);
    } else {
        setErrorContext(ret_unknown_variable);
    }
    _stack.pop();
}

/// @brief vars keyword implementation
///
void program::rpn_vars(void) {
    object* obj;
    program* parent = _parent;
    string name;
    int index = 1;

    // heap variables
    for (auto i : _heap) {
        cout << "var " << index++ << ": name '" << i.first << "', type " << i.second->name() << ", value ";
        i.second->show(cout) << endl;
    }

    // local variables
    for (auto i : _local_heap) {
        cout << "var " << index++ << ": name '" << i.first << "', type " << i.second->name() << ", value ";
        i.second->show(cout) << endl;
    }

    // parents local variables
    while (parent != nullptr) {
        for (auto i : parent->_local_heap) {
            cout << "parent var " << index++ << ": name '" << i.first << "', type " << i.second->name() << ", value ";
            obj->show(cout) << endl;
        }
        parent = parent->_parent;
    }
}

/// @brief clusr keyword implementation
///
void program::rpn_clusr(void) { _heap.clear(); }
