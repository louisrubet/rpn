// default mode and number of printed digits
//
#define DEFAULT_MODE number::std
#define DEFAULT_PRECISION 20

// MPFR related constants
//

// rounding method
#define MPFR_DEF_RND MPFR_RNDN
// 128 bits significand precision
#define MPFR_DEFAULT_PREC_BITS 128
// 128 bits significand storing length in bytes, result of mpfr_custom_get_size(128)
#define MPFR_DEFAULT_STORING_LENGTH_BYTES 16

// show formats
#define MPFR_FORMAT_BEG "%."
#define MPFR_FORMAT_STD "Rg"
#define MPFR_FORMAT_FIX "Rf"
#define MPFR_FORMAT_SCI "Re"
#define MPFR_FORMAT_HEX "%Ra"
#define MPFR_FORMAT "%.20Rg"

#define MPFR_RND_STRINGS { "nearest", "toward zero", "toward +inf", "toward -inf", "away from zero" }

// return values
//
#define RET_VALUE_STRINGS { \
        "ok", "unknown command", "missing operand", "bad operand type", "out of range", "unknown variable", "internal error, aborting", \
        "deadly", "goodbye", "not implemented", "no operation", "syntax error", "division by zero", "runtime error" \
    }

// command types
//
#define CMD_TYPE_STRINGS { "undef", "number", "string", "symbol", "program", "keyword", "keyword" }
