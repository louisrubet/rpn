/*
 * rpn.cpp
 *
 * Copyright 2013  <louis@rubet.fr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 *
 */
#include <stdlib.h>
#include <stdint.h> //for intmax_t
#include <mpfr.h>
#include <math.h>

#include "constant.h"

#include "debug.h"

extern "C" {
#include <readline/readline.h>
#include <readline/history.h>
}

#include <iostream>
#include <iomanip>
#include <vector>
#include <sstream>
#include <fstream>
using namespace std;

static string s_mpfr_printf_format = string(MPFR_FORMAT);
static mpfr_prec_t s_mpfr_prec = MPFR_DEFAULT_PREC_BITS;
static unsigned int s_mpfr_prec_bytes = MPFR_DEFAULT_STORING_LENGTH_BYTES;

static mpfr_rnd_t s_mpfr_rnd = MPFR_DEF_RND;
static const char* s_mpfr_rnd_str[5] = MPFR_RND_STRINGS;

//
#include "escape.h"
#include "version.h"

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
    ret_max
} ret_value;

static const char* ret_value_string[ret_max] = RET_VALUE_STRINGS;

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

const char* cmd_type_string[cmd_max] = CMD_TYPE_STRINGS;

// MPFR object
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
};

class program;
class object;
class branch;

typedef void (program::*program_fn_t)(void);
typedef int (program::*branch_fn_t)(branch&);

//
struct object
{
    // object type
    cmd_type_t _type;
    unsigned int _size;

    //
    unsigned int size() { return _size; }
    void show(ostream& stream = cout);
};

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
        memcpy(_value.mpfr->_mpfr_d, op._value.mpfr->_mpfr_d, s_mpfr_prec_bytes);
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
        return (unsigned int)(sizeof(number)+s_mpfr_prec_bytes);
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
};

number::mode_enum number::s_mode = DEFAULT_MODE;
int number::s_current_precision = DEFAULT_PRECISION;

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

void object::show(ostream& stream)
{
    //char buffer[512];

    switch(_type)
    {
    case cmd_number:
        switch(((number*)this)->_representation)
        {
            case number::dec:
                (void)mpfr_printf(s_mpfr_printf_format.c_str(), ((number*)this)->_value.mpfr);
                //stream<<buffer;
                break;
            case number::hex:
                (void)mpfr_printf(string(MPFR_FORMAT_HEX).c_str(), ((number*)this)->_value.mpfr);                
                //stream<<buffer;
                break;
            case number::bin:
                cout<<"<binary representation TODO>";
        }
        break;
    case cmd_string:
        stream << "\"" << ((ostring*)this)->_value << "\"";
        break;
    case cmd_program:
        stream << "<<" << ((oprogram*)this)->_value << ">>";
        break;
    case cmd_symbol:
        stream << "'" << ((symbol*)this)->_value << "'";
        break;
    case cmd_keyword:
    case cmd_branch:
        stream << ((keyword*)this)->_value;
        break;
    default:
        stream << "< unknown object representation >";
        break;
    }
}

struct if_layout_t
{
    if_layout_t():index_then(-1),index_else(-1),index_end(-1) { }
    int index_if;
    int index_then;
    int index_else;
    int index_end;
};

#include "stack.h"

class program : public stack
{
public:
    program() { }

