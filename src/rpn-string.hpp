void instr()
{
    MIN_ARGUMENTS(1);

    // stringify only if not already a string
    if (_stack->get_type(0) != cmd_string)
    {
        // write the object in stack(0) in a string and remove this obj
        FILE* tmp = tmpfile();
        if (tmp != NULL)
        {
            ((object*)_stack->pop_back())->show(tmp);

            // reserve the correct size on stack
            unsigned int str_size = (unsigned int)ftell(tmp);
            ostring* str = (ostring*)_stack->allocate_back(str_size+1+sizeof(ostring), cmd_string);
            str->_size = str_size+1;

            // fill the obj
            rewind(tmp);
            if (fread(str->_value, str_size, 1, tmp) != 1)
                ERR_CONTEXT(ret_runtime_error);
            str->_value[str_size] = 0;
            fclose(tmp);
        }
        else
            ERR_CONTEXT(ret_runtime_error);
    }
}

void strout()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_string);

    string entry(((ostring*)_stack->pop_back())->_value);

    program prog;

    // make program from string in stack level 1
    if (program::parse(entry.c_str(), prog) == ret_ok)
        // run it
        prog.run(*_stack, *_heap);
}

void chr()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    // get arg as number % 256
    char the_chr = (char)mpfr_get_d(((number*)_stack->pop_back())->_value.mpfr, floating_t::s_mpfr_rnd);
    if (the_chr<32 || the_chr>126)
        the_chr = '.';

    // reserve the correct size on stack (1 char)
    unsigned int str_size = 1;
    ostring* str = (ostring*)_stack->allocate_back(str_size+1+sizeof(ostring), cmd_string);
    str->_len = str_size;
    str->_value[0] = the_chr;
    str->_value[1] = 0;
}

void num()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_string);

    double the_chr = (double)((ostring*)_stack->pop_back())->_value[0];
    number* numb = (number*)_stack->allocate_back(number::calc_size(), cmd_number);
    numb->_value = the_chr;
}
