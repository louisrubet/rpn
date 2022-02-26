// Copyright (c) 2014-2022 Louis Rubet

#include "mpreal-out.h"

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(h, i) ((h) > (i) ? (h) : (i))

// some defs for mpfr
#if _MPFR_EXP_FORMAT == 1
#define MPFR_EXP_MAX (SHRT_MAX)
#define MPFR_EXP_MIN (SHRT_MIN)
#elif _MPFR_EXP_FORMAT == 2
#define MPFR_EXP_MAX (INT_MAX)
#define MPFR_EXP_MIN (INT_MIN)
#elif _MPFR_EXP_FORMAT == 3
#define MPFR_EXP_MAX (LONG_MAX)
#define MPFR_EXP_MIN (LONG_MIN)
#elif _MPFR_EXP_FORMAT == 4
#define MPFR_EXP_MAX (MPFR_INTMAX_MAX)
#define MPFR_EXP_MIN (MPFR_INTMAX_MIN)
#else
#error "Invalid MPFR Exp format"
#endif

#define MPFR_EXP_INF (MPFR_EXP_MIN + 3)
#define MPFR_EXP_NAN (MPFR_EXP_MIN + 2)
#define MPFR_EXP(x) ((x)->_mpfr_exp)
#define MPFR_IS_SINGULAR(x) (MPFR_EXP(x) <= MPFR_EXP_INF)
#define MPFR_UNLIKELY(x) (__builtin_expect(!!(x), 0))
#define MPFR_IS_NAN(x) (MPFR_EXP(x) == MPFR_EXP_NAN)
#define MPFR_IS_INF(x) (MPFR_EXP(x) == MPFR_EXP_INF)
#define MPFR_IS_NEG(x) (MPFR_SIGN(x) < 0)
#define MPFR_IS_POS(x) (MPFR_SIGN(x) > 0)
#define MPFR_PREC(x) ((x)->_mpfr_prec)

ostream& MprealOutput10Base(ostream& out, const string& fmt, const mpreal& value) {
    // cf std::ostream& mpreal::output(std::ostream& os) const
    char* s = NULL;
    if (!(mpfr_asprintf(&s, fmt.c_str(), mpreal::get_default_rnd(), value.mpfr_srcptr()) < 0)) {
        out << string(s);
        mpfr_free_str(s);
    }
    return out;
}

static bool is_min(const mpreal& p, mpfr_prec_t prec) {
    // see mpfr_vasprintf code
    // TODO(louis):  here use mpreal functions like <=0, isinf etc
    bool ret;
    int round_away;
    switch (mpreal::get_default_rnd()) {
        case MPFR_RNDA:
            round_away = 1;
            break;
        case MPFR_RNDD:
            round_away = MPFR_IS_NEG(p.mpfr_srcptr());
            break;
        case MPFR_RNDU:
            round_away = MPFR_IS_POS(p.mpfr_srcptr());
            break;
        case MPFR_RNDN: {
            /* compare |p| to y = 0.5*10^(-prec) */
            mpfr_t y;
            mpfr_exp_t e = MAX(MPFR_PREC(p.mpfr_srcptr()), 56);
            mpfr_init2(y, e + 8);
            do {
                /* find a lower approximation of
                0.5*10^(-prec) different from |p| */
                e += 8;
                mpfr_set_prec(y, e);
                mpfr_set_si(y, -prec, MPFR_RNDN);
                mpfr_exp10(y, y, MPFR_RNDD);
                mpfr_div_2ui(y, y, 1, MPFR_RNDN);
            } while (mpfr_cmpabs(y, p.mpfr_srcptr()) == 0);

            round_away = mpfr_cmpabs(y, p.mpfr_srcptr()) < 0;
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

static void _out_base(ostream& out, int base) {
    if (base == 16)
        out << "0x";
    else if (base == 2)
        out << "0b";
    else
        out << base << "b";
}

ostream& _out_singular(ostream& out, int base, const mpreal& value) {
    const char* write_after_sign = NULL;  // unused for now
    int digits = 0;                       // forced 0 digits after separator
    if (MPFR_IS_NAN(value.mpfr_srcptr())) {
        out << "nan";
    } else if (MPFR_IS_INF(value.mpfr_srcptr())) {
        if (MPFR_IS_NEG(value.mpfr_srcptr())) out << '-';
        out << "inf";
    } else {
        // zero
        if (MPFR_IS_NEG(value.mpfr_srcptr())) out << '-';  // signed zero is allowed
        if (write_after_sign != NULL) out << write_after_sign;
        _out_base(out, base);
        out << '0';
        if (digits > 0) {
            out << '.';
            for (int i = 0; i < digits; i++) out << '0';
        }
    }
    return out;
}

ostream& _out_little_number(ostream& out, int base, const mpreal& value) {
    const char* write_after_sign = NULL;  // unused for now
    int digits = 0;                       // forced 0 digits after separator

    if (MPFR_IS_NEG(value.mpfr_srcptr())) out << '-';
    if (write_after_sign != NULL) out << write_after_sign;
    _out_base(out, base);
    out << '0';
    if (digits > 0) {
        out << '.';
        for (int i = 0; i < digits - 1; i++) out << '0';

        if (is_min(value.mpfr_srcptr(), digits))
            out << '1';
        else
            out << '0';
    }
    return out;
}

ostream& _out_number(ostream& out, int base, const mpreal& value) {
    const char* write_after_sign = NULL;  // unused for now
    int digits = 0;                       // forced 0 digits after separator
    mpfr_exp_t exp = mpfr_get_exp(value.mpfr_srcptr());

    char* str = mpfr_get_str(NULL, &exp, base, digits + static_cast<int>(exp) + 1, value.mpfr_srcptr(),
                             mpreal::get_default_rnd());
    int nexp = static_cast<int>(exp);
    char* print_from;
    if (str != NULL) {
        int len = strlen(str);
        print_from = str;
        if (len > 0) {
            if (print_from[0] == '-') {
                out << print_from[0];
                len--;
                print_from++;
            } else if (print_from[0] == '+') {
                len--;
                print_from++;
            }
            if (write_after_sign != NULL) out << write_after_sign;
            if (base == 16)
                out << "0x";
            else if (base == 2)
                out << "0b";
            else
                out << base << "b";
            if (nexp < 0) {
                out << '0';
                if (digits > 0) {
                    out << '.';
                    for (int i = 0; i < -nexp; i++) out << '0';
                    out << str;
                    for (int i = 0; i < digits - len + nexp; i++) out << '0';
                }
            } else {
                if (nexp == 0) {
                    out << '0';
                } else {
                    for (int i = 0; i < nexp; i++) out << print_from[i];
                }
                if (digits > 0) {
                    out << '.';

                    int remaining = MIN(static_cast<int>(strlen(print_from)) - nexp - 1, digits) + 1;
                    for (int i = nexp; i < remaining + nexp; i++) out << print_from[i];
                    for (int i = 0; i < nexp + digits - len; i++) out << '0';
                }
            }
        }
        mpfr_free_str(str);
    }
    return out;
}

ostream& MprealOutputNBase(ostream& out, int base, const mpreal& value) {
    // see mpfr_vasprintf code
    int digits = 0;  // forced 0 digits after separator

    if (MPFR_UNLIKELY(MPFR_IS_SINGULAR(value.mpfr_srcptr()))) return _out_singular(out, base, value);

    mpfr_exp_t exp = mpfr_get_exp(value.mpfr_srcptr());
    if (exp < -digits) return _out_little_number(out, base, value);

    return _out_number(out, base, value);
}
