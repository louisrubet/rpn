#include <regex>

#include "program.hpp"

/// @brief completion callback as asked by linenoise-ng
/// this is called by linenoise-ng whenever the user enters TAB
///
/// @param text the text after wich the user entered TAB
/// @param lc the completion object to add strings with linenoiseAddCompletion()
///
void program::entry_completion_generator(const char* text, linenoiseCompletions* lc) {
    int i = 0;
    int text_len = strnlen(text, 6);

    // propose all keywords
    if (text_len == 0) {
        while (program::s_keywords[i].type != cmd_max) {
            if (program::s_keywords[i].fn != NULL)
                // add all keywords
                linenoiseAddCompletion(lc, program::s_keywords[i].name);
            i++;
        }
    }
    // propose keywords matching to text begining
    else {
        while (program::s_keywords[i].type != cmd_max) {
            if (program::s_keywords[i].fn != NULL) {
                // compare list entry with text, return if match
                if (strncmp(program::s_keywords[i].name, text, text_len) == 0)
                    linenoiseAddCompletion(lc, program::s_keywords[i].name);
            }
            i++;
        }
    }
}

/// @brief interactive entry and decoding
///
/// @param prog the program to add entered objects
/// @return ret_value see this type
///
ret_value program::entry(program& prog) {
    string entry_str;
    char* entry;
    int entry_len;
    int total_entry_len;
    ret_value ret = ret_max;
    bool multiline = false;

    // linenoise for entry
    linenoiseSetCompletionCallback(entry_completion_generator);

    while (ret == ret_max) {
        // get user entry
        if (multiline)
            entry = linenoise(MULTILINE_PROMPT, &entry_len);
        else
            entry = linenoise(PROMPT, &entry_len);

        // Ctrl-C
        if (linenoiseKeyType() == 1) {
            if (entry_len > 0 || multiline)
                ret = ret_abort_current_entry;
            else
                ret = ret_good_bye;
        } else if (linenoiseKeyType() == 3) {
            multiline = true;
            if (entry != NULL) entry_str += entry;
            entry_str += " ";
        } else {
            if (entry != NULL) {
                entry_str += entry;

                // parse it
                ret = parse(entry_str, prog);

                // keep history
                if (entry[0] != 0) (void)linenoiseHistoryAdd(entry_str.c_str());
            } else
                ret = ret_internal;
        }

        free(entry);
    }

    return ret;
}

struct SynElement {
    cmd_type_t type;
    string value;
    mpreal re;
    mpreal im;
    int reBase;
    int imBase;
    program_fn_t fn;
    bool autoEval;
};

struct SynError {
    size_t indx;
    string err;
};

struct ReservedWord {
    cmd_type_t type;
    program_fn_t fn;
};

static map<string, ReservedWord> _keywordsMap;

static void trim(string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }));
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), s.end());
}

static bool parseString(string& entry, size_t idx, size_t& nextIdx, vector<SynError>& errors,
                        vector<SynElement>& elements) {
    // here we are sure that entry[0] is at least '"'
    for (size_t i = idx + 1; i < entry.size(); i++) {
        if (entry[i] == '"') {
            if (entry[i] - 1 != '\\') {
                elements.push_back({cmd_string, .value = entry.substr(idx + 1, i - idx - 1)});
                nextIdx = i + 1;
                return true;
            }
        }
    }
    elements.push_back({cmd_string, .value = entry.substr(idx + 1, entry.size() - idx - 1)});
    nextIdx = entry.size();
    return true;
}

static bool parseSymbol(string& entry, size_t idx, size_t& nextIdx, vector<SynError>& errors,
                        vector<SynElement>& elements) {
    // here we are sure that entry[0] is at least '\''
    for (size_t i = idx + 1; i < entry.size(); i++) {
        if (entry[i] == '\'') {
            elements.push_back({cmd_symbol, .value = entry.substr(idx + 1, i - idx - 1), .autoEval = false});
            nextIdx = i + 1;
            return true;
        }
    }
    elements.push_back({cmd_symbol, .value = entry.substr(idx + 1, entry.size() - idx - 1)});
    nextIdx = entry.size();
    return true;
}

