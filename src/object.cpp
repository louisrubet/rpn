#include <math.h>
#include <string>
using namespace std;

#include "constant.h"
#define MPFR_USE_NO_MACRO
#include <mpfr.h>
#include "object.hpp"

// TODO remove

// number statics
number::mode_enum number::s_mode = DEFAULT_MODE;
int number::s_decimal_digits = DEFAULT_DECIMAL_DIGITS;

#if 0
/// @brief return if a mpfr is higher to a given precision
/// this function is directly copied from mpfr 
///
/// @param p the mpfr to test
/// @param prec the precision
/// @return true p is higher than 10^prec
/// @return false p is lower than 10^prec
///
static bool is_min(mpfr_t p, mpfr_prec_t prec) {
    // see mpfr_vasprintf code
    bool ret;
    int round_away;
    switch (mpreal::get_default_rnd()) {
        case MPFR_RNDA:
            round_away = 1;
            break;
        case MPFR_RNDD:
            round_away = MPFR_IS_NEG(p);
            break;
        case MPFR_RNDU:
            round_away = MPFR_IS_POS(p);
            break;
        case MPFR_RNDN: {
            /* compare |p| to y = 0.5*10^(-prec) */
            mpfr_t y;
            mpfr_exp_t e = MAX(MPFR_PREC(p), 56);
            mpfr_init2(y, e + 8);
            do {
                /* find a lower approximation of
                0.5*10^(-prec) different from |p| */
                e += 8;
                mpfr_set_prec(y, e);
                mpfr_set_si(y, -prec, MPFR_RNDN);
                mpfr_exp10(y, y, MPFR_RNDD);
                mpfr_div_2ui(y, y, 1, MPFR_RNDN);
            } while (mpfr_cmpabs(y, p) == 0);

            round_away = mpfr_cmpabs(y, p) < 0;
            mpfr_clear(y);
        } break;
        default:
            round_away = 0;
    }

    if (round_away) /* round away from zero: the last output digit is '1' */
        ret = true;
    else
        /* only zeros in fractional part */
        ret = false;
    return ret;
}

/// @brief print a mpfr in fixed format according to a base
/// this function is based copied on mpfr library
/// 
/// @param stream the stream to write to
/// @param real the real to print
/// @param base the base to print the real
/// @param write_after_sign substring to write between the sign and the real
///
static void print_fix(FILE* stream, mpfr_t real, int base, const char* write_after_sign = NULL) {
    // see mpfr_vasprintf code
    mpfr_exp_t exp = mpfr_get_exp(real);
    int digits = 0;  // forced 0 digits after separator
    int i;

    if (MPFR_UNLIKELY(MPFR_IS_SINGULAR(real))) {
        if (MPFR_IS_NAN(real))
            fputs("nan", stream);
        else if (MPFR_IS_INF(real)) {
            if (MPFR_IS_NEG(real)) fputc('-', stream);
            fputs("inf", stream);
        } else {
            // zero
            if (MPFR_IS_NEG(real)) fputc('-', stream);  // signed zero is allowed
            if (write_after_sign != NULL) fputs(write_after_sign, stream);
            fputc('0', stream);
            if (digits > 0) {
                fputc('.', stream);
                for (i = 0; i < digits; i++) fputc('0', stream);
            }
        }
    } else if (exp < -digits) {
        if (MPFR_IS_NEG(real)) fputc('-', stream);
        if (write_after_sign != NULL) fputs(write_after_sign, stream);
        fputc('0', stream);
        if (digits > 0) {
            fputc('.', stream);
            for (i = 0; i < digits - 1; i++) fputc('0', stream);

            if (is_min(real, digits))
                fputc('1', stream);
            else
                fputc('0', stream);
        }
    } else {
        char* str = mpfr_get_str(NULL, &exp, base, digits + exp + 1, real, mpreal::get_default_rnd());
        char* print_from;
        if (str != NULL) {
            int len = strlen(str);
            print_from = str;
            if (len > 0) {
                if (print_from[0] == '-') {
                    fputc(print_from[0], stream);
                    len--;
                    print_from++;
                } else if (print_from[0] == '+') {
                    len--;
                    print_from++;
                }
                if (write_after_sign != NULL) fputs(write_after_sign, stream);
                if (exp < 0) {
                    fputc('0', stream);
                    if (digits > 0) {
                        fputc('.', stream);
                        for (i = 0; i < -(int)exp; i++) fputc('0', stream);
                        fputs(str, stream);
                        for (i = 0; i < (int)(digits - len + exp); i++) fputc('0', stream);
                    }
                } else {
                    if (exp == 0)
                        fputc('0', stream);
                    else
                        for (i = 0; i < (int)exp; i++) fputc(print_from[i], stream);
                    if (digits > 0) {
                        fputc('.', stream);

                        int remaining = (int)MIN(strlen(print_from) - exp - 1, digits) + 1;
                        for (i = (int)exp; i < remaining + (int)exp; i++) fputc(print_from[i], stream);
                        for (i = 0; i < (int)(exp + digits - len); i++) fputc('0', stream);
                    }
                }
            }
            mpfr_free_str(str);
        }
    }
}

/// @brief show an object representation according to its type
/// 
/// @param stream the stream to write to
///
void object::show(FILE* stream) {
    int digits;
    char* str;
    char base[32];

    switch (_type) {
        case cmd_number:
            switch (((number*)this)->repr) {
                case number::dec:
                    mpfr_fprintf(stream, number::s_mpfr_printf_format.c_str(), ((number*)this)->_value.mpfr);
                    break;
                case number::hex:
                    print_fix(stream, ((number*)this)->_value.mpfr, 16, "0x");
                    break;
                case number::bin:
                    print_fix(stream, ((number*)this)->_value.mpfr, 2, "0b");
                    break;
                case number::base:
                    sprintf(base, "%db", ((number*)this)->_base);
                    print_fix(stream, ((number*)this)->_value.mpfr, ((number*)this)->_base, base);
                    break;
                default:
                    fprintf(stream, "<unknown number representation>");
                    break;
            }
            break;
        case cmd_complex:
            switch (((complex*)this)->repr) {
                case number::dec:
                    fprintf(stream, "(");
                    mpfr_fprintf(stream, number::s_mpfr_printf_format.c_str(), ((complex*)this)->re()->mpfr);
                    fprintf(stream, ",");
                    mpfr_fprintf(stream, number::s_mpfr_printf_format.c_str(), ((complex*)this)->im()->mpfr);
                    fprintf(stream, ")");
                    break;
                case number::hex:
                    fprintf(stream, "(");
                    mpfr_fprintf(stream, string(MPFR_FORMAT_HEX).c_str(), ((complex*)this)->re()->mpfr);
                    fprintf(stream, ",");
                    mpfr_fprintf(stream, string(MPFR_FORMAT_HEX).c_str(), ((complex*)this)->im()->mpfr);
                    fprintf(stream, ")");
                    break;
                default:
                    fprintf(stream, "<unknown complex representation>");
            }
            break;
        case cmd_string:
            fprintf(stream, "\"%s\"", ((ostring*)this)->_value);
            break;
        case cmd_program:
            fprintf(stream, "<<%s>>", ((oprogram*)this)->_value);
            break;
        case cmd_symbol:
            fprintf(stream, "'%s'", ((symbol*)this)->_value);
            break;
        case cmd_keyword:
        case cmd_branch:
            fprintf(stream, "%s", ((keyword*)this)->_value);
            break;
        default:
            fprintf(stream, "< unknown object representation >");
            break;
    }
}
#endif
