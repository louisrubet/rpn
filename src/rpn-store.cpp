// Copyright (c) 2014-2022 Louis Rubet

#include "input.hpp"
#include "program.hpp"

/// @brief sto keyword implementation
///
void program::rpn_sto(void) {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, kSymbol);

    // store symbol with first value
    const auto it = heap_.find(stack_.value<String>(0));
    if (it != heap_.end()) {
        delete it->second;
        heap_.erase(it);
    }
    heap_[stack_.value<String>(0)] = stack_.at(1)->clone();
    stack_.erase(0, 2);
}

/// @brief sto+ keyword implementation
///
void program::rpn_stoadd(void) {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, kSymbol);
    if (heap_.find(stack_.value<String>(0)) == heap_.end()) {
        setErrorContext(kUnknownVariable);
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
    ARG_MUST_BE_OF_TYPE(0, kSymbol);
    if (heap_.find(stack_.value<String>(0)) == heap_.end()) {
        setErrorContext(kUnknownVariable);
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
    ARG_MUST_BE_OF_TYPE(0, kSymbol);
    if (heap_.find(stack_.value<String>(0)) == heap_.end()) {
        setErrorContext(kUnknownVariable);
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
    ARG_MUST_BE_OF_TYPE(0, kSymbol);
    if (heap_.find(stack_.value<String>(0)) == heap_.end()) {
        setErrorContext(kUnknownVariable);
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
    ARG_MUST_BE_OF_TYPE(0, kSymbol);
    if (heap_.find(stack_.value<String>(0)) == heap_.end()) {
        setErrorContext(kUnknownVariable);
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
    ARG_MUST_BE_OF_TYPE(0, kSymbol);
    if (heap_.find(stack_.value<String>(0)) == heap_.end()) {
        setErrorContext(kUnknownVariable);
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
    ARG_MUST_BE_OF_TYPE(0, kSymbol);

    // recall a variable
    Object* obj;
    string variable(stack_.value<Symbol>(0));

    // mind the order of heaps
    if (find_variable(variable, obj)) {
        (void)stack_.pop();
        stack_.push_front(obj->clone());
    } else {
        setErrorContext(kUnknownVariable);
    }
}

/// @brief edit keyword implementation
///
void program::rpn_edit(void) {
    MIN_ARGUMENTS(1);

    ostringstream st;

    // re-write stack_ objet in a stream
    stack_.at(0)->show(st);
    stack_.pop();

    // set it as the linenoise line entry
    Input::preload(st.str().c_str());
}

/// @brief recall then eval a symbol variable if it is auto-evaluable
///
/// @param symb the smlbol to recall and autoeval
///
void program::auto_rcl(Symbol* symb) {
    if (symb->auto_eval) {
        Object* obj;
        string variable(symb->value);

        // mind the order of heaps
        if (find_variable(variable, obj)) {
            stack_.push_front(obj->clone());
            if (obj->_type == kProgram) rpn_eval();
        } else {
            stack_.push_front(symb->clone());
        }
    } else {
        stack_.push_front(symb->clone());
    }
}

/// @brief purge keyword implementation
///
void program::rpn_purge(void) {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kSymbol);

    const auto i = heap_.find(stack_.value<Symbol>(0));
    if (i != heap_.end()) {
        delete i->second;
        heap_.erase(i);
    } else {
        setErrorContext(kUnknownVariable);
    }
    stack_.pop();
}

/// @brief vars keyword implementation
///
void program::rpn_vars(void) {
    Object* obj;
    program* parent = parent_;
    string name;
    int index = 1;

    // heap variables
    for (auto i : heap_) {
        cout << "var " << index++ << ": name '" << i.first << "', type " << i.second->name() << ", value ";
        i.second->show(cout) << endl;
    }

    // local variables
    for (auto i : local_heap_) {
        cout << "var " << index++ << ": name '" << i.first << "', type " << i.second->name() << ", value ";
        i.second->show(cout) << endl;
    }

    // parents local variables
    while (parent != nullptr) {
        for (auto i : parent->local_heap_) {
            cout << "parent var " << index++ << ": name '" << i.first << "', type " << i.second->name() << ", value ";
            obj->show(cout) << endl;
        }
        parent = parent->parent_;
    }
}

/// @brief clusr keyword implementation
///
void program::rpn_clusr(void) { heap_.clear(); }
