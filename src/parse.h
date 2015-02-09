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
#if 0
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
#endif
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
    if (get_fn(entry, fn, type) == ret_ok)
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

static ret_value parse_line(const string& entry, program& prog)
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
