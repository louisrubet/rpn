#include <string>
#include <ostream>
using namespace std;

#define MPFR_USE_NO_MACRO
#include <mpfr.h>
#include <mpreal.h>
using namespace mpfr;

ostream& mpreal_output10base(ostream& out, const string& fmt, const mpreal& value);
ostream& mpreal_outputNbase(ostream& out, int base, const mpreal& value);
