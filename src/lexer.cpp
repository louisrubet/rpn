#include <regex>
using namespace std;

#include "lexer.hpp"

bool Lexer::lexer(string& entry, map<string, ReservedWord>& keywords, vector<SynElement>& elements,
                  vector<SynError>& errors) {
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

void Lexer::trim(string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }));
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), s.end());
}

bool Lexer::parseString(string& entry, size_t idx, size_t& nextIdx, vector<SynError>& errors,
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

bool Lexer::parseSymbol(string& entry, size_t idx, size_t& nextIdx, vector<SynError>& errors,
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

bool Lexer::parseProgram(string& entry, size_t idx, size_t& nextIdx, vector<SynError>& errors,
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

int Lexer::getBaseAt(string& entry, int idxStart, bool& positive) {
    #if 0
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
    #endif
    return 10;
}

bool Lexer::getNumberAt(string& entry, size_t idx, size_t& nextIdx, int& base, mpreal** r, char delim) {
    stringstream ss;
    int idxNumber = 0;
    string token;
    bool positive = true;

    ss.str(entry.substr(idx));
    if (getline(ss, token, delim)) {
        nextIdx = token.size() + idx + 1;
        base = getBaseAt(token, idx, positive);
        if (base < BASE_MIN || base > BASE_MAX) return false;
        trim(token);
        *r = new mpreal;
        if (mpfr_set_str((*r)->mpfr_ptr(), token.c_str(), base, mpreal::get_default_rnd()) == 0) {
            if (!positive) *(*r) = -*(*r);
            return true;
        } else
            return false;
    }
    nextIdx = token.size() + idx + 1;
    return false;
}

bool Lexer::parseNumber(string& entry, size_t idx, size_t& nextIdx, vector<SynError>& errors,
                        vector<SynElement>& elements) {
    mpreal* r;
    int base = 10;
    if (getNumberAt(entry, idx, nextIdx, base, &r)) {
        elements.push_back({cmd_number, .re = r, .reBase = base});
        return true;
    } else {
        errors.push_back({entry.size(), "unterminated number"});
        return false;
    }
}

bool Lexer::parseComplex(string& entry, size_t idx, size_t& nextIdx, vector<SynError>& errors,
                         vector<SynElement>& elements) {
    mpreal* re;
    mpreal* im;
    int reBase, imBase = 10;
    if (idx + 1 == entry.size()) {
        elements.push_back({cmd_symbol, .value = entry.substr(idx, entry.size() - idx)});
        nextIdx = entry.size();
        return true;  // complex format error, return a symbol
    }
    if (!getNumberAt(entry, idx + 1, nextIdx, reBase, &re, ',')) {
        elements.push_back({cmd_symbol, .value = entry.substr(idx, entry.size() - idx)});
        nextIdx = entry.size();
        return true;  // complex format error, return a symbol
    }

    size_t i = nextIdx;
    // while (i < entry.size() && entry[i] != ',') i++;
    if (i >= entry.size()) {
        elements.push_back({cmd_symbol, .value = entry.substr(idx, entry.size() - idx)});
        nextIdx = entry.size();
        return true;  // complex format error, return a symbol
    }

    if (!getNumberAt(entry, i, nextIdx, imBase, &im, ')')) {
        elements.push_back({cmd_symbol, .value = entry.substr(idx, entry.size() - idx)});
        nextIdx = entry.size();
        return true;  // complex format error, return a symbol
    }
    elements.push_back({cmd_complex, .re = re, .im = im, .reBase = reBase, .imBase = imBase});
    nextIdx++;
    return true;
}

bool Lexer::parseReserved(string& entry, size_t idx, size_t& nextIdx, vector<SynElement>& elements,
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

bool Lexer::parseUnknown(string& entry, size_t idx, size_t& nextIdx, vector<SynElement>& elements) {
    stringstream ss(entry.substr(idx));
    string token;
    ss >> token;
    elements.push_back({cmd_symbol, .value = token, .autoEval = true});
    nextIdx = token.size() + idx;
    return true;
}
