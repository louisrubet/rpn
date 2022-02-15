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
    cout << endl << ATTR_BOLD << uname << ATTR_OFF << endl;

    // description
    cout << description << endl << endl;

    // syntax
    cout << syntax << endl;

    // keywords
    unsigned int i = 0;
    while (s_keywords[i].type != cmd_max) {
        if (s_keywords[i].comment.size() != 0) {
            // titles in bold
            if (s_keywords[i].type == cmd_undef) cout << ATTR_BOLD;
            // show title or keyword + comment
            cout << s_keywords[i].name << '\t' << s_keywords[i].comment << endl;
            if (s_keywords[i].type == cmd_undef) cout << ATTR_OFF;
        }
        i++;
    }
    cout << endl;

    // show mode
    cout << "Current float mode is ";
    switch (number::s_mode) {
        case number::std:
            cout << "'std'";
            break;
        case number::fix:
            cout << "'fix'";
            break;
        case number::sci:
            cout << "'sci'";
            break;
        default:
            cout << "unknown";
            break;
    }

    // bits precision, decimal digits and rounding mode
    cout << " with " << number::s_digits << " digits after the decimal point" << endl;
    cout << "Current floating point precision is " << (int)mpreal::get_default_prec() << " bits" << endl;
    struct RndStrings {
        string name;
        mp_rnd_t rnd;
    };
    vector<RndStrings> rndStrings{MPFR_ROUND_STRINGS};
    for (RndStrings r : rndStrings)
        if (r.rnd == mpreal::get_default_rnd()) {
            cout << "Current rounding mode is " << r.name << endl;
            break;
        }
    cout << endl << endl;
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

    double digits = double(_stack->value<number>(0));

    if (check_decimal_digits(digits)) {
        // set mode, decimal digits and print format
        number::s_mode = number::std;
        number::s_digits = (int)digits;
        _stack->pop();
    } else
        ERR_CONTEXT(ret_out_of_range);
}

/// @brief fix keyword implementation
///
void program::rpn_fix() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    double digits = double(_stack->value<number>(0));

    if (check_decimal_digits(digits)) {
        // set mode, decimal digits and print format
        number::s_mode = number::fix;
        number::s_digits = (int)digits;
        _stack->pop();
    } else
        ERR_CONTEXT(ret_out_of_range);
}

/// @brief sci keyword implementation
///
void program::rpn_sci() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    double digits = double(_stack->value<number>(0));

    if (check_decimal_digits(digits)) {
        // set mode, decimal digits and print format
        number::s_mode = number::sci;
        number::s_digits = (int)digits;
        _stack->pop();
    } else
        ERR_CONTEXT(ret_out_of_range);
}

/// @brief version keyword implementation
///
void program::rpn_version() { _stack->push_front(new ostring(version)); }

/// @brief uname keyword implementation
///
void program::rpn_uname() { _stack->push_front(new ostring(uname)); }

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
    _stack->push(new ostring(_stack->at(0)->name()));
    _stack->erase(1);
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
    unsigned long prec = _stack->value<number>(0).toULong();
    if (prec >= (unsigned long)MPFR_PREC_MIN && prec <= (unsigned long)MPFR_PREC_MAX) {
        mpreal::set_default_prec(prec);

        // modify digits seen by user if std mode
        if (number::s_mode == number::std) {
            // calc max nb of digits user can see with the current bit precision
            number::s_digits = bits2digits(mpreal::get_default_prec());
        }
        _stack->pop();
    } else
        ERR_CONTEXT(ret_out_of_range);
}

/// @brief round keyword implementation
///
void program::rpn_round() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_string);

    map<string, mpfr_rnd_t> matchRound{MPFR_ROUND_STRINGS};

    auto found = matchRound.find(_stack->value<ostring>(0));
    if (found != matchRound.end())
        mpreal::set_default_rnd(found->second);
    else
        ERR_CONTEXT(ret_out_of_range);
    _stack->pop();
}
