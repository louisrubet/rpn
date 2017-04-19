void dec()
{
    binary::s_mode = binary::dec;
}

void hex()
{
    binary::s_mode = binary::hex;
}

void oct()
{
    binary::s_mode = binary::oct;
}

void bin()
{
    binary::s_mode = binary::bin;
}

void rtob()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    number* left = (number*)_stack->back();
    _stack->pop_back();

    cout<<"TODO"<<endl;

    binary bin;
    bin._value = mpfr_get_sj(&left->_value.mpfr, s_mpfr_rnd);
    _stack->push_back(&bin, bin.size(), cmd_binary);
}

void btor()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_binary);

    integer_t bin = getb();

    cout<<"TODO"<<endl;

    _stack->push_back(NULL, sizeof(number), cmd_number, true);
    number* left = (number*)_stack->back();
    left->_value = bin;
    left->ensure_significand();
}
