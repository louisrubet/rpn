#ifndef MPREAL_OUT_HPP_
#define MPREAL_OUT_HPP_

#define MPFR_USE_NO_MACRO
#include <mpfr.h>
#include <mpreal.h>
using namespace mpfr;

#include <string>
#include <ostream>
using namespace std;

ostream& mpreal_output10base(ostream& out, const string& fmt, const mpreal& value);
ostream& mpreal_outputNbase(ostream& out, int base, const mpreal& value);

#endif