    // run this program
    ret_value run(stack& stk, heap& hp, heap* parent_local_hp = NULL)
    {
        bool go_out = false;
        ret_value ret = ret_ok;
        cmd_type_t type;

        // stack comes from outside
        _stack = &stk;

        // global heap comes from outside
        _global_heap = &hp;

        // parent local heap comes from outside
        _parent_local_heap = parent_local_hp;

        _err = ret_ok;
        _err_context = "";

        // branches for 'if'
        ret = preprocess();
        if (ret != ret_ok)
            return ret;

        // iterate commands
        for(int i = 0; (go_out==false) && (i<(int)size());)
        {
            type = (cmd_type_t)seq_type(i);

            // could be an auto-evaluated symbol
            if (type == cmd_symbol)
            {
                auto_rcl((symbol*)seq_obj(i));
                i++;
            }

            // a keyword
            else if (type == cmd_keyword)
            {
                keyword* k = (keyword*)seq_obj(i);
                // call matching function
                (this->*(k->_fn))();
                switch(_err)
                {
                    // no pb -> go on
                    case ret_ok:
                        break;
                    // explicit go out software
                    case ret_good_bye:
                        go_out = true;
                        ret = ret_good_bye;
                        break;
                    default:
                        // error: abort prog
                        go_out = true;

                        // error: show it
                        if (show_error(_err, _err_context) == ret_deadly)
                        {
                            // pb showing error -> go out software
                            ret = ret_good_bye;
                        }
                        break;
                }
                i++;
            }

            // a branch keyword
            else if (type == cmd_branch)
            {
                // call matching function
                branch* b = (branch*)seq_obj(i);
                int next_cmd = (this->*(b->_fn))(*b);
                switch (next_cmd)
                {
                    case -1:
                        i++; // meaning 'next command'
                        break;
                    case -(int)ret_runtime_error:
                        // error: show it
                        (void)show_error(_err, _err_context);
                        go_out = true;// end of run
                        break;
                    default:
                        i = next_cmd;// new direction
                        break;
                }
            }

            // not a command, but a stack entry, manage it
            else
            {
                // copy the program stack entry to the running stack
                stack::copy_and_push_back(*this, i, stk);
                i++;
            }
        }

        return ret;
    }

    bool compare_keyword(keyword* k, const char* str_to_compare, int len)
    {
        if (k->_len >= len)
            return strncmp(k->_value, str_to_compare, len) == 0;
        else
            return false;
    }

    bool compare_branch(branch* b, const char* str_to_compare, int len)
    {
        if (b->_len >= len)
            return strncmp(b->_value, str_to_compare, len) == 0;
        else
            return false;
    }

