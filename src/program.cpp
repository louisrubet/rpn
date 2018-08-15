#include "program.hpp"

//< return type strings
const char* program::s_ret_value_string[ret_max] = RET_VALUE_STRINGS;

//< kanguage reserved keywords (allowed types are cmd_keyword, cmd_branch or cmd_undef)
program::keyword_t program::s_keywords[] = {
    // GENERAL
    {cmd_undef, "", NULL, "\nGENERAL"},
    {cmd_keyword, "nop", &program::rpn_nop, "no operation"},
    {cmd_keyword, "help", &program::rpn_help, "this help message"},
    {cmd_keyword, "h", &program::rpn_help, ""},
    {cmd_keyword, "?", &program::rpn_help, ""},
    {cmd_keyword, "quit", &program::rpn_good_bye, "quit software"},
    {cmd_keyword, "q", &program::rpn_good_bye, ""},
    {cmd_keyword, "exit", &program::rpn_good_bye, ""},
    {cmd_keyword, "test", &program::rpn_test, ""},  // not seen by user
    {cmd_keyword, "version", &program::rpn_version, "show rpn version"},
    {cmd_keyword, "uname", &program::rpn_uname, "show rpn complete identification string"},
    {cmd_keyword, "history", &program::rpn_history, "see commands history"},

    // USUAL OPERATIONS ON REALS AND COMPLEXES
    {cmd_undef, "", NULL, "\nUSUAL OPERATIONS ON REALS AND COMPLEXES"},
    {cmd_keyword, "+", &program::rpn_plus, "addition"},
    {cmd_keyword, "-", &program::rpn_minus, "substraction"},
    {cmd_keyword, "chs", &program::rpn_neg, "negation"},
    {cmd_keyword, "neg", &program::rpn_neg, ""},
    {cmd_keyword, "*", &program::rpn_mul, "multiplication"},
    {cmd_keyword, "/", &program::rpn_div, "division"},
    {cmd_keyword, "inv", &program::rpn_inv, "inverse"},
    {cmd_keyword, "^", &program::rpn_power, "power"},
    {cmd_keyword, "pow", &program::rpn_power, ""},
    {cmd_keyword, "sqrt", &program::rpn_squareroot, "rpn_square root"},
    {cmd_keyword, "sq", &program::rpn_square, "rpn_square"},
    {cmd_keyword, "sqr", &program::rpn_square, ""},
    {cmd_keyword, "abs", &program::rpn_abs, "absolute value"},
    {cmd_keyword, "dec", &program::rpn_dec, "decimal representation"},
    {cmd_keyword, "hex", &program::rpn_hex, "hexadecimal representation"},
    {cmd_keyword, "bin", &program::rpn_bin, "decimal representation"},
    {cmd_keyword, "base", &program::rpn_base, "arbitrary base representation"},
    {cmd_keyword, "sign", &program::rpn_sign, "1 if number at stack level 1 is > 0, 0 if == 0, -1 if <= 0"},

    // OPERATIONS ON REALS
    {cmd_undef, "", NULL, "\nOPERATIONS ON REALS"},
    {cmd_keyword, "%", &program::rpn_purcent, "purcent"},
    {cmd_keyword, "%CH", &program::rpn_purcentCH, "inverse purcent"},
    {cmd_keyword, "mod", &program::rpn_modulo, "modulo"},
    {cmd_keyword, "fact", &program::rpn_fact, "n! for integer n or Gamma(x+1) for fractional x"},
    {cmd_keyword, "mant", &program::rpn_mant, "mantissa of a real number"},
    {cmd_keyword, "xpon", &program::rpn_xpon, "exponant of a real number"},
    {cmd_keyword, "floor", &program::rpn_floor, "largest number <="},
    {cmd_keyword, "ceil", &program::rpn_ceil, "smallest number >="},
    {cmd_keyword, "ip", &program::rpn_ip, "integer part"},
    {cmd_keyword, "fp", &program::rpn_fp, "fractional part"},
    {cmd_keyword, "min", &program::rpn_min, "min of 2 real numbers"},
    {cmd_keyword, "max", &program::rpn_max, "max of 2 real numbers"},

    // nOPERATIONS ON COMPLEXES
    {cmd_undef, "", NULL, "\nOPERATIONS ON COMPLEXES"},
    {cmd_keyword, "re", &program::rpn_re, "complex real part"},
    {cmd_keyword, "im", &program::rpn_im, "complex imaginary part"},
    {cmd_keyword, "conj", &program::rpn_conj, "complex conjugate"},
    {cmd_keyword, "arg", &program::rpn_arg, "complex argument in radians"},
    {cmd_keyword, "c->r", &program::rpn_c2r, "transform a complex in 2 reals"},
    {cmd_keyword, "r->c", &program::rpn_r2c, "transform 2 reals in a complex"},
    {cmd_keyword, "p->r", &program::rpn_p2r, "cartesian to polar"},
    {cmd_keyword, "r->p", &program::rpn_r2p, "polar to cartesian"},

    // MODE
    {cmd_undef, "", NULL, "\nMODE"},
    {cmd_keyword, "std", &program::rpn_std, "standard floating numbers representation. ex: std"},
    {cmd_keyword, "fix", &program::rpn_fix, "fixed point representation. ex: 6 fix"},
    {cmd_keyword, "sci", &program::rpn_sci, "scientific floating point representation. ex: 20 sci"},
    {cmd_keyword, "prec", &program::rpn_precision, "set float precision in bits. ex: 256 prec"},
    {cmd_keyword, "round", &program::rpn_round,
     "set float rounding mode.\n\tex: [\"nearest\", \"toward zero\", \"toward "
     "+inf\", \"toward -inf\", \"away from zero\"] round"},
    {cmd_keyword, "default", &program::rpn_default, "set float representation and precision to default"},
    {cmd_keyword, "type", &program::rpn_type, "show type of stack first entry"},

    // TESTS
    {cmd_undef, "", NULL, "\nTEST"},
    {cmd_keyword, ">", &program::rpn_sup, "binary operator >"},
    {cmd_keyword, ">=", &program::rpn_sup_eq, "binary operator >="},
    {cmd_keyword, "<", &program::rpn_inf, "binary operator <"},
    {cmd_keyword, "<=", &program::rpn_inf_eq, "binary operator <="},
    {cmd_keyword, "!=", &program::rpn_diff, "binary operator != (different)"},
    {cmd_keyword, "==", &program::rpn_eq, "binary operator == (equal)"},
    {cmd_keyword, "and", &program::rpn_test_and, "boolean operator and"},
    {cmd_keyword, "or", &program::rpn_test_or, "boolean operator or"},
    {cmd_keyword, "xor", &program::rpn_test_xor, "boolean operator xor"},
    {cmd_keyword, "not", &program::rpn_test_not, "boolean operator not"},
    {cmd_keyword, "same", &program::rpn_same, "boolean operator same (equal)"},

    // STACK
    {cmd_undef, "", NULL, "\nSTACK"},
    {cmd_keyword, "swap", &program::rpn_swap, "swap 2 first stack entries"},
    {cmd_keyword, "drop", &program::rpn_drop, "drop first stack entry"},
    {cmd_keyword, "drop2", &program::rpn_drop2, "drop 2 first stack entries"},
    {cmd_keyword, "dropn", &program::rpn_dropn, "drop n first stack entries"},
    {cmd_keyword, "del", &program::rpn_erase, "drop all stack entries"},
    {cmd_keyword, "erase", &program::rpn_erase, ""},
    {cmd_keyword, "rot", &program::rpn_rot, "rotate 3 first stack entries"},
    {cmd_keyword, "dup", &program::rpn_dup, "duplicate first stack entry"},
    {cmd_keyword, "dup2", &program::rpn_dup2, "duplicate 2 first stack entries"},
    {cmd_keyword, "dupn", &program::rpn_dupn, "duplicate n first stack entries"},
    {cmd_keyword, "pick", &program::rpn_pick, "push a copy of  the given stack level onto the stack"},
    {cmd_keyword, "depth", &program::rpn_depth, "give stack depth"},
    {cmd_keyword, "roll", &program::rpn_roll, "move a stack entry to the top of the stack"},
    {cmd_keyword, "rolld", &program::rpn_rolld, "move the element on top of the stack to a higher stack position"},
    {cmd_keyword, "over", &program::rpn_over, "push a copy of the element in stack level 2 onto the stack"},

    // STRING
    {cmd_undef, "", NULL, "\nSTRING"},
    {cmd_keyword, "->str", &program::rpn_instr, "convert an object into a string"},
    {cmd_keyword, "str->", &program::rpn_strout, "convert a string into an object"},
    {cmd_keyword, "chr", &program::rpn_chr, "convert ASCII character code in stack level 1 into a string"},
    {cmd_keyword, "num", &program::rpn_num,
     "return ASCII code of the first character of the string in stack level 1 "
     "as a real number"},
    {cmd_keyword, "size", &program::rpn_strsize, "return the length of the string"},
    {cmd_keyword, "pos", &program::rpn_strpos, "seach for the string in level 1 within the string in level 2"},
    {cmd_keyword, "sub", &program::rpn_strsub, "return a substring of the string in level 3"},

    // BRANCH
    {cmd_undef, "", NULL, "\nBRANCH"},
    {cmd_branch, "if", (program_fn_t)&program::rpn_if,
     "if <test-instruction> then <true-instructions> else <false-instructions> "
     "end"},
    {cmd_branch, "then", (program_fn_t)&program::rpn_then, "used with if"},
    {cmd_branch, "else", (program_fn_t)&program::rpn_else, "used with if"},
    {cmd_branch, "end", (program_fn_t)&program::rpn_end, "used with various branch instructions"},
    {cmd_branch, "start", (program_fn_t)&program::rpn_start, "<start> <end> start <instructions> next|<step> step"},
    {cmd_branch, "for", (program_fn_t)&program::rpn_for,
     "<start> <end> for <variable> <instructions> next|<step> step"},
    {cmd_branch, "next", (program_fn_t)&program::rpn_next, "used with start and for"},
    {cmd_branch, "step", (program_fn_t)&program::rpn_step, "used with start and for"},
    {cmd_keyword, "ift", &program::rpn_ift, "similar to if-then-end, <test-instruction> <true-instruction> ift"},
    {cmd_keyword, "ifte", &program::rpn_ifte,
     "similar to if-then-else-end, <test-instruction> <true-instruction> "
     "<false-instruction> ifte"},
    {cmd_branch, "do", (program_fn_t)&program::rpn_do, "do <instructions> until <condition> end"},
    {cmd_branch, "until", (program_fn_t)&program::rpn_until, "used with do"},
    {cmd_branch, "unti", (program_fn_t)&program::rpn_until, ""},
    {cmd_branch, "while", (program_fn_t)&program::rpn_while, "while <test-instruction> repeat <loop-instructions> end"},
    {cmd_branch, "whil", (program_fn_t)&program::rpn_while, ""},
    {cmd_branch, "repeat", (program_fn_t)&program::rpn_repeat, "used with while"},
    {cmd_branch, "repea", (program_fn_t)&program::rpn_repeat, ""},

    // STORE
    {cmd_undef, "", NULL, "\nSTORE"},
    {cmd_keyword, "sto", &program::rpn_sto, "store a variable. ex: 1 'name' sto"},
    {cmd_keyword, "rcl", &program::rpn_rcl, "recall a variable. ex: 'name' rcl"},
    {cmd_keyword, "purge", &program::rpn_purge, "delete a variable. ex: 'name' purge"},
    {cmd_keyword, "vars", &program::rpn_vars, "list all variables"},
    {cmd_keyword, "clusr", &program::rpn_clusr, "erase all variables"},
    {cmd_keyword, "edit", &program::rpn_edit, "edit a variable content"},
    {cmd_keyword, "sto+", &program::rpn_stoadd, "add to a stored variable. ex: 1 'name' sto+ 'name' 2 sto+"},
    {cmd_keyword, "sto-", &program::rpn_stosub, "substract to a stored variable. ex: 1 'name' sto- 'name' 2 sto-"},
    {cmd_keyword, "sto*", &program::rpn_stomul, "multiply a stored variable. ex: 3 'name' sto* 'name' 2 sto*"},
    {cmd_keyword, "sto/", &program::rpn_stodiv, "divide a stored variable. ex: 3 'name' sto/ 'name' 2 sto/"},
    {cmd_keyword, "sneg", &program::rpn_stoneg, "negate a variable. ex: 'name' sneg"},
    {cmd_keyword, "sinv", &program::rpn_stoinv, "inverse a variable. ex: 1 'name' sinv"},

    // PROGRAM
    {cmd_undef, "", NULL, "\nPROGRAM"},
    {cmd_keyword, "eval", &program::rpn_eval, "evaluate (run) a program, or recall a variable. ex: 'my_prog' eval"},
    {cmd_branch, "->", (program_fn_t)&program::rpn_inprog,
     "load program local variables. ex: << -> n m << 0 n m for i i + next >> "
     ">>"},

    // TRIG ON REALS AND COMPLEXES
    {cmd_undef, "", NULL, "\nTRIG ON REALS AND COMPLEXES"},
    {cmd_keyword, "pi", &program::rpn_pi, "pi constant"},
    {cmd_keyword, "sin", &program::rpn_sin, "sinus"},
    {cmd_keyword, "asin", &program::rpn_asin, "arg sinus"},
    {cmd_keyword, "cos", &program::rpn_cos, "cosinus"},
    {cmd_keyword, "acos", &program::rpn_acos, "arg cosinus"},
    {cmd_keyword, "tan", &program::rpn_tan, "tangent"},
    {cmd_keyword, "atan", &program::rpn_atan, "arg tangent"},
    {cmd_keyword, "d->r", &program::rpn_d2r, "convert degrees to radians"},
    {cmd_keyword, "r->d", &program::rpn_r2d, "convert radians to degrees"},

    // LOGS ON REALS AND COMPLEXES
    {cmd_undef, "", NULL, "\nLOGS ON REALS AND COMPLEXES"},
    {cmd_keyword, "e", &program::rpn_e, "Euler constant"},
    {cmd_keyword, "ln", &program::rpn_ln, "logarithm base e"},
    {cmd_keyword, "log", &program::rpn_ln, ""},
    {cmd_keyword, "lnp1", &program::rpn_lnp1, "ln(1+x) which is useful when x is close to 0"},
    {cmd_keyword, "exp", &program::rpn_exp, "exponential"},
    {cmd_keyword, "expm", &program::rpn_expm, "exp(x)-1 which is useful when x is close to 0"},
    {cmd_keyword, "log10", &program::rpn_log10, "logarithm base 10"},
    {cmd_keyword, "alog10", &program::rpn_alog10, "exponential base 10"},
    {cmd_keyword, "exp10", &program::rpn_alog10, ""},
    {cmd_keyword, "log2", &program::rpn_log2, "logarithm base 2"},
    {cmd_keyword, "alog2", &program::rpn_alog2, "exponential base 2"},
    {cmd_keyword, "exp2", &program::rpn_alog2, ""},
    {cmd_keyword, "sinh", &program::rpn_sinh, "hyperbolic sine"},
    {cmd_keyword, "asinh", &program::rpn_asinh, "inverse hyperbolic sine"},
    {cmd_keyword, "cosh", &program::rpn_sinh, "hyperbolic cosine"},
    {cmd_keyword, "acosh", &program::rpn_acosh, "inverse hyperbolic cosine"},
    {cmd_keyword, "tanh", &program::rpn_tanh, "hyperbolic tangent"},
    {cmd_keyword, "atanh", &program::rpn_atanh, "inverse hyperbolic tangent"},

    // TIME AND DATE
    {cmd_undef, "", NULL, "\nTIME AND DATE"},
    {cmd_keyword, "time", &program::rpn_time, "time in local format"},
    {cmd_keyword, "date", &program::rpn_date, "date in local format"},
    {cmd_keyword, "ticks", &program::rpn_ticks, "system tick in µs"},

    // end
    {cmd_max, "", NULL, ""},
};

