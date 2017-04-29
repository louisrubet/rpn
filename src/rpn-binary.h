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

    number* left = (number*)_stack->pop_back();

    binary bin;
    bin.set(mpfr_get_sj(left->_value.mpfr, s_mpfr_rnd));
    _stack->push_back(&bin, bin.size(), cmd_binary);
}

void btor()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_binary);

    integer_t bin = getb();

    void* significand;
    number* left = (number*)_stack->allocate_back(sizeof(number), cmd_number, MPFR_128BITS_STORING_LENGTH, &significand);
    left->init(significand);
    left->set(bin);
}
