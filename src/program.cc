// Copyright (c) 2014-2022 Louis Rubet

#include "program.h"

//< language reserved keywords (allowed types are kKeyword, kBranch or kUndef)
// clang-format off
vector<Program::keyword_t> Program::keywords_ = {
    // GENERAL
    {kUndef, "", {.prog = nullptr}, "\nGENERAL"},
    {kKeyword, "help",    {.prog = &Program::RpnHelp}, "this help message"},
    {kKeyword, "h",       {.prog = &Program::RpnHelp}, ""},
    {kKeyword, "?",       {.prog = &Program::RpnHelp}, ""},
    {kKeyword, "quit",    {.prog = &Program::RpnQuit}, "quit software"},
    {kKeyword, "q",       {.prog = &Program::RpnQuit}, ""},
    {kKeyword, "exit",    {.prog = &Program::RpnQuit}, ""},
    {kKeyword, "test",    {.prog = &Program::RpnTest}, ""},
    {kKeyword, "version", {.prog = &Program::RpnVersion}, "show rpn version"},
    {kKeyword, "uname",   {.prog = &Program::RpnUname}, "show rpn complete identification string"},
    {kKeyword, "history", {.prog = &Program::RpnHistory}, "see commands history"},

    // USUAL OPERATIONS ON REALS AND COMPLEXES
    {kUndef, "", {.prog = nullptr}, "\nUSUAL OPERATIONS ON REALS AND COMPLEXES"},
    {kKeyword, "+",    {.prog = &Program::RpnPlus}, "addition"},
    {kKeyword, "-",    {.prog = &Program::RpnMinus}, "substraction"},
    {kKeyword, "*",    {.prog = &Program::RpnMul}, "multiplication"},
    {kKeyword, "/",    {.prog = &Program::RpnDiv}, "division"},
    {kKeyword, "inv",  {.prog = &Program::RpnInv}, "inverse"},
    {kKeyword, "chs",  {.prog = &Program::RpnNeg}, "negation"},
    {kKeyword, "neg",  {.prog = &Program::RpnNeg}, ""},
    {kKeyword, "^",    {.prog = &Program::RpnPower}, "power"},
    {kKeyword, "pow",  {.prog = &Program::RpnPower}, ""},
    {kKeyword, "sqrt", {.prog = &Program::RpnSquareroot}, "RpnSquare root"},
    {kKeyword, "sq",   {.prog = &Program::RpnSquare}, "RpnSquare"},
    {kKeyword, "abs",  {.prog = &Program::RpnAbs}, "absolute value (norm for a complex)"},
    {kKeyword, "sign", {.prog = &Program::RpnSign}, "sign of a number or z/|z| for a complex"},

    // OPERATIONS ON REALS
    {kUndef, "", {.prog = nullptr}, "\nOPERATIONS ON REALS"},
    {kKeyword, "%",     {.prog = &Program::RpnPurcent}, "purcent"},
    {kKeyword, "%CH",   {.prog = &Program::RpnPurcentCH}, "inverse purcent"},
    {kKeyword, "mod",   {.prog = &Program::RpnModulo}, "modulo"},
    {kKeyword, "fact",  {.prog = &Program::RpnFact}, "n! for integer n or Gamma(x+1) for fractional x"},
    {kKeyword, "mant",  {.prog = &Program::RpnMant}, "mantissa of a real number"},
    {kKeyword, "xpon",  {.prog = &Program::RpnXpon}, "exponant of a real number"},
    {kKeyword, "floor", {.prog = &Program::RpnFloor}, "largest number <="},
    {kKeyword, "ceil",  {.prog = &Program::RpnCeil}, "smallest number >="},
    {kKeyword, "ip",    {.prog = &Program::RpnIp}, "integer part"},
    {kKeyword, "fp",    {.prog = &Program::RpnFp}, "fractional part"},
    {kKeyword, "min",   {.prog = &Program::RpnMin}, "min of 2 real numbers"},
    {kKeyword, "max",   {.prog = &Program::RpnMax}, "max of 2 real numbers"},

    // OPERATIONS ON COMPLEXES
    {kUndef, "", {.prog = nullptr}, "\nOPERATIONS ON COMPLEXES"},
    {kKeyword, "re",   {.prog = &Program::RpnReal}, "complex real part"},
    {kKeyword, "im",   {.prog = &Program::RpnImag}, "complex imaginary part"},
    {kKeyword, "conj", {.prog = &Program::RpnConj}, "complex conjugate"},
    {kKeyword, "arg",  {.prog = &Program::RpnArg}, "complex argument in radians"},
    {kKeyword, "c->r", {.prog = &Program::RpnC2r}, "transform a complex in 2 reals"},
    {kKeyword, "r->c", {.prog = &Program::RpnR2c}, "transform 2 reals in a complex"},
    {kKeyword, "p->r", {.prog = &Program::RpnP2r}, "cartesian to polar"},
    {kKeyword, "r->p", {.prog = &Program::RpnR2p}, "polar to cartesian"},

    // MODE
    {kUndef, "", {.prog = nullptr}, "\nMODE"},
    {kKeyword, "std",   {.prog = &Program::RpnStd}, "standard floating numbers representation. ex: std"},
    {kKeyword, "fix",   {.prog = &Program::RpnFix}, "fixed point representation. ex: 6 fix"},
    {kKeyword, "sci",   {.prog = &Program::RpnSci}, "scientific floating point representation. ex: 20 sci"},
    {kKeyword, "prec",  {.prog = &Program::RpnPrecision}, "set float precision in bits. ex: 256 prec"},
    {kKeyword, "round", {.prog = &Program::RpnRound},
        "set float rounding mode in \n\t\"nearest (even)\", \"toward zero\", \"toward "
        "+inf\", \"toward -inf\", \"away from zero\", \"faithful rounding\", \"nearest (away from zero)\""
        "\n\tex: \"nearest (even)\" round"},
    {kKeyword, "default", {.prog = &Program::RpnDefault}, "set float representation and precision to default"},
    {kKeyword, "type",    {.prog = &Program::RpnType}, "show type of stack first entry"},
    {kKeyword, "hex",     {.prog = &Program::RpnHex}, "hexadecimal representation, applies on stack level 0 only"},
    {kKeyword, "dec",     {.prog = &Program::RpnDec}, "decimal representation, applies on stack level 0 only"},
    {kKeyword, "bin",     {.prog = &Program::RpnBin}, "binary representation, applies on stack level 0 only"},
    {kKeyword, "base",    {.prog = &Program::RpnBase}, "arbitrary base representation, applies on stack level 0 only"},

    // TESTS
    {kUndef, "", {.prog = nullptr}, "\nTEST"},
    {kKeyword, ">",     {.prog = &Program::RpnSup}, "binary operator >"},
    {kKeyword, ">=",    {.prog = &Program::RpnSupEq}, "binary operator >="},
    {kKeyword, "<",     {.prog = &Program::RpnInf}, "binary operator <"},
    {kKeyword, "<=",    {.prog = &Program::RpnInfEq}, "binary operator <="},
    {kKeyword, "!=",    {.prog = &Program::RpnDiff}, "binary operator != (different)"},
    {kKeyword, "==",    {.prog = &Program::RpnEq}, "binary operator == (equal)"},
    {kKeyword, "and",   {.prog = &Program::RpnTestAnd}, "boolean operator and"},
    {kKeyword, "or",    {.prog = &Program::RpnTestOr}, "boolean operator or"},
    {kKeyword, "xor",   {.prog = &Program::RpnTestXor}, "boolean operator xor"},
    {kKeyword, "not",   {.prog = &Program::RpnTestNot}, "boolean operator not"},
    {kKeyword, "same",  {.prog = &Program::RpnSame}, "boolean operator same (equal)"},

    // STACK
    {kUndef, "", {.prog = nullptr}, "\nSTACK"},
    {kKeyword, "swap",  {.prog = &Program::RpnSwap}, "swap 2 first stack entries"},
    {kKeyword, "drop",  {.prog = &Program::RpnDrop}, "drop first stack entry"},
    {kKeyword, "drop2", {.prog = &Program::RpnDrop2}, "drop 2 first stack entries"},
    {kKeyword, "dropn", {.prog = &Program::RpnDropn}, "drop n first stack entries"},
    {kKeyword, "del",   {.prog = &Program::RpnErase}, "drop all stack entries"},
    {kKeyword, "erase", {.prog = &Program::RpnErase}, ""},
    {kKeyword, "rot",   {.prog = &Program::RpnRot}, "rotate 3 first stack entries"},
    {kKeyword, "dup",   {.prog = &Program::RpnDup}, "duplicate first stack entry"},
    {kKeyword, "dup2",  {.prog = &Program::RpnDup2}, "duplicate 2 first stack entries"},
    {kKeyword, "dupn",  {.prog = &Program::RpnDupn}, "duplicate n first stack entries"},
    {kKeyword, "pick",  {.prog = &Program::RpnPick}, "push a copy of  the given stack level onto the stack"},
    {kKeyword, "depth", {.prog = &Program::RpnDepth}, "give stack depth"},
    {kKeyword, "roll",  {.prog = &Program::RpnRoll}, "move a stack entry to the top of the stack"},
    {kKeyword, "rolld", {.prog = &Program::RpnRolld}, "move the element on top of the stack to a higher stack position"},
    {kKeyword, "over",  {.prog = &Program::RpnOver}, "push a copy of the element in stack level 2 onto the stack"},

    // STRING
    {kUndef, "", {.prog = nullptr}, "\nSTRING"},
    {kKeyword, "->str", {.prog = &Program::RpnInstr}, "convert an object into a string"},
    {kKeyword, "str->", {.prog = &Program::RpnStrout}, "convert a string into an object"},
    {kKeyword, "chr",   {.prog = &Program::RpnChr}, "convert ASCII character code in stack level 1 into a string"},
    {kKeyword, "num",   {.prog = &Program::RpnNum}, "return ASCII code of the first character of the string in stack level 1 as a real number"},
    {kKeyword, "size",  {.prog = &Program::RpnStrsize}, "return the length of the string"},
    {kKeyword, "pos",   {.prog = &Program::RpnStrpos}, "seach for the string in level 1 within the string in level 2"},
    {kKeyword, "sub",   {.prog = &Program::RpnStrsub}, "return a substring of the string in level 3"},
    {kKeyword, "endl",  {.prog = &Program::RpnEndl}, "end line character"},

    // BRANCH
    {kUndef, "", {.prog = nullptr}, "\nBRANCH"},
    {kBranch, "if",     {.branch = &Program::RpnIf}, "if <test-instruction> then <true-instructions> else <false-instructions> end"},
    {kBranch, "then",   {.branch = &Program::RpnThen}, "used with if"},
    {kBranch, "else",   {.branch = &Program::RpnElse}, "used with if"},
    {kBranch, "end",    {.branch = &Program::RpnEnd}, "used with various branch instructions"},
    {kBranch, "start",  {.branch = &Program::RpnStart}, "<start> <end> start <instructions> next|<step> step"},
    {kBranch, "for",    {.branch = &Program::RpnFor}, "<start> <end> for <variable> <instructions> next|<step> step"},
    {kBranch, "next",   {.branch = &Program::RpnNext}, "used with start and for"},
    {kBranch, "step",   {.branch = &Program::RpnStep}, "used with start and for"},
    {kKeyword, "ift",   {.prog = &Program::RpnIft}, "similar to if-then-end, <test-instruction> <true-instruction> ift"},
    {kKeyword, "ifte",  {.prog = &Program::RpnIfte}, "similar to if-then-else-end, <test-instruction> <true-instruction> <false-instruction> ifte"},
    {kBranch, "do",     {.branch = &Program::RpnDo}, "do <instructions> until <condition> end"},
    {kBranch, "until",  {.branch = &Program::RpnUntil}, "used with do"},
    {kBranch, "while",  {.branch = &Program::RpnWhile}, "while <test-instruction> repeat <loop-instructions> end"},
    {kBranch, "repeat", {.branch = &Program::RpnRepeat}, "used with while"},

    // STORE
    {kUndef, "", {.prog = nullptr}, "\nSTORE"},
    {kKeyword, "sto",   {.prog = &Program::RpnSto}, "store a variable. ex: 1 'name' sto"},
    {kKeyword, "rcl",   {.prog = &Program::RpnRcl}, "recall a variable. ex: 'name' rcl"},
    {kKeyword, "purge", {.prog = &Program::RpnPurge}, "delete a variable. ex: 'name' purge"},
    {kKeyword, "vars",  {.prog = &Program::RpnVars}, "list all variables"},
    {kKeyword, "clusr", {.prog = &Program::RpnClusr}, "erase all variables"},
    {kKeyword, "edit",  {.prog = &Program::RpnEdit}, "edit a variable content"},
    {kKeyword, "sto+",  {.prog = &Program::RpnStoadd}, "add to a stored variable. ex: 1 'name' sto+ 'name' 2 sto+"},
    {kKeyword, "sto-",  {.prog = &Program::RpnStosub}, "substract to a stored variable. ex: 1 'name' sto- 'name' 2 sto-"},
    {kKeyword, "sto*",  {.prog = &Program::RpnStomul}, "multiply a stored variable. ex: 3 'name' sto* 'name' 2 sto*"},
    {kKeyword, "sto/",  {.prog = &Program::RpnStodiv}, "divide a stored variable. ex: 3 'name' sto/ 'name' 2 sto/"},
    {kKeyword, "sneg",  {.prog = &Program::RpnStoneg}, "negate a variable. ex: 'name' sneg"},
    {kKeyword, "sinv",  {.prog = &Program::RpnStoinv}, "inverse a variable. ex: 1 'name' sinv"},

    // PROGRAM
    {kUndef, "", {.prog = nullptr}, "\nPROGRAM"},
    {kKeyword, "eval", {.prog = &Program::RpnEval}, "evaluate (run) a program, or recall a variable. ex: 'my_prog' eval"},
    {kBranch, "->", {.branch = &Program::RpnInprog}, "load program local variables. ex: << -> n m << 0 n m for i i + next >> >>"},

    // TRIG ON REALS AND COMPLEXES
    {kUndef, "", {.prog = nullptr}, "\nTRIG ON REALS AND COMPLEXES"},
    {kKeyword, "pi",    {.prog = &Program::RpnPi}, "pi constant"},
    {kKeyword, "sin",   {.prog = &Program::RpnSin}, "sinus"},
    {kKeyword, "asin",  {.prog = &Program::RpnAsin}, "arg sinus"},
    {kKeyword, "cos",   {.prog = &Program::RpnCos}, "cosinus"},
    {kKeyword, "acos",  {.prog = &Program::RpnAcos}, "arg cosinus"},
    {kKeyword, "tan",   {.prog = &Program::RpnTan}, "tangent"},
    {kKeyword, "atan",  {.prog = &Program::RpnAtan}, "arg tangent"},
    {kKeyword, "d->r",  {.prog = &Program::RpnD2r}, "convert degrees to radians"},
    {kKeyword, "r->d",  {.prog = &Program::RpnR2d}, "convert radians to degrees"},

    // LOGS ON REALS AND COMPLEXES
    {kUndef, "", {.prog = nullptr}, "\nLOGS ON REALS AND COMPLEXES"},
    {kKeyword, "e",      {.prog = &Program::RpnE}, "Euler constant"},
    {kKeyword, "ln",     {.prog = &Program::RpnLn}, "logarithm base e"},
    {kKeyword, "log",    {.prog = &Program::RpnLn}, ""},
    {kKeyword, "lnp1",   {.prog = &Program::RpnLnp1}, "ln(1+x) which is useful when x is close to 0"},
    {kKeyword, "exp",    {.prog = &Program::RpnExp}, "exponential"},
    {kKeyword, "expm",   {.prog = &Program::RpnExpm}, "exp(x)-1 which is useful when x is close to 0"},
    {kKeyword, "log10",  {.prog = &Program::RpnLog10}, "logarithm base 10"},
    {kKeyword, "alog10", {.prog = &Program::RpnAlog10}, "exponential base 10"},
    {kKeyword, "exp10",  {.prog = &Program::RpnAlog10}, ""},
    {kKeyword, "log2",   {.prog = &Program::RpnLog2}, "logarithm base 2"},
    {kKeyword, "alog2",  {.prog = &Program::RpnAlog2}, "exponential base 2"},
    {kKeyword, "exp2",   {.prog = &Program::RpnAlog2}, ""},
    {kKeyword, "sinh",   {.prog = &Program::RpnSinh}, "hyperbolic sine"},
    {kKeyword, "asinh",  {.prog = &Program::RpnAsinh}, "inverse hyperbolic sine"},
    {kKeyword, "cosh",   {.prog = &Program::RpnCosh}, "hyperbolic cosine"},
    {kKeyword, "acosh",  {.prog = &Program::RpnAcosh}, "inverse hyperbolic cosine"},
    {kKeyword, "tanh",   {.prog = &Program::RpnTanh}, "hyperbolic tangent"},
    {kKeyword, "atanh",  {.prog = &Program::RpnAtanh}, "inverse hyperbolic tangent"},

    // TIME AND DATE
    {kUndef, "", {.prog = nullptr}, "\nTIME AND DATE"},
    {kKeyword, "time",  {.prog = &Program::RpnTime}, "local time in ISO 8601 format"},
    {kKeyword, "date",  {.prog = &Program::RpnDate}, "local date in ISO 8601 format"},
    {kKeyword, "ticks", {.prog = &Program::RpnTicks}, "local date and time in µs"}
};
// clang-format on

