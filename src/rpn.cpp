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
#ifdef WIN32
#include <tchar.h>
#else
#define _TCHAR char
#define _tmain main
#endif
#include <math.h>

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

#include "stack.h"

//
static int g_verbose = 0;

//
#include "escape.h"
#include "version.h"

typedef enum {
	ret_ok,
	ret_unknown_err,
	ret_missing_operand,
	ret_bad_operand_type,
	ret_unknown_variable,
	ret_internal,
	ret_deadly,
	ret_good_bye,
	ret_not_impl,
	ret_nop,
	ret_syntax,
	ret_div_by_zero,
	ret_max
} ret_value;

const char* ret_value_string[ret_max] = {
	"ok", "unknown command", "missing operand", "bad operand type", "unknown variable", "internal error, aborting",
	"deadly", "goodbye", "not implemented", "no operation", "syntax", "division by zero"
};

typedef enum {
	cmd_undef,
	cmd_number,/* floating value to put in stack */
	cmd_binary,/* binary (integer) value to put in stack */
	cmd_string,/* string value to put in stack */
	cmd_symbol,/* symbol value to put in stack */
	cmd_program,/* program */
	cmd_keyword,/* langage keyword */
	cmd_branch,/* langage branch keyword */
	cmd_max
} cmd_type_t;

const char* cmd_type_string[cmd_max] = {
    "undef", "number", "binary", "string", "symbol", "program", "keyword", "keyword"
};

//
typedef long double floating_t;
typedef long long integer_t;
class program;
class object;
class branch;
typedef void (program::*program_fn_t)(void);
typedef union
{
	program_fn_t _fn;
} operand;
typedef int (program::*branch_fn_t)(branch&);

//
struct object
{
    // object type
    cmd_type_t _type;

    //
    void show(ostream& stream = cout);
} __attribute__((packed));

struct number : public object
{
	floating_t _value;

    //
    void set(floating_t value)
    {
        _type = cmd_number;
        _value = value;
    }
    unsigned int size() { return sizeof(number); }

    // representation mode
	typedef enum {
		std,
		fix,
		sci
	} mode_enum;
	static mode_enum s_default_mode;
	static mode_enum s_mode;
	
	// precision
	static int s_default_precision;
	static int s_current_precision;
} __attribute__((packed));
number::mode_enum number::s_default_mode = number::std;
number::mode_enum number::s_mode = number::s_default_mode;
int number::s_default_precision = 20;
int number::s_current_precision = number::s_default_precision;

struct binary : public object
{
	integer_t _value;
	
    //
    void set(integer_t value)
    {
        _type = cmd_binary;
        _value = value;
    }
    unsigned int size() { return sizeof(binary); }

	// representation mode
	typedef enum {
		dec,
		hex,
		oct,
		bin,
	} binary_enum;
	static binary_enum s_default_mode;
	static binary_enum s_mode;
} __attribute__((packed));
binary::binary_enum binary::s_default_mode = binary::dec;
binary::binary_enum binary::s_mode = binary::s_default_mode;

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
    int size() { return sizeof(ostring)+_len+1; }

    //
    unsigned int _len;
    char _value[0];
} __attribute__((packed));

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
    int size() { return sizeof(oprogram)+_len+1; }

    //
    unsigned int _len;
    char _value[0];
} __attribute__((packed));

struct symbol : public object
{
    //
    void set(const char* value, unsigned int len)
    {
        _type = cmd_symbol;
        _auto_eval = false;
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
    int size() { return sizeof(symbol)+_len+1; }

    //
    bool _auto_eval;
    unsigned int _len;
    char _value[0];
} __attribute__((packed));

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
    int size() { return sizeof(keyword)+_len+1; }

    //
    program_fn_t _fn;
    unsigned int _len;
    char _value[0];
} __attribute__((packed));

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
        farg1 = 0;
        farg2 = 0;
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
    int size() { return sizeof(branch)+_len+1; }

    // branch function
    branch_fn_t _fn;
    // args used by cmd_branch cmds
    int arg1, arg2, arg3;
    floating_t farg1, farg2;
    bool arg_bool;
    unsigned int _len;
    char _value[0];
} __attribute__((packed));

