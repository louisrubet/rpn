// version and soft name
#define RPN_VERSION "2.4"
static const string version{RPN_VERSION};
static const string uname{"rpn v" RPN_VERSION ", (c) 2017 <louis@rubet.fr>, GNU LGPL v3"};

#define XSTR(a) STR(a)
#define STR(a) #a

// description
static const string description{
    ATTR_BOLD "R" ATTR_OFF "everse " ATTR_BOLD "P" ATTR_OFF "olish " ATTR_BOLD "N" ATTR_OFF
              "otation language\n\n"
              "using " ATTR_BOLD "GMP" ATTR_OFF
              " v" XSTR(__GNU_MP_VERSION) "." XSTR(__GNU_MP_VERSION_MINOR) "." XSTR(
                  __GNU_MP_VERSION_PATCHLEVEL) " under GNU LGPL\n" ATTR_BOLD "MPFR" ATTR_OFF " v" MPFR_VERSION_STRING
                                               " under GNU LGPL\n"
                                               "and " ATTR_BOLD "linenoise-ng" ATTR_OFF " v" LINENOISE_VERSION
                                               " under BSD\n"};

// syntax
static const string syntax{ATTR_BOLD "Syntax" ATTR_OFF
                               ": rpn [command]\n"
                               "with optional command = list of commands"};
