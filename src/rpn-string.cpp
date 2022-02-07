#include <fcntl.h>

#include "program.hpp"

/// @brief ->str keyword implementation
///
void program::rpn_instr() {
    MIN_ARGUMENTS(1);

    // stringify only if not already a string
    if (_stack->at(0)->_type != cmd_string) {
        // TODO really ugly, to change
        // write the object in stack(0) in a string and remove this obj
        FILE* tmp = tmpfile();
        if (tmp != NULL) {
            ((object*)_stack->pop_back())->show(tmp);

            // reserve the correct size on stack
            unsigned int str_size = (unsigned int)ftell(tmp);
            char* buf = (char*)malloc(str_size);
            if (buf == nullptr) {
                ERR_CONTEXT(ret_out_of_memory);
                goto destroy_file;
            }

            // fill the obj
            rewind(tmp);
            if (fread(buf, str_size, 1, tmp) != 1) ERR_CONTEXT(ret_runtime_error);
            _stack->push_back(new ostring(buf));

        destroy_file:
            char filePath[PATH_MAX];
            fclose(tmp);
            if (fcntl(fileno(tmp), 12 /*F_GETPATH*/, filePath) != -1) remove(filePath);
        } else
            ERR_CONTEXT(ret_runtime_error);
    }
}

/// @brief str-> keyword implementation
///
void program::rpn_strout() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_string);

    string entry(((ostring*)_stack->pop_back())->_value);

    program prog;

    // make program from string in stack level 1
    if (program::parse(entry, prog) == ret_ok)
        // run it
        prog.run(*_stack, *_heap);
}

/// @brief chr keyword implementation
///
void program::rpn_chr() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    // get arg as number % 256
    char the_chr = (char)mpfr_get_d(((number*)_stack->pop_back())->_value.mpfr, floating_t::s_mpfr_rnd);
    if (the_chr < 32 || the_chr > 126) the_chr = '.';
    _stack->push_back(new ostring(string(1, the_chr)));
}

/// @brief num keyword implementation
///
void program::rpn_num() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_string);

    double the_chr = (double)((ostring*)_stack->pop_back())->_value[0];
    _stack->push_back(new number(the_chr));
}

/// @brief size keyword implementation
///
void program::rpn_strsize() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_string);

    double len = ((ostring*)_stack->pop_back())->_value.size();
    _stack->push_back(new number(len));
}

/// @brief pos keyword implementation
///
void program::rpn_strpos() {
    MIN_ARGUMENTS(2);
    ARG_MUST_BE_OF_TYPE(0, cmd_string);
    ARG_MUST_BE_OF_TYPE(1, cmd_string);

    size_t pos = static_cast<ostring*>(_stack->at(1))->_value.find(((ostring*)_stack->at(0))->_value);
    _stack->pop_back(2);
    _stack->push_back(new number(pos));
}

/// @brief sub keyword implementation
///
void program::rpn_strsub() {
#if 0
    MIN_ARGUMENTS(3);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ARG_MUST_BE_OF_TYPE(1, cmd_number);
    ARG_MUST_BE_OF_TYPE(2, cmd_string);

    long first = long(((number*)_stack->at(1))->_value) - 1;
    long last = long(((number*)_stack->at(0))->_value) - 1;
    long len = ((ostring*)_stack->at(2))->_len;
    bool result_is_void = false;

    _stack->pop_back(2);

    if (first < 0) first = 0;
    if (last < 0) last = 0;
    if (first > len && last > len)
        result_is_void = true;
    else {
        if (first > len) first = len - 1;
        if (last > len) last = len - 1;
        if (first > last) result_is_void = true;
    }

    if (!result_is_void) {
        unsigned int str_size = last - first + 1;
        ostring* str = (ostring*)_calc_stack.allocate_back(str_size + 1 + sizeof(ostring), cmd_string);
        str->_len = str_size;

        memcpy(((ostring*)_calc_stack.back())->_value, ((ostring*)_stack->at(0))->_value + first, str_size);
        ((ostring*)_calc_stack.back())->_value[str_size] = 0;

        _stack->pop_back();
        rpnstack::copy_and_push_back(_calc_stack, _calc_stack.size() - 1, *_stack);
        _calc_stack.pop_back();
    } else {
        _stack->pop_back();
        ostring* str = (ostring*)_stack->allocate_back(1 + sizeof(ostring), cmd_string);
        str->_len = 0;
        str->_value[0] = 0;
    }
#endif
}
