// Copyright (c) 2014-2022 Louis Rubet

#include "program.hpp"

//< language reserved keywords (allowed types are kKeyword, kBranch or kUndef)
vector<program::keyword_t> program::_keywords{
    // GENERAL
    {kUndef, "", nullptr, "\nGENERAL"},
    {kKeyword, "nop", &program::rpn_nop, "no operation"},
    {kKeyword, "help", &program::rpn_help, "this help message"},
    {kKeyword, "h", &program::rpn_help, ""},
    {kKeyword, "?", &program::rpn_help, ""},
    {kKeyword, "quit", &program::rpn_good_bye, "quit software"},
    {kKeyword, "q", &program::rpn_good_bye, ""},
    {kKeyword, "exit", &program::rpn_good_bye, ""},
    {kKeyword, "test", &program::rpn_test, ""},  // not seen by user
    {kKeyword, "version", &program::rpn_version, "show rpn version"},
    {kKeyword, "uname", &program::rpn_uname, "show rpn complete identification string"},
    {kKeyword, "history", &program::rpn_history, "see commands history"},

    // USUAL OPERATIONS ON REALS AND COMPLEXES
    {kUndef, "", nullptr, "\nUSUAL OPERATIONS ON REALS AND COMPLEXES"},
    {kKeyword, "+", &program::rpn_plus, "addition"},
    {kKeyword, "-", &program::rpn_minus, "substraction"},
    {kKeyword, "*", &program::rpn_mul, "multiplication"},
    {kKeyword, "/", &program::rpn_div, "division"},
    {kKeyword, "inv", &program::rpn_inv, "inverse"},
    {kKeyword, "chs", &program::rpn_neg, "negation"},
    {kKeyword, "neg", &program::rpn_neg, ""},
    {kKeyword, "^", &program::rpn_power, "power"},
    {kKeyword, "pow", &program::rpn_power, ""},
    {kKeyword, "sqrt", &program::rpn_squareroot, "rpn_square root"},
    {kKeyword, "sq", &program::rpn_square, "rpn_square"},
    {kKeyword, "abs", &program::rpn_abs, "absolute value (norm for a complex)"},
    {kKeyword, "sign", &program::rpn_sign, "sign of a number or z/|z| for a complex"},

    // OPERATIONS ON REALS
    {kUndef, "", nullptr, "\nOPERATIONS ON REALS"},
    {kKeyword, "%", &program::rpn_purcent, "purcent"},
    {kKeyword, "%CH", &program::rpn_purcentCH, "inverse purcent"},
    {kKeyword, "mod", &program::rpn_modulo, "modulo"},
    {kKeyword, "fact", &program::rpn_fact, "n! for integer n or Gamma(x+1) for fractional x"},
    {kKeyword, "mant", &program::rpn_mant, "mantissa of a real number"},
    {kKeyword, "xpon", &program::rpn_xpon, "exponant of a real number"},
    {kKeyword, "floor", &program::rpn_floor, "largest number <="},
    {kKeyword, "ceil", &program::rpn_ceil, "smallest number >="},
    {kKeyword, "ip", &program::rpn_ip, "integer part"},
    {kKeyword, "fp", &program::rpn_fp, "fractional part"},
    {kKeyword, "min", &program::rpn_min, "min of 2 real numbers"},
    {kKeyword, "max", &program::rpn_max, "max of 2 real numbers"},

    // OPERATIONS ON COMPLEXES
    {kUndef, "", nullptr, "\nOPERATIONS ON COMPLEXES"},
    {kKeyword, "re", &program::rpn_re, "complex real part"},
    {kKeyword, "im", &program::rpn_im, "complex imaginary part"},
    {kKeyword, "conj", &program::rpn_conj, "complex conjugate"},
    {kKeyword, "arg", &program::rpn_arg, "complex argument in radians"},
    {kKeyword, "c->r", &program::rpn_c2r, "transform a complex in 2 reals"},
    {kKeyword, "r->c", &program::rpn_r2c, "transform 2 reals in a complex"},
    {kKeyword, "p->r", &program::rpn_p2r, "cartesian to polar"},
    {kKeyword, "r->p", &program::rpn_r2p, "polar to cartesian"},

    // MODE
    {kUndef, "", nullptr, "\nMODE"},
    {kKeyword, "std", &program::rpn_std, "standard floating numbers representation. ex: std"},
    {kKeyword, "fix", &program::rpn_fix, "fixed point representation. ex: 6 fix"},
    {kKeyword, "sci", &program::rpn_sci, "scientific floating point representation. ex: 20 sci"},
    {kKeyword, "prec", &program::rpn_precision, "set float precision in bits. ex: 256 prec"},
    {kKeyword, "round", &program::rpn_round,
     "set float rounding mode.\n\tex: [\"nearest (even)\", \"toward zero\", \"toward "
     "+inf\", \"toward -inf\", \"away from zero\", \"faithful rounding\", \"nearest (away from zero)\"] round"},

    {kKeyword, "default", &program::rpn_default, "set float representation and precision to default"},
    {kKeyword, "type", &program::rpn_type, "show type of stack first entry"},
    {kKeyword, "hex", &program::rpn_hex, "hexadecimal representation, applies on stack level 0 only"},
    {kKeyword, "dec", &program::rpn_dec, "decimal representation, applies on stack level 0 only"},
    {kKeyword, "bin", &program::rpn_bin, "binary representation, applies on stack level 0 only"},
    {kKeyword, "base", &program::rpn_base, "arbitrary base representation, applies on stack level 0 only"},

    // TESTS
    {kUndef, "", nullptr, "\nTEST"},
    {kKeyword, ">", &program::rpn_sup, "binary operator >"},
    {kKeyword, ">=", &program::rpn_sup_eq, "binary operator >="},
    {kKeyword, "<", &program::rpn_inf, "binary operator <"},
    {kKeyword, "<=", &program::rpn_inf_eq, "binary operator <="},
    {kKeyword, "!=", &program::rpn_diff, "binary operator != (different)"},
    {kKeyword, "==", &program::rpn_eq, "binary operator == (equal)"},
    {kKeyword, "and", &program::rpn_test_and, "boolean operator and"},
    {kKeyword, "or", &program::rpn_test_or, "boolean operator or"},
    {kKeyword, "xor", &program::rpn_test_xor, "boolean operator xor"},
    {kKeyword, "not", &program::rpn_test_not, "boolean operator not"},
    {kKeyword, "same", &program::rpn_same, "boolean operator same (equal)"},

    // STACK
    {kUndef, "", nullptr, "\nSTACK"},
    {kKeyword, "swap", &program::rpn_swap, "swap 2 first stack entries"},
    {kKeyword, "drop", &program::rpn_drop, "drop first stack entry"},
    {kKeyword, "drop2", &program::rpn_drop2, "drop 2 first stack entries"},
    {kKeyword, "dropn", &program::rpn_dropn, "drop n first stack entries"},
    {kKeyword, "del", &program::rpn_erase, "drop all stack entries"},
    {kKeyword, "erase", &program::rpn_erase, ""},
    {kKeyword, "rot", &program::rpn_rot, "rotate 3 first stack entries"},
    {kKeyword, "dup", &program::rpn_dup, "duplicate first stack entry"},
    {kKeyword, "dup2", &program::rpn_dup2, "duplicate 2 first stack entries"},
    {kKeyword, "dupn", &program::rpn_dupn, "duplicate n first stack entries"},
    {kKeyword, "pick", &program::rpn_pick, "push a copy of  the given stack level onto the stack"},
    {kKeyword, "depth", &program::rpn_depth, "give stack depth"},
    {kKeyword, "roll", &program::rpn_roll, "move a stack entry to the top of the stack"},
    {kKeyword, "rolld", &program::rpn_rolld, "move the element on top of the stack to a higher stack position"},
    {kKeyword, "over", &program::rpn_over, "push a copy of the element in stack level 2 onto the stack"},

    // STRING
    {kUndef, "", nullptr, "\nSTRING"},
    {kKeyword, "->str", &program::rpn_instr, "convert an object into a string"},
    {kKeyword, "str->", &program::rpn_strout, "convert a string into an object"},
    {kKeyword, "chr", &program::rpn_chr, "convert ASCII character code in stack level 1 into a string"},
    {kKeyword, "num", &program::rpn_num,
     "return ASCII code of the first character of the string in stack level 1 as a real number"},
    {kKeyword, "size", &program::rpn_strsize, "return the length of the string"},
    {kKeyword, "pos", &program::rpn_strpos, "seach for the string in level 1 within the string in level 2"},
    {kKeyword, "sub", &program::rpn_strsub, "return a substring of the string in level 3"},

    // BRANCH
    {kUndef, "", nullptr, "\nBRANCH"},
    {kBranch, "if", (program_fn_t)&program::rpn_if,
     "if <test-instruction> then <true-instructions> else <false-instructions> "
     "end"},
    {kBranch, "then", (program_fn_t)&program::rpn_then, "used with if"},
    {kBranch, "else", (program_fn_t)&program::rpn_else, "used with if"},
    {kBranch, "end", (program_fn_t)&program::rpn_end, "used with various branch instructions"},
    {kBranch, "start", (program_fn_t)&program::rpn_start, "<start> <end> start <instructions> next|<step> step"},
    {kBranch, "for", (program_fn_t)&program::rpn_for,
     "<start> <end> for <variable> <instructions> next|<step> step"},
    {kBranch, "next", (program_fn_t)&program::rpn_next, "used with start and for"},
    {kBranch, "step", (program_fn_t)&program::rpn_step, "used with start and for"},
    {kKeyword, "ift", &program::rpn_ift, "similar to if-then-end, <test-instruction> <true-instruction> ift"},
    {kKeyword, "ifte", &program::rpn_ifte,
     "similar to if-then-else-end, <test-instruction> <true-instruction> "
     "<false-instruction> ifte"},
    {kBranch, "do", (program_fn_t)&program::rpn_do, "do <instructions> until <condition> end"},
    {kBranch, "until", (program_fn_t)&program::rpn_until, "used with do"},
    {kBranch, "while", (program_fn_t)&program::rpn_while, "while <test-instruction> repeat <loop-instructions> end"},
    {kBranch, "repeat", (program_fn_t)&program::rpn_repeat, "used with while"},

    // STORE
    {kUndef, "", nullptr, "\nSTORE"},
    {kKeyword, "sto", &program::rpn_sto, "store a variable. ex: 1 'name' sto"},
    {kKeyword, "rcl", &program::rpn_rcl, "recall a variable. ex: 'name' rcl"},
    {kKeyword, "purge", &program::rpn_purge, "delete a variable. ex: 'name' purge"},
    {kKeyword, "vars", &program::rpn_vars, "list all variables"},
    {kKeyword, "clusr", &program::rpn_clusr, "erase all variables"},
    {kKeyword, "edit", &program::rpn_edit, "edit a variable content"},
    {kKeyword, "sto+", &program::rpn_stoadd, "add to a stored variable. ex: 1 'name' sto+ 'name' 2 sto+"},
    {kKeyword, "sto-", &program::rpn_stosub, "substract to a stored variable. ex: 1 'name' sto- 'name' 2 sto-"},
    {kKeyword, "sto*", &program::rpn_stomul, "multiply a stored variable. ex: 3 'name' sto* 'name' 2 sto*"},
    {kKeyword, "sto/", &program::rpn_stodiv, "divide a stored variable. ex: 3 'name' sto/ 'name' 2 sto/"},
    {kKeyword, "sneg", &program::rpn_stoneg, "negate a variable. ex: 'name' sneg"},
    {kKeyword, "sinv", &program::rpn_stoinv, "inverse a variable. ex: 1 'name' sinv"},
    // PROGRAM
    {kUndef, "", nullptr, "\nPROGRAM"},
    {kKeyword, "eval", &program::rpn_eval, "evaluate (run) a program, or recall a variable. ex: 'my_prog' eval"},
    {kBranch, "->", (program_fn_t)&program::rpn_inprog,
     "load program local variables. ex: << -> n m << 0 n m for i i + next >> "
     ">>"},

    // TRIG ON REALS AND COMPLEXES
    {kUndef, "", nullptr, "\nTRIG ON REALS AND COMPLEXES"},
    {kKeyword, "pi", &program::rpn_pi, "pi constant"},
    {kKeyword, "sin", &program::rpn_sin, "sinus"},
    {kKeyword, "asin", &program::rpn_asin, "arg sinus"},
    {kKeyword, "cos", &program::rpn_cos, "cosinus"},
    {kKeyword, "acos", &program::rpn_acos, "arg cosinus"},
    {kKeyword, "tan", &program::rpn_tan, "tangent"},
    {kKeyword, "atan", &program::rpn_atan, "arg tangent"},
    {kKeyword, "d->r", &program::rpn_d2r, "convert degrees to radians"},
    {kKeyword, "r->d", &program::rpn_r2d, "convert radians to degrees"},

    // LOGS ON REALS AND COMPLEXES
    {kUndef, "", nullptr, "\nLOGS ON REALS AND COMPLEXES"},
    {kKeyword, "e", &program::rpn_e, "Euler constant"},
    {kKeyword, "ln", &program::rpn_ln, "logarithm base e"},
    {kKeyword, "log", &program::rpn_ln, ""},
    {kKeyword, "lnp1", &program::rpn_lnp1, "ln(1+x) which is useful when x is close to 0"},
    {kKeyword, "exp", &program::rpn_exp, "exponential"},
    {kKeyword, "expm", &program::rpn_expm, "exp(x)-1 which is useful when x is close to 0"},
    {kKeyword, "log10", &program::rpn_log10, "logarithm base 10"},
    {kKeyword, "alog10", &program::rpn_alog10, "exponential base 10"},
    {kKeyword, "exp10", &program::rpn_alog10, ""},
    {kKeyword, "log2", &program::rpn_log2, "logarithm base 2"},
    {kKeyword, "alog2", &program::rpn_alog2, "exponential base 2"},
    {kKeyword, "exp2", &program::rpn_alog2, ""},
    {kKeyword, "sinh", &program::rpn_sinh, "hyperbolic sine"},
    {kKeyword, "asinh", &program::rpn_asinh, "inverse hyperbolic sine"},
    {kKeyword, "cosh", &program::rpn_cosh, "hyperbolic cosine"},
    {kKeyword, "acosh", &program::rpn_acosh, "inverse hyperbolic cosine"},
    {kKeyword, "tanh", &program::rpn_tanh, "hyperbolic tangent"},
    {kKeyword, "atanh", &program::rpn_atanh, "inverse hyperbolic tangent"},

    // TIME AND DATE
    {kUndef, "", nullptr, "\nTIME AND DATE"},
    {kKeyword, "time", &program::rpn_time, "time in local format"},
    {kKeyword, "date", &program::rpn_date, "date in local format"},
    {kKeyword, "ticks", &program::rpn_ticks, "system tick in µs"},
};

