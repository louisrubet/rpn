#define _USE_MATH_DEFINES

#ifdef WIN32
#define M_PI 3.141592653589793238462643383279502797479068098137295573004504331874296718662975536062731407582759857177734375
#define M_E 2.71828182845904523536028747135266231435842186719354886266923086032766716801933881697550532408058643341064453125
#endif

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
