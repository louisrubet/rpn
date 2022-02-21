#include "program.hpp"

/// @brief find variable by its name in local heap, successive parents heaps, global heap
///
/// @param variable the variable name to find
/// @param obj the variable object found
/// @return true variable was found
/// @return false variable was not found
///
bool program::find_variable(string& variable, object*& obj) {
    bool found = false;
    program* parent = _parent;

    if (_local_heap.get(variable, obj))
        found = true;
    else {
        while (parent != nullptr) {
            if (parent->_local_heap.get(variable, obj)) {
                found = true;
                break;
            }
            parent = parent->_parent;
        }
        if (!found) {
            if (_heap.get(variable, obj)) found = true;
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
    if (_stack.type(0) == cmd_symbol) {
        // recall a variable
        object* obj;
        string variable(_stack.value<symbol>(0));
        _stack.pop();

        // if variable holds a program, run this program
        if (find_variable(variable, obj)) {
            if (obj->_type == cmd_program) {
                prog_text = _stack.value<oprogram>(0);
                _stack.pop();
                run_prog = true;
            } else {
                // else recall this variable (i.e. stack its content)
                _stack.push_front(obj);
            }
        } else
            setErrorContext(ret_unknown_variable);
    } else if (_stack.type(0) == cmd_program) {
        // eval a program
        prog_text = _stack.value<oprogram>(0);
        _stack.pop();
        run_prog = true;
    } else
        setErrorContext(ret_bad_operand_type);

    // run prog if any
    if (run_prog) {
        program prog(_stack, _heap, this);

        // make program from entry
        if (prog.parse(prog_text) == ret_ok) {
            // run it
            prog.run();
        }
    }
}

/// @brief -> keyword (branch) implementation
///
int program::rpn_inprog(branch& myobj) {
    string context("->");  // for showing errors
    int count_symbols = 0;
    bool prog_found = false;

    if (myobj.arg1 == -1) {
        setErrorContext(ret_unknown_err);
        return -1;
    }

    // syntax must be
    // -> <auto evaluated symbol #1> ... <auto evaluated symbol #n> <oprogram>

    // find next oprogram object
    for (unsigned int i = myobj.arg1 + 1; i < size(); i++) {
        // count symbol
        if (at(i)->_type == cmd_symbol) count_symbols++;
        // stop if prog
        else if (at(i)->_type == cmd_program) {
            prog_found = true;
            break;
        }
        // found something other than symbol
        else {
            setErrorContext(ret_bad_operand_type);
            show_error(_err, context);
            return -1;
        }
    }

    // found 0 symbols
    if (count_symbols == 0) {
        setErrorContext(ret_syntax);
        show_error(_err, context);
        return -1;
    }

    // <oprogram> is missing
    if (!prog_found) {
        setErrorContext(ret_syntax);
        show_error(_err, context);
        return -1;
    }

    // check symbols number vs stack size
    if (stack_size() < count_symbols) {
        setErrorContext(ret_missing_operand);
        show_error(_err, context);
        return -1;
    }

    // load variables
    for (unsigned int i = myobj.arg1 + count_symbols; i > myobj.arg1; i--) {
        _local_heap[string(((symbol*)at(i))->value)] = _stack.at(0)->clone();
        _stack.pop_front();
    }

    // run the program
    string entry(((oprogram*)at(myobj.arg1 + count_symbols + 1))->value);
    program prog(_stack, _heap, this);

    // make the program from entry
    if (prog.parse(entry) == ret_ok) {
        // run it
        prog.run();
    }

    // point on next command
    return myobj.arg1 + count_symbols + 2;
}
