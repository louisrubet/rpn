// version and soft name
static const char version[] = "1.3.2";
static const char uname[] = "rpn v1.3.2, (c) 2015 <louis@rubet.fr>";

static const char g_cursor[] = "> ";
static const string g_show_stack_separator = "> ";

// syntax
static const char* syntax[] = {
	"Reverse Polish Notation language, based on hewlett-Packard RPL",
	"Syntax: rpn [command]",
	"with optional command = list of commands",
	NULL
};

static const char prompt[] = ATTR_BOLD "rpn" ATTR_OFF "> ";
