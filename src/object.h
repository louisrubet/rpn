// Copyright (c) 2014-2022 Louis Rubet

#ifndef SRC_OBJECT_H_
#define SRC_OBJECT_H_

#include <mpreal.h>
using mpfr::mpreal;

#include <complex>
#include <ostream>
#include <sstream>
#include <string>
using std::complex, std::ostream, std::string, std::stringstream;

#include "mpreal-out.h"

// definitions for objects
///
typedef enum {
    kOk,
    kUnknownError,
    kMissingOperand,
    kBadOperandType,
    kOutOfRange,
    kUnknownVariable,
    kInternalError,
    kDeadlyError,
    kGoodbye,
    kNotImplemented,
    kNop,
    kSyntaxError,
    kDivByZero,
    kRuntimeError,
    kAbortCurrentEntry,
    kOutOfMemory,
    kBadValue,
    kTestFailed
} RetValue;

typedef enum {
    kUndef,
    kNumber,   // 3.1416, 1e-1234, 0x12ab, 2b110, 50ba12
    kComplex,  // (1,2)
    kString,   // "string"
    kSymbol,   // 'symbol'
    kProgram,  // << instructions >> «instructions»
    kKeyword,  // langage (reserved) keyword (rot, dup, swap ..)
    kBranch    // langage (reserved) branch keyword (for, if, then ..)
} ObjectType;

class program;
class Branch;

typedef void (program::*program_fn_t)(void);
typedef size_t (program::*branch_fn_t)(Branch&);

/// @brief Object - a generic stack object
///
struct Object {
    explicit Object(ObjectType type = kUndef) : _type(type) {}
    virtual ~Object() {}
    ObjectType _type;
    virtual Object* Clone() {
        Object* o = new Object();
        if (o != nullptr) *o = *this;
        return o;
    }

    virtual string Name() { return string("Object"); }
    virtual ostream& Show(ostream& out) {
        out << "(" << Name() << " - unknown representation)";
        return out;
    }
    friend ostream& operator<<(ostream& os, Object* o) { return o->Show(os); }
};

/// @brief stack objects derived from Object
///
struct Number : Object {
    Number() : Object(kNumber), base(10) {}
    explicit Number(const mpreal& value__, int base__ = 10) : Object(kNumber), base(base__), value(value__) {}
    explicit Number(int value__, int base__ = 10) : Object(kNumber), base(base__), value(value__) {}

    int base;
    mpreal value;

    virtual Object* Clone() { return new Number(value, base); }
    virtual string Name() { return string("number"); }
    virtual ostream& Show(ostream& out) { return ShowValue(out, value, mode, digits, base); }

    // representation mode
    typedef enum { kStd, kFix, kSci } mode_enum;
    static mode_enum mode;
    static constexpr mode_enum kDefaultMode = Number::kStd;

    // precision
    static constexpr mpfr_prec_t kMpfrDefaultPrecBits = 128;
    static constexpr int kDefaultDecimalDigits = 38;
    static int digits;

    // clang-format on
    static ostream& ShowValue(ostream& out, const mpreal& value, mode_enum mode, int digits, int base);
};

/// @brief stack objects inheriting Object
///
struct Complex : Object {
    Complex() : Object(kComplex), re_base(10), im_base(10) {}
    explicit Complex(complex<mpreal>& value__, int re_base__ = 10, int im_base__ = 10)
        : Object(kComplex), re_base(re_base__), im_base(im_base__) {
        value = value__;
    }
    explicit Complex(mpreal& re__, mpreal& im__, int re_base__ = 10, int im_base__ = 10)
        : Object(kComplex), re_base(re_base__), im_base(im_base__) {
        value.real(re__);
        value.imag(im__);
    }

    int re_base, im_base;
    complex<mpreal> value;

    virtual Object* Clone() { return new Complex(value, re_base, im_base); }
    virtual string Name() { return string("complex"); }
    virtual ostream& Show(ostream& out) {
        out << '(';
        Number::ShowValue(out, value.real(), Number::mode, Number::digits, re_base);
        out << ',';
        Number::ShowValue(out, value.imag(), Number::mode, Number::digits, im_base);
        return out << ')';
    }
};

struct String : Object {
    String() : Object(kString) {}
    explicit String(const string& value__) : Object(kString), value(value__) {}
    virtual Object* Clone() { return new String(value); }
    virtual string Name() { return string("string"); }
    virtual ostream& Show(ostream& out) { return out << "\"" << value << "\""; }
    string value;
};

struct Program : Object {
    Program() : Object(kProgram) {}
    explicit Program(const string& value__) : Object(kProgram), value(value__) {}
    virtual Object* Clone() { return new Program(value); }
    virtual string Name() { return string("program"); }
    virtual ostream& Show(ostream& out) { return out << "«" << value << "»"; }
    string value;
};

struct Symbol : Object {
    explicit Symbol(bool auto_eval__ = true) : Object(kSymbol), auto_eval(auto_eval__) {}
    explicit Symbol(const string& value__, bool auto_eval__ = true)
        : Object(kSymbol), value(value__), auto_eval(auto_eval__) {}
    virtual Object* Clone() { return new Symbol(value, auto_eval); }
    virtual string Name() { return string("symbol"); }
    virtual ostream& Show(ostream& out) { return out << "'" << value << "'"; }
    string value;
    bool auto_eval;
};

struct Keyword : Object {
    Keyword() : Object(kKeyword) {}
    explicit Keyword(program_fn_t fn__, const string& value__) : Object(kKeyword), fn(fn__), value(value__) {}
    virtual Object* Clone() { return new Keyword(fn, value); }
    virtual string Name() { return string("keyword"); }
    program_fn_t fn;
    string value;
};

struct Branch : Object {
    Branch() : Object(kBranch) {}
    explicit Branch(branch_fn_t fn__, const string& value__) : Object(kBranch), fn(fn__), value(value__) {
        arg1 = static_cast<size_t>(-1);
        arg2 = static_cast<size_t>(-1);
        arg3 = static_cast<size_t>(-1);
        arg_bool = 0;
    }
    explicit Branch(Branch& other) : Object(kBranch) {
        fn = other.fn;
        arg1 = other.arg1;
        arg2 = other.arg2;
        arg3 = other.arg3;
        first_index = other.first_index;
        last_index = other.last_index;
        arg_bool = other.arg_bool;
        value = other.value;
    }
    virtual Object* Clone() { return new Branch(*this); }
    virtual string Name() { return string("branch"); }
    branch_fn_t fn;
    size_t arg1, arg2, arg3;
    mpreal first_index, last_index;
    bool arg_bool;
    string value;
};

#endif  // SRC_OBJECT_H_