static bool parseProgram(string& entry, size_t idx, size_t& nextIdx, vector<SynError>& errors,
                         vector<SynElement>& elements) {
    // here we are sure that entry is at least "<<"
    // find last ">>" or "»"
    int countNested = 0;
    for (size_t i = idx + 2; i < entry.size() - 1; i++) {
        if ((entry[i] == '<' && entry[i + 1] == '<') || (entry.substr(i, 2) == "«"))
            countNested++;
        else if ((entry[i] == '>' && entry[i + 1] == '>') || (entry.substr(i, 2) == "»")) {
            if (countNested == 0) {
                string prg = entry.substr(idx + 2, i - idx - 2);
                trim(prg);
                elements.push_back({cmd_program, .value = prg});
                nextIdx = i + 2;
                return true;
            } else
                countNested--;
        }
    }
    string prg = entry.substr(idx + 2, entry.size() - idx - 2);
    trim(prg);
    elements.push_back({cmd_program, .value = prg});
    nextIdx = entry.size();
    return true;
}

static int getBase(string& entry, int idxStart, bool& positive) {
    regex baseRegex("([+-])?((0[xX])|([0-9][0-9]?[bB]))");
    smatch match;
    if (regex_search(entry, match, baseRegex) && match.size() >= 5) {
        string sign = match[1].str();
        string base = match[2].str();
        // sign out, permits expressions like -0xAB3F
        positive = sign.size() > 0 && sign[0] == '-' ? false : true;
        // base
        entry = entry.substr(base.size() + sign.size());
        if (base[1] == 'X' || base[1] == 'x') return 16;
        if (base.size() > 0) {
            int b = stoi(base.substr(0, base.size() - 1));
            if (b == 0) b = 2;  // admit "0b" as binary suffix
            return b;
        }
    }
    return 10;
}

static bool getNumberAt(string& entry, size_t idx, size_t& nextIdx, int& base, mpreal& r, char delim = ' ') {
    stringstream ss;
    int idxNumber = 0;
    string token;
    bool positive = true;

    ss.str(entry.substr(idx));
    if (getline(ss, token, delim)) {
        nextIdx = token.size() + idx + 1;
        base = getBase(token, idx, positive);
        if (base < BASE_MIN || base > BASE_MAX) return false;
        trim(token);
        if (mpfr_set_str(r.mpfr_ptr(), token.c_str(), base, mpreal::get_default_rnd()) == 0) {
            if (!positive) r = -r;
            return true;
        } else
            return false;
    }
    nextIdx = token.size() + idx + 1;
    return false;
}

static bool parseNumber(string& entry, size_t idx, size_t& nextIdx, vector<SynError>& errors,
                        vector<SynElement>& elements) {
    mpreal r;
    int base = 10;
    if (getNumberAt(entry, idx, nextIdx, base, r)) {
        elements.push_back({cmd_number, .re = r, .reBase = base});
        return true;
    } else {
        errors.push_back({entry.size(), "unterminated number"});
        return false;
    }
}

static bool parseComplex(string& entry, size_t idx, size_t& nextIdx, vector<SynError>& errors,
                         vector<SynElement>& elements) {
    mpreal re, im;
    int reBase, imBase = 10;
    if (idx + 1 == entry.size()) {
        elements.push_back({cmd_symbol, .value = entry.substr(idx, entry.size() - idx)});
        nextIdx = entry.size();
        return true;  // complex format error, return a symbol
    }
    if (!getNumberAt(entry, idx + 1, nextIdx, reBase, re, ',')) {
        elements.push_back({cmd_symbol, .value = entry.substr(idx, entry.size() - idx)});
        nextIdx = entry.size();
        return true;  // complex format error, return a symbol
    }

    size_t i = nextIdx;
    //while (i < entry.size() && entry[i] != ',') i++;
    if (i >= entry.size()) {
        elements.push_back({cmd_symbol, .value = entry.substr(idx, entry.size() - idx)});
        nextIdx = entry.size();
        return true;  // complex format error, return a symbol
    }

    if (!getNumberAt(entry, i, nextIdx, imBase, im, ')')) {
        elements.push_back({cmd_symbol, .value = entry.substr(idx, entry.size() - idx)});
        nextIdx = entry.size();
        return true;  // complex format error, return a symbol
    }
    elements.push_back({cmd_complex, .re = re, .im = im, .reBase = reBase, .imBase = imBase});
    nextIdx++;
    return true;
}

