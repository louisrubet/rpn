// Copyright (c) 2014-2022 Louis Rubet

#include "program.hpp"

/// @brief find variable by its name in local heap, successive parents heaps, global heap
///
/// @param variable the variable name to find
/// @param obj the variable object found
/// @return true variable was found
/// @return false variable was not found
///
bool program::find_variable(string& variable, Object*& obj) {
    bool found = false;
    program* parent = parent_;

    if (local_heap_.get(variable, obj)) {
        found = true;
    } else {
        while (parent != nullptr) {
            if (parent->local_heap_.get(variable, obj)) {
                found = true;
                break;
            }
            parent = parent->parent_;
        }
        if (!found) {
            if (heap_.get(variable, obj)) found = true;
        }
    }

    return found;
}

/// @brief eval keyword implementation
///
void program::rpn_eval(void) {
    bool run_prog = false;
    string prog_text;

    MIN_ARGUMENTS(1);
    if (stack_.type(0) == kSymbol) {
        // recall a variable
        Object* obj;
        string variable(stack_.value<Symbol>(0));
        stack_.pop();

        // if variable holds a program, run this program
        if (find_variable(variable, obj)) {
            if (obj->_type == kProgram) {
                prog_text = stack_.value<Program>(0);
                stack_.pop();
                run_prog = true;
            } else {
                // else recall this variable (i.e. stack_ its content)
                stack_.push_front(obj);
            }
        } else {
            setErrorContext(kUnknownVariable);
        }
    } else if (stack_.type(0) == kProgram) {
        // eval a program
        prog_text = stack_.value<Program>(0);
        stack_.pop();
        run_prog = true;
    } else {
        setErrorContext(kBadOperandType);
    }

    // run prog if any
    if (run_prog) {
        program prog(stack_, heap_, this);

        // make program from entry
        if (prog.parse(prog_text) == kOk) {
            // run it
            prog.run();
        }
    }
}

/// @brief -> keyword (Branch) implementation
///
int program::rpn_inprog(Branch& inprog_obj) {
    string context("->");  // for showing errors
    int count_symbols = 0;
    bool prog_found = false;

    if (inprog_obj.arg1 == -1) {
        setErrorContext(kUnknownError);
        return -1;
    }

    // syntax must be
    // -> <auto evaluated symbol #1> ... <auto evaluated symbol #n> <program>

    // find next Program object
    for (unsigned int i = inprog_obj.arg1 + 1; i < size(); i++) {
        // count symbol
        if (at(i)->_type == kSymbol) {
            count_symbols++;
        } else if (at(i)->_type == kProgram) {
            // stop if prog
            prog_found = true;
            break;
        } else {
            // found something other than symbol
            setErrorContext(kBadOperandType);
            show_error(err_, context);
            return -1;
        }
    }

    // found 0 symbols
    if (count_symbols == 0) {
        setErrorContext(kSyntaxError);
        show_error(err_, context);
        return -1;
    }

    // <program> is missing
    if (!prog_found) {
        setErrorContext(kSyntaxError);
        show_error(err_, context);
        return -1;
    }

    // check symbols Number vs stack_ size
    if (stack_.size() < count_symbols) {
        setErrorContext(kMissingOperand);
        show_error(err_, context);
        return -1;
    }

    // load variables
    for (unsigned int i = inprog_obj.arg1 + count_symbols; i > inprog_obj.arg1; i--) {
        local_heap_[reinterpret_cast<Symbol*>(at(i))->value] = stack_.at(0)->clone();
        stack_.pop();
    }

    // run the program
    string& entry = reinterpret_cast<Program*>(at(inprog_obj.arg1 + count_symbols + 1))->value;
    program prog(stack_, heap_, this);

    // make the program from entry
    if (prog.parse(entry) == kOk) {
        // run it
        prog.run();
    }

    // point on next command
    return inprog_obj.arg1 + count_symbols + 2;
}
