// Copyright (c) 2014-2022 Louis Rubet

#ifndef SRC_OBJECT_HPP_
#define SRC_OBJECT_HPP_

#include <mpreal.h>
using mpfr::mpreal;

#include <ostream>
#include <string>
#include <sstream>
#include <complex>
using std::ostream;
using std::string;
using std::stringstream;
using std::complex;

#include "mpreal-out.hpp"

// definitions for objects
///
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
    ret_test_failed
} ret_value;

typedef enum {
    cmd_undef,
    cmd_number,   // floating point number
    cmd_complex,  // complex, couple of floating point numbers
    cmd_string,   // "string"
    cmd_symbol,   // 'symbol'
    cmd_program,  // << instructions >> «instructions»
    cmd_keyword,  // langage (reserved) keyword
    cmd_branch,   // langage (reserved) branch keyword
    cmd_max
} cmd_type_t;

class program;
class Branch;

typedef void (program::*program_fn_t)(void);
typedef size_t (program::*branch_fn_t)(Branch&);

/// @brief Object - a generic stack object
///
struct Object {
    explicit Object(cmd_type_t type = cmd_undef) : _type(type) {}
    virtual ~Object() {}
    cmd_type_t _type;
    virtual Object* clone() {
        Object* o = new Object();
        if (o != nullptr) *o = *this;
        return o;
    }

    virtual string name() { return string("Object"); }
    virtual ostream& show(ostream& out) {
        out << "(" << name() << " - unknown representation)";
        return out;
    }
    friend ostream& operator<<(ostream& os, Object* o) { return o->show(os); }

    unsigned int size() { return sizeof(*this); }
};

/// @brief stack objects derived from Object
///
struct Number : Object {
    Number() : Object(cmd_number), base(10) {}
    explicit Number(const mpreal& value_, int base_ = 10) : Object(cmd_number), base(base_), value(value_) {}
    explicit Number(int value_, int base_ = 10) : Object(cmd_number), base(base_), value(value_) {}

    int base;
    mpreal value;

    virtual Object* clone() { return new Number(value, base); }
    virtual string name() { return string("number"); }
    virtual ostream& show(ostream& out) { return showValue(out, value, s_mode, s_digits, base); }

    // representation mode
    typedef enum { std, fix, sci } mode_enum;
    static mode_enum s_mode;
    static constexpr mode_enum DEFAULT_MODE = Number::std;

    // precision
    static constexpr mpfr_prec_t MPFR_DEFAULT_PREC_BITS = 128;
    static constexpr int DEFAULT_DECIMAL_DIGITS = 38;
    static int s_digits;

    // clang-format off
    static string _makeNumberFormat(mode_enum mode, int digits) {
        stringstream format;
        format << "%." << digits;
        switch ( mode ) {
            case std: format << "R*g"; break;
            case fix: format << "R*f"; break;
            case sci: format << "R*e"; break;
        }
        return format.str();
    }
    // clang-format on

    static ostream& showValue(ostream& out, const mpreal& value, mode_enum mode, int digits, int base) {
        if (base == 10)
            return MprealOutput10Base(out, _makeNumberFormat(s_mode, s_digits), value);
        else
            return MprealOutputNBase(out, base, value);
    }
};

/// @brief stack objects inheriting Object
///
struct Complex : Object {
    Complex() : Object(cmd_complex), reBase(10), imBase(10) {}
    explicit Complex(complex<mpreal>& value_, int reb = 10, int imb = 10)
        : Object(cmd_complex), reBase(reb), imBase(imb) {
        value = value_;
    }
    explicit Complex(mpreal& re_, mpreal& im_, int reb = 10, int imb = 10)
        : Object(cmd_complex), reBase(reb), imBase(imb) {
        value.real(re_);
        value.imag(im_);
    }

    int reBase, imBase;
    complex<mpreal> value;

    virtual Object* clone() { return new Complex(value, reBase, imBase); }
    virtual string name() { return string("complex"); }
    virtual ostream& show(ostream& out) {
        out << '(';
        Number::showValue(out, value.real(), Number::s_mode, Number::s_digits, reBase);
        out << ',';
        Number::showValue(out, value.imag(), Number::s_mode, Number::s_digits, imBase);
        return out << ')';
    }
};

struct String : Object {
    String() : Object(cmd_string) {}
    explicit String(const string& value_) : Object(cmd_string), value(value_) {}
    virtual Object* clone() { return new String(value); }
    virtual string name() { return string("string"); }
    virtual ostream& show(ostream& out) { return out << "\"" << value << "\""; }
    string value;
};

struct Program : Object {
    Program() : Object(cmd_program) {}
    explicit Program(const string& value_) : Object(cmd_program), value(value_) {}
    virtual Object* clone() { return new Program(value); }
    virtual string name() { return string("program"); }
    virtual ostream& show(ostream& out) { return out << "«" << value << "»"; }
    string value;
};

struct Symbol : Object {
    explicit Symbol(bool autoEval_ = true) : Object(cmd_symbol), autoEval(autoEval_) {}
    explicit Symbol(const string& value_, bool autoEval_ = true)
        : Object(cmd_symbol), value(value_), autoEval(autoEval_) {}
    virtual Object* clone() { return new Symbol(value, autoEval); }
    virtual string name() { return string("symbol"); }
    virtual ostream& show(ostream& out) { return out << "'" << value << "'"; }
    bool autoEval;
    string value;
};

struct Keyword : Object {
    Keyword() : Object(cmd_keyword) {}
    explicit Keyword(program_fn_t fn_, const string& value_) : Object(cmd_keyword), fn(fn_), value(value_) {}
    virtual Object* clone() { return new Keyword(fn, value); }
    virtual string name() { return string("keyword"); }
    program_fn_t fn;
    string value;
};

struct Branch : Object {
    Branch() : Object(cmd_branch) {}
    explicit Branch(branch_fn_t fn_, const string& value_) : Object(cmd_branch) {
        fn = fn_;
        arg1 = static_cast<size_t>(-1);
        arg2 = static_cast<size_t>(-1);
        arg3 = static_cast<size_t>(-1);
        arg_bool = 0;
        value = value_;
    }
    explicit Branch(Branch& other) : Object(cmd_branch) {
        fn = other.fn;
        arg1 = other.arg1;
        arg2 = other.arg2;
        arg3 = other.arg3;
        arg_bool = other.arg_bool;
        value = other.value;
    }
    virtual Object* clone() { return new Branch(*this); }
    virtual string name() { return string("branch"); }
    branch_fn_t fn;
    size_t arg1, arg2, arg3;
    mpreal firstIndex, lastIndex;
    bool arg_bool;
    string value;
};

#endif  // SRC_OBJECT_HPP_
