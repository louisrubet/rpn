#ifndef PROGRAM_HPP
#define PROGRAM_HPP

// std c
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/mman.h>

// std c++
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>
using namespace std;

// external libs
#define MPFR_USE_NO_MACRO
#include <mpfr.h>

#include "linenoise.h"

// internal includes
#include "constant.h"
#include "escape.h"
#include "object.hpp"
#include "stack.hpp"
#include "version.h"

//< convinient structure to preprocess a program
struct if_layout_t {
    if_layout_t()
        : index_then_or_unti_or_repeat(-1), index_else(-1), index_end(-1), is_do_unti(false), is_while_repeat(false) {}
    int index_if_or_do_or_while;
    int index_then_or_unti_or_repeat;
    int index_else;
    int index_end;
    bool is_do_unti;
    bool is_while_repeat;
};

//< program class: the class containing a string parser, all the programs keywords, a stack for running the program
class program : public deque<object*> {
   public:
    program(rpnstack* stk, heap* hp, program* parent = nullptr):_stack(stk),_heap(hp),_parent(parent) {
        interrupt_now = false;
    }
    virtual ~program() {
        _local_heap.clear();
        clear();
    }

    // parser
    static ret_value parse(string& entry, program& prog);
    static ret_value entry(program& prog);
    static void entry_completion_generator(const char* text, linenoiseCompletions* lc);
    static ret_value get_fn(const char* fn_name, program_fn_t& fn, cmd_type_t& type);

    // running
    ret_value run();
    void stop();
    bool compare_branch(branch* b, const char* str_to_compare, int len);
    ret_value preprocess(void);

    ret_value show_error();
    ret_value show_error(ret_value err, string& context);
    ret_value show_error(ret_value err, const char* context);
    void show_syntax_error(const char* context);
    ret_value get_err(void);

    static void show_stack(rpnstack& st, bool show_separator = true);

    static void apply_default();

   private:
    bool interrupt_now;

    // current error and its context
    ret_value _err;
    string _err_context;

    // global stack holding results for user
    rpnstack* _stack;

    // global heap (sto, rcl)
    heap* _heap;

    // local heap for local loop variables (for..next)
    heap _local_heap;

    // parent prog for inheriting heaps
    program* _parent;

    int stack_size() { return _stack->size(); }

   private:
    static const char* s_ret_value_string[ret_max];

    // keywords
    struct keyword_t {
        cmd_type_t type;
        char name[MAX_COMMAND_LENGTH];
        program_fn_t fn;
        string comment;
    };
    static keyword_t s_keywords[];

    // keywords implementation
    ////

    // branch
    int rpn_if(branch& myobj);
    int rpn_then(branch& myobj);
    int rpn_else(branch& myobj);
    int rpn_end(branch& myobj);
    int rpn_do(branch& myobj);
    int rpn_until(branch& myobj);
    void rpn_ift(void);
    void rpn_ifte(void);
    int rpn_while(branch& myobj);
    int rpn_repeat(branch& myobj);
    int rpn_start(branch& myobj);
    int rpn_for(branch& myobj);
    int rpn_next(branch& myobj);
    int rpn_step(branch& myobj);

    // complex
    void rpn_re();
    void rpn_im();
    void rpn_arg();
    void rpn_conj();
    void rpn_r2c();
    void rpn_c2r();
    void rpn_r2p();
    void rpn_p2r();

    // general
    void rpn_nop();
    void rpn_good_bye();
    void rpn_help();
    void rpn_std();
    void rpn_fix();
    void rpn_sci();
    void rpn_version();
    void rpn_uname();
    void rpn_history();
    void rpn_type();
    void rpn_default();
    void rpn_precision();
    void rpn_round();

    // logs
    void rpn_e(void);
    void rpn_log10();
    void rpn_alog10();
    void rpn_log2();
    void rpn_alog2();
    void rpn_ln();
    void rpn_exp();
    void rpn_expm();
    void rpn_lnp1();
    void rpn_sinh();
    void rpn_asinh();
    void rpn_cosh();
    void rpn_acosh();
    void rpn_tanh();
    void rpn_atanh();

