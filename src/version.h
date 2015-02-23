// version and soft name
static const char version[] = "1.3-beta";
static const char uname[] = "rpn v1.3-beta, (c) 2015 <louis@rubet.fr>";

// syntax
static const char* syntax[] = {
	"Reverse Polish Notation language, based on hewlett-Packard RPL",
	"Syntax: rpn [command]",
	"with optional command = list of commands",
	NULL
};

static const char prompt[] = ATTR_BOLD "rpn" ATTR_OFF "> ";