/// autocompletion vector for linenoise autocompletion
vector<string>& program::getAutocompletionWords() {
    static vector<string> autocompletion_words;
    if (autocompletion_words.empty())
        for (auto& kw : _keywords)
            if (!kw.name.empty()) autocompletion_words.push_back(kw.name);
    return autocompletion_words;
}

/// @brief run a program on a stack and a heap
///
/// @return RetValue see this type
///
RetValue program::run() {
    bool go_out = false;
    RetValue ret = kOk;
    ObjectType type;

    _err = kOk;
    _err_context = "";

    // branches for 'if'
    ret = preprocess();
    if (ret != kOk) {
        // free allocated
        for (Object* o : *this) delete o;
        _local_heap.clear();
        return ret;
    }

    // iterate commands
    for (size_t i = 0; (go_out == false) && (i < size());) {
        Object* o = at(i);
        switch (o->_type) {
            // could be an auto-evaluated symbol
            case kSymbol:
                auto_rcl(reinterpret_cast<Symbol*>(o));
                i++;
                break;

            // a keyword
            case kKeyword: {
                Keyword* k = reinterpret_cast<Keyword*>(o);
                // call the matching function
                (this->*(k->fn))();
                switch (_err) {
                    // no pb -> go on
                    case kOk:
                        break;
                    // explicit go out software
                    case kGoodbye:
                        go_out = true;
                        ret = kGoodbye;
                        break;
                    default:
                        // error: abort prog
                        go_out = true;

                        // test error: make rpn return EXIT_FAILURE
                        if (_err == kTestFailed) ret = kTestFailed;

                        // error: show it
                        if (show_error(_err, _err_context) == kDeadlyError)
                            // pb showing error -> go out software
                            ret = kGoodbye;
                        break;
                }
                i++;
                break;
            }

            // a branch keyword
            case kBranch: {
                // call matching function
                Branch* b = reinterpret_cast<Branch*>(o);
                size_t next_cmd = (this->*(b->fn))(*b);
                switch (next_cmd) {
                    case kStepOut:  // step out
                        i++;        // meaning 'next command'
                        break;
                    case kRtError:  // runtime error
                        (void)show_error(_err, _err_context);
                        go_out = true;
                        break;
                    default:
                        i = next_cmd;
                        break;
                }
                break;
            }

            default:
                // not a command, but a stack entry, manage it
                // copy the program stack entry to the running stack
                _stack.push_front(o->clone());
                i++;
                break;
        }
    }

    // free allocated
    for (Object* o : *this) delete o;
    _local_heap.clear();

    return ret;
}

