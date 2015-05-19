#define _USE_MATH_DEFINES

//
void rpn_e(void)
{
    putf(M_E);
}

void rpn_log()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ((number*)_stack->get_obj(0))->_value = log10(((number*)_stack->get_obj(0))->_value);
}

void rpn_alog()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ((number*)_stack->get_obj(0))->_value = pow(((number*)_stack->get_obj(0))->_value, 10);
}

void rpn_ln()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ((number*)_stack->get_obj(0))->_value = log(((number*)_stack->get_obj(0))->_value);
}

void rpn_exp()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ((number*)_stack->get_obj(0))->_value = exp(((number*)_stack->get_obj(0))->_value);
}

void rpn_sinh()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ((number*)_stack->get_obj(0))->_value = sinh(((number*)_stack->get_obj(0))->_value);
}

void rpn_asinh()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    floating_t value = ((number*)_stack->get_obj(0))->_value;
    if(value>0)
        value = log(value + sqrt(value * value + 1));
    else
        value = -log(-value + sqrt(value * value + 1));

    ((number*)_stack->get_obj(0))->_value = value;
}

void rpn_cosh()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ((number*)_stack->get_obj(0))->_value = cosh(((number*)_stack->get_obj(0))->_value);
}

void rpn_acosh()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    floating_t value = ((number*)_stack->get_obj(0))->_value;
    if(value>0)
        value = log(value + sqrt(value * value - 1));
    else
        value = -log(-value + sqrt(value * value - 1));

    ((number*)_stack->get_obj(0))->_value = value;
}

void rpn_tanh()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);
    ((number*)_stack->get_obj(0))->_value = tanh(((number*)_stack->get_obj(0))->_value);
}

void rpn_atanh()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_number);

    floating_t value = ((number*)_stack->get_obj(0))->_value;
    value = (log(1 + value) - log(1 - value)) / 2;
    ((number*)_stack->get_obj(0))->_value = value;
}