/// autocompletion vector for linenoise autocompletion
vector<string>& Program::GetAutocompletionWords() {
    static vector<string> autocompletion_words;
    if (autocompletion_words.empty())
        for (auto& kw : keywords_)
            if (!kw.name.empty()) autocompletion_words.push_back(kw.name);
    return autocompletion_words;
}

/// @brief run a program on a stack and a heap
///
/// @return RetValue see this type
///
RetValue Program::Run() {
    bool go_out = false;
    RetValue ret = kOk;

    err_ = kOk;
    err_context_ = "";

    // iterate objects
    for (size_t i = 0; (go_out == false) && (i < size());) {
        Object* o = at(i);
        switch (o->type) {
            // could be an auto-evaluated symbol
            case kSymbol:
                AutoRcl(reinterpret_cast<Symbol*>(o));
                i++;
                break;

            // a keyword
            case kKeyword: {
                Keyword* k = reinterpret_cast<Keyword*>(o);
                // call the matching function
                (this->*(k->fn))();
                switch (err_) {
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
                        if (err_ == kTestFailed) ret = kTestFailed;

                        // other deadly error
                        if (ShowError(err_, err_context_) == kDeadlyError)
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
                        (void)ShowError(err_, err_context_);
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
                stack_.push_front(o->Clone());
                i++;
                break;
        }
    }

    return ret;
}

/// @brief prepare a program for execution
/// this is needed before a program can be run
/// inner members of branch or keyword objects are filled by this function
/// these inner members store for example the index of the next keyword to execute etc.
///
/// @return RetValue see this type
///
RetValue Program::Preprocess() {
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
        if (at(i)->type == kBranch) {
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
                    ShowSyntaxError("missing end after then");
                    return kSyntaxError;
                }
                if (layout_index < 0) {
                    // error: show it
                    ShowSyntaxError("missing if before then");
                    return kSyntaxError;
                }
                if (vlayout[layout_index].index_then_or_unti_or_repeat != -1) {
                    // error: show it
                    ShowSyntaxError("duplicate then");
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
                    ShowSyntaxError("missing end after else");
                    return kSyntaxError;
                }
                if (layout_index < 0) {
                    // error: show it
                    ShowSyntaxError("missing if before else");
                    return kSyntaxError;
                }
                if (vlayout[layout_index].index_then_or_unti_or_repeat == -1) {
                    // error: show it
                    ShowSyntaxError("missing then before else");
                    return kSyntaxError;
                }
                if (vlayout[layout_index].index_else != -1) {
                    // error: show it
                    ShowSyntaxError("duplicate else");
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
                    ShowSyntaxError("missing start or for before next");
                    return kSyntaxError;
                }
                k->arg1 = vstart_index[vstart_index.size() - 1];  // 'next' arg1 = 'start' index
                reinterpret_cast<Branch*>(at(vstart_index[vstart_index.size() - 1]))->arg2 =
                    i;  // 'for' or 'start' arg2 = 'next' index
                vstart_index.pop_back();
            } else if (k->value == "step") {
                if (vstart_index.size() == 0) {
                    // error: show it
                    ShowSyntaxError("missing start or for before step");
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
                    ShowSyntaxError("missing end");
                    return kSyntaxError;
                }
                if (layout_index < 0 || !vlayout[layout_index].is_do_unti) {
                    // error: show it
                    ShowSyntaxError("missing do");
                    return kSyntaxError;
                }
                if (vlayout[layout_index].index_then_or_unti_or_repeat != -1) {
                    // error: show it
                    ShowSyntaxError("duplicate until");
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
                    ShowSyntaxError("missing while");
                    return kSyntaxError;
                }
                if (vlayout[layout_index].index_then_or_unti_or_repeat != -1) {
                    // error: show it
                    ShowSyntaxError("duplicate repeat");
                    return kSyntaxError;
                }
                vlayout[layout_index].index_then_or_unti_or_repeat = i;
            } else if (k->value == "end") {
                size_t next = i + 1;
                if (next >= size()) next = kStepOut;

                if (layout_index < 0) {
                    // error: show it
                    ShowSyntaxError("missing branch instruction before end");
                    return kSyntaxError;
                } else {
                    if (vlayout[layout_index].is_do_unti) {
                        // this end closes a do..unti
                        if (vlayout[layout_index].index_end != -1) {
                            // error: show it
                            ShowSyntaxError("duplicate end");
                            return kSyntaxError;
                        }

                        if (vlayout[layout_index].index_then_or_unti_or_repeat == -1) {
                            // error: show it
                            ShowSyntaxError("missing until");
                            return kSyntaxError;
                        }

                        k->arg1 = vlayout[layout_index].index_if_or_do_or_while + 1;
                        layout_index--;
                    } else if (vlayout[layout_index].is_while_repeat) {
                        // this end closes a while..repeat
                        if (vlayout[layout_index].index_end != -1) {
                            // error: show it
                            ShowSyntaxError("duplicate end");
                            return kSyntaxError;
                        }

                        k->arg2 = vlayout[layout_index].index_if_or_do_or_while + 1;

                        if (vlayout[layout_index].index_then_or_unti_or_repeat == -1) {
                            // error: show it
                            ShowSyntaxError("missing repeat");
                            return kSyntaxError;
                        }

                        // fill 'repeat' arg1 with 'end+1'
                        reinterpret_cast<Branch*>(at(vlayout[layout_index].index_then_or_unti_or_repeat))->arg1 = i + 1;
                        layout_index--;
                    } else {
                        // this end closes an if..then..(else)
                        if (vlayout[layout_index].index_end != -1) {
                            // error: show it
                            ShowSyntaxError("duplicate end");
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
                                ShowSyntaxError("missing then");
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
        ShowSyntaxError("missing end");
        return kSyntaxError;
    }
    if (vstart_index.size() > 0) {
        // error: show it
        ShowSyntaxError("missing next or step after for or start");
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
RetValue Program::Parse(const string& entry) {
    static map<string, Lexer::ReservedWord> keywords_map;
    vector<Lexer::SynElement> elements;
    vector<Lexer::SynError> errors;
    RetValue ret = kOk;

    // prepare map for finding reserved keywords
    if (keywords_map.empty())
        for (auto& kw : keywords_)
            if (!kw.name.empty()) keywords_map[kw.name] = {kw.type, kw.fn};

    // separate the entry string
    if (Analyse(entry, keywords_map, elements, errors)) {
        // make objects from parsed elements
        for (Lexer::SynElement& element : elements) {
            switch (element.type) {
                case kNumber:
                    push_back(new Number(*element.re, element.re_base));
                    break;
                case kComplex:
                    push_back(new Complex(*element.re, *element.im, element.re_base, element.im_base));
                    break;
                case kString:
                    push_back(new String(element.value));
                    break;
                case kSymbol:
                    push_back(new Symbol(element.value, element.auto_eval));
                    break;
                case kProgram: {
                    Program* p = new Program(stack_, heap_, this);
                    if (p->Parse(element.value) == kOk) {
                        // give a clean format to the program string
                        stringstream ss;
                        for (size_t i = 0; i < p->size(); i++) ss << ((i > 0) ? " " : "") << p->at(i);
                        p->value = ss.str();
                        push_back(p);
                    }
                } break;
                case kKeyword:
                    push_back(new Keyword(element.fn.prog, element.value));
                    break;
                case kBranch:
                    push_back(new Branch(element.fn.branch, element.value));
                    break;
                default:
                    ShowError(kUnknownError, "error creating program from entry");
                    break;
            }
            if (element.re != nullptr) delete element.re;
            if (element.im != nullptr) delete element.im;
        }

        // compute links between the objects
        return Preprocess();
    } else {
        for (SynError& err : errors) ShowSyntaxError(err.err.c_str());
    }

    return ret;
}

/// @brief show the last error set
///
/// @return RetValue see this type
///
RetValue Program::ShowError() {
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
    cerr << err_context_ << ": error " << err_ << ": " << errorStrings[err_] << endl;
    switch (err_) {
        case kInternalError:
        case kDeadlyError:
            ret = kDeadlyError;
            break;
        default:
            ret = kOk;
            break;
    }

    return ret;
}

/// @brief record an error as the last error set and show it
///
/// @param err the error to record
/// @param context a context string
/// @return RetValue see this type
///
RetValue Program::ShowError(RetValue err, string& context) {
    // record error
    err_ = err;
    err_context_ = context;
    return ShowError();
}

/// @brief record an error as the last error set and show it
///
/// @param err the error to record
/// @param context a context string
/// @return RetValue see this type
///
RetValue Program::ShowError(RetValue err, const char* context) {
    // record error
    err_ = err;
    err_context_ = context;
    return ShowError();
}

/// @brief set the last error as being a syntax error and show it
///
/// @param err the error to record
/// @param context a context string
/// @return RetValue see this type
///
void Program::ShowSyntaxError(const char* context) {
    // record error
    err_ = kSyntaxError;
    err_context_ = context;
    (void)ShowError();
}

/// @brief return the last error set
///
/// @return RetValue see this type
///
RetValue Program::GetLastError(void) { return err_; }

/// @brief show a stack (show its different objects)
/// generally a stack is associated to a running program
///
/// @param show_separator whether to show a stack level prefix or not
///
void Program::ShowStack(bool show_separator) {
    if (stack_.size() == 1) {
        cout << stack_[0] << endl;
    } else {
        for (int i = stack_.size() - 1; i >= 0; i--) {
            if (show_separator) cout << i + 1 << "> ";
            cout << stack_[i] << endl;
        }
    }
}

/// @brief apply default precision mode and digits
///
void Program::ApplyDefault() {
    // default float precision, float mode
    Number::mode = Number::kDefaultMode;
    Number::digits = Number::kDefaultDecimalDigits;
    mpreal::set_default_prec(Number::kMpfrDefaultPrecBits);

    static mp_rnd_t def_rnd = mpreal::get_default_rnd();
    mpreal::set_default_rnd(def_rnd);
}