/// @brief prepare a program for execution
/// this is needed before a program can be run
/// inner members of branch or keyword objects are filled by this function
/// these inner members store for example the index of the next keyword to execute etc.
///
/// @return RetValue see this type
///
RetValue program::preprocess(void) {
    struct if_layout_t {
        if_layout_t()
            : index_then_or_unti_or_repeat(-1),
              index_else(-1),
              index_end(-1),
              is_do_unti(false),
              is_while_repeat(false) {}
        int index_if_or_do_or_while;
        int index_then_or_unti_or_repeat;
        int index_else;
        int index_end;
        bool is_do_unti;
        bool is_while_repeat;
    };
    // for if-then-else-end
    vector<struct if_layout_t> vlayout;
    int layout_index = -1;
    // for start-end-step
    vector<int> vstart_index;

    // analyse if-then-else-end branches
    // analyse start-{next, step} branches
    for (size_t i = 0; i < size(); i++) {
        if (at(i)->_type == kBranch) {
            Branch* k = reinterpret_cast<Branch*>(at(i));
            if (k->value == "if") {
                if_layout_t layout;
                layout.index_if_or_do_or_while = i;
                vlayout.push_back(layout);
                layout_index++;
            } else if (k->value == "then") {
                size_t next = i + 1;
                if (next >= size()) next = kStepOut;

                // nothing after 'then' -> error
                if (next == kStepOut) {
                    // error: show it
                    show_syntax_error("missing end after then");
                    return kSyntaxError;
                }
                if (layout_index < 0) {
                    // error: show it
                    show_syntax_error("missing if before then");
                    return kSyntaxError;
                }
                if (vlayout[layout_index].index_then_or_unti_or_repeat != -1) {
                    // error: show it
                    show_syntax_error("duplicate then");
                    return kSyntaxError;
                }
                vlayout[layout_index].index_then_or_unti_or_repeat = i;
                k->arg1 = next;
                k->arg3 = vlayout[layout_index].index_if_or_do_or_while;
            } else if (k->value == "else") {
                size_t next = i + 1;
                if (next >= size()) next = kStepOut;

                // nothing after 'else' -> error
                if (next == kStepOut) {
                    // error: show it
                    show_syntax_error("missing end after else");
                    return kSyntaxError;
                }
                if (layout_index < 0) {
                    // error: show it
                    show_syntax_error("missing if before else");
                    return kSyntaxError;
                }
                if (vlayout[layout_index].index_then_or_unti_or_repeat == -1) {
                    // error: show it
                    show_syntax_error("missing then before else");
                    return kSyntaxError;
                }
                if (vlayout[layout_index].index_else != -1) {
                    // error: show it
                    show_syntax_error("duplicate else");
                    return kSyntaxError;
                }
                vlayout[layout_index].index_else = i;
                k->arg1 = next;  // fill branch1 (if was false) of 'else'
                k->arg3 = vlayout[layout_index].index_if_or_do_or_while;
                reinterpret_cast<Branch*>(at(vlayout[layout_index].index_then_or_unti_or_repeat))->arg2 =
                    next;  // fill branch2 (if was false) of 'then'
            } else if (k->value == "start") {
                vstart_index.push_back(i);
            } else if (k->value == "for") {
                vstart_index.push_back(i);
                k->arg1 = i + 1;  // arg1 points on symbol variable
            } else if (k->value == "next") {
                if (vstart_index.size() == 0) {
                    // error: show it
                    show_syntax_error("missing start or for before next");
                    return kSyntaxError;
                }
                k->arg1 = vstart_index[vstart_index.size() - 1];  // 'next' arg1 = 'start' index
                reinterpret_cast<Branch*>(at(vstart_index[vstart_index.size() - 1]))->arg2 =
                    i;  // 'for' or 'start' arg2 = 'next' index
                vstart_index.pop_back();
            } else if (k->value == "step") {
                if (vstart_index.size() == 0) {
                    // error: show it
                    show_syntax_error("missing start or for before step");
                    return kSyntaxError;
                }
                k->arg1 = vstart_index[vstart_index.size() - 1];  // fill 'step' branch1 = 'start' index
                reinterpret_cast<Branch*>(at(vstart_index[vstart_index.size() - 1]))->arg2 =
                    i;  // 'for' or 'start' arg2 = 'next' index
                vstart_index.pop_back();
            } else if (k->value == "->") {
                k->arg1 = i;  // arg1 is '->' command index in program
            } else if (k->value == "do") {
                if_layout_t layout;
                layout.index_if_or_do_or_while = i;
                layout.is_do_unti = true;
                vlayout.push_back(layout);
                layout_index++;
            } else if (k->value == "until") {
                size_t next = i + 1;
                if (next >= size()) next = kStepOut;

                // nothing after 'unti' -> error
                if (next == kStepOut) {
                    // error: show it
                    show_syntax_error("missing end");
                    return kSyntaxError;
                }
                if (layout_index < 0 || !vlayout[layout_index].is_do_unti) {
                    // error: show it
                    show_syntax_error("missing do");
                    return kSyntaxError;
                }
                if (vlayout[layout_index].index_then_or_unti_or_repeat != -1) {
                    // error: show it
                    show_syntax_error("duplicate until");
                    return kSyntaxError;
                }
                vlayout[layout_index].index_then_or_unti_or_repeat = i;
            } else if (k->value == "while") {
                if_layout_t layout;
                layout.index_if_or_do_or_while = i;
                layout.is_while_repeat = true;
                vlayout.push_back(layout);
                layout_index++;
            } else if (k->value == "repeat") {
                if (layout_index < 0 || !vlayout[layout_index].is_while_repeat) {
                    // error: show it
                    show_syntax_error("missing while");
                    return kSyntaxError;
                }
                if (vlayout[layout_index].index_then_or_unti_or_repeat != -1) {
                    // error: show it
                    show_syntax_error("duplicate repeat");
                    return kSyntaxError;
                }
                vlayout[layout_index].index_then_or_unti_or_repeat = i;
            } else if (k->value == "end") {
                size_t next = i + 1;
                if (next >= size()) next = kStepOut;

                if (layout_index < 0) {
                    // error: show it
                    show_syntax_error("missing branch instruction before end");
                    return kSyntaxError;
                } else {
                    if (vlayout[layout_index].is_do_unti) {
                        // this end closes a do..unti
                        if (vlayout[layout_index].index_end != -1) {
                            // error: show it
                            show_syntax_error("duplicate end");
                            return kSyntaxError;
                        }

                        if (vlayout[layout_index].index_then_or_unti_or_repeat == -1) {
                            // error: show it
                            show_syntax_error("missing until");
                            return kSyntaxError;
                        }

                        k->arg1 = vlayout[layout_index].index_if_or_do_or_while + 1;
                        layout_index--;
                    } else if (vlayout[layout_index].is_while_repeat) {
                        // this end closes a while..repeat
                        if (vlayout[layout_index].index_end != -1) {
                            // error: show it
                            show_syntax_error("duplicate end");
                            return kSyntaxError;
                        }

                        k->arg2 = vlayout[layout_index].index_if_or_do_or_while + 1;

                        if (vlayout[layout_index].index_then_or_unti_or_repeat == -1) {
                            // error: show it
                            show_syntax_error("missing repeat");
                            return kSyntaxError;
                        }

                        // fill 'repeat' arg1 with 'end+1'
                        reinterpret_cast<Branch*>(at(vlayout[layout_index].index_then_or_unti_or_repeat))->arg1 = i + 1;
                        layout_index--;
                    } else {
                        // this end closes an if..then..(else)
                        if (vlayout[layout_index].index_end != -1) {
                            // error: show it
                            show_syntax_error("duplicate end");
                            return kSyntaxError;
                        }
                        if (vlayout[layout_index].index_else != -1) {
                            // fill 'end' branch of 'else'
                            reinterpret_cast<Branch*>(at(vlayout[layout_index].index_else))->arg2 = i;
                        } else {
                            // fill 'end' branch of 'then'
                            if (vlayout[layout_index].index_then_or_unti_or_repeat != -1) {
                                reinterpret_cast<Branch*>(at(vlayout[layout_index].index_then_or_unti_or_repeat))
                                    ->arg2 = i;
                            } else {
                                // error: show it
                                show_syntax_error("missing then");
                                return kSyntaxError;
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
        return kSyntaxError;
    }
    if (vstart_index.size() > 0) {
        // error: show it
        show_syntax_error("missing next or step after for or start");
        return kSyntaxError;
    }
    return kOk;
}

/// @brief parse an entry string: cut it into objects chunks and add them to a program
///
/// @param entry the entry string
/// @param prog the program to be filled
/// @return RetValue see this type
///
RetValue program::parse(string& entry) {
    static map<string, Lexer::ReservedWord> keywords_map;
    vector<Lexer::SynElement> elements;
    vector<Lexer::SynError> errors;
    RetValue ret = kOk;

    // prepare map for finding reserved keywords
    if (keywords_map.empty())
        for (auto& kw : _keywords)
            if (!kw.name.empty()) keywords_map[kw.name] = {kw.type, kw.fn};

    // separate the entry string
    if (lexer(entry, keywords_map, elements, errors)) {
        // make objects from parsed elements
        for (Lexer::SynElement& element : elements) {
            switch (element.type) {
                case kNumber:
                    push_back(new Number(*element.re, element.reBase));
                    break;
                case kComplex:
                    push_back(new Complex(*element.re, *element.im, element.reBase, element.imBase));
                    break;
                case kString:
                    push_back(new String(element.value));
                    break;
                case kSymbol:
                    push_back(new Symbol(element.value, element.autoEval));
                    break;
                case kProgram:
                    push_back(new Program(element.value));
                    break;
                case kKeyword:
                    push_back(new Keyword(element.fn, element.value));
                    break;
                case kBranch:
                    push_back(new Branch((branch_fn_t)element.fn, element.value));
                    break;
                default:
                    show_error(kUnknownError, "error creating program from entry");
                    break;
            }
            if (element.re != nullptr) delete element.re;
            if (element.im != nullptr) delete element.im;
        }
    } else {
        for (SynError& err : errors) show_syntax_error(err.err.c_str());
    }

    return ret;
}

/// @brief show the last error set
///
/// @return RetValue see this type
///
RetValue program::show_error() {
    RetValue ret;
    // clang-format off
    map<RetValue, string> errorStrings{{kOk, "ok"}, {kUnknownError, "unknown command"},
        {kMissingOperand, "missing operand"}, {kBadOperandType, "bad operand type"},
        {kOutOfRange, "out of range"}, {kUnknownVariable, "unknown variable"},
        {kInternalError, "internal error, aborting"}, {kDeadlyError, "deadly"},
        {kGoodbye, "goodbye"}, {kNotImplemented, "not implemented"},
        {kNop, "no operation"}, {kSyntaxError, "syntax error"},
        {kDivByZero, "division by zero"}, {kRuntimeError, "runtime error"},
        {kAbortCurrentEntry, "aborted current entry"}, {kOutOfMemory, "out of memory"},
        {kBadValue, "bad value"}, {kTestFailed, "test failed"}
    };
    // clang-format on
    cerr << _err_context << ": error " << _err << ": " << errorStrings[_err] << endl;
    switch (_err) {
        case kInternalError:
        case kDeadlyError:
            ret = kDeadlyError;
        default:
            ret = kOk;
    }

    return ret;
}

/// @brief record an error as the last error set and show it
///
/// @param err the error to record
/// @param context a context string
/// @return RetValue see this type
///
RetValue program::show_error(RetValue err, string& context) {
    // record error
    _err = err;
    _err_context = context;
    return show_error();
}

/// @brief record an error as the last error set and show it
///
/// @param err the error to record
/// @param context a context string
/// @return RetValue see this type
///
RetValue program::show_error(RetValue err, const char* context) {
    // record error
    _err = err;
    _err_context = context;
    return show_error();
}

/// @brief set the last error as being a syntax error and show it
///
/// @param err the error to record
/// @param context a context string
/// @return RetValue see this type
///
void program::show_syntax_error(const char* context) {
    // record error
    _err = kSyntaxError;
    _err_context = context;
    (void)show_error();
}

/// @brief return the last error set
///
/// @return RetValue see this type
///
RetValue program::get_err(void) { return _err; }

/// @brief show a stack (show its different objects)
/// generally a stack is associated to a running program
///
/// @param show_separator whether to show a stack level prefix or not
///
void program::show_stack(bool show_separator) {
    if (_stack.size() == 1) {
        cout << _stack[0] << endl;
    } else {
        for (int i = _stack.size() - 1; i >= 0; i--) {
            if (show_separator) cout << i + 1 << "> ";
            cout << _stack[i] << endl;
        }
    }
}

/// @brief apply default precision mode and digits
///
void program::apply_default() {
    // default float precision, float mode
    Number::s_mode = Number::DEFAULT_MODE;
    Number::s_digits = Number::DEFAULT_DECIMAL_DIGITS;
    mpreal::set_default_prec(Number::MPFR_DEFAULT_PREC_BITS);

    static mp_rnd_t def_rnd = mpreal::get_default_rnd();
    mpreal::set_default_rnd(def_rnd);
}