/// @brief run a program on a stack and a heap
///
/// @param stk the stack, storing prog result
/// @param hp the heap, storing variables
/// @return ret_value see this type
///
ret_value program::run(stack& stk, heap& hp) {
    bool go_out = false;
    ret_value ret = ret_ok;
    cmd_type_t type;

    // stack comes from outside
    _stack = &stk;

    // global heap comes from outside
    _heap = &hp;

    _err = ret_ok;
    _err_context = "";

    // branches for 'if'
    ret = preprocess();
    if (ret != ret_ok) return ret;

    // iterate commands
    for (int i = 0; (go_out == false) && (interrupt_now == false) && (i < (int)size());) {
        type = (cmd_type_t)seq_type(i);

        // could be an auto-evaluated symbol
        if (type == cmd_symbol) {
            auto_rcl((symbol*)seq_obj(i));
            i++;
        }

        // a keyword
        else if (type == cmd_keyword) {
            keyword* k = (keyword*)seq_obj(i);
            // call matching function
            (this->*(k->_fn))();
            switch (_err) {
                // no pb -> go on
                case ret_ok:
                    break;
                // explicit go out software
                case ret_good_bye:
                    go_out = true;
                    ret = ret_good_bye;
                    break;
                default:
                    // error: abort prog
                    go_out = true;

                    // error: show it
                    if (show_error(_err, _err_context) == ret_deadly) {
                        // pb showing error -> go out software
                        ret = ret_good_bye;
                    }
                    break;
            }
            i++;
        }

        // a branch keyword
        else if (type == cmd_branch) {
            // call matching function
            branch* b = (branch*)seq_obj(i);
            int next_cmd = (this->*(b->_fn))(*b);
            switch (next_cmd) {
                case -1:
                    i++;  // meaning 'next command'
                    break;
                case -(int)ret_runtime_error:
                    // error: show it
                    (void)show_error(_err, _err_context);
                    go_out = true;  // end of run
                    break;
                default:
                    i = next_cmd;  // new direction
                    break;
            }
        }

        // not a command, but a stack entry, manage it
        else {
            // copy the program stack entry to the running stack
            stack::copy_and_push_back(*this, i, stk);
            i++;
        }
    }

    if (interrupt_now) {
        fprintf(stderr, "\nInterrupted\n");
        interrupt_now = false;
    }

    return ret;
}

