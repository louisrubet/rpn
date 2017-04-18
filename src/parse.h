// keywords declaration
struct keyword_t
{
    cmd_type_t type;
    char name[24];
    program_fn_t fn;
    string comment;
};
static const int g_max_commands = 128;
static keyword_t _keywords[g_max_commands];

static ret_value get_fn(const char* fn_name, program_fn_t& fn, cmd_type_t& type)
{
    for(unsigned int i=0; (i<sizeof(_keywords)/sizeof(_keywords[0])) && (_keywords[i].type != cmd_max); i++)
    {
        if ((strnlen(_keywords[i].name, sizeof(_keywords[i].name))>0)
            && (strncmp(fn_name, _keywords[i].name, sizeof(_keywords[i].name)) == 0))
        {
            fn = _keywords[i].fn;
            type = _keywords[i].type;
            return ret_ok;
        }
    }
    return ret_unknown_err;
}

static bool _cut(const char* entry, vector<string>& entries)
{
    string tmp;
    //TODO borner
    int len = strlen(entry);

    for (int i=0; i < len; i++)
    {
        switch(entry[i])
        {
            //symbol
            case '\'':
                //push prec entry if exists
                if (tmp.size() > 0)
                {
                    entries.push_back(tmp);
                    tmp.clear();
                }
                //get symbol
                tmp='\'';
                i++;
                while((i < len) && (isalnum(entry[i]) || entry[i]=='_'))
                    tmp += entry[i++];
                if ((i < len) && entry[i] == '\'')
                    tmp += '\'';
                entries.push_back(tmp);
                tmp.clear();
                break;

            //string
            case '"':
                //push prec entry if exists
                if (tmp.size() > 0)
                {
                    entries.push_back(tmp);
                    tmp.clear();
                }
                //get expression
                tmp='"';
                i++;
                while(i < len && entry[i] >= ' ')
                {
                    tmp += entry[i];
                    if (entry[i++] == '"')
                        break;
                }
                entries.push_back(tmp);
                tmp.clear();
                break;

            //program
            case '<':
                //push prec entry if exists
                if (tmp.size()>0)
                {
                    entries.push_back(tmp);
                    tmp.clear();
                }

                if (strncmp(&entry[i], "<<", 2) == 0)
                {
                    int up = 1;

                    // found a program begin
                    i += 2;
                    tmp = "<< ";

                    // trim leading spaces
                    while (i < len && (entry[i]==' ' || entry[i]=='\t'))
                        i++;

                    while(i < len)
                    {
                        if (strncmp(&entry[i], "<<", 2) == 0)
                        {
                            up++;
                            i += 2;
                            tmp += " << ";
                            // trim leading spaces
                            while (i < len && (entry[i] == ' ' || entry[i] == '\t'))
                                i++;
                        }
                        else if (strncmp(&entry[i], ">>", 2) == 0)
                        {
                            up--;
                            i += 2;
                            tmp += " >>";

                            // trim trailing spaces
                            while (i < len && (entry[i] == ' ' || entry[i] == '\t'))
                                i++;
                            // found end
                            if (up == 0)
                                break;
                        }
                        else
                        {
                            tmp += entry[i];
                            i++;
                        }
                    }
                    while((up--)>0)
                    {
                        tmp += " >>";
                    }
                    if (tmp.size()>0)
                    {
                        entries.push_back(tmp);
                        tmp.clear();
                    }
                    i--;// i has move 1 too far
                }
                else
                {
                    // reinject '<'' which is not a prog begin
                    tmp = "<";
                }
                break;

            //other
            default:
                if (entry[i] != ' ' && entry[i] != '\t')
                {
                    tmp += entry[i];
                }
                else
                {
                    if (tmp.size()>0)
                    {
                        entries.push_back(tmp);
                        tmp.clear();
                    }
                }
                break;
        }
    }
    if (tmp.size()>0)
    {
        entries.push_back(tmp);
        tmp.clear();
    }
    return entries.size()>0;
}

static bool get_keyword(const string& entry, object*& obj, unsigned int& obj_size, cmd_type_t& type)
{
    program_fn_t fn;
    bool ret = false;

    // could be a command
    if (get_fn(entry.c_str(), fn, type) == ret_ok)
    {
        if (type == cmd_keyword)
        {
            // allocate keyword object
            obj_size = sizeof(keyword)+entry.size()+1;
            obj = (object*)malloc(obj_size);
            ((keyword*)obj)->set(fn, entry.c_str(), entry.size());
            ret = true;
        }
        else if (type == cmd_branch)
        {
            // allocate branch object
            obj_size = sizeof(branch)+entry.size()+1;
            obj = (object*)malloc(obj_size);
            ((branch*)obj)->set((branch_fn_t)fn, entry.c_str(), entry.size());
            ret = true;
        }
    }

    return ret;
}