    // program
    bool find_variable(string& variable, object*& obj);
    void rpn_eval(void);
    int rpn_inprog(branch& myobj);

    // real
    void rpn_plus();
    void rpn_minus();
    void rpn_mul();
    void do_divide_complexes();
    void rpn_div();
    void rpn_neg();
    void rpn_inv();
    void rpn_purcent();
    void rpn_purcentCH();
    void rpn_power();
    void rpn_squareroot();
    void rpn_square();
    void rpn_modulo();
    void rpn_abs();
    void rpn_hex();
    void rpn_bin();
    void rpn_dec();
    void rpn_base();
    void rpn_fact();
    void rpn_sign();
    void rpn_mant();
    void rpn_xpon();
    void rpn_floor();
    void rpn_ceil();
    void rpn_fp();
    void rpn_ip();
    void rpn_min();
    void rpn_max();

    // stack
    void rpn_swap(void);
    void rpn_drop(void);
    void rpn_drop2(void);
    void rpn_dropn(void);
    void rpn_erase(void);
    void rpn_dup(void);
    void rpn_dup2(void);
    void rpn_dupn(void);
    void rpn_pick(void);
    void rpn_rot(void);
    void rpn_depth(void);
    void rpn_roll(void);
    void rpn_rolld(void);
    void rpn_over(void);

    // store
    void rpn_sto(void);
    void rpn_stoadd(void);
    void rpn_stosub(void);
    void rpn_stomul(void);
    void rpn_stodiv(void);
    void rpn_stoneg(void);
    void rpn_stoinv(void);
    void rpn_rcl(void);
    void rpn_edit(void);
    void auto_rcl(symbol* symb);
    void rpn_purge(void);
    void rpn_vars(void);
    void rpn_clusr(void);

    // string
    void rpn_instr();
    void rpn_strout();
    void rpn_chr();
    void rpn_num();
    void rpn_strsize();
    void rpn_strpos();
    void rpn_strsub();

    // test-core
    void test_get_stack(string& stack_is, rpnstack& stk);
    void test_show_result(string title, int tests, int tests_failed, int steps, int steps_failed);
    void rpn_test();
    void test(string test_filename, int& total_tests, int& total_tests_failed, int& total_steps,
              int& total_steps_failed);

    // test
    long cmp_strings_on_stack_top();
    void rpn_sup(void);
    void rpn_sup_eq(void);
    void rpn_inf(void);
    void rpn_inf_eq(void);
    void rpn_diff(void);
    void rpn_eq(void);
    void rpn_test_and(void);
    void rpn_test_or(void);
    void rpn_test_xor(void);
    void rpn_test_not(void);
    void rpn_same(void);

    // trig
    void rpn_pi(void);
    void rpn_d2r(void);
    void rpn_r2d(void);
    void rpn_sin(void);
    void rpn_asin(void);
    void rpn_cos(void);
    void rpn_acos(void);
    void rpn_tan(void);
    void rpn_atan(void);

    // time
    void rpn_time();
    void rpn_date();
    void rpn_ticks();
};

// clang-format off

// convenience macros for rpn_xx function
// carefull : some of these macros modify program flow
#define ERR_CONTEXT(err) do { _err = (err); _err_context = __FUNCTION__; } while (0)

#define MIN_ARGUMENTS(num) do { \
        if (stack_size() < (num)) { ERR_CONTEXT(ret_missing_operand); return; } \
    } while (0)

#define MIN_ARGUMENTS_RET(num, ret) do { \
        if (stack_size() < (num)) { ERR_CONTEXT(ret_missing_operand); return (ret); } \
    } while (0)

#define ARG_MUST_BE_OF_TYPE(num, type) do { \
        if (_stack->at(num)->_type != (type)) { ERR_CONTEXT(ret_bad_operand_type); return; } \
    } while (0)

#define ARG_MUST_BE_OF_TYPE_RET(num, type, ret) do { \
        if (_stack->at(num)->_type != (type)) { ERR_CONTEXT(ret_bad_operand_type); return (ret); } \
    } while (0)

#define IS_ARG_TYPE(num, type) (_stack->at(num)->_type == (type))

#define CHECK_MPFR(op) do { (void)(op); } while (0)

#endif