/// @brief stop a program
///
///
void program::stop() { interrupt_now = true; }

/// @brief return whether a branch object has a given name
///
/// @param b the branch object
/// @param str_to_compare the name
/// @param len the name length
/// @return true the branch name is str_to_compare
/// @return false the branch name is NOT str_to_compare
///
bool program::compare_branch(branch* b, const char* str_to_compare, int len) {
    if (b->_len >= len)
        return strncasecmp(b->_value, str_to_compare, len) == 0;
    else
        return false;
}

/// @brief prepare a program for execution
/// this is needed before a program can be run
/// inner members of branch or keyword objects are filled by this function
/// these inner members store for example the index of the next keyword to execute etc.
///
/// @return ret_value see this type
///
ret_value program::preprocess(void) {
    // for if-then-else-end
    vector<struct if_layout_t> vlayout;
    int layout_index = -1;
    // for start-end-step
    vector<int> vstartindex;

    // analyse if-then-else-end branches
    // analyse start-{next, step} branches
    for (int i = 0; i < (int)size(); i++) {
        int type = seq_type(i);
        if (type == cmd_branch) {
            branch* k = (branch*)seq_obj(i);
            if (compare_branch(k, "if", 2)) {
                if_layout_t layout;
                layout.index_if_or_do_or_while = i;
                vlayout.push_back(layout);
                layout_index++;
            } else if (compare_branch(k, "then", 4)) {
                int next = i + 1;
                if (next >= (int)size()) next = -1;

                // nothing after 'then' -> error
                if (next == -1) {
                    // error: show it
                    show_syntax_error("missing end after then");
                    return ret_syntax;
                }
                if (layout_index < 0) {
                    // error: show it
                    show_syntax_error("missing if before then");
                    return ret_syntax;
                }
                if (vlayout[layout_index].index_then_or_unti_or_repeat != -1) {
                    // error: show it
                    show_syntax_error("duplicate then");
                    return ret_syntax;
                }
                vlayout[layout_index].index_then_or_unti_or_repeat = i;
                k->arg1 = next;
                k->arg3 = vlayout[layout_index].index_if_or_do_or_while;
            } else if (compare_branch(k, "else", 4)) {
                int next = i + 1;
                if (next >= (int)size()) next = -1;

                // nothing after 'else' -> error
                if (next == -1) {
                    // error: show it
                    show_syntax_error("missing end after else");
                    return ret_syntax;
                }
                if (layout_index < 0) {
                    // error: show it
                    show_syntax_error("missing if before else");
                    return ret_syntax;
                }
                if (vlayout[layout_index].index_then_or_unti_or_repeat == -1) {
                    // error: show it
                    show_syntax_error("missing then before else");
                    return ret_syntax;
                }
                if (vlayout[layout_index].index_else != -1) {
                    // error: show it
                    show_syntax_error("duplicate else");
                    return ret_syntax;
                }
                vlayout[layout_index].index_else = i;
                k->arg1 = next;  // fill branch1 (if was false) of 'else'
                k->arg3 = vlayout[layout_index].index_if_or_do_or_while;
                ((branch*)seq_obj(vlayout[layout_index].index_then_or_unti_or_repeat))->arg2 =
                    next;  // fill branch2 (if was false) of 'then'
            } else if (compare_branch(k, "start", 5))
                vstartindex.push_back(i);
            else if (compare_branch(k, "for", 3)) {
                vstartindex.push_back(i);
                k->arg1 = i + 1;  // arg1 points on symbol variable
            } else if (compare_branch(k, "next", 4)) {
                if (vstartindex.size() == 0) {
                    // error: show it
                    show_syntax_error("missing start or for before next");
                    return ret_syntax;
                }
                k->arg1 = vstartindex[vstartindex.size() - 1];  // 'next' arg1 = 'start' index
                ((branch*)seq_obj(vstartindex[vstartindex.size() - 1]))->arg2 =
                    i;  // 'for' or 'start' arg2 = 'next' index
                vstartindex.pop_back();
            } else if (compare_branch(k, "step", 4)) {
                if (vstartindex.size() == 0) {
                    // error: show it
                    show_syntax_error("missing start or for before step");
                    return ret_syntax;
                }
                k->arg1 = vstartindex[vstartindex.size() - 1];  // fill 'step' branch1 = 'start' index
                ((branch*)seq_obj(vstartindex[vstartindex.size() - 1]))->arg2 =
                    i;  // 'for' or 'start' arg2 = 'next' index
                vstartindex.pop_back();
            } else if (compare_branch(k, "->", 2)) {
                k->arg1 = i;  // arg1 is '->' command index in program
            } else if (compare_branch(k, "do", 2)) {
                if_layout_t layout;
                layout.index_if_or_do_or_while = i;
                layout.is_do_unti = true;
                vlayout.push_back(layout);
                layout_index++;
            } else if (compare_branch(k, "until", 4)) {
                int next = i + 1;
                if (next >= (int)size()) next = -1;

                // nothing after 'unti' -> error
                if (next == -1) {
                    // error: show it
                    show_syntax_error("missing end");
                    return ret_syntax;
                }
                if (layout_index < 0 || !vlayout[layout_index].is_do_unti) {
                    // error: show it
                    show_syntax_error("missing do");
                    return ret_syntax;
                }
                if (vlayout[layout_index].index_then_or_unti_or_repeat != -1) {
                    // error: show it
                    show_syntax_error("duplicate until");
                    return ret_syntax;
                }
                vlayout[layout_index].index_then_or_unti_or_repeat = i;
            } else if (compare_branch(k, "while", 4)) {
                if_layout_t layout;
                layout.index_if_or_do_or_while = i;
                layout.is_while_repeat = true;
                vlayout.push_back(layout);
                layout_index++;
            } else if (compare_branch(k, "repeat", 5)) {
                if (layout_index < 0 || !vlayout[layout_index].is_while_repeat) {
                    // error: show it
                    show_syntax_error("missing while");
                    return ret_syntax;
                }
                if (vlayout[layout_index].index_then_or_unti_or_repeat != -1) {
                    // error: show it
                    show_syntax_error("duplicate repeat");
                    return ret_syntax;
                }
                vlayout[layout_index].index_then_or_unti_or_repeat = i;
            } else if (compare_branch(k, "end", 3)) {
                int next = i + 1;
                if (next >= (int)size()) next = -1;

                if (layout_index < 0) {
                    // error: show it
                    show_syntax_error("missing branch instruction before end");
                    return ret_syntax;
                } else {
                    if (vlayout[layout_index].is_do_unti) {
                        // this end closes a do..unti
                        if (vlayout[layout_index].index_end != -1) {
                            // error: show it
                            show_syntax_error("duplicate end");
                            return ret_syntax;
                        }

                        if (vlayout[layout_index].index_then_or_unti_or_repeat == -1) {
                            // error: show it
                            show_syntax_error("missing until");
                            return ret_syntax;
                        }

                        k->arg1 = vlayout[layout_index].index_if_or_do_or_while + 1;
                        layout_index--;
                    } else if (vlayout[layout_index].is_while_repeat) {
                        // this end closes a while..repeat
                        if (vlayout[layout_index].index_end != -1) {
                            // error: show it
                            show_syntax_error("duplicate end");
                            return ret_syntax;
                        }

                        k->arg2 = vlayout[layout_index].index_if_or_do_or_while + 1;

                        if (vlayout[layout_index].index_then_or_unti_or_repeat == -1) {
                            // error: show it
                            show_syntax_error("missing repeat");
                            return ret_syntax;
                        }

                        // fill 'repeat' arg1 with 'end+1'
                        ((branch*)seq_obj(vlayout[layout_index].index_then_or_unti_or_repeat))->arg1 = i + 1;
                        layout_index--;
                    } else {
                        // this end closes an if..then..(else)
                        if (vlayout[layout_index].index_end != -1) {
                            // error: show it
                            show_syntax_error("duplicate end");
                            return ret_syntax;
                        }
                        if (vlayout[layout_index].index_else != -1)
                            // fill 'end' branch of 'else'
                            ((branch*)seq_obj(vlayout[layout_index].index_else))->arg2 = i;
                        else {
                            // fill 'end' branch of 'then'
                            if (vlayout[layout_index].index_then_or_unti_or_repeat != -1)
                                ((branch*)seq_obj(vlayout[layout_index].index_then_or_unti_or_repeat))->arg2 = i;
                            else {
                                // error: show it
                                show_syntax_error("missing then");
                                return ret_syntax;
                            }
                        }
                        layout_index--;
                    }
                }
            }
        }
    }
    if (layout_index >= 0) {
        // error: show it
        show_syntax_error("missing end");
        return ret_syntax;
    }
    if (vstartindex.size() > 0) {
        // error: show it
        show_syntax_error("missing next or step after for or start");
        return ret_syntax;
    }
    return ret_ok;
}

