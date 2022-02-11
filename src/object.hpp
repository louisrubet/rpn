#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <mpreal.h>
using namespace mpfr;

#include <complex>
#include <iomanip>
#include <ostream>
using namespace std;

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
typedef int (program::*branch_fn_t)(branch&);

/// @brief object - a generic stack object
///
struct object {
    object(cmd_type_t type = cmd_undef) : _type(type) {}
    // object type
    cmd_type_t _type;
    //
    auto duplicate() { return *new decltype(this); }
    virtual string name() { return string("object"); }
    virtual void show(ostream& out) { out << "(" << name() << " - unknown representation)"; }
    unsigned int size() { return sizeof(*this); }

    //
    static const char* s_cmd_type_string[cmd_max];
};

/// @brief stack objects derived from object
///
struct number : object {
    typedef enum { dec, hex, bin, base } repr_enum;
    number(repr_enum representation = dec) : object(cmd_number), _representation(representation) { value = 0L; }
    number(mpreal& value_, repr_enum representation = dec) : number(representation) { value = value_; }
    number(long value_, repr_enum representation = dec) : number(representation) { value = value_; }
    number(unsigned long value_, repr_enum representation = dec) : number(representation) { value = value_; }
    number(double value_, repr_enum representation = dec) : number(representation) { value = value_; }

    repr_enum _representation;
    int _base;  // carefull: _base is used only if _representation = base

    mpreal value;

    void set(unsigned long value_) { value = value_; }

    virtual string name() { return string("number"); }
    virtual void show(ostream& out) {
        if (_representation != number::dec) {
            object::show(out);
            return;
        }
        switch (number::s_mode) {
            case number::std:
                out.unsetf(ios::floatfield);
                out << setprecision(s_decimal_digits) << value;
                break;
            case number::fix:
                out << fixed << setprecision(s_decimal_digits) << value;
                break;
            case number::sci:
                out << scientific << setprecision(s_decimal_digits) << value;
                break;
            default:
                object::show(out);
                break;
        }
    }

    // representation mode
    typedef enum { std, fix, sci } mode_enum;
    static mode_enum s_mode;

    // precision
    static int s_decimal_digits;
};

/// @brief stack objects derived from object
///
struct ocomplex : object {
    number::repr_enum _representation;
    ocomplex() : object(cmd_complex) {}
    ocomplex(mpreal& re_, mpreal& im_) : object(cmd_complex) {
        value.real(re_);
        value.imag(im_);
    }
    complex<mpreal> value;
    virtual string name() { return string("complex"); }
    virtual void show(ostream& out) {
        if (_representation != number::dec) {
            object::show(out);
            return;
        }
        switch (number::s_mode) {
            case number::std:
                out.unsetf(ios::floatfield);
                out << setprecision(number::s_decimal_digits) << "(" << value.real() << "," << value.imag() << ")";
                break;
            case number::fix:
                out << fixed << setprecision(number::s_decimal_digits) << "(" << value.real() << "," << value.imag()
                    << ")";
                break;
            case number::sci:
                out << scientific << setprecision(number::s_decimal_digits) << "(" << value.real() << ","
                    << value.imag() << ")";
                break;
            default:
                object::show(out);
                break;
        }
    }
};

/// @brief object string
///
struct ostring : object {
    ostring() : object(cmd_string) {}
    ostring(const string& value_) : object(cmd_string) { set(value_); }
    ostring(const char* value_) : object(cmd_string) { value = string(value_); }
    void set(const string& value_) { value = value_; }
    virtual string name() { return string("complex"); }
    virtual void show(ostream& out) { out << "\"" << value << "\""; }
    string value;
};

/// @brief object program
///
struct oprogram : object {
    oprogram() : object(cmd_program) {}
    oprogram(const string& value_) : object(cmd_program) { set(value_); }
    void set(const string& value_) { value = value_; }
    virtual string name() { return string("program"); }
    virtual void show(ostream& out) { out << "«" << value << "»"; }
    string value;
};

/// @brief object symbol
///
struct symbol : object {
    symbol(bool auto_eval_ = true) : object(cmd_symbol), auto_eval(auto_eval_) {}
    symbol(const string& value_, bool auto_eval_ = true) : object(cmd_symbol) { set(value_, auto_eval_); }
    void set(string& value_, bool auto_eval_) {
        value = value_;
        auto_eval = auto_eval_;
    }
    void set(const string& value_, bool auto_eval_) {
        value = value_;
        auto_eval = auto_eval_;
    }
    virtual string name() { return string("symbol"); }
    virtual void show(ostream& out) { out << "'" << value << "'"; }
    bool auto_eval;
    string value;
};

/// @brief object keyword
///
struct keyword : object {
    keyword() : object(cmd_keyword) {}
    keyword(program_fn_t fn_, const string& value_) : object(cmd_keyword) { set(fn_, value_); }
    void set(program_fn_t fn_, const string& value_) {
        fn = fn_;
        value = value_;
    }
    virtual string name() { return string("keyword"); }
    program_fn_t fn;
    string value;
};

/// @brief object branch
///
struct branch : object {
    branch() : object(cmd_branch) {}
    branch(branch_fn_t fn_, const string& value_) : object(cmd_branch) { set(fn_, value_); }
    virtual string name() { return string("branch"); }
    //
    void set(branch_fn_t fn_, const string& value_) {
        fn = fn_;
        arg1 = -1;
        arg2 = -1;
        arg3 = -1;
        farg1 = NULL;
        farg2 = NULL;
        arg_bool = 0;
        value = value_;
    }
    branch_fn_t fn;
    int arg1, arg2, arg3;
    number *farg1, *farg2;
    bool arg_bool;
    string value;
};

#endif
