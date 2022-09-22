// Copyright (c) 2014-2022 Louis Rubet

#include "program.h"

/// @brief find variable by its name in local heap, successive parents heaps, global heap
///
/// @param variable the variable name to find
/// @param obj the variable object found
/// @return true variable was found
/// @return false variable was not found
///
bool Program::FindVariable(string& variable, Object*& obj) {
    bool found = false;
    Program* parent = parent_;

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
void Program::RpnEval(void) {
    bool run_prog = false;
    Program* prog = nullptr;

    MIN_ARGUMENTS(1);
    if (stack_.type(0) == kSymbol) {
        // recall a variable
        Object* obj;
        string variable(stack_.value<Symbol>(0));
        stack_.pop();

        // if variable holds a program, run this program
        if (FindVariable(variable, obj)) {
            if (obj->type == kProgram) {
                prog = reinterpret_cast<Program*>(stack_[0]);
                stack_.erase(0, 1, false);
                run_prog = true;
            } else {
                // else recall this variable (i.e. stack_ its content)
                stack_.push_front(obj);
            }
        } else {
            ERROR_CONTEXT(kUnknownVariable);
        }
    } else if (stack_.type(0) == kProgram) {
        // eval a program
        prog = reinterpret_cast<Program*>(stack_[0]);
        stack_.erase(0, 1, false);
        run_prog = true;
    } else {
        ERROR_CONTEXT(kBadOperandType);
    }

    // run prog if any
    if (run_prog && prog != nullptr) {
        prog->Run();
        delete prog;
    }
}

/// @brief -> keyword (Branch) implementation
///
size_t Program::RpnInprog(Branch& inprog_obj) {
    string context("->");  // for showing errors
    size_t count_symbols = 0;
    bool prog_found = false;

    if (inprog_obj.arg1 == kStepOut) {
        ERROR_CONTEXT(kUnknownError);
        return kStepOut;
    }

    // syntax must be
    // -> <auto evaluated symbol #1> ... <auto evaluated symbol #n> <program>

    // find next Program object
    for (unsigned int i = inprog_obj.arg1 + 1; i < size(); i++) {
        // count symbol
        if (at(i)->type == kSymbol) {
            count_symbols++;
        } else if (at(i)->type == kProgram) {
            // stop if prog
            prog_found = true;
            break;
        } else {
            // found something other than symbol
            ERROR_CONTEXT(kBadOperandType);
            ShowError(err_, context);
            return kStepOut;
        }
    }

    // found 0 symbols
    if (count_symbols == 0) {
        ERROR_CONTEXT(kSyntaxError);
        ShowError(err_, context);
        return kStepOut;
    }

    // <program> is missing
    if (!prog_found) {
        ERROR_CONTEXT(kSyntaxError);
        ShowError(err_, context);
        return kStepOut;
    }

    // check symbols Number vs stack_ size
    if (stack_.size() < count_symbols) {
        ERROR_CONTEXT(kMissingOperand);
        ShowError(err_, context);
        return kStepOut;
    }

    // load variables
    for (size_t i = inprog_obj.arg1 + count_symbols; i > inprog_obj.arg1; i--) {
        local_heap_[reinterpret_cast<Symbol*>(at(i))->value] = stack_.at(0)->Clone();
        stack_.pop();
    }

    // run the program
    string& entry = reinterpret_cast<Program*>(at(inprog_obj.arg1 + count_symbols + 1))->value;
    Program prog(stack_, heap_, this);

    // make the program from entry
    if (prog.Parse(entry) == kOk) {
        // run it
        prog.Run();
    }

    // point on next command
    return inprog_obj.arg1 + count_symbols + 2;
}