/// @brief show the last error set
///
/// @return ret_value see this type
///
ret_value program::show_error() {
    ret_value ret;

    // show last recorded error
    cerr << _err_context << ": error " << _err << ": " << s_ret_value_string[_err] << endl;
    switch (_err) {
        case ret_internal:
        case ret_deadly:
            ret = ret_deadly;
        default:
            ret = ret_ok;
    }

    return ret;
}

/// @brief record an error as the last error set and show it
///
/// @param err the error to record
/// @param context a context string
/// @return ret_value see this type
///
ret_value program::show_error(ret_value err, string& context) {
    // record error
    _err = err;
    _err_context = context;
    return show_error();
}

/// @brief record an error as the last error set and show it
///
/// @param err the error to record
/// @param context a context string
/// @return ret_value see this type
///
ret_value program::show_error(ret_value err, const char* context) {
    // record error
    _err = err;
    _err_context = context;
    return show_error();
}

/// @brief set the last error as being a syntax error and show it
///
/// @param err the error to record
/// @param context a context string
/// @return ret_value see this type
///
void program::show_syntax_error(const char* context) {
    // record error
    _err = ret_syntax;
    _err_context = context;
    (void)show_error();
}

/// @brief return the last error set
///
/// @return ret_value see this type
///
ret_value program::get_err(void) { return _err; }

