// Copyright (c) 2014-2022 Louis Rubet

#include "mpreal-out.h"

#include <algorithm>

ostream& MprealFmtOutput(ostream& out, const string& fmt, const mpreal& value) {
    // cf std::ostream& mpreal::output(std::ostream& os) const
    char* s = NULL;
    if (!(mpfr_asprintf(&s, fmt.c_str(), mpreal::get_default_rnd(), value.mpfr_srcptr()) < 0)) {
        out << string(s);
        mpfr_free_str(s);
    }
    return out;
}

static bool IsMin(const mpreal& p, mpfr_prec_t prec) {
    // see mpfr_vasprintf code
    bool ret;
    int round_away;
    switch (mpreal::get_default_rnd()) {
        case MPFR_RNDA:
            round_away = 1;
            break;
        case MPFR_RNDD:
            round_away = (p < 0.0);
            break;
        case MPFR_RNDU:
            round_away = (p > 0.0);
            break;
        case MPFR_RNDN: {
            /* compare |p| to y = 0.5*10^(-prec) */
            mpfr_t y;
            mpfr_exp_t e = std::max(p.get_prec(), static_cast<mpfr_exp_t>(56));
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

static void OutBase(ostream& out, int base) {
    if (base == 16)
        out << "0x";
    else if (base == 2)
        out << "0b";
    else
        out << base << "b";
}

static ostream& OutSingular(ostream& out, int base, const mpreal& value) {
    const char* write_after_sign = NULL;  // unused for now
    int digits = 0;                       // forced 0 digits after separator
    if (isnan(value)) {
        out << "nan";
    } else if (isinf(value)) {
        if (value < 0) out << '-';
        out << "inf";
    } else {
        // zero
        if (value < 0) out << '-';  // signed zero is allowed
        if (write_after_sign != NULL) out << write_after_sign;
        OutBase(out, base);
        out << '0';
        if (digits > 0) {
            out << '.';
            for (int i = 0; i < digits; i++) out << '0';
        }
    }
    return out;
}

static ostream& OutLittleNumber(ostream& out, int base, const mpreal& value) {
    const char* write_after_sign = NULL;  // unused for now
    int digits = 0;                       // forced 0 digits after separator

    if (value < 0) out << '-';
    if (write_after_sign != NULL) out << write_after_sign;
    OutBase(out, base);
    out << '0';
    if (digits > 0) {
        out << '.';
        for (int i = 0; i < digits - 1; i++) out << '0';

        if (IsMin(value, digits))
            out << '1';
        else
            out << '0';
    }
    return out;
}

static ostream& OutNumber(ostream& out, int base, const mpreal& value) {
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

                    int remaining = std::min(static_cast<int>(strlen(print_from)) - nexp - 1, digits) + 1;
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

    // singular = nan, +/-inf or zero
    if (iszero(value) || isnan(value) || isinf(value)) return OutSingular(out, base, value);

    mpfr_exp_t exp = mpfr_get_exp(value.mpfr_srcptr());
    if (exp < -digits) return OutLittleNumber(out, base, value);

    return OutNumber(out, base, value);
}
