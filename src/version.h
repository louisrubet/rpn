// version and soft name
static const char version[] = "2.0 RC1";
static const char uname[] =
    "rpn v2.0 RC1, (c) 2013 <louis@rubet.fr>, GNU General Public License";

static const char g_cursor[] = "> ";
static const string g_show_stack_separator = "> ";

// description
static const char description[] =
    ATTR_BOLD "R" ATTR_OFF "everse "
    ATTR_BOLD "P" ATTR_OFF "olish "
    ATTR_BOLD "N" ATTR_OFF "otation language, based on "
    "Hewlett-Packard RPL language\n"
    "using MPFR library v";
static const char description_more[] =
    " released under the GNU Lesser General Public License,\n"
    "version 3 or any later version\n";

// syntax
static const char syntax[] =
    ATTR_BOLD "Syntax" ATTR_OFF ": rpn [command]\n"
    "with optional command = list of commands";

static const char prompt[] = ATTR_BOLD "rpn" ATTR_OFF "> ";
