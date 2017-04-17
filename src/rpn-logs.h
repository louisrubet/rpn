#define _USE_MATH_DEFINES

//
void rpn_e(void)
{
    //TODO
    //putf(M_E);
}

void rpn_log()
{
    //TODO
    //MIN_ARGUMENTS(1);
    //ARG_MUST_BE_OF_TYPE(0, cmd_number);
    //((number*)_stack->get_obj(0))->_value = log10(((number*)_stack->get_obj(0))->_value);
}

void rpn_alog()
{
    //TODO
    //MIN_ARGUMENTS(1);
    //ARG_MUST_BE_OF_TYPE(0, cmd_number);
    //((number*)_stack->get_obj(0))->_value = pow(((number*)_stack->get_obj(0))->_value, 10);
}

void rpn_ln()
{
    //TODO
    //MIN_ARGUMENTS(1);
    //ARG_MUST_BE_OF_TYPE(0, cmd_number);
    //((number*)_stack->get_obj(0))->_value = log(((number*)_stack->get_obj(0))->_value);
}

void rpn_exp()
{
    //TODO
    //MIN_ARGUMENTS(1);
    //ARG_MUST_BE_OF_TYPE(0, cmd_number);
    //((number*)_stack->get_obj(0))->_value = exp(((number*)_stack->get_obj(0))->_value);
}

void rpn_sinh()
{
    //TODO
    //MIN_ARGUMENTS(1);
    //ARG_MUST_BE_OF_TYPE(0, cmd_number);
    //((number*)_stack->get_obj(0))->_value = sinh(((number*)_stack->get_obj(0))->_value);
}

void rpn_asinh()
{
    //TODO
    //MIN_ARGUMENTS(1);
    //ARG_MUST_BE_OF_TYPE(0, cmd_number);

    //floating_t value = ((number*)_stack->get_obj(0))->_value;
    //if(value>0)
    //    value = log(value + sqrt(value * value + 1));
    //else
    //    value = -log(-value + sqrt(value * value + 1));

    //((number*)_stack->get_obj(0))->_value = value;
}

void rpn_cosh()
{
    //TODO
    //MIN_ARGUMENTS(1);
    //ARG_MUST_BE_OF_TYPE(0, cmd_number);
    //((number*)_stack->get_obj(0))->_value = cosh(((number*)_stack->get_obj(0))->_value);
}

void rpn_acosh()
{
    //TODO
    //MIN_ARGUMENTS(1);
    //ARG_MUST_BE_OF_TYPE(0, cmd_number);

    //floating_t value = ((number*)_stack->get_obj(0))->_value;
    //if(value>0)
    //    value = log(value + sqrt(value * value - 1));
    //else
    //    value = -log(-value + sqrt(value * value - 1));

    //((number*)_stack->get_obj(0))->_value = value;
}

void rpn_tanh()
{
    //TODO
    //MIN_ARGUMENTS(1);
    //ARG_MUST_BE_OF_TYPE(0, cmd_number);
    //((number*)_stack->get_obj(0))->_value = tanh(((number*)_stack->get_obj(0))->_value);
}

void rpn_atanh()
{
    //TODO
    //MIN_ARGUMENTS(1);
    //ARG_MUST_BE_OF_TYPE(0, cmd_number);

    //floating_t value = ((number*)_stack->get_obj(0))->_value;
    //value = (log(1 + value) - log(1 - value)) / 2;
    //((number*)_stack->get_obj(0))->_value = value;
}