static bool get_symbol(const string& entry, object*& obj, unsigned int& obj_len)
{
    bool ret = false;
    int entry_len = entry.size();

    if (entry_len>=1 && entry[0]=='\'')
    {
        // symbol entry, like 'toto'
        if (entry_len == 1)
        {
            // total object length
            obj_len = sizeof(symbol)+1;

            // allocate object
            obj = (symbol*)malloc(obj_len);

            //set it
            ((symbol*)obj)->set("", 0);
        }
        else
        {
            int naked_entry_len;

            // entry length without prefix / postfix
            naked_entry_len = entry[entry_len-1]=='\''?(entry_len-2):(entry_len-1);
            // total object length
            obj_len = sizeof(symbol)+naked_entry_len+1;

            // allocate object
            obj = (symbol*)malloc(obj_len);

            // set it
            ((symbol*)obj)->set(entry.substr(1, naked_entry_len).c_str(), naked_entry_len);
        }
        ret = true;
    }

    return ret;
}

static bool get_other(const string& entry, object*& obj, unsigned int& obj_len, cmd_type_t& type)
{
    bool ret = false;
    int entry_len = entry.size();

    if (entry_len>=1)
    {
        // entry which is nothing is considered as an auto-evaluated symbol
        int naked_entry_len;

        // entry length without prefix / postfix
        naked_entry_len = entry[entry_len-1]=='\''?(entry_len-1):(entry_len);
        // total object length
        obj_len = sizeof(symbol)+naked_entry_len+1;

        // allocate object
        obj = (symbol*)malloc(obj_len);

        // set it
        ((symbol*)obj)->set(entry.c_str(), naked_entry_len);
        ((symbol*)obj)->_auto_eval = true;
        type = cmd_symbol;
        ret = true;
    }

    return ret;
}

static bool get_string(const string& entry, object*& obj, unsigned int& obj_len)
{
    bool ret = false;
    int entry_len = entry.size();
    if (entry_len>=1 && entry[0]=='"')
    {
        if (entry_len == 1)
        {
            // total object length
            obj_len = sizeof(ostring)+1;

            // allocate object
            obj = (ostring*)malloc(obj_len);

            //set it
            ((ostring*)obj)->set("", 0);
        }
        else
        {
            int naked_entry_len;

            // entry length without prefix / postfix
            naked_entry_len = entry[entry_len-1]=='"'?(entry_len-2):(entry_len-1);

            // total object length
            obj_len = sizeof(ostring)+naked_entry_len+1;

            // allocate object
            obj = (ostring*)malloc(obj_len);

            // set it
            ((ostring*)obj)->set(entry.substr(1, naked_entry_len).c_str(), naked_entry_len);
        }
        ret = true;
    }

    return ret;
}

static bool get_program(const string& entry, object*& obj, unsigned int& obj_len)
{
    bool ret = false;
    int entry_len = entry.size();
    if (entry_len>=2 && entry[0]=='<' && entry[1]=='<')
    {
        int naked_entry_len;

        // entry length without prefix / postfix
        if (entry_len>=4 && entry[entry_len-1]=='>' && entry[entry_len-2]=='>')
            naked_entry_len = entry_len-4;
        else
            naked_entry_len = entry_len-2;

        // total object length
        obj_len = sizeof(oprogram)+naked_entry_len+1;

        // allocate object
        obj = (object*)malloc(obj_len);

        // set it
        ((oprogram*)obj)->set(&entry[2], naked_entry_len);

        ret = true;
    }
    return ret;
}

// care: not threadsafe
static bool get_float(const string& entry, object*& obj, unsigned int& obj_size, string& remaining_entry)
{
    static number new_number;
    char* endptr;
    bool ret = false;

    if (entry.size() > 0)
    {
        if ((mpfr_strtofr(&new_number._value.mpfr, entry.c_str(), &endptr, 0, MPFR_DEF_RND) != -1) && (endptr != entry.c_str()))
        {
            obj = &new_number;
            obj_size = sizeof(number);
            ret = true;

            // remaining string if any
            if (endptr != NULL)
                remaining_entry = endptr;
        }
    }

    return ret;
}

// care: not threadsafe
static bool get_binary(const string& entry, object*& obj, unsigned int& obj_size)
{
    static binary new_binary;
    integer_t val;
    bool ret = false;

    if ((entry.size() >= 2) && (entry[0] == '#'))
    {
        stringstream token;
        char type = entry[entry.size() - 1];
        bool syntax;

        switch(type)
        {
            case 'd':
                token<<std::dec<<entry.substr(1);
                syntax = true;
                break;
            case 'h':
                token<<std::hex<<entry.substr(1);
                syntax = true;
                break;
            case 'o':
                token<<std::oct<<entry.substr(1);
                syntax = true;
                break;
            default:
                syntax = false;
                break;
        }

        token>>val;
        if(syntax && !token.fail())
        {
            new_binary.set(val);
            obj = &new_binary;
            obj_size = sizeof(binary);
            ret = true;
        }
    }

    return ret;
}

