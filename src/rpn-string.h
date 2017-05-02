void instr()
{
    MIN_ARGUMENTS(1);

    // stringify only if not already a string
    if (_stack->get_type(0) != cmd_string)
    {
        // write the object in stack(0) in a string and remove this obj
        stringstream out;
        ((object*)_stack->pop_back())->show(out);

        // reserve the correct size in stack
        unsigned int str_size = (unsigned int)out.str().size();
        ostring* str = (ostring*)_stack->allocate_back(str_size+1+sizeof(ostring), cmd_string);

        // fill the obj
        str->set(out.str().c_str(), str_size);
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
    {
        // run it
        prog.run(*_stack, *_global_heap, &_local_heap);
    }
}
