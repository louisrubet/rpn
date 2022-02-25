// Copyright (c) 2014-2022 Louis Rubet

#include "input.hpp"
#include "program.hpp"

/// @brief sto keyword implementation
///
void program::RpnSto(void) {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, kSymbol);

    // store symbol with first value
    const auto it = heap_.find(stack_.value<String>(0));
    if (it != heap_.end()) {
        delete it->second;
        heap_.erase(it);
    }
    heap_[stack_.value<String>(0)] = stack_.at(1)->Clone();
    stack_.erase(0, 2);
}

/// @brief sto+ keyword implementation
///
void program::RpnStoadd(void) {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, kSymbol);
    if (heap_.find(stack_.value<String>(0)) == heap_.end()) {
        ERROR_CONTEXT(kUnknownVariable);
        return;
    }
    RpnDup();
    RpnRcl();  // TODO(louis) is rcl the good one? it will recall local variables too
    RpnRot();
    RpnPlus();
    RpnSwap();
    RpnSto();
}

/// @brief sto- keyword implementation
///
void program::RpnStosub(void) {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, kSymbol);
    if (heap_.find(stack_.value<String>(0)) == heap_.end()) {
        ERROR_CONTEXT(kUnknownVariable);
        return;
    }
    RpnDup();
    RpnRcl();
    RpnRot();
    RpnMinus();
    RpnSwap();
    RpnSto();
}

/// @brief sto* keyword implementation
///
void program::RpnStomul(void) {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, kSymbol);
    if (heap_.find(stack_.value<String>(0)) == heap_.end()) {
        ERROR_CONTEXT(kUnknownVariable);
        return;
    }
    RpnDup();
    RpnRcl();
    RpnRot();
    RpnMul();
    RpnSwap();
    RpnSto();
}

/// @brief sto/ keyword implementation
///
void program::RpnStodiv(void) {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, kSymbol);
    if (heap_.find(stack_.value<String>(0)) == heap_.end()) {
        ERROR_CONTEXT(kUnknownVariable);
        return;
    }
    RpnDup();
    RpnRcl();
    RpnRot();
    RpnDiv();
    RpnSwap();
    RpnSto();
}

/// @brief stosneg keyword implementation
///
void program::RpnStoneg(void) {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kSymbol);
    if (heap_.find(stack_.value<String>(0)) == heap_.end()) {
        ERROR_CONTEXT(kUnknownVariable);
        return;
    }
    RpnDup();
    RpnRcl();
    RpnNeg();
    RpnSwap();
    RpnSto();
}

/// @brief sinv keyword implementation
///
void program::RpnStoinv(void) {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kSymbol);
    if (heap_.find(stack_.value<String>(0)) == heap_.end()) {
        ERROR_CONTEXT(kUnknownVariable);
        return;
    }
    RpnDup();
    RpnRcl();
    RpnInv();
    RpnSwap();
    RpnSto();
}

/// @brief rcl keyword implementation
///
void program::RpnRcl(void) {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kSymbol);

    // recall a variable
    Object* obj;
    string variable(stack_.value<Symbol>(0));

    // mind the order of heaps
    if (FindVariable(variable, obj)) {
        (void)stack_.pop();
        stack_.push_front(obj->Clone());
    } else {
        ERROR_CONTEXT(kUnknownVariable);
    }
}

/// @brief edit keyword implementation
///
void program::RpnEdit(void) {
    MIN_ARGUMENTS(1);

    ostringstream st;

    // re-write stack_ objet in a stream
    stack_.at(0)->Show(st);
    stack_.pop();

    // set it as the linenoise line entry
    Input::Preload(st.str().c_str());
}

/// @brief recall then eval a symbol variable if it is auto-evaluable
///
/// @param symb the smlbol to recall and autoeval
///
void program::AutoRcl(Symbol* symb) {
    if (symb->auto_eval) {
        Object* obj;
        string variable(symb->value);

        // mind the order of heaps
        if (FindVariable(variable, obj)) {
            stack_.push_front(obj->Clone());
            if (obj->_type == kProgram) RpnEval();
        } else {
            stack_.push_front(symb->Clone());
        }
    } else {
        stack_.push_front(symb->Clone());
    }
}

/// @brief purge keyword implementation
///
void program::RpnPurge(void) {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kSymbol);

    const auto i = heap_.find(stack_.value<Symbol>(0));
    if (i != heap_.end()) {
        delete i->second;
        heap_.erase(i);
    } else {
        ERROR_CONTEXT(kUnknownVariable);
    }
    stack_.pop();
}

/// @brief vars keyword implementation
///
void program::RpnVars(void) {
    Object* obj;
    program* parent = parent_;
    string name;
    int index = 1;

    // heap variables
    for (auto i : heap_) {
        cout << "var " << index++ << ": name '" << i.first << "', type " << i.second->Name() << ", value ";
        i.second->Show(cout) << endl;
    }

    // local variables
    for (auto i : local_heap_) {
        cout << "var " << index++ << ": name '" << i.first << "', type " << i.second->Name() << ", value ";
        i.second->Show(cout) << endl;
    }

    // parents local variables
    while (parent != nullptr) {
        for (auto i : parent->local_heap_) {
            cout << "parent var " << index++ << ": name '" << i.first << "', type " << i.second->Name() << ", value ";
            obj->Show(cout) << endl;
        }
        parent = parent->parent_;
    }
}

/// @brief clusr keyword implementation
///
void program::RpnClusr(void) { heap_.clear(); }
