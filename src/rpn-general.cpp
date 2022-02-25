// Copyright (c) 2014-2022 Louis Rubet

#include <iostream>
#include <string>
#include <utility>
using std::cout;
using std::string;
using std::pair;

#include "linenoise.h"
#include "program.hpp"
#include "version.h"

// description
#define ATTR_BOLD "\33[1m"
#define ATTR_OFF "\33[0m"

#define XSTR(a) STR(a)
#define STR(a) #a
static const char _description[] = ATTR_BOLD
    "R" ATTR_OFF "everse " ATTR_BOLD "P" ATTR_OFF "olish " ATTR_BOLD "N" ATTR_OFF "otation language\n\nusing " ATTR_BOLD
    "GMP" ATTR_OFF " v" XSTR(__GNU_MP_VERSION) "." XSTR(__GNU_MP_VERSION_MINOR) "." XSTR(
        __GNU_MP_VERSION_PATCHLEVEL) " under GNU LGPL\n" ATTR_BOLD "MPFR" ATTR_OFF " v" MPFR_VERSION_STRING
                                     " under GNU LGPL\nand " ATTR_BOLD "linenoise-ng" ATTR_OFF " v" LINENOISE_VERSION
                                     " under BSD\n";

static const char _syntax[] = ATTR_BOLD "Syntax" ATTR_OFF ": rpn [command]\nwith optional command = list of commands";

#define MPFR_ROUND                                                                                                     \
    {"nearest (even)", MPFR_RNDN}, {"toward zero", MPFR_RNDZ}, {"toward +inf", MPFR_RNDU}, {"toward -inf", MPFR_RNDD}, \
        {"away from zero", MPFR_RNDA}, {"faithful rounding", MPFR_RNDF}, {                                             \
        "nearest (away from zero)", MPFR_RNDNA                                                                         \
    }

/// @brief nop keyword implementation
///
void program::rpn_nop() {
    // nop
}

/// @brief quit keyword implementation
///
void program::rpn_good_bye() { setErrorContext(kGoodbye); }

/// @brief nop keyword implementation
/// the result is written on stdout
///
void program::rpn_help() {
    // software name
    cout << endl << ATTR_BOLD << RPN_UNAME << ATTR_OFF << endl;

    // _description
    cout << _description << endl << endl;

    // _syntax
    cout << _syntax << endl;

    // keywords
    unsigned int i = 0;
    for (auto& kw : _keywords)
        if (!kw.comment.empty()) {
            // titles in bold
            if (kw.type == kUndef) cout << ATTR_BOLD;
            // show title or keyword + comment
            cout << kw.name << '\t' << kw.comment << endl;
            if (kw.type == kUndef) cout << ATTR_OFF;
        }
    cout << endl;

    // show mode
    cout << "Current float mode is ";
    switch (Number::s_mode) {
        case Number::kStd:
            cout << "'std'";
            break;
        case Number::kFix:
            cout << "'fix'";
            break;
        case Number::kSci:
            cout << "'sci'";
            break;
        default:
            cout << "unknown";
            break;
    }

    // bits precision, decimal digits and rounding mode
    cout << " with " << Number::s_digits << " digits after the decimal point" << endl;
    cout << "Current floating point precision is " << static_cast<int>(mpreal::get_default_prec()) << " bits" << endl;
    vector<pair<string, mpfr_rnd_t>> rnd{MPFR_ROUND};
    for (auto& rn : rnd)
        if (rn.second == mpreal::get_default_rnd()) {
            cout << "Current rounding mode is '" << rn.first << '\'' << endl;
            break;
        }
    cout << endl << endl;
}

/// @brief whether a printed precision is in the precision min/max
///
/// @param precision the precision in digits
/// @return true the precision is good
/// @return false the precision is not good
///
static bool check_decimal_digits(int precision) { return precision >= 0; }

/// @brief std keyword implementation
///
void program::rpn_std() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kNumber);

    int digits = static_cast<int>(_stack.value<Number>(0).toLong());

    if (check_decimal_digits(digits)) {
        // set mode, decimal digits and print format
        Number::s_mode = Number::kStd;
        Number::s_digits = digits;
        _stack.pop();
    } else {
        setErrorContext(kOutOfRange);
    }
}

/// @brief fix keyword implementation
///
void program::rpn_fix() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kNumber);

    int digits = static_cast<int>(_stack.value<Number>(0).toLong());

    if (check_decimal_digits(digits)) {
        // set mode, decimal digits and print format
        Number::s_mode = Number::kFix;
        Number::s_digits = digits;
        _stack.pop();
    } else {
        setErrorContext(kOutOfRange);
    }
}

/// @brief sci keyword implementation
///
void program::rpn_sci() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kNumber);

    int digits = static_cast<int>(_stack.value<Number>(0).toLong());

    if (check_decimal_digits(digits)) {
        // set mode, decimal digits and print format
        Number::s_mode = Number::kSci;
        Number::s_digits = digits;
        _stack.pop();
    } else {
        setErrorContext(kOutOfRange);
    }
}

/// @brief _version keyword implementation
///
void program::rpn_version() { _stack.push_front(new String(RPN_VERSION)); }

/// @brief _uname keyword implementation
///
void program::rpn_uname() { _stack.push_front(new String(RPN_UNAME)); }

/// @brief history keyword implementation
///
void program::rpn_history() {
    // see command history on stdout
    int index = 0;
    char* line = linenoiseHistoryLine(index);
    while (line != nullptr) {
        cout << line << endl;
        free(line);
        line = linenoiseHistoryLine(++index);
    }
}

/// @brief type keyword implementation
///
void program::rpn_type() {
    MIN_ARGUMENTS(1);
    _stack.push(new String(_stack.at(0)->name()));
    _stack.erase(1);
}

/// @brief default keyword implementation
///
void program::rpn_default() { program::apply_default(); }

/// @brief prec keyword implementation
///
void program::rpn_precision() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kNumber);

    // set precision
    int prec = static_cast<int>(_stack.value<Number>(0).toLong());
    if (prec >= MPFR_PREC_MIN && prec <= MPFR_PREC_MAX) {
        mpreal::set_default_prec(prec);

        // modify digits seen by user if std mode
        if (Number::s_mode == Number::kStd) {
            // calc max nb of digits user can see with the current bit precision
            Number::s_digits = mpfr::bits2digits(mpreal::get_default_prec());
        }
        _stack.pop();
    } else {
        setErrorContext(kOutOfRange);
    }
}

/// @brief round keyword implementation
///
void program::rpn_round() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kString);

    map<string, mpfr_rnd_t> matchRound{MPFR_ROUND};

    auto found = matchRound.find(_stack.value<String>(0));
    if (found != matchRound.end())
        mpreal::set_default_rnd(found->second);
    else
        setErrorContext(kOutOfRange);
    _stack.pop();
}