// care: not threadsafe
static bool get_binary_bin(const string& entry, object*& obj, unsigned int& obj_size)
{
    static binary new_binary;
    integer_t val;
    int len = entry.size();
    bool ret = false;
    
    if ((len > 2) && (entry[0] == '#') && (entry[len - 1] == 'b'))
    {
        integer_t val(0);
        integer_t exponent = (1 << (len-3));
        for(int i=0; i<(len-2); i++)
        {
            if (entry.at(i+1)=='1')
            {
                val+=exponent;
                exponent/=2;
            }
        }
        new_binary.set(val);
        obj = &new_binary;
        obj_size = sizeof(binary);
        ret = true;
    }

    return ret;
}

static bool _obj_from_string(const string& entry, object*& obj, unsigned int& obj_size, cmd_type_t& type, string& remaining_entry)
{
    bool ret = false;
    
    remaining_entry.erase();

    if (get_float(entry, obj, obj_size, remaining_entry))
    {
        type = cmd_number;
        ret = true;
    }
    else if (get_binary(entry, obj, obj_size))
    {
        type = cmd_binary;
        ret = true;
    }
    else if (get_binary_bin(entry, obj, obj_size))
    {
        type = cmd_binary;
        ret = true;
    }
    else if (get_symbol(entry, obj, obj_size))
    {
        type = cmd_symbol;
        ret = true;
    }
    else if (get_string(entry, obj, obj_size))
    {
        type = cmd_string;
        ret = true;
    }
    else if (get_program(entry, obj, obj_size))
    {
        type = cmd_program;
        ret = true;
    }
    else if (get_keyword(entry, obj, obj_size, type))
    {
        ret = true;
    }
    else
    {
        // nothing, considered as an auto-evaluated symbol
        if (get_other(entry, obj, obj_size, type))
        {
            ret = true;
        }
    }

    return ret;
}

static void _delete_obj_from_string(object* obj)
{
    if (obj != NULL)
    {
        switch (obj->_type)
        {
        case cmd_number:
        case cmd_binary:
            break;
        default:
            free(obj);
            break;
        }
    }
}

static char* entry_completion_generator(const char* text, int state)
{
    static int list_index, len, too_far;
 
    if (state == 0)
    {
        list_index = 0;
        too_far = 0;
        len = strlen(text);
    }
 
    while(too_far == 0)
    {
        list_index++;
        if (_keywords[list_index].fn != NULL)
        {
            // compare list entry with text, return if match
            if (strncmp(_keywords[list_index].name, text, len) == 0)
                return entry_completion_dupstr(_keywords[list_index].name);
        }
        else
        {
            // fn=NULL and size=0 = last entry in list -> go out
            if (_keywords[list_index].comment.size() == 0)
                too_far = 1;
        }
    }
 
    /* If no names matched, then return NULL. */
    return (char*)NULL;
 
}

static char* entry_completion_dupstr(char* s)
{
    char* r = (char*)malloc((strlen(s)+1));
    if (r != NULL)
        strcpy(r, s);
    return r;
}

static ret_value parse(const char* entry, program& prog)
{
    vector<string> entries;
    ret_value ret = ret_ok;
    cmd_type_t type;
    unsigned int obj_size;
    object* obj;

    //1. cut global entry string into shorter strings
    if (_cut(entry, entries))
    {
        //2. make an object from each entry, and add it to the program
        for (vector<string>::iterator it = entries.begin(); it != entries.end(); it++)
        {
            string remaining_entry;
            string main_entry = (*it);
            while(main_entry.size()>0)
            {
                // remaining_entry is used only in case of concatenated entry
                // ex:  entry="1 2+" -> vector<string> = {"1", "2+"} -> first "1", second "2" and remaining_entry="+"
                // this remaining entry is treated as an entry
                if(_obj_from_string(main_entry, obj, obj_size, type, remaining_entry))
                {
                    prog.push_back(obj, obj_size, type);
                    //TODO wtf !?
                    //_delete_obj_from_string(obj);
                }
                else
                {
                    // no syntax error for now, an unknown obj form is considered as a symbol
                }
                main_entry = remaining_entry;
            }
        }
    }

    return ret;
}

// interactive entry and decoding
static ret_value entry(program& prog)
{
    char* buf;
    ret_value ret;

    // declare completion fn (bound to '\t' by default)
    rl_completion_entry_function = entry_completion_generator;

    // get user entry
    buf = readline(prompt);
    if (buf != NULL)
    {
        // parse it
        ret = parse(buf, prog);

        // keep history
        if (buf[0]!=0)
            add_history(buf);
    }
    else
        ret = ret_internal;

    free(buf);
}