    ret_value preprocess(void)
    {
        // for if-then-else-end
        vector<struct if_layout_t> vlayout;
        int layout_index=-1;
        // for start-end-step
        vector<int> vstartindex;

        // analyse if-then-else-end branches
        // analyse start-{next, step} branches
        for(int i=0; i<(int)size(); i++)
        {
            int type = seq_type(i);
            if (type == cmd_keyword)
            {
                keyword* k = (keyword*)seq_obj(i);
                if (compare_keyword(k, "end", 3))
                {
                    int next = i + 1;
                    if (next >= (int)size())
                        next = -1;

                    if (layout_index<0)
                    {
                        // error: show it
                        show_syntax_error("missing if before end");
                        return ret_syntax;
                    }
                    if (vlayout[layout_index].index_end != -1)
                    {
                        // error: show it
                        show_syntax_error("duplicate end");
                        return ret_syntax;
                    }
                    if (vlayout[layout_index].index_else != -1)
                        //fill 'end' branch of 'else'
                        ((branch*)seq_obj(vlayout[layout_index].index_else))->arg2 = i;
                    else
                    {
                        //fill 'end' branch of 'then'
                        if (vlayout[layout_index].index_then != -1)
                            ((branch*)seq_obj(vlayout[layout_index].index_then))->arg2 = i;
                        else
                        {
                            // error: show it
                            show_syntax_error("missing then before end");
                            return ret_syntax;
                        }
                    }
                    layout_index--;
                }
            }
            else if (type == cmd_branch)
            {
                branch* k = (branch*)seq_obj(i);
                if (compare_branch(k, "if", 2))
                {
                    if_layout_t layout;
                    layout.index_if = i;
                    vlayout.push_back(layout);
                    layout_index++;
                }
                else if (compare_branch(k, "then", 4))
                {
                    int next = i + 1;
                    if (next >= (int)size())
                        next = -1;

                    // nothing after 'then' -> error
                    if (next == -1)
                    {
                        // error: show it
                        show_syntax_error("missing end after then");
                        return ret_syntax;
                    }
                    if (layout_index<0)
                    {
                        // error: show it
                        show_syntax_error("missing if before then");
                        return ret_syntax;
                    }
                    if (vlayout[layout_index].index_then != -1)
                    {
                        // error: show it
                        show_syntax_error("duplicate then");
                        return ret_syntax;
                    }
                    vlayout[layout_index].index_then = i;
                    k->arg1 = next;
                    k->arg3 = vlayout[layout_index].index_if;
                }
                else if (compare_branch(k, "else", 4))
                {
                    int next = i + 1;
                    if (next >= (int)size())
                        next = -1;

                    // nothing after 'else' -> error
                    if (next == -1)
                    {
                        // error: show it
                        show_syntax_error("missing end after else");
                        return ret_syntax;
                    }
                    if (layout_index<0)
                    {
                        // error: show it
                        show_syntax_error("missing if before else");
                        return ret_syntax;
                    }
                    if (vlayout[layout_index].index_then == -1)
                    {
                        // error: show it
                        show_syntax_error("missing then before else");
                        return ret_syntax;
                    }
                    if (vlayout[layout_index].index_else != -1)
                    {
                        // error: show it
                        show_syntax_error("duplicate else");
                        return ret_syntax;
                    }
                    vlayout[layout_index].index_else = i;
                    k->arg1 = next;// fill branch1 (if was false) of 'else'
                    k->arg3 = vlayout[layout_index].index_if;
                    ((branch*)seq_obj(vlayout[layout_index].index_then))->arg2 = next;// fill branch2 (if was false) of 'then'
                }
                else if (compare_branch(k, "start", 5))
                {
                    vstartindex.push_back(i);
                }
                else if (compare_branch(k, "for", 3))
                {
                    vstartindex.push_back(i);
                    k->arg1 = i + 1;// arg1 points on symbol variable
                }
                else if(compare_branch(k, "next", 4))
                {
                    if (vstartindex.size() == 0)
                    {
                        // error: show it
                        show_syntax_error("missing start or for before next");
                        return ret_syntax;
                    }
                    k->arg1 = vstartindex[vstartindex.size() - 1]; // 'next' arg1 = 'start' index
                    ((branch*)seq_obj(vstartindex[vstartindex.size() - 1]))->arg2 = i; // 'for' or 'start' arg2 = 'next' index
                    vstartindex.pop_back();
                }
                else if (compare_branch(k, "step", 4))
                {
                    if (vstartindex.size() == 0)
                    {
                        // error: show it
                        show_syntax_error("missing start or for before step");
                        return ret_syntax;
                    }
                    k->arg1 = vstartindex[vstartindex.size() - 1];// fill 'step' branch1 = 'start' index
                    ((branch*)seq_obj(vstartindex[vstartindex.size() - 1]))->arg2 = i; // 'for' or 'start' arg2 = 'next' index
                    vstartindex.pop_back();
                }
                else if (compare_branch(k, "->", 2))
                {
                    k->arg1 = i;// arg1 is '->' command index in program
                }
            }
        }
        if (layout_index >= 0)
        {
            // error: show it
            show_syntax_error("missing end");
            return ret_syntax;
        }
        if (vstartindex.size() > 0)
        {
            // error: show it
            show_syntax_error("missing next or step after for or start");
            return ret_syntax;
        }
        return ret_ok;
    }

    ret_value show_error()
    {
        ret_value ret;

        // show last recorded error
        cerr<<ret_value_string[_err]<<"("<<_err<<"): "<<_err_context<<endl;
        switch(_err)
        {
            case ret_internal:
            case ret_deadly:
                ret = ret_deadly;
            default:
                ret = ret_ok;
        }
        
        return ret;
    }

    ret_value show_error(ret_value err, string& context)
    {
        // record error
        _err = err;
        _err_context = context;
        return show_error();
    }

    ret_value show_error(ret_value err, const char* context)
    {
        // record error
        _err = err;
        _err_context = context;
        return show_error();
    }

