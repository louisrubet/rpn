#include <math.h>
#include <string>
using namespace std;

#include "constant.h"
#define MPFR_USE_NO_MACRO
#include <mpfr.h>
#include "object.hpp"

// number statics
number::mode_enum number::s_mode = DEFAULT_MODE;
int number::s_digits = DEFAULT_DECIMAL_DIGITS;
