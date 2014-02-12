#define _USE_MATH_DEFINES

//
void pi(void)
{
	putf(M_PI);
}

void d2r(void)
{
	MIN_ARGUMENTS(1);
	ARG_IS_OF_TYPE(0, cmd_number);
	((number*)_stack->get_obj(0))->_value *= M_PI / 360;
}

void r2d(void)
{
	MIN_ARGUMENTS(1);
	ARG_IS_OF_TYPE(0, cmd_number);
	((number*)_stack->get_obj(0))->_value *= 360 / M_PI;
}

void rpn_sin(void)
{
	MIN_ARGUMENTS(1);
	ARG_IS_OF_TYPE(0, cmd_number);
	((number*)_stack->get_obj(0))->_value = sin(((number*)_stack->get_obj(0))->_value);
}

void rpn_asin(void)
{
	MIN_ARGUMENTS(1);
	ARG_IS_OF_TYPE(0, cmd_number);
	((number*)_stack->get_obj(0))->_value = asin(((number*)_stack->get_obj(0))->_value);
}

void rpn_cos(void)
{
	MIN_ARGUMENTS(1);
	ARG_IS_OF_TYPE(0, cmd_number);
	((number*)_stack->get_obj(0))->_value = cos(((number*)_stack->get_obj(0))->_value);
}

void rpn_acos(void)
{
	MIN_ARGUMENTS(1);
	ARG_IS_OF_TYPE(0, cmd_number);
	((number*)_stack->get_obj(0))->_value = acos(((number*)_stack->get_obj(0))->_value);
}

void rpn_tan(void)
{
	MIN_ARGUMENTS(1);
	ARG_IS_OF_TYPE(0, cmd_number);
	((number*)_stack->get_obj(0))->_value = tan(((number*)_stack->get_obj(0))->_value);
}

void rpn_atan(void)
{
	MIN_ARGUMENTS(1);
	ARG_IS_OF_TYPE(0, cmd_number);
	((number*)_stack->get_obj(0))->_value = atan(((number*)_stack->get_obj(0))->_value);
}
