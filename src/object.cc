// Copyright (c) 2014-2022 Louis Rubet

#include "object.h"

#include <stdlib.h>

// number statics
Number::mode_enum Number::mode = Number::kDefaultMode;
limb_t Number::digits = kDefaultDecimalDigits;

ostream& Number::ShowValue(ostream& out, const Bfdec& value, mode_enum mode, int digits, int base) {
    return out << bfdec_ftoa(NULL, &value.toBfdec(), (limb_t)BF_PREC_INF, (bf_flags_t)(BF_RNDZ | BF_FTOA_FORMAT_FREE));
    #if 0
    stringstream format;
    switch (base) {
        case 10:
            switch (mode) {
                case kStd:
                    format << "%." << digits << "R*g";
                    break;
                case kFix:
                    format << "%." << digits << "R*f";
                    break;
                case kSci:
                    format << "%." << digits << "R*e";
                    break;
            }
            return MprealFmtOutput(out, format.str(), value);
        case 16:
            if (mode == kStd) {
                return MprealOutputNBase(out, base, value);  // custom function, as none available in mpfr
            } else {
                format << "%." << digits << "R*a";  // C99 format
                return MprealFmtOutput(out, format.str(), value);
            }
        case 2:
            if (mode == kStd) {
                return MprealOutputNBase(out, base, value);  // custom function, as none available in mpfr
            } else {
                format << "%." << digits << "R*b";  // C99 format
                return MprealFmtOutput(out, format.str(), value);
            }
        default:
            // base other than 2, 10, 16
            return MprealOutputNBase(out, base, value);
    }
    #endif
}
