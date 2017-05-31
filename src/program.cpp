#include "program.hpp"

//
const char* program::s_ret_value_string[ret_max] = RET_VALUE_STRINGS;

//
program::keyword_t program::s_keywords[] =
{
    //GENERAL
    { cmd_undef, "", NULL, "\nGENERAL"},
    { cmd_keyword, "nop", &program::nop, "no operation"},
    { cmd_keyword, "?", &program::help, "" },
    { cmd_keyword, "h", &program::help, "" },
    { cmd_keyword, "help", &program::help, "(or h or ?) this help message" },
    { cmd_keyword, "q", &program::good_bye, "" },
    { cmd_keyword, "quit", &program::good_bye, "(or q or exit) quit software" },
    { cmd_keyword, "exit", &program::good_bye, "" },
    { cmd_keyword, "test", &program::test, "" }, //not seen by user
    { cmd_keyword, "version", &program::rpn_version, "show rpn version" },
    { cmd_keyword, "uname", &program::rpn_uname, "show rpn complete identification string" },
    { cmd_keyword, "type", &program::type, "show type of stack first entry" },
    { cmd_keyword, "default", &program::rpn_default, "set float representation and precision to default" },

    //MODE
    { cmd_undef, "", NULL, "\nMODE"},
    { cmd_keyword, "std", &program::std, "standard floating numbers representation. ex: [25] std" },
    { cmd_keyword, "fix", &program::fix, "fixed point representation. ex: 6 fix" },
    { cmd_keyword, "sci", &program::sci, "scientific floating point representation. ex: 20 sci" },

    //REAL
    { cmd_undef, "", NULL, "\nREAL"},
    { cmd_keyword, "+", &program::plus, "addition" },
    { cmd_keyword, "-", &program::minus, "substraction" },
    { cmd_keyword, "chs", &program::neg , "(or neg) negation" },
    { cmd_keyword, "neg", &program::neg , "" },
    { cmd_keyword, "*", &program::mul, "multiplication" },
    { cmd_keyword, "/", &program::div, "division" },
    { cmd_keyword, "inv", &program::inv, "inverse" },
    { cmd_keyword, "%", &program::purcent, "purcent" },
    { cmd_keyword, "%CH", &program::purcentCH, "inverse purcent" },
    { cmd_keyword, "^", &program::power, "(or pow) power" },
    { cmd_keyword, "pow", &program::power, "" },
    { cmd_keyword, "sqrt", &program::squareroot, "square root" },
    { cmd_keyword, "sq", &program::square, "(or sqr) square" },
    { cmd_keyword, "sqr", &program::square, "" },
    { cmd_keyword, "mod", &program::modulo, "modulo" },
    { cmd_keyword, "abs", &program::rpn_abs, "absolute value" },
    { cmd_keyword, "dec", &program::dec, "decimal representation" },
    { cmd_keyword, "hex", &program::hex, "hexadecimal representation" },
    { cmd_keyword, "prec", &program::precision, "get float precision in bits when first stack is not a number\n\t"
                                                "set float precision in bits when first stack entry is a number. ex: 256 prec" },
    { cmd_keyword, "round", &program::round, "set float rounding mode.\n\tex: [\"nearest\", \"toward zero\", \"toward +inf\", \"toward -inf\", \"away from zero\"] round" },
    { cmd_keyword, "fact", &program::fact, "n! for integer n or Gamma(x+1) for fractional x" },
    { cmd_keyword, "sign", &program::sign, "1 if number at stack level 1 is > 0, 0 if == 0, -1 if <= 0" },
    { cmd_keyword, "mant", &program::mant, "mantissa of a real number" },
    { cmd_keyword, "xpon", &program::xpon, "exponant of a real number" },
    { cmd_keyword, "min", &program::rpn_min, "min of 2 real numbers" },
    { cmd_keyword, "max", &program::rpn_max, "max of 2 real numbers" },

    //TEST
    { cmd_undef, "", NULL, "\nTEST"},
    { cmd_keyword, ">", &program::sup, "binary operator >" },
    { cmd_keyword, ">=", &program::sup_eq, "binary operator >=" },
    { cmd_keyword, "<", &program::inf, "binary operator <" },
    { cmd_keyword, "<=", &program::inf_eq, "binary operator <=" },
    { cmd_keyword, "!=", &program::diff, "binary operator != (different)" },
    { cmd_keyword, "==", &program::eq , "binary operator == (equal)" },
    { cmd_keyword, "and", &program::test_and , "boolean operator and" },
    { cmd_keyword, "or", &program::test_or , "boolean operator or" },
    { cmd_keyword, "xor", &program::test_xor , "boolean operator xor" },
    { cmd_keyword, "not", &program::test_not , "boolean operator not" },
    { cmd_keyword, "same", &program::same , "boolean operator same (equal)" },

    //STACK
    { cmd_undef, "", NULL, "\nSTACK"},
    { cmd_keyword, "swap", &program::swap, "swap 2 first stack entries" },
    { cmd_keyword, "drop", &program::drop, "drop first stack entry" },
    { cmd_keyword, "drop2", &program::drop2, "drop 2 first stack entries" },
    { cmd_keyword, "dropn", &program::dropn, "drop n first stack entries" },
    { cmd_keyword, "erase", &program::erase, "drop all stack entries" },
    { cmd_keyword, "rot", &program::rot, "rotate 3 first stack entries" },
    { cmd_keyword, "dup", &program::dup, "duplicate first stack entry" },
    { cmd_keyword, "dup2", &program::dup2, "duplicate 2 first stack entries" },
    { cmd_keyword, "dupn", &program::dupn, "duplicate n first stack entries" },
    { cmd_keyword, "pick", &program::pick, "push a copy of  the given stack level onto the stack" },
    { cmd_keyword, "depth", &program::depth, "give stack depth" },
    { cmd_keyword, "roll", &program::roll, "move a stack entry to the top of the stack" },
    { cmd_keyword, "rolld", &program::rolld, "move the element on top of the stack to a higher stack position" },
    { cmd_keyword, "over", &program::over, "push a copy of the element in stack level 2 onto the stack" },

    //STRING
    { cmd_undef, "", NULL, "\nSTRING"},
    { cmd_keyword, "->str", &program::instr, "convert an object into a string" },
    { cmd_keyword, "str->", &program::strout, "convert a string into an object" },
    { cmd_keyword, "chr", &program::chr, "convert ASCII character code in stack level 1 into a string" },
    { cmd_keyword, "num", &program::num, "return ASCII code of the first character of the string in stack level 1 as a real number" },

    //BRANCH
    { cmd_undef, "", NULL, "\nBRANCH"},
    { cmd_branch, "if", (program_fn_t)&program::rpn_if, "<test-instructions>" },
    { cmd_branch, "then", (program_fn_t)&program::rpn_then, "<true-instructions>" },
    { cmd_branch, "else", (program_fn_t)&program::rpn_else, "<false-instructions>" },
    { cmd_keyword, "end", &program::rpn_end, "(end of if structure)" },
    { cmd_branch, "start", (program_fn_t)&program::rpn_start, "repeat instructions several times" },
    { cmd_branch, "for", (program_fn_t)&program::rpn_for, "repeat instructions several times with variable" },
    { cmd_branch, "next", (program_fn_t)&program::rpn_next, "ex: 1 10 start <instructions> next" },
    { cmd_branch, "step", (program_fn_t)&program::rpn_step, "ex: 1 100 start <instructions> 4 step" },

    //STORE
    { cmd_undef, "", NULL, "\nSTORE"},
    { cmd_keyword, "sto", &program::sto, "store a variable. ex: 1 'name' sto" },
    { cmd_keyword, "rcl", &program::rcl, "recall a variable. ex: 'name' rcl" },
    { cmd_keyword, "purge", &program::purge, "delete a variable. ex: 'name' purge" },
    { cmd_keyword, "vars", &program::vars, "list all variables" },
    { cmd_keyword, "edit", &program::edit, "edit a variable content" },
    { cmd_keyword, "sto+", &program::stoadd, "add to a stored variable. ex: 1 'name' sto+ 'name' 2 sto+" },
    { cmd_keyword, "sto-", &program::stosub, "substract to a stored variable. ex: 1 'name' sto- 'name' 2 sto-" },
    { cmd_keyword, "sto*", &program::stomul, "multiply a stored variable. ex: 3 'name' sto* 'name' 2 sto*" },
    { cmd_keyword, "sto/", &program::stodiv, "divide a stored variable. ex: 3 'name' sto/ 'name' 2 sto/" },
    { cmd_keyword, "sneg", &program::stoneg, "negate a variable. ex: 'name' sneg" },
    { cmd_keyword, "sinv", &program::stoinv, "inverse a variable. ex: 1 'name' sinv" },

    //PROGRAM
    { cmd_undef, "", NULL, "\nPROGRAM"},
    { cmd_keyword, "eval", &program::eval, "evaluate (run) a program, or recall a variable. ex: 'my_prog' eval" },
    { cmd_branch, "->", (program_fn_t)&program::inprog, "load program local variables. ex: << -> n m << 0 n m for i i + next >> >>" },

    //TRIG
    { cmd_undef, "", NULL, "\nTRIG"},
    { cmd_keyword, "pi", &program::pi, "pi constant" },
    { cmd_keyword, "sin", &program::rpn_sin, "sinus" },
    { cmd_keyword, "asin", &program::rpn_asin, "arg sinus" },
    { cmd_keyword, "cos", &program::rpn_cos , "cosinus" },
    { cmd_keyword, "acos", &program::rpn_acos, "arg cosinus" },
    { cmd_keyword, "tan", &program::rpn_tan, "tangent" },
    { cmd_keyword, "atan", &program::rpn_atan, "arg tangent" },
    { cmd_keyword, "d->r", &program::d2r, "convert degrees to radians" },
    { cmd_keyword, "r->d", &program::r2d, "convert radians to degrees" },    

    //LOGS
    { cmd_undef, "", NULL, "\nLOGS"},
    { cmd_keyword, "e", &program::rpn_e, "exp(1) constant" },
    { cmd_keyword, "log", &program::rpn_log, "logarithm base 10" },
    { cmd_keyword, "alog", &program::rpn_alog, "(or exp10) exponential base 10" },
    { cmd_keyword, "exp10", &program::rpn_alog, "" },
    { cmd_keyword, "log2", &program::rpn_log2, "logarithm base 2" },
    { cmd_keyword, "alog2", &program::rpn_alog2, "(or exp2) exponential base 2" },
    { cmd_keyword, "exp2", &program::rpn_alog2, "" },
    { cmd_keyword, "ln", &program::rpn_ln, "logarithm base e" },
    { cmd_keyword, "exp", &program::rpn_exp, "exponential" },
    { cmd_keyword, "sinh", &program::rpn_sinh, "hyperbolic sine" },
    { cmd_keyword, "asinh", &program::rpn_asinh, "inverse hyperbolic sine" },
    { cmd_keyword, "cosh", &program::rpn_sinh, "hyperbolic cosine" },
    { cmd_keyword, "acosh", &program::rpn_acosh, "inverse hyperbolic cosine" },
    { cmd_keyword, "tanh", &program::rpn_tanh, "hyperbolic tangent" },
    { cmd_keyword, "atanh", &program::rpn_atanh, "inverse hyperbolic tangent" },

    // end
    { cmd_max, "", NULL, "" },
};
