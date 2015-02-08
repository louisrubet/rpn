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

#include <iostream>
#include <iomanip>
#include <vector>
#include <sstream>
#include <fstream>
//#include <regex>
using namespace std;

#include "stack.h"

//TODO faut-il que ces variables soient globales ?
static const char g_cursor[] = "> ";
static const string g_show_stack_separator = ":\t";
static const int g_max_commands = 100;

//
static int g_verbose = 0;

//
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
	ret_max
} ret_value;

const char* ret_value_string[ret_max] = {
	"ok", "unknown command", "missing operand", "bad operand type", "unknown variable", "internal error, aborting",
	"deadly", "goodbye", "not implemented", "no operation", "syntax"
};

typedef enum {
	cmd_undef,
	cmd_number,/* floating value to put in stack */
	cmd_binary,/* binary (integer) value to put in stack */
	cmd_symbol,/* symbol value to put in stack */
	cmd_keyword,/* langage keyword */
	cmd_branch,/* langage branch keyword */
	cmd_max
} cmd_type_t;

const char* cmd_type_string[cmd_max] = {
	"undef", "number", "binary", "symbol", "keyword", "keyword"
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
class object
{
public:
	cmd_type_t _type;// object type
	object(cmd_type_t type = cmd_undef):_type(type) { }
	virtual void show(ostream& stream = cout) { }
};

class number : public object
{
public:
	number(floating_t value) : object(cmd_number) { _value = value; }
	virtual void show(ostream& stream = cout) { stream << _value; }
	floating_t _value;

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
};
number::mode_enum number::s_default_mode = number::std;
number::mode_enum number::s_mode = number::s_default_mode;
int number::s_default_precision = 20;
int number::s_current_precision = number::s_default_precision;

class binary : public object
{
public:
	binary(integer_t value) : object(cmd_binary) { _value = value; }
	
	virtual void show(ostream& stream = cout)
	{
		cout << "# ";
		switch(s_mode)
		{
			case dec: cout<<std::right<<std::setw(8)<<std::dec<<_value<<" d"; break;
			case hex: cout<<std::right<<std::setw(8)<<std::hex<<_value<<" h"; break;
			case oct: cout<<std::right<<std::setw(8)<<std::oct<<_value<<" o"; break;
			case bin:
			{
				string mybin;
				for (int i = (int)(log((floating_t)_value) / log(2.)); i>=0; i--)
				{
					if (_value & (1 << i))
						mybin+='1';
					else
						mybin+='0';
				}
				cout<<std::right<<std::setw(16)<<std::oct<<mybin<<" b";
			}
			break;
		}
	}
	
	integer_t _value;
	
	// representation mode
	typedef enum {
		dec,
		hex,
		oct,
		bin,
	} binary_enum;
	static binary_enum s_default_mode;
	static binary_enum s_mode;
};
binary::binary_enum binary::s_default_mode = binary::dec;
binary::binary_enum binary::s_mode = binary::s_default_mode;

class symbol : public object
{
public:
	symbol(string& name, cmd_type_t type = cmd_symbol) : object(type), _auto_eval(false)
	{
	    _name = new string;
	    *_name = name;
	}
	symbol(const char* name, cmd_type_t type = cmd_symbol) : object(type), _auto_eval(false)
	{
	    _name = new string;
	    *_name = name;
	}
	virtual void show(ostream& stream = cout) { stream << "'" << *_name << "'"; }
	string* _name;
	bool _auto_eval;
};

class keyword : public symbol
{
public:
	keyword(program_fn_t fn, string& name, cmd_type_t type = cmd_keyword) : symbol(name, type) { _fn = fn; }
	program_fn_t _fn;
	virtual void show(ostream& stream = cout) { stream << *_name; }
};

class branch : public keyword
{
public:
	branch(branch_fn_t fn, string& name) : keyword(NULL, name, cmd_branch), arg1(-1), arg2(-1), arg3(-1), arg_bool(false)
	{
		_type = cmd_branch;
		_fn = fn;
	}
	// branch function
	branch_fn_t _fn;
	// args used by cmd_branch cmds
	int arg1, arg2, arg3;
	floating_t farg1, farg2;
	bool arg_bool;
};

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

			// not a command, but a stack entry, manage it
			if ((type == cmd_number) || (type == cmd_binary))
			{
				stk.push_back(seq_obj(i), seq_len(i), type);
				i++;
			}

			// could be an auto-evaluated symbol
			else if (type == cmd_symbol)
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
		}
		return ret;
	}

	ret_value preprocess(void)
	{
		// for if-then-else-end
		vector<struct if_layout_t> vlayout;
		int layout_index=-1;// TODO remplaçable par vlayout.size()-1
		//for start-end-step
		vector<int> vstartindex;

		// analyse if-then-else-end branches
		// analyse start-{next, step} branches
		for(int i=0; i<(int)size(); i++)
		{
			int type = seq_type(i);
			if (type == cmd_keyword)
			{
				keyword* k = (keyword*)seq_obj(i);
				if(k->_name->compare("end") == 0)
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
				if (k->_name->compare("if") == 0)
				{
					if_layout_t layout;
					layout.index_if = i;
					vlayout.push_back(layout);
					layout_index++;
				}
				else if(k->_name->compare("then") == 0)
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
				else if(k->_name->compare("else") == 0)
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
				else if(k->_name->compare("start") == 0)
				{
					vstartindex.push_back(i);
				}
				else if(k->_name->compare("for") == 0)
				{
					vstartindex.push_back(i);
					k->arg1 = i + 1;// arg1 points on symbol variable
				}
				else if(k->_name->compare("next") == 0)
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
				else if(k->_name->compare("step") == 0)
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

	// keywords declaration
	struct keyword_t
	{
		cmd_type_t type;
		string name;
		program_fn_t fn;
		string comment;
	};
    static keyword_t _keywords[g_max_commands];

	static ret_value get_fn(const string& fn_name, program_fn_t& fn, cmd_type_t& type)
	{
		for(unsigned int i=0; (i<sizeof(_keywords)/sizeof(_keywords[0])) && (_keywords[i].type != cmd_max); i++)
		{
			if ((_keywords[i].name.size()>0) && (fn_name == _keywords[i].name))
			{
				fn = _keywords[i].fn;
				type = _keywords[i].type;
				return ret_ok;
			}
		}
		return ret_unknown_err;
	}

	ret_value get_err(void)	{ return _err; }

	// interactive entry and decoding
	static ret_value entry(program& prog)
	{
		ret_value ret;
		string entry;

		// show cursor
		cout<<g_cursor;

		// get user line
		getline(cin, entry);
		if (cin.good())
			// parse it
			ret = parse(entry, prog);
		else
			ret = ret_internal;

		return ret;
	}

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
		number num(value);
        _stack->push_back(&num, sizeof(number), cmd_number);
	}

	string getn()
	{
		/* warning, caller must check object type before */
		string* a = ((symbol*)_stack->back())->_name;
		_stack->pop_back();
		return *a;
	}

	void putn(string& a)
	{
		/* warning, caller must check object type before */
		symbol sym(a);
        _stack->push_back(&sym, sizeof(symbol), cmd_symbol);
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
	#define ARG_IS_OF_TYPE(num, type) do { if (_stack->get_type(num) != (type)) { ERR_CONTEXT(ret_bad_operand_type); return; } } while(0)
	#define ARG_IS_OF_TYPE_RET(num, type, ret) do { if (_stack->get_type(num) != (type)) { ERR_CONTEXT(ret_bad_operand_type); return (ret); } } while(0)

	// keywords implementation
	#include "rpn-general.h"
	#include "rpn-algebra.h"
	#include "rpn-binary.h"
	#include "rpn-test.h"
	#include "rpn-stack.h"
	#include "rpn-branch.h"
	#include "rpn-store.h"
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
		ret = program::parse(entry, prog);
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
