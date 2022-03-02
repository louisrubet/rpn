// Copyright (c) 2014-2022 Louis Rubet

#include <iostream>
#include <string>
#include <utility>
using std::cout, std::string, std::pair;

#include "linenoise.h"
#include "program.h"
#include "version.h"

#define ATTR_BOLD "\33[1m"
#define ATTR_OFF "\33[0m"

#define MPFR_ROUND                                                                                                     \
    {"nearest (even)", MPFR_RNDN}, {"toward zero", MPFR_RNDZ}, {"toward +inf", MPFR_RNDU}, {"toward -inf", MPFR_RNDD}, \
        {"away from zero", MPFR_RNDA}, {"faithful rounding", MPFR_RNDF}, {                                             \
        "nearest (away from zero)", MPFR_RNDNA                                                                         \
    }

static const char _description[] =
    ATTR_BOLD "R" ATTR_OFF "everse " ATTR_BOLD "P" ATTR_OFF "olish " ATTR_BOLD "N" ATTR_OFF "otation CLI calculator";

static const char _syntax[] = ATTR_BOLD "Syntax" ATTR_OFF ": rpn [command]\nwith optional command = list of commands";

static const char _uname[] = ATTR_BOLD "rpn " RPN_VERSION ", (c) 2022 <louis@rubet.fr>" ATTR_OFF;

static const char _welcome[] = ATTR_BOLD "rpn " RPN_VERSION ATTR_OFF "\nType h or help for more information.";

/// @brief quit keyword implementation
///
void program::RpnQuit() { ERROR_CONTEXT(kGoodbye); }

/// @brief nop keyword implementation
/// the result is written on stdout
///
void program::RpnHelp() {
    // software name
    cout << endl << _uname << endl;

    // _description
    cout << _description << endl << endl;

    // _syntax
    cout << _syntax << endl;

    // keywords
    for (auto& kw : keywords_)
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
    switch (Number::mode) {
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
    cout << " with " << Number::digits << " digits after the decimal point" << endl;
    cout << "Current floating point precision is " << static_cast<int>(mpreal::get_default_prec()) << " bits" << endl;
    vector<pair<string, mpfr_rnd_t>> rnd{MPFR_ROUND};
    for (auto& rn : rnd)
        if (rn.second == mpreal::get_default_rnd()) {
            cout << "Current rounding mode is '" << rn.first << '\'' << endl;
            break;
        }
    cout << endl;
}

/// @brief welcome string
///
void program::Welcome() { cout << _welcome << endl; }

/// @brief whether a printed precision is in the precision min/max
///
/// @param precision the precision in digits
/// @return true the precision is good
/// @return false the precision is not good
///
static bool check_decimal_digits(int precision) { return precision >= 0; }

/// @brief std keyword implementation
///
void program::RpnStd() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kNumber);

    int digits = static_cast<int>(stack_.value<Number>(0).toLong());

    if (check_decimal_digits(digits)) {
        // set mode, decimal digits and print format
        Number::mode = Number::kStd;
        Number::digits = digits;
        stack_.pop();
    } else {
        ERROR_CONTEXT(kOutOfRange);
    }
}

/// @brief fix keyword implementation
///
void program::RpnFix() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kNumber);

    int digits = static_cast<int>(stack_.value<Number>(0).toLong());

    if (check_decimal_digits(digits)) {
        // set mode, decimal digits and print format
        Number::mode = Number::kFix;
        Number::digits = digits;
        stack_.pop();
    } else {
        ERROR_CONTEXT(kOutOfRange);
    }
}

/// @brief sci keyword implementation
///
void program::RpnSci() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kNumber);

    int digits = static_cast<int>(stack_.value<Number>(0).toLong());

    if (check_decimal_digits(digits)) {
        // set mode, decimal digits and print format
        Number::mode = Number::kSci;
        Number::digits = digits;
        stack_.pop();
    } else {
        ERROR_CONTEXT(kOutOfRange);
    }
}

/// @brief _version keyword implementation
///
void program::RpnVersion() { stack_.push_front(new String(RPN_VERSION)); }

/// @brief _uname keyword implementation
///
void program::RpnUname() { stack_.push_front(new String(_uname)); }

/// @brief history keyword implementation
///
void program::RpnHistory() {
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
void program::RpnType() {
    MIN_ARGUMENTS(1);
    stack_.push(new String(stack_.at(0)->Name()));
    stack_.erase(1);
}

/// @brief default keyword implementation
///
void program::RpnDefault() { program::ApplyDefault(); }

/// @brief prec keyword implementation
///
void program::RpnPrecision() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kNumber);

    // set precision
    int prec = static_cast<int>(stack_.value<Number>(0).toLong());
    if (prec >= MPFR_PREC_MIN && prec <= MPFR_PREC_MAX) {
        mpreal::set_default_prec(prec);

        // modify digits seen by user if std mode
        if (Number::mode == Number::kStd) {
            // calc max nb of digits user can see with the current bit precision
            Number::digits = mpfr::bits2digits(mpreal::get_default_prec());
        }
        stack_.pop();
    } else {
        ERROR_CONTEXT(kOutOfRange);
    }
}

/// @brief round keyword implementation
///
void program::RpnRound() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kString);

    map<string, mpfr_rnd_t> matchRound{MPFR_ROUND};

    auto found = matchRound.find(stack_.value<String>(0));
    if (found != matchRound.end())
        mpreal::set_default_rnd(found->second);
    else
        ERROR_CONTEXT(kOutOfRange);
    stack_.pop();
}