/// @brief show a stack (show its different objects)
/// generally a stack is associated to a running program
///
/// @param st the stack to show
/// @param show_separator whether to show a stack level prefix or not
///
void program::show_stack(stack& st, bool show_separator) {
    if (st.size() == 1) {
        ((object*)st.back())->show();
        printf("\n");
    } else {
        for (int i = st.size() - 1; i >= 0; i--) {
            if (show_separator) printf("%d%s", i + 1, SHOW_STACK_SEPARATOR);
            ((object*)st[i])->show();
            printf("\n");
        }
    }
}

/// @brief apply default precision mode and digits
///
void program::apply_default() {
    // default float precision, float mode
    number::s_mode = DEFAULT_MODE;
    number::s_decimal_digits = DEFAULT_DECIMAL_DIGITS;

    // format for mpfr_printf
    stringstream ss;
    ss << number::s_decimal_digits;
    number::s_mpfr_printf_format = string(MPFR_FORMAT_BEG) + ss.str() + string(MPFR_FORMAT_STD);

    // default calc precision for MPFR
    floating_t::s_mpfr_prec = (mpfr_prec_t)MPFR_DEFAULT_PREC_BITS;
    floating_t::s_mpfr_prec_bytes = MPFR_DEFAULT_STORING_LENGTH_BYTES;
}
