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
    //TODO
    //MIN_ARGUMENTS(1);
    //ARG_MUST_BE_OF_TYPE(0, cmd_number);

    //binary bin;
    //bin.set(((integer_t)getf()));
    //_stack->push_back(&bin, bin.size(), cmd_binary);
}

void btor()
{
    // TODO
    //MIN_ARGUMENTS(1);
    //ARG_MUST_BE_OF_TYPE(0, cmd_binary);

    //putf((floating_t)getb());
}
