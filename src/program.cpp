#include "program.hpp"

//
const char* program::s_ret_value_string[ret_max] = RET_VALUE_STRINGS;

//
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
    {cmd_keyword, "uname", &program::rpn_uname,
     "show rpn complete identification string"},
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
    {cmd_keyword, "sign", &program::rpn_sign,
     "1 if number at stack level 1 is > 0, 0 if == 0, -1 if <= 0"},

    // OPERATIONS ON REALS
    {cmd_undef, "", NULL, "\nOPERATIONS ON REALS"},
    {cmd_keyword, "%", &program::rpn_purcent, "purcent"},
    {cmd_keyword, "%CH", &program::rpn_purcentCH, "inverse purcent"},
    {cmd_keyword, "mod", &program::rpn_modulo, "modulo"},
    {cmd_keyword, "fact", &program::rpn_fact,
     "n! for integer n or Gamma(x+1) for fractional x"},
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
    {cmd_keyword, "std", &program::rpn_std,
     "standard floating numbers representation. ex: std"},
    {cmd_keyword, "fix", &program::rpn_fix,
     "fixed point representation. ex: 6 fix"},
    {cmd_keyword, "sci", &program::rpn_sci,
     "scientific floating point representation. ex: 20 sci"},
    {cmd_keyword, "prec", &program::rpn_precision,
     "set float precision in bits. ex: 256 prec"},
    {cmd_keyword, "round", &program::rpn_round,
     "set float rounding mode.\n\tex: [\"nearest\", \"toward zero\", \"toward "
     "+inf\", \"toward -inf\", \"away from zero\"] round"},
    {cmd_keyword, "default", &program::rpn_default,
     "set float representation and precision to default"},
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
    {cmd_keyword, "dup2", &program::rpn_dup2,
     "duplicate 2 first stack entries"},
    {cmd_keyword, "dupn", &program::rpn_dupn,
     "duplicate n first stack entries"},
    {cmd_keyword, "pick", &program::rpn_pick,
     "push a copy of  the given stack level onto the stack"},
    {cmd_keyword, "depth", &program::rpn_depth, "give stack depth"},
    {cmd_keyword, "roll", &program::rpn_roll,
     "move a stack entry to the top of the stack"},
    {cmd_keyword, "rolld", &program::rpn_rolld,
     "move the element on top of the stack to a higher stack position"},
    {cmd_keyword, "over", &program::rpn_over,
     "push a copy of the element in stack level 2 onto the stack"},

    // STRING
    {cmd_undef, "", NULL, "\nSTRING"},
    {cmd_keyword, "->str", &program::rpn_instr,
     "convert an object into a string"},
    {cmd_keyword, "str->", &program::rpn_strout,
     "convert a string into an object"},
    {cmd_keyword, "chr", &program::rpn_chr,
     "convert ASCII character code in stack level 1 into a string"},
    {cmd_keyword, "num", &program::rpn_num,
     "return ASCII code of the first character of the string in stack level 1 "
     "as a real number"},
    {cmd_keyword, "size", &program::rpn_strsize,
     "return the length of the string"},
    {cmd_keyword, "pos", &program::rpn_strpos,
     "seach for the string in level 1 within the string in level 2"},
    {cmd_keyword, "sub", &program::rpn_strsub,
     "return a substring of the string in level 3"},

    // BRANCH
    {cmd_undef, "", NULL, "\nBRANCH"},
    {cmd_branch, "if", (program_fn_t)&program::rpn_if,
     "if <test-instruction> then <true-instructions> else <false-instructions> "
     "end"},
    {cmd_branch, "then", (program_fn_t)&program::rpn_then, "used with if"},
    {cmd_branch, "else", (program_fn_t)&program::rpn_else, "used with if"},
    {cmd_branch, "end", (program_fn_t)&program::rpn_end,
     "used with various branch instructions"},
    {cmd_branch, "start", (program_fn_t)&program::rpn_start,
     "<start> <end> start <instructions> next|<step> step"},
    {cmd_branch, "for", (program_fn_t)&program::rpn_for,
     "<start> <end> for <variable> <instructions> next|<step> step"},
    {cmd_branch, "next", (program_fn_t)&program::rpn_next,
     "used with start and for"},
    {cmd_branch, "step", (program_fn_t)&program::rpn_step,
     "used with start and for"},
    {cmd_keyword, "ift", &program::rpn_ift,
     "similar to if-then-end, <test-instruction> <true-instruction> ift"},
    {cmd_keyword, "ifte", &program::rpn_ifte,
     "similar to if-then-else-end, <test-instruction> <true-instruction> "
     "<false-instruction> ifte"},
    {cmd_branch, "do", (program_fn_t)&program::rpn_do,
     "do <instructions> until <condition> end"},
    {cmd_branch, "until", (program_fn_t)&program::rpn_until, "used with do"},
    {cmd_branch, "unti", (program_fn_t)&program::rpn_until, ""},
    {cmd_branch, "while", (program_fn_t)&program::rpn_while,
     "while <test-instruction> repeat <loop-instructions> end"},
    {cmd_branch, "whil", (program_fn_t)&program::rpn_while, ""},
    {cmd_branch, "repeat", (program_fn_t)&program::rpn_repeat,
     "used with while"},
    {cmd_branch, "repea", (program_fn_t)&program::rpn_repeat, ""},

    // STORE
    {cmd_undef, "", NULL, "\nSTORE"},
    {cmd_keyword, "sto", &program::rpn_sto,
     "store a variable. ex: 1 'name' sto"},
    {cmd_keyword, "rcl", &program::rpn_rcl,
     "recall a variable. ex: 'name' rcl"},
    {cmd_keyword, "purge", &program::rpn_purge,
     "delete a variable. ex: 'name' purge"},
    {cmd_keyword, "vars", &program::rpn_vars, "list all variables"},
    {cmd_keyword, "clusr", &program::rpn_clusr, "erase all variables"},
    {cmd_keyword, "edit", &program::rpn_edit, "edit a variable content"},
    {cmd_keyword, "sto+", &program::rpn_stoadd,
     "add to a stored variable. ex: 1 'name' sto+ 'name' 2 sto+"},
    {cmd_keyword, "sto-", &program::rpn_stosub,
     "substract to a stored variable. ex: 1 'name' sto- 'name' 2 sto-"},
    {cmd_keyword, "sto*", &program::rpn_stomul,
     "multiply a stored variable. ex: 3 'name' sto* 'name' 2 sto*"},
    {cmd_keyword, "sto/", &program::rpn_stodiv,
     "divide a stored variable. ex: 3 'name' sto/ 'name' 2 sto/"},
    {cmd_keyword, "sneg", &program::rpn_stoneg,
     "negate a variable. ex: 'name' sneg"},
    {cmd_keyword, "sinv", &program::rpn_stoinv,
     "inverse a variable. ex: 1 'name' sinv"},

    // PROGRAM
    {cmd_undef, "", NULL, "\nPROGRAM"},
    {cmd_keyword, "eval", &program::rpn_eval,
     "evaluate (run) a program, or recall a variable. ex: 'my_prog' eval"},
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
    {cmd_keyword, "lnp1", &program::rpn_lnp1,
     "ln(1+x) which is useful when x is close to 0"},
    {cmd_keyword, "exp", &program::rpn_exp, "exponential"},
    {cmd_keyword, "expm", &program::rpn_expm,
     "exp(x)-1 which is useful when x is close to 0"},
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

    // end
    {cmd_max, "", NULL, ""},
};
