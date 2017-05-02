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
    binary* bin = (binary*)_stack->allocate_back(sizeof(binary), cmd_binary);
    bin->set(mpfr_get_sj(left->_value.mpfr, s_mpfr_rnd));
}

void btor()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_binary);

    integer_t bin = getb();
    number* left = (number*)_stack->allocate_back(number::calc_size(), cmd_number);
    left->set(bin);
}
