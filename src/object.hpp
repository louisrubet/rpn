#ifndef OBJECT_HPP
#define OBJECT_HPP

#define MPFR_USE_NO_MACRO
#include <mpfr.h>
#include <mpreal.h>
using namespace mpfr;

#include <ostream>
#include <string>
#include <sstream>
using namespace std;

#include "mpreal-out.hpp"

// definitions for objects
///
typedef enum {
    cmd_undef,
    cmd_number,   // floating point number
    cmd_complex,  // complex, couple of floating point numbers
    cmd_string,   // string like "string"
    cmd_symbol,   // symbol like 'symbol'
    cmd_program,  // program like << instructions >>
    cmd_keyword,  // langage keyword
    cmd_branch,   // langage branch keyword
    cmd_max
} cmd_type_t;

class program;
class branch;

typedef void (program::*program_fn_t)(void);
typedef size_t (program::*branch_fn_t)(branch&);

/// @brief object - a generic stack object
///
struct object {
    object(cmd_type_t type = cmd_undef) : _type(type) {}
    virtual ~object() {}
    cmd_type_t _type;
    virtual object* clone() {
        object* o = new object();
        if (o != nullptr) *o = *this;
        return o;
    }

    virtual string name() { return string("object"); }
    virtual ostream& show(ostream& out) {
        out << "(" << name() << " - unknown representation)";
        return out;
    }
    friend ostream& operator<<(ostream& os, object* o) { return o->show(os); }

    unsigned int size() { return sizeof(*this); }
};

/// @brief stack objects derived from object
///
struct number : object {
    number() : object(cmd_number), base(10) {}
    number(const mpreal& value_, int base_ = 10) : object(cmd_number), base(base_), value(value_) {}
    number(long value_, int base_ = 10) : object(cmd_number), base(base_), value(value_) {}

    int base;
    mpreal value;

    virtual object* clone() { return new number(value, base); }
    virtual string name() { return string("number"); }
    virtual ostream& show(ostream& out) { return showValue(out, value, s_mode, s_digits, base); }

    // representation mode
    typedef enum { std, fix, sci } mode_enum;
    static mode_enum s_mode;

    // precision
    static int s_digits;

    // clang-format off
    static string _makeNumberFormat(mode_enum mode, int digits) {
        stringstream format;
        format<<"%."<<digits;
        switch(mode) {
            case std: format<<"R*g"; break;
            case fix: format<<"R*f"; break;
            case sci: format<<"R*e"; break;
        }
        return format.str();
    }
    // clang-format on

    static ostream& showValue(ostream& out, const mpreal& value, mode_enum mode, int digits, int base) {
        if (base == 10)
            return mpreal_output10base(out, _makeNumberFormat(s_mode, s_digits), value);
        else
            return mpreal_outputNbase(out, base, value);
    }
};

/// @brief stack objects derived from object
///
struct ocomplex : object {
    ocomplex() : object(cmd_complex), reBase(10), imBase(10) {}
    ocomplex(complex<mpreal>& value_, int reb = 10, int imb = 10) : object(cmd_complex), reBase(reb), imBase(imb) {
        value = value_;
    }
    ocomplex(mpreal& re_, mpreal& im_, int reb = 10, int imb = 10) : object(cmd_complex), reBase(reb), imBase(imb) {
        value.real(re_);
        value.imag(im_);
    }

    int reBase, imBase;
    complex<mpreal> value;

    virtual object* clone() { return new ocomplex(value, reBase, imBase); }
    virtual string name() { return string("complex"); }
    virtual ostream& show(ostream& out) {
        out << '(';
        number::showValue(out, value.real(), number::s_mode, number::s_digits, reBase);
        out << ',';
        number::showValue(out, value.imag(), number::s_mode, number::s_digits, imBase);
        return out << ')';
    }
};

/// @brief object string
///
struct ostring : object {
    ostring() : object(cmd_string) {}
    ostring(const string& value_) : object(cmd_string), value(value_) {}
    virtual object* clone() { return new ostring(value); }
    virtual string name() { return string("string"); }
    virtual ostream& show(ostream& out) { return out << "\"" << value << "\""; }
    string value;
};

/// @brief object program
///
struct oprogram : object {
    oprogram() : object(cmd_program) {}
    oprogram(const string& value_) : object(cmd_program), value(value_) {}
    virtual object* clone() { return new oprogram(value); }
    virtual string name() { return string("program"); }
    virtual ostream& show(ostream& out) { return out << "«" << value << "»"; }
    string value;
};

/// @brief object symbol
///
struct symbol : object {
    symbol(bool autoEval_ = true) : object(cmd_symbol), autoEval(autoEval_) {}
    symbol(const string& value_, bool autoEval_ = true) : object(cmd_symbol), value(value_), autoEval(autoEval_) {}
    virtual object* clone() { return new symbol(value, autoEval); }
    virtual string name() { return string("symbol"); }
    virtual ostream& show(ostream& out) { return out << "'" << value << "'"; }
    bool autoEval;
    string value;
};

/// @brief object keyword
///
struct keyword : object {
    keyword() : object(cmd_keyword) {}
    keyword(program_fn_t fn_, const string& value_) : object(cmd_keyword), fn(fn_), value(value_) {}
    virtual object* clone() { return new keyword(fn, value); }
    virtual string name() { return string("keyword"); }
    program_fn_t fn;
    string value;
};

/// @brief object branch
///
struct branch : object {
    branch() : object(cmd_branch) {}
    branch(branch_fn_t fn_, const string& value_) : object(cmd_branch) {
        fn = fn_;
        arg1 = (size_t)-1;
        arg2 = (size_t)-1;
        arg3 = (size_t)-1;
        arg_bool = 0;
        value = value_;
    }
    branch(branch& other) : object(cmd_branch) {
        fn = other.fn;
        arg1 = other.arg1;
        arg2 = other.arg2;
        arg3 = other.arg3;
        arg_bool = other.arg_bool;
        value = other.value;
    }
    virtual object* clone() { return new branch(*this); }
    virtual string name() { return string("branch"); }
    branch_fn_t fn;
    size_t arg1, arg2, arg3;
    mpreal firstIndex, lastIndex;
    bool arg_bool;
    string value;
};

#endif
