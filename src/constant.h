#ifndef CONSTANT_H
#define CONSTANT_H

#define MPFR_USE_NO_MACRO
#include <mpfr.h>

// default values
//

// default mode and number of printed digits
//
#define DEFAULT_MODE number::std

/* directly calculated from 128 bits precision
   ceil(128 * log10(2)) - 1 = 38 */
#define DEFAULT_DECIMAL_DIGITS 38

// MPFR related defaults
//

// 128 bits significand precision
#define MPFR_DEFAULT_PREC_BITS 128

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
    ret_abort_current_entry,
    ret_out_of_memory,
    ret_bad_value,
    ret_max
} ret_value;

// base min and max
#define BASE_MIN 2
#define BASE_MAX 62

#endif
