static bool _cut(const string& entry, vector<string>& entries)
{
    string tmp;

    for (int i=0; i<entry.size(); i++)
    {
        switch(entry.at(i))
        {
            //symbol
            case '\'':
                //push prec entry if exists
                if (tmp.size()>0)
                {
                    entries.push_back(tmp);
                    tmp.clear();
                }
                //get symbol
                tmp='\'';
                i++;
                while((i<entry.size()) && (isalnum(entry.at(i)) || entry.at(i)=='_'))
                    tmp+=entry.at(i++);
                if ((i<entry.size()) && entry.at(i)=='\'')
                    tmp+='\'';
                entries.push_back(tmp);
                tmp.clear();
                break;

            //string
            case '"':
                //push prec entry if exists
                if (tmp.size()>0)
                {
                    entries.push_back(tmp);
                    tmp.clear();
                }
                //get expression
                tmp='"';
                i++;
                while(i<entry.size() && entry.at(i)>=' ')
                {
                    tmp+=entry.at(i);
                    if (entry.at(i++) == '"')
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
                if (i<(entry.size()-1) && entry.at(i+1)=='<')
                {
                    //get expression
                    i+=2;
                    tmp = "<<";
                    // trim leading spaces
                    while (i<entry.size() && (entry.at(i)==' ' || entry.at(i)=='\t'))
                        i++;                    
                    //get the rest
                    while(i<entry.size())
                    {
                        tmp+=entry.at(i);
                        i++;
                    }
                    entries.push_back(tmp);
                    tmp.clear();
                }
                else
                {
                    // reinject < which is not a program begin
                    tmp = "<";
                }
                break;

            //other
            default:
                if (entry.at(i)!=' ' && entry.at(i)!='\t')
                {
                    tmp+=entry.at(i);
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
            obj = new keyword(fn, (string&)entry);
            obj_size = sizeof(keyword);
            ret = true;
        }
        else if (type == cmd_branch)
        {
            obj = new branch((branch_fn_t)fn, (string&)entry);
            obj_size = sizeof(branch);
            ret = true;
        }
    }

    return ret;
}

static bool get_symbol(const string& entry, object*& obj)
{
    bool ret = false;
    int len = entry.size();
    if (len>1 && entry[0]=='\'')
    {
        int last = entry[len-1]=='\''?(len-2):(len-1);
        obj = new symbol(entry.substr(1, last).c_str());
        ret = true;
    }
    return ret;
}

static bool get_string(const string& entry, object*& obj)
{
    bool ret = false;
    int len = entry.size();
    if (len>1 && entry[0]=='\"')
    {
        int last = entry[len-1]=='\"'?(len-2):(len-1);
        obj = new ostring(entry.substr(1, last).c_str());
        ret = true;
    }
    return ret;
}

static bool get_program(const string& entry, object*& obj)
{
    bool ret = false;
    int len = entry.size();
    if (len>=2 && entry[0]=='<' && entry[1]=='<')
    {
        int last;
        if (len>=4 && entry[len-1]=='>' && entry[len-2]=='>')
            last = len-4;
        else
            last = len-2;
        obj = new oprogram(entry.substr(2, last).c_str());
        ret = true;
    }
    return ret;
}

static bool get_float(const string& entry, object*& obj)
{
    stringstream token;
    floating_t val;
    bool ret = false;

    token<<entry;
    token>>val;

    if ( (!token.fail()) && (!token.bad()) )
    {
        obj = new number(val);
        ret = true;
    }

    return ret;
}

static bool get_binary(const string& entry, object*& obj)
{
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
            obj = new binary(val);
            ret = true;
        }
    }

    return ret;
}

static bool get_binary_bin(const string& entry, object*& obj)
{
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
        obj = new binary(val);
        ret = true;
    }

    return ret;
}

static bool _obj_from_string(const string& entry, object*& obj, unsigned int& obj_size, cmd_type_t& type)
{
    bool ret = false;

    if (get_float(entry, obj))
    {
        type = cmd_number;
        obj_size = sizeof(number);
        ret = true;
    }
    else if (get_binary(entry, obj))
    {
        type = cmd_binary;
        obj_size = sizeof(binary);
        ret = true;
    }
    else if (get_binary_bin(entry, obj))
    {
        type = cmd_binary;
        obj_size = sizeof(binary);
        ret = true;
    }
    else if (get_symbol(entry, obj))
    {
        type = cmd_symbol;
        obj_size = sizeof(symbol);
        ret = true;
    }
    else if (get_string(entry, obj))
    {
        type = cmd_string;
        obj_size = sizeof(ostring);
        ret = true;
    }
    else if (get_program(entry, obj))
    {
        type = cmd_program;
        obj_size = sizeof(oprogram);
        ret = true;
    }
    else if (get_keyword(entry, obj, obj_size, type))
    {
        ret = true;
    }
    else
    {
        // nothing, considered as an auto-evaluated symbol
        obj = new symbol((string&)entry);
        ((symbol*)obj)->_auto_eval = true;
        obj_size = sizeof(symbol);
        type = cmd_symbol;
        ret = true;
    }

    return ret;
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

static ret_value parse(const string& entry, program& prog)
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
            if(_obj_from_string((*it), obj, obj_size, type))
            {
                prog.push_back(obj, obj_size, type);
            }
            else
            {
                // no syntax error for now, an unknown obj form is considered as a symbol
            }
        }
    }

    return ret;
}

// interactive entry and decoding
static ret_value entry(program& prog)
{
    char *buf;
    ret_value ret;

    // declare completion fn (bound to '\t' by default)
    rl_completion_entry_function = entry_completion_generator;

    // get user entry
    buf = readline(prompt);
    if (buf != NULL)
    {
        string entry(buf);

        // parse it
        ret = parse(entry, prog);

        // keep history
        if (buf[0]!=0)
            add_history(buf);
    }
    else
        ret = ret_internal;

    free(buf);
}
