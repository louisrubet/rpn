// version and soft name
#define RPN_VERSION "2.3.2"
static const char version[] = RPN_VERSION;
static const char uname[] = "rpn v" RPN_VERSION ", (c) 2017 <louis@rubet.fr>, GNU LGPL v3\n";

#define STRINGIFY(a) STRINGIFY_ONE(a)
#define STRINGIFY_ONE(a) #a

// description
static const char description[] = ATTR_BOLD
    "R" ATTR_OFF "everse " ATTR_BOLD "P" ATTR_OFF "olish " ATTR_BOLD "N" ATTR_OFF
    "otation language\n\n"
    "using " ATTR_BOLD "GMP" ATTR_OFF
    " v" STRINGIFY(__GNU_MP_VERSION) "." STRINGIFY(__GNU_MP_VERSION_MINOR) "." STRINGIFY(
        __GNU_MP_VERSION_PATCHLEVEL) " under GNU LGPL\n" ATTR_BOLD "MPFR" ATTR_OFF " v" MPFR_VERSION_STRING
                                     " under GNU LGPL\n"
                                     "and " ATTR_BOLD "linenoise-ng" ATTR_OFF " v" LINENOISE_VERSION " under BSD\n";

// syntax
static const char syntax[] = ATTR_BOLD "Syntax" ATTR_OFF
                                       ": rpn [command]\n"
                                       "with optional command = list of commands";
