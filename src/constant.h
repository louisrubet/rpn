#ifndef CONSTANT_H
#define CONSTANT_H

// default values
//

// default mode and number of printed digits
//
#define DEFAULT_MODE number::std
#define MPFR_DEFAULT_FORMAT "%.xxRg"

/* directly calculated from 128 bits precision
   ceil(128 * log10(2) = 39) */
#define DEFAULT_DECIMAL_DIGITS 39

// MPFR related defaults
//

// rounding method
#define MPFR_DEFAULT_RND MPFR_RNDN

// 128 bits significand precision
#define MPFR_DEFAULT_PREC_BITS 128

// 128 bits significand storing length in bytes, result of mpfr_custom_get_size(128)
#define MPFR_DEFAULT_STORING_LENGTH_BYTES 16

// constants
//

// commands and entry related constants
#define MAX_COMMAND_LENGTH 24
#define AUTOCOMPLETE_KEY '\t'
#define SHOW_STACK_SEPARATOR "> "
#define PROMPT "rpn> "

// show formats
#define MPFR_FORMAT_BEG "%."
#define MPFR_FORMAT_STD "Rg"
#define MPFR_FORMAT_FIX "Rf"
#define MPFR_FORMAT_SCI "Re"
#define MPFR_FORMAT_HEX "%Ra"

#define MPFR_RND_STRINGS { "nearest", "toward zero", "toward +inf", "toward -inf", "away from zero" }

// return values, used by all classes
//
typedef enum {
    ret_ok,
    ret_unknown_err,
    ret_missing_operand,
    ret_bad_operand_type,
    ret_out_of_range,
    ret_unknown_variable,
    ret_internal,
    ret_deadly,
    ret_good_bye,
    ret_not_impl,
    ret_nop,
    ret_syntax,
    ret_div_by_zero,
    ret_runtime_error,
    ret_max
} ret_value;

#define RET_VALUE_STRINGS { \
        "ok", "unknown command", "missing operand", "bad operand type", "out of range", "unknown variable", "internal error, aborting", \
        "deadly", "goodbye", "not implemented", "no operation", "syntax error", "division by zero", "runtime error" \
    }

// command types
//
#define CMD_TYPE_STRINGS { "undef", "number", "complex", "string", "symbol", "program", "keyword", "keyword" }

// history
#define HISTORY_FILE ".rpn_history"
#define HISTORY_FILE_MAX_LINES (100)

#endif
