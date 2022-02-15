#ifndef CONSTANT_H
#define CONSTANT_H

#define MPFR_USE_NO_MACRO
#include <mpfr.h>

// default values
//

// default mode and number of printed digits
//
#define DEFAULT_MODE number::std
#define MPFR_DEFAULT_FORMAT "%.xxRg"

/* directly calculated from 128 bits precision
   ceil(128 * log10(2)) - 1 = 38 */
#define DEFAULT_DECIMAL_DIGITS 38

// MPFR related defaults
//

// rounding method
#define MPFR_DEFAULT_RND MPFR_RNDN
#define MPFR_ROUND_STRINGS                                                                                             \
    {"nearest (even)", MPFR_RNDN}, {"toward zero", MPFR_RNDZ}, {"toward +inf", MPFR_RNDU}, {"toward -inf", MPFR_RNDD}, \
        {"away from zero", MPFR_RNDA}, {"faithful rounding", MPFR_RNDF}, {                                             \
        "nearest (away from zero)", MPFR_RNDNA                                                                         \
    }

// 128 bits significand precision
#define MPFR_DEFAULT_PREC_BITS 128

// constants
//

// commands and entry related constants
#define MAX_COMMAND_LENGTH 24
#define AUTOCOMPLETE_KEY '\t'
#define SHOW_STACK_SEPARATOR "> "
#define PROMPT "rpn> "
#define MULTILINE_PROMPT "> "

// show formats
#define MPFR_FORMAT_BEG "%."
#define MPFR_FORMAT_STD "Rg"
#define MPFR_FORMAT_FIX "Rf"
#define MPFR_FORMAT_SCI "Re"
#define MPFR_FORMAT_HEX "%Ra"

// return values, used by all classes
//
typedef enum {
    ret_ok,
    ret_unknown_err,
    ret_missing_operand,
    ret_bad_operand_type,
    ret_out_of_range,
    ret_bad_value,
    ret_unknown_variable,
    ret_internal,
    ret_deadly,
    ret_good_bye,
    ret_not_impl,
    ret_nop,
    ret_syntax,
    ret_div_by_zero,
    ret_runtime_error,
    ret_abort_current_entry,
    ret_out_of_memory,
    ret_max
} ret_value;

#define RET_VALUE_STRINGS                                                                                           \
    {                                                                                                               \
        "ok", "unknown command", "missing operand", "bad operand type", "out of range", "bad value",                \
            "unknown variable", "internal error, aborting", "deadly", "goodbye", "not implemented", "no operation", \
            "syntax error", "division by zero", "runtime error", "aborted current entry", "out of memory"           \
    }

// command types
//

// history
#define HISTORY_FILE ".rpn_history"
#define HISTORY_FILE_MAX_LINES (100)

// base min and max
#define BASE_MIN 2
#define BASE_MAX 62

#endif
