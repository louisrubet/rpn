// version and soft name
static const char version[] = "2.1";
static const char uname[] =
    "rpn v2.1, (c) 2017 <louis@rubet.fr>, GNU LGPL v3";

// description
static const char description[] =
    ATTR_BOLD "R" ATTR_OFF "everse "
    ATTR_BOLD "P" ATTR_OFF "olish "
    ATTR_BOLD "N" ATTR_OFF "otation language, based on "
    "Hewlett-Packard RPL language\n\n"
    "using " ATTR_BOLD "MPFR library" ATTR_OFF " v" MPFR_VERSION_STRING " under GNU LGPL v3\n"
    "for multiple-precision floating-point computations with correct rounding\n\n"
    "and " ATTR_BOLD "linenoise-ng" ATTR_OFF " v" LINENOISE_VERSION " under BSD license\n"
    "for line editing, autocompletion and history\n";
    
// syntax
static const char syntax[] =
    ATTR_BOLD "Syntax" ATTR_OFF ": rpn [command]\n"
    "with optional command = list of commands";
