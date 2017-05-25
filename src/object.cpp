#include <string>
using namespace std;

#include "constant.h"
#include "object.hpp"

// floating_t statics
mpfr_prec_t floating_t::s_mpfr_prec = MPFR_DEFAULT_PREC_BITS;
mpfr_rnd_t floating_t::s_mpfr_rnd = MPFR_DEFAULT_RND;
unsigned int floating_t::s_mpfr_prec_bytes = MPFR_DEFAULT_STORING_LENGTH_BYTES;
const char* floating_t::s_mpfr_rnd_str[5] = MPFR_RND_STRINGS;

// number statics
number::mode_enum number::s_mode = DEFAULT_MODE;
int number::s_current_precision = DEFAULT_PRECISION;
string number::s_mpfr_printf_format = string(MPFR_DEFAULT_FORMAT);

//
const char* object::s_cmd_type_string[cmd_max] = CMD_TYPE_STRINGS;

void object::show(FILE* stream)
{
    switch(_type)
    {
    case cmd_number:
        switch(((number*)this)->_representation)
        {
            case number::dec:
                mpfr_fprintf(stream, number::s_mpfr_printf_format.c_str(), ((number*)this)->_value.mpfr);
                break;
            case number::hex:
                mpfr_fprintf(stream, string(MPFR_FORMAT_HEX).c_str(), ((number*)this)->_value.mpfr);                
                break;
            case number::bin:
                fprintf(stream, "<binary representation TODO>\n");
        }
        break;
    case cmd_string:
        fprintf(stream, "\"%s\"", ((ostring*)this)->_value);
        break;
    case cmd_program:
        fprintf(stream, "<<%s>>", ((oprogram*)this)->_value);
        break;
    case cmd_symbol:
        fprintf(stream, "'%s'", ((symbol*)this)->_value);
        break;
    case cmd_keyword:
    case cmd_branch:
        fprintf(stream, "%s", ((keyword*)this)->_value);
        break;
    default:
        fprintf(stream, "< unknown object representation >");
        break;
    }
}