static bool parseReserved(string& entry, size_t idx, size_t& nextIdx, vector<SynElement>& elements,
                          map<string, ReservedWord>& keywords) {
    stringstream ss(entry.substr(idx));
    string token;
    ss >> token;

    auto resa = keywords.find(token);
    if (resa != keywords.end()) {
        elements.push_back({resa->second.type, .value = token, .fn = resa->second.fn});
        nextIdx = token.size() + idx;
        return true;
    }
    return false;
}

static bool parseUnknown(string& entry, size_t idx, size_t& nextIdx, vector<SynElement>& elements) {
    stringstream ss(entry.substr(idx));
    string token;
    ss >> token;
    elements.push_back({cmd_symbol, .value = token, .autoEval = true});
    nextIdx = token.size() + idx;
    return true;
}

/// @brief lexical analysis of an entry string
///
/// @param entry the entry string
/// @param elements syntax elements vector
/// @param errors errors vector
/// @param keywords reserved keywords
/// @return false=errors, the lexer must stop
///
static bool lexer(string& entry, vector<SynElement>& elements, vector<SynError>& errors,
                  map<string, ReservedWord>& keywords) {
    size_t jump;
    for (size_t i = 0; i < entry.size(); i++) {
        if (isspace(entry[i])) continue;
        SynElement element;
        switch (entry[i]) {
            case '"':
                if (!parseString(entry, i, jump, errors, elements)) return false;
                i = jump - 1;
                continue;
            case '\'':
                if (!parseSymbol(entry, i, jump, errors, elements)) return false;
                i = jump - 1;
                continue;
            case '(':
                if (!parseComplex(entry, i, jump, errors, elements)) return false;
                i = jump - 1;
                continue;
        }
        if (i < entry.size() - 1 && (entry.substr(i, 2) == "<<" || entry.substr(i, 2) == "«")) {
            if (!parseProgram(entry, i, jump, errors, elements)) return false;
            i = jump - 1;
            continue;
        } else if (parseReserved(entry, i, jump, elements, keywords)) {
            // found a keywords word, add it with its correct type
            i = jump - 1;
            continue;
        } else if (parseNumber(entry, i, jump, errors, elements)) {
            i = jump - 1;
            continue;
        }
        if (parseUnknown(entry, i, jump, elements))
            // last chance, this unknown entry is treated as a symbol
            i = jump - 1;
    }

    return true;
}

static bool progFromElements(vector<SynElement>& elements, program& prog) {
    // TODO control elements creation
    for (SynElement& element : elements) {
        switch (element.type) {
            case cmd_number:
                prog.push_back(new number(element.re, element.reBase));
                break;
            case cmd_complex:
                prog.push_back(new ocomplex(element.re, element.im, element.reBase, element.imBase));
                break;
            case cmd_string:
                prog.push_back(new ostring(element.value));
                break;
            case cmd_symbol:
                prog.push_back(new symbol(element.value, element.autoEval));
                break;
            case cmd_program:
                prog.push_back(new oprogram(element.value));
                break;
            case cmd_keyword:
                prog.push_back(new keyword(element.fn, element.value));
                break;
            case cmd_branch:
                prog.push_back(new branch((branch_fn_t)element.fn, element.value));
                break;
            default:
                return false;
        }
    }
    return true;
}

/// @brief parse an entry string: cut it into objects chunks and add them to a program
///
/// @param entry the entry string
/// @param prog the program to be filled
/// @return ret_value see this type
///
ret_value program::parse(string& entry, program& prog) {
    vector<SynElement> elements;
    vector<SynError> errors;
    ret_value ret = ret_ok;

    // prepare map for finding reserved keywords
    if (_keywordsMap.size() == 0)
        for (int i = 0; s_keywords[i].type != cmd_max; i++)
            _keywordsMap[s_keywords[i].name] = {s_keywords[i].type, s_keywords[i].fn};

    // separate the entry string
    if (lexer(entry, elements, errors, _keywordsMap)) {
        // make objects from parsed elements
        if (!progFromElements(elements, prog)) prog.show_error(ret_unknown_err, "error creating program from entry");
    } else
        for (SynError& err : errors) prog.show_syntax_error(err.err.c_str());

    return ret;
}