    void show_syntax_error(const char* context)
    {
        // record error
        _err = ret_syntax;
        _err_context = context;
        (void)show_error();
    }

    ret_value get_err(void) { return _err; }

#include "parse.h"

    static void show_stack(stack& st, const string& separator = g_show_stack_separator)
    {
        if (st.size() == 1)
        {
            ((object*)st.back())->show();
            cout<<endl;
        }
        else
        {
            bool show_sep = (! separator.empty());
            for (int i = st.size()-1; i>=0; i--)
            {
                if (show_sep)
                    cout<<i+1<<separator;
                ((object*)st[i])->show();
                cout<<endl;
            }
        }
    }
    
    static void apply_default()
    {
        //default float precision, float mode, verbosity
        number::s_mode = DEFAULT_MODE;
        number::s_current_precision = DEFAULT_PRECISION;

        // format for mpfr_printf 
        stringstream ss;
        ss << number::s_current_precision;
        s_mpfr_printf_format = string(MPFR_FORMAT_BEG) + ss.str() + string(MPFR_FORMAT_STD);
    }

private:
    ret_value _err;
    string _err_context;

    stack* _stack;
    stack _branch_stack;

    heap* _global_heap;
    heap _local_heap;
    heap* _parent_local_heap;

    int stack_size()
    {
        return _stack->size();
    }

private:
    // carefull : some of these macros modify program flow
    #define ERR_CONTEXT(err) do { _err = (err); _err_context = __FUNCTION__; } while(0)
    #define MIN_ARGUMENTS(num) do { if (stack_size()<(num)) { ERR_CONTEXT(ret_missing_operand); return; } } while(0)
    #define MIN_ARGUMENTS_RET(num, ret) do { if (stack_size()<(num)) { ERR_CONTEXT(ret_missing_operand); return (ret); } } while(0)
    #define ARG_MUST_BE_OF_TYPE(num, type) do { if (_stack->get_type(num) != (type)) { ERR_CONTEXT(ret_bad_operand_type); return; } } while(0)
    #define ARG_MUST_BE_OF_TYPE_RET(num, type, ret) do { if (_stack->get_type(num) != (type)) { ERR_CONTEXT(ret_bad_operand_type); return (ret); } } while(0)
    #define IS_ARG_TYPE(num, type) (_stack->get_type(num) == (type))
    #define CHECK_MPFR(op) do { (void)(op); } while(0)

    // keywords implementation
    #include "rpn-general.h"
    #include "rpn-real.h"
    #include "rpn-test.h"
    #include "rpn-stack.h"
    #include "rpn-string.h"
    #include "rpn-branch.h"
    #include "rpn-store.h"
    #include "rpn-program.h"
    #include "rpn-trig.h"
    #include "rpn-logs.h"
};

//keywords declaration
#include "rpn-cmd.h"
#include "rpn-test-core.h"

//
int main(int argc, char* argv[])
{
    heap global_heap;
    stack global_stack;
    int ret = 0;

    // apply default configuration
    program::apply_default();

    // run with interactive prompt
    if (argc == 1)
    {
        //
        for (;;)
        {
            // make program from interactive entry
            program prog;
            if (program::entry(prog) == ret_good_bye)
                break;
            else
            {
                // run it
                if (prog.run(global_stack, global_heap) == ret_good_bye)
                    break;
                else
                    program::show_stack(global_stack);
            }
        }
    }
    // run with cmd line arguments
    else
    {
        program prog;
        string entry;
        int i;

        // make one string from entry
        for (i=1; i<argc; i++)
        {
            entry += argv[i];
            entry += ' ';
        }

        // make program
        ret = program::parse(entry.c_str(), prog);
        if (ret == ret_ok)
        {
            string separator = "";

            // run it
            ret = prog.run(global_stack, global_heap);
            program::show_stack(global_stack, separator);
        }
    }
 
    mpfr_free_cache();
 
    return ret;
}