void object::show(ostream& stream)
{
    switch(_type)
    {
    case cmd_number:
        stream << ((number*)this)->_value;
        break;
    case cmd_binary:
        {
            cout << "# ";
            switch(((binary*)this)->s_mode)
            {
                case binary::dec: cout<<std::right<<std::setw(8)<<std::dec<<((binary*)this)->_value<<" d"; break;
                case binary::hex: cout<<std::right<<std::setw(16)<<std::hex<<((binary*)this)->_value<<" h"; break;
                case binary::oct: cout<<std::right<<std::setw(16)<<std::oct<<((binary*)this)->_value<<" o"; break;
                case binary::bin:
                {
                    string mybin;
                    for (int i = (int)(log((floating_t)((binary*)this)->_value) / log(2.)); i>=0; i--)
                    {
                        if (((binary*)this)->_value & (1 << i))
                            mybin+='1';
                        else
                            mybin+='0';
                    }
                    cout<<std::right<<std::setw(20)<<std::oct<<mybin<<" b";
                }
                break;
            }
        }
        break;
    case cmd_string:
        stream << "\"" << ((ostring*)this)->_value << "\"";
        break;
    case cmd_program:
        stream << "<< " << ((oprogram*)this)->_value << " >>";
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

class program : public stack
{
public:
	program() { }

	// run this program
	ret_value run(stack& stk, heap& hp)
	{
		bool go_out = false;
		ret_value ret = ret_ok;
		cmd_type_t type;

		_stack = &stk;
		_heap = &hp;
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

			//
			if (g_verbose >= 2)
			{
				cout << "(" << i << ") ";
				((object*)seq_obj(i))->show();
				cout << endl;
			}

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
				int tmp = (this->*(b->_fn))(*b);
				if (tmp == -1)
					i++;
				else
					i = tmp;
			}

			// not a command, but a stack entry, manage it
			else
			{
				stk.push_back(seq_obj(i), seq_len(i), type);
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
        int layout_index=-1;// TODO remplacable par vlayout.size()-1
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
						//fill 'end' branch of 'then'
						((branch*)seq_obj(vlayout[layout_index].index_then))->arg2 = i;
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
					k->arg1 = vstartindex[vstartindex.size() - 1];// fill 'next' branch1 = 'start' index
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

	static ret_value show_error(ret_value err, string& context)
	{
		cerr<<context<<": "<<ret_value_string[err]<<endl;
		switch(err)
		{
			case ret_internal:
			case ret_deadly:
				return ret_deadly;
			default:
				return ret_ok;
		}
	}

	static ret_value show_error(ret_value err, char* context)
	{
		string context_string(context);
		return show_error(err, context_string);
	}

	static void show_syntax_error(const char* context)
	{
		cerr<<"syntax error: "<<context<<endl;
	}

	ret_value get_err(void)	{ return _err; }

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

private:
	ret_value _err;
	string _err_context;
	stack* _stack;
	heap* _heap;

	// helpers for keywords implementation
	floating_t getf()
	{
		/* warning, caller must check object type before */
		floating_t a = ((number*)_stack->back())->_value;
		_stack->pop_back();
		return a;
	}

	void putf(floating_t value)
	{
		/* warning, caller must check object type before */
        number num;
        num.set(value);
        _stack->push_back(&num, num.size(), cmd_number);
	}

	integer_t getb()
	{
		/* warning, caller must check object type before */
		integer_t a = ((binary*)_stack->back())->_value;
		_stack->pop_back();
		return a;
	}

	void putb(integer_t value)
	{
		/* warning, caller must check object type before */
        binary num;
        num.set(value);
        _stack->push_back(&num, num.size(), cmd_binary);
	}

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

	// keywords implementation
	#include "rpn-general.h"
	#include "rpn-algebra.h"
	#include "rpn-binary.h"
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
static void apply_default(void)
{
	//default precision
	cout << setprecision(number::s_default_precision);
	number::s_mode = number::s_default_mode;

	//default binary mode
	binary::s_mode = binary::s_default_mode;
}

//
int _tmain(int argc, _TCHAR* argv[])
{
	heap hp;
	stack st;
	int ret = 0;

	// apply default configuration
	apply_default();

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
				if (prog.run(st, hp) == ret_good_bye)
					break;
				else
					program::show_stack(st);
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
			ret = prog.run(st, hp);
			program::show_stack(st, separator);
		}
	}

	return ret;
}
