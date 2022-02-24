// Copyright (c) 2014-2022 Louis Rubet

#ifndef SRC_PROGRAM_HPP_
#define SRC_PROGRAM_HPP_

// std c++
#include <deque>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

#include <mpreal.h>
using namespace mpfr;

// internal includes
#include "lexer.hpp"
#include "object.hpp"
#include "stack.hpp"

//< program class: the class containing a string parser, all the programs keywords, a stack for running the program
class program : public deque<object*>, public Lexer {
 public:
    program(rpnstack& stk, heap& hp, program* parent = nullptr) : _stack(stk), _heap(hp), _parent(parent) {}
    virtual ~program() {
        _local_heap.clear();
        clear();
    }

    // parser
    ret_value parse(string& entry);
    static ret_value get_fn(const char* fn_name, program_fn_t& fn, cmd_type_t& type);

    // running
    ret_value run();
    void stop();
    ret_value preprocess(void);

    ret_value show_error();
    ret_value show_error(ret_value err, string& context);
    ret_value show_error(ret_value err, const char* context);
    void show_syntax_error(const char* context);
    ret_value get_err(void);

    void show_stack(bool show_separator = true);

    static void apply_default();

    static vector<string>& getAutocompletionWords();

 private:
    // current error and its context
    ret_value _err;
    string _err_context;

    // global stack holding results for user
    rpnstack& _stack;

    // global heap (sto, rcl)
    heap& _heap;

    // local heap for local loop variables (for..next)
    heap _local_heap;

    // parent prog for inheriting heaps
    program* _parent;

 private:
    // keywords
    struct keyword_t {
        cmd_type_t type;
        string name;
        program_fn_t fn;
        string comment;
    };
    static vector<keyword_t> _keywords;

    // keywords implementation
    ////

    // branch
    size_t rpn_if(branch& myobj);
    size_t rpn_then(branch& myobj);
    size_t rpn_else(branch& myobj);
    size_t rpn_end(branch& myobj);
    size_t rpn_do(branch& myobj);
    size_t rpn_until(branch& myobj);
    void rpn_ift(void);
    void rpn_ifte(void);
    size_t rpn_while(branch& myobj);
    size_t rpn_repeat(branch& myobj);
    size_t rpn_start(branch& myobj);
    size_t rpn_for(branch& myobj);
    size_t rpn_next(branch& myobj);
    size_t rpn_step(branch& myobj);
    enum { step_out = static_cast<size_t>(-1), runtime_error = static_cast<size_t>(-2) };

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
    void rpn_test();
    void test(string test_filename, int& total_tests, int& total_tests_failed, int& total_steps,
              int& total_steps_failed);

    // test
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

// convenience macros for rpn_xx function
// carefull : some of these macros modify program flow

#define setErrorContext(err)         \
    do {                             \
        _err = (err);                \
        _err_context = __FUNCTION__; \
    } while (0)

#define MIN_ARGUMENTS(num)                         \
    do {                                           \
        if ((num) >= 0 && _stack.size() < (num)) { \
            setErrorContext(ret_missing_operand);  \
            return;                                \
        }                                          \
    } while (0)

#define MIN_ARGUMENTS_RET(num, ret)                \
    do {                                           \
        if ((num) >= 0 && _stack.size() < (num)) { \
            setErrorContext(ret_missing_operand);  \
            return (ret);                          \
        }                                          \
    } while (0)

#define ARG_MUST_BE_OF_TYPE(num, type)                       \
    do {                                                     \
        if ((num) >= 0 && _stack.at(num)->_type != (type)) { \
            setErrorContext(ret_bad_operand_type);           \
            return;                                          \
        }                                                    \
    } while (0)

#define ARG_MUST_BE_OF_TYPE_RET(num, type, ret)              \
    do {                                                     \
        if ((num) >= 0 && _stack.at(num)->_type != (type)) { \
            setErrorContext(ret_bad_operand_type);           \
            return (ret);                                    \
        }                                                    \
    } while (0)

#endif  // SRC_PROGRAM_HPP_
