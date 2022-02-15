#include "program.hpp"

/// @brief nop keyword implementation
///
void program::rpn_nop() {
    // nop
}

/// @brief quit keyword implementation
///
void program::rpn_good_bye() { ERR_CONTEXT(ret_good_bye); }

/// @brief nop keyword implementation
/// the result is written on stdout
///
void program::rpn_help() {
    // software name
    printf("\n" ATTR_BOLD "%s" ATTR_OFF "\n", uname);

    // description
    printf("%s\n\n", description);

    // syntax
    printf("%s\n", syntax);

    // keywords
    unsigned int i = 0;
    while (s_keywords[i].type != cmd_max) {
        if (s_keywords[i].comment.size() != 0) {
            // titles in bold
            if (s_keywords[i].type == cmd_undef) printf(ATTR_BOLD);
            // show title or keyword + comment
            printf("%s\t%s\n", s_keywords[i].name, s_keywords[i].comment.c_str());
            if (s_keywords[i].type == cmd_undef) printf(ATTR_OFF);
        }
        i++;
    }
    printf("\n");

    // show mode
    printf("Current float mode is ");
    switch (number::s_mode) {
        case number::std:
            printf("'std'");
            break;
        case number::fix:
            printf("'fix'");
            break;
        case number::sci:
            printf("'sci'");
            break;
        default:
            printf("unknown");
            break;
    }
    printf(" with %d digits after the decimal point\n", number::s_decimal_digits);

    // bits precision, decimal digits and rounding mode
    printf("Current floating point precision is %d bits\n", (int)number::s_mpfr_prec);
    printf("Current rounding mode is \"%s\"\n", number::s_mpfr_rnd_str[number::s_mpfr_rnd]);
    printf("\n\n");
}

/// @brief calculate a number of digits for a given base from a precision in bits
/// 
/// @param base the base
/// @param bit_precision the precision in bits
/// @return int the number of digits
///
static int base_digits_from_bit_precision(int base, int bit_precision) {
    return (int)ceil(bit_precision * log(2.0) / log((double)base)) - 1;
}

/// @brief print a decimal digit in a given MPFR format
/// 
/// @param decimal_digits the number
/// @param printf_format the format
/// @return string the result string
///
static string make_digit_format(int decimal_digits, const char* printf_format) {
    stringstream ss;
    ss << MPFR_FORMAT_BEG;
    ss << number::s_decimal_digits;
    ss << printf_format;
    return ss.str();
}

/// @brief whether a precision is in the precision min/max
/// 
/// @param precision the precision in bits
/// @return true the precision is good
/// @return false the precision is not good
///
static bool check_decimal_digits(double precision) {
    bool ret = true;

    // MPFR_PREC_MAX mpfr_prec_t depends on _MPFR_PREC_FORMAT macro (see mpfr.h)
    // this could not exceed 63 bits max (0x7FFFFFFFFFFFFFFF)
    double prec_max = (double)MPFR_PREC_MAX;
    double prec_min = 0.0;

    if (precision < prec_min || precision > prec_max) ret = false;

    return ret;
}

/// @brief std keyword implementation
///
void program::rpn_std() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    double digits = double(((number*)_stack->pop_back())->_value);

    if (check_decimal_digits(digits)) {
        // set mode, decimal digits and print format
        number::s_mode = number::std;
        number::s_decimal_digits = (int)digits;
        number::s_mpfr_printf_format = make_digit_format(number::s_decimal_digits, MPFR_FORMAT_STD);
    } else
        ERR_CONTEXT(ret_out_of_range);
}

/// @brief fix keyword implementation
///
void program::rpn_fix() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    double digits = double(((number*)_stack->pop_back())->_value);

    if (check_decimal_digits(digits)) {
        // set mode, decimal digits and print format
        number::s_mode = number::fix;
        number::s_decimal_digits = (int)digits;
        number::s_mpfr_printf_format = make_digit_format(number::s_decimal_digits, MPFR_FORMAT_FIX);
    } else
        ERR_CONTEXT(ret_out_of_range);
}

/// @brief sci keyword implementation
///
void program::rpn_sci() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    double digits = double(((number*)_stack->pop_back())->_value);

    if (check_decimal_digits(digits)) {
        // set mode, decimal digits and print format
        number::s_mode = number::sci;
        number::s_decimal_digits = (int)digits;
        number::s_mpfr_printf_format = make_digit_format(number::s_decimal_digits, MPFR_FORMAT_SCI);
    } else
        ERR_CONTEXT(ret_out_of_range);
}

/// @brief version keyword implementation
///
void program::rpn_version() {
    _stack->push_back(new ostring(version));
}

/// @brief uname keyword implementation
///
void program::rpn_uname() {
    _stack->push_back(new ostring(uname));
}

/// @brief history keyword implementation
///
void program::rpn_history() {
    // see command history on stdout
    int index = 0;
    char* line = linenoiseHistoryLine(index);
    while (line != NULL) {
        cout << line << endl;
        free(line);
        line = linenoiseHistoryLine(++index);
    }
}

/// @brief type keyword implementation
///
void program::rpn_type() {
    MIN_ARGUMENTS(1);

    int type = _stack->pop_back()->_type;
    if (type < 0 || type >= (int)cmd_max) type = (int)cmd_undef;
    _stack->push_back(new ostring(object::s_cmd_type_string[type]));
}

/// @brief default keyword implementation
///
void program::rpn_default() { program::apply_default(); }

/// @brief prec keyword implementation
///
void program::rpn_precision() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    // set precision
    unsigned long prec = static_cast<number*>(_stack->pop_back())->_value.toULong();
    if (prec >= (unsigned long)MPFR_PREC_MIN && prec <= (unsigned long)MPFR_PREC_MAX) {
        number::s_mpfr_prec = (mpfr_prec_t)prec;

        // modify digits seen by user if std mode
        if (number::s_mode == number::std) {
            // calc max nb of digits user can see with the current bit precision
            number::s_decimal_digits = base_digits_from_bit_precision(10, number::s_mpfr_prec);
            number::s_mpfr_printf_format = make_digit_format(number::s_decimal_digits, MPFR_FORMAT_STD);
        }
    } else
        ERR_CONTEXT(ret_out_of_range);
}

/// @brief round keyword implementation
///
void program::rpn_round() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_string);

    ostring* str = (ostring*)_stack->pop_back();
    bool done = false;
    for (int rnd = (int)MPFR_DEFAULT_RND; rnd <= (int)MPFR_RNDA; rnd++) {
        if (string(number::s_mpfr_rnd_str[rnd]) == str->_value) {
            number::s_mpfr_rnd = (mpfr_rnd_t)rnd;
            done = true;
        }
    }
    if (!done) ERR_CONTEXT(ret_out_of_range);
}
