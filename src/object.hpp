#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <mpfr.h>
#include <string.h>

// definitions for objects
////
typedef enum {
    cmd_undef,
    cmd_number,/* floating value to put in stack */
    cmd_string,/* string value to put in stack */
    cmd_symbol,/* symbol value to put in stack */
    cmd_program,/* program */
    cmd_keyword,/* langage keyword */
    cmd_branch,/* langage branch keyword */
    cmd_max
} cmd_type_t;

class program;
class branch;

typedef void (program::*program_fn_t)(void);
typedef int (program::*branch_fn_t)(branch&);

// MPFR object
////
struct floating_t
{
    mpfr_t mpfr;

    void init(void* significand)
    {
        mpfr_custom_init(significand, MPFR_DEFAULT_PREC_BITS);
        mpfr_custom_init_set(mpfr, MPFR_ZERO_KIND, 0, s_mpfr_prec, significand);
    }

    void set_significand(void* significand)
    {
        mpfr->_mpfr_d = (mp_limb_t*)significand;
    }

    floating_t& operator=(const long int val)
    {
        mpfr_set_si(mpfr, val, s_mpfr_rnd);
    }

    floating_t& operator=(const unsigned long val)
    {
        mpfr_set_ui(mpfr, val, s_mpfr_rnd);
    }

    operator int()
    {
        return (int)mpfr_get_si(mpfr, s_mpfr_rnd);
    }
    
    bool operator>(const floating_t right)
    {
        return mpfr_cmp(mpfr, right.mpfr) > 0;
    }

    bool operator<(const floating_t right)
    {
        return mpfr_cmp(mpfr, right.mpfr) < 0;
    }

    // default precision in bits, precision length in bytes, rounding mode
    static mpfr_prec_t s_mpfr_prec;
    static unsigned int s_mpfr_prec_bytes;
    static mpfr_rnd_t s_mpfr_rnd;
    static const char* s_mpfr_rnd_str[5];
};

// object - a generic stack object
////
struct object
{
    // object type
    cmd_type_t _type;
    unsigned int _size;

    //
    unsigned int size() { return _size; }

    void show(FILE* stream = stdout);

    //
    static const char* s_cmd_type_string[cmd_max];
};

// stack objects derived from object
////
struct number : public object
{
    number() { _type = cmd_number; }
    floating_t _value;

    void init(void* significand)
    {
        _type = cmd_number;
        _representation = dec;
        _value.init(significand);
    }

    void copy(number& op)
    {
        _value = op._value;
        memcpy(_value.mpfr->_mpfr_d, op._value.mpfr->_mpfr_d, floating_t::s_mpfr_prec_bytes);
    }

    //
    void set(const floating_t& value)
    {
        _type = cmd_number;
        _value.mpfr->_mpfr_d = value.mpfr->_mpfr_d;
    }

    void set(long value)
    {
        _type = cmd_number;
        _value = value;
    }

    void set(unsigned long value)
    {
        _type = cmd_number;
        _value = value;
    }

    static unsigned int calc_size()
    {
        return (unsigned int)(sizeof(number)+floating_t::s_mpfr_prec_bytes);
    }

    //
    number operator=(const number& op)
    {
        number num;
        num.set((const floating_t&)op._value);
        return num;
    }

    // representation mode
    typedef enum {
        std,
        fix,
        sci
    } mode_enum;
    static mode_enum s_mode;
    
    enum {
        dec,
        hex,
        bin
    } _representation;

    // precision
    static int s_current_precision;
    static string s_mpfr_printf_format;
};

struct ostring : public object
{
    //
    void set(const char* value, unsigned int len)
    {
        _type = cmd_string;
        if (value != NULL)
        {
            if (len>0)
                (void)memcpy(_value, value, len);
            _value[len] = 0;
            _len = len;
        }
        else
            len = 0;
    }

    //
    unsigned int _len;
    char _value[0];
};

struct oprogram : public object
{
    //
    void set(const char* value, unsigned int len)
    {
        _type = cmd_program;
        if (value != NULL)
        {
            if (len>0)
                (void)memcpy(_value, value, len);
            _value[len] = 0;
            _len = len;
        }
        else
            len = 0;
    }

    //
    unsigned int _len;
    char _value[0];
};

struct symbol : public object
{
    //
    void set(const char* value, unsigned int len, bool auto_eval)
    {
        _type = cmd_symbol;
        _auto_eval = auto_eval;

        if (value != NULL)
        {
            if (len>0)
                (void)memcpy(_value, value, len);
            _value[len] = 0;
            _len = len;
        }
        else
            len = 0;
    }

    //
    bool _auto_eval;
    unsigned int _len;
    char _value[0];
};

struct keyword : public object
{
    //
    void set(program_fn_t fn, const char* value, unsigned int len)
    {
        _type = cmd_keyword;
        _fn = fn;
        if (value != NULL)
        {
            if (len>0)
                (void)memcpy(_value, value, len);
            _value[len] = 0;
            _len = len;
        }
        else
            len = 0;
    }

    //
    program_fn_t _fn;
    unsigned int _len;
    char _value[0];
};

struct branch : public object
{
    //
    void set(branch_fn_t fn, const char* value, unsigned int len)
    {
        _type = cmd_branch;
        _fn = fn;
        arg1 = -1;
        arg2 = -1;
        arg3 = -1;
        farg1 = NULL;
        farg2 = NULL;
        arg_bool = 0;
        if (value != NULL)
        {
            if (len>0)
                (void)memcpy(_value, value, len);
            _value[len] = 0;
            _len = len;
        }
        else
            len = 0;
    }

    // branch function
    branch_fn_t _fn;
    // args used by cmd_branch cmds
    int arg1, arg2, arg3;
    number *farg1, *farg2;
    bool arg_bool;
    unsigned int _len;
    char _value[0];
};

#endif
