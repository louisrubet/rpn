#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <mpreal.h>
using namespace mpfr;

#include <ostream>
#include <iomanip>
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
    auto duplicate() {
        cout << "typeid(this).name()=" << typeid(this).name() << endl;
        return *new decltype(this);
    }
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
    number(repr_enum representation = dec) : object(cmd_number), _representation(representation) { _value = 0L; }
    number(mpreal& value, repr_enum representation = dec) : number(representation) { _value = value; }
    number(long value, repr_enum representation = dec) : number(representation) { _value = value; }
    number(unsigned long value, repr_enum representation = dec) : number(representation) { _value = value; }
    number(double value, repr_enum representation = dec) : number(representation) { _value = value; }

    repr_enum _representation;
    int _base;  // carefull: _base is used only if _representation = base

    mpreal _value;

    void init() {}

    void set(unsigned long value) { _value = value; }

    virtual string name() { return string("number"); }
    virtual void show(ostream& out) {
        if (_representation != number::dec) {
            object::show(out);
            return;
        }
        switch (number::s_mode) {
            case number::std:
                out.unsetf(ios::floatfield);
                out << setprecision(s_decimal_digits) << _value;
                break;
            case number::fix:
                out << fixed << setprecision(s_decimal_digits) << _value;
                break;
            case number::sci:
                out << scientific << setprecision(s_decimal_digits) << _value;
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
    static mpfr_prec_t s_mpfr_prec;
    static mpfr_rnd_t s_mpfr_rnd;
    static string s_mpfr_printf_format;
    static const char* s_mpfr_rnd_str[5];
};

/// @brief stack objects derived from object
///
struct ocomplex : object {
    number::repr_enum _representation;
    ocomplex() : object(cmd_complex) { init(); }

    mpreal _re;
    mpreal _im;

    // re and im float values are at the end of the object
    mpreal* re() { return &_re; }
    mpreal* im() { return &_im; }

    void init() {}

    virtual string name() { return string("complex"); }
    virtual void show(ostream& out) {
        if (_representation != number::dec) {
            object::show(out);
            return;
        }
        switch (number::s_mode) {
            case number::std:
                out.unsetf(ios::floatfield);
                out << setprecision(number::s_decimal_digits) << "(" << _re << ", " << _im << ")";
            case number::fix:
                out << fixed << setprecision(number::s_decimal_digits) << "(" << _re << ", " << _im << ")";
                break;
            case number::sci:
                out << scientific << setprecision(number::s_decimal_digits) << "(" << _re << ", " << _im << ")";
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
    ostring(const string& value) : object(cmd_string) { set(value); }
    ostring(const char* value) : object(cmd_string) { _value = string(value); }
    void set(const string& value) { _value = value; }
    virtual string name() { return string("complex"); }
    virtual void show(ostream& out) { out << "\"" << _value << "\""; }

    string _value;
};

/// @brief object program
///
struct oprogram : object {
    oprogram() : object(cmd_program) {}
    oprogram(const string& value) : object(cmd_program) { set(value); }
    void set(const string& value) { _value = value; }
    virtual string name() { return string("program"); }
    virtual void show(ostream& out) { out << "<< " << _value << " >>"; }
    string _value;
};

/// @brief object symbol
///
struct symbol : object {
    symbol(bool auto_eval = true) : object(cmd_symbol), _auto_eval(auto_eval) {}
    symbol(const string& value, bool auto_eval = true) : object(cmd_symbol) { set(value, auto_eval); }
    void set(string& value, bool auto_eval) {
        _value = value;
        _auto_eval = auto_eval;
    }
    void set(const string& value, bool auto_eval) {
        _value = value;
        _auto_eval = auto_eval;
    }
    virtual string name() { return string("symbol"); }
    virtual void show(ostream& out) { out << "'" << _value << "'"; }
    bool _auto_eval;
    string _value;
};

/// @brief object keyword
///
struct keyword : object {
    keyword() : object(cmd_keyword) {}
    keyword(program_fn_t fn, const string& value) : object(cmd_keyword) { set(fn, value); }
    void set(program_fn_t fn, const string& value) {
        _fn = fn;
        _value = value;
    }
    virtual string name() { return string("keyword"); }
    program_fn_t _fn;
    string _value;
};

/// @brief object branch
///
struct branch : object {
    branch() : object(cmd_branch) {}
    branch(branch_fn_t fn, const string& value) : object(cmd_branch) { set(fn, value); }
    virtual string name() { return string("branch"); }
    //
    void set(branch_fn_t fn, const string& value) {
        _fn = fn;
        arg1 = -1;
        arg2 = -1;
        arg3 = -1;
        farg1 = NULL;
        farg2 = NULL;
        arg_bool = 0;
        _value = value;
    }
    branch_fn_t _fn;
    int arg1, arg2, arg3;
    number *farg1, *farg2;
    bool arg_bool;
    string _value;
};

#endif
