// Copyright (c) 2014-2022 Louis Rubet

#include "lexer.h"

bool Lexer::Analyse(const string& entry, map<string, ReservedWord>& keywords, vector<SynElement>& elements,
                    vector<SynError>& errors) {
    size_t jump;
    for (size_t i = 0; i < entry.size(); i++) {
        if (isspace(entry[i])) continue;
        SynElement element{.re = nullptr, .im = nullptr};
        switch (entry[i]) {
            case '"':
                if (!ParseString(entry, i, jump, errors, elements)) return false;
                i = jump - 1;
                continue;
            case '\'':
                if (!ParseSymbol(entry, i, jump, errors, elements)) return false;
                i = jump - 1;
                continue;
            case '(':
                if (!ParseComplex(entry, i, jump, errors, elements)) return false;
                i = jump - 1;
                continue;
        }
        if (i < entry.size() - 1 && (entry.substr(i, 2) == "<<" || entry.substr(i, 2) == "«")) {
            if (!ParseProgram(entry, i, jump, errors, elements)) return false;
            i = jump - 1;
            continue;
        } else if (ParseReserved(entry, i, jump, elements, keywords)) {
            // found a keywords word, add it with its correct type
            i = jump - 1;
            continue;
        } else if (ParseNumber(entry, i, jump, errors, elements)) {
            i = jump - 1;
            continue;
        }
        if (ParseUnknown(entry, i, jump, elements))
            // last chance, this unknown entry is treated as a symbol
            i = jump - 1;
        else
            return false;  // no object of any type could be found, this is a lexer error
    }

    return true;
}

void Lexer::Trim(string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }));
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), s.end());
}

bool Lexer::ParseString(const string& entry, size_t idx, size_t& next_idx, vector<SynError>& errors __attribute__((unused)),
                        vector<SynElement>& elements) {
    // here we are sure that entry[0] is at least '"'
    for (size_t i = idx + 1; i < entry.size(); i++) {
        if (entry[i] == '"') {
            if (entry[i] - 1 != '\\') {
                elements.push_back({.type = kString, .value = entry.substr(idx + 1, i - idx - 1)});
                next_idx = i + 1;
                return true;
            }
        }
    }
    elements.push_back({.type = kString, .value = entry.substr(idx + 1, entry.size() - idx - 1)});
    next_idx = entry.size();
    return true;
}

bool Lexer::ParseSymbol(const string& entry, size_t idx, size_t& next_idx, vector<SynError>& errors __attribute__((unused)),
                        vector<SynElement>& elements) {
    // here we are sure that entry[0] is at least '\''
    for (size_t i = idx + 1; i < entry.size(); i++) {
        if (entry[i] == '\'') {
            elements.push_back({.type = kSymbol, .value = entry.substr(idx + 1, i - idx - 1), .auto_eval = false});
            next_idx = i + 1;
            return true;
        }
    }
    elements.push_back({.type = kSymbol, .value = entry.substr(idx + 1, entry.size() - idx - 1)});
    next_idx = entry.size();
    return true;
}

bool Lexer::ParseProgram(const string& entry, size_t idx, size_t& next_idx, vector<SynError>& errors __attribute__((unused)),
                         vector<SynElement>& elements) {
    // here we are sure that entry is at least "<<"
    // find last ">>" or "»"
    int countNested = 0;
    for (size_t i = idx + 2; i < entry.size() - 1; i++) {
        if ((entry[i] == '<' && entry[i + 1] == '<') || (entry.substr(i, 2) == "«")) {
            countNested++;
        } else if ((entry[i] == '>' && entry[i + 1] == '>') || (entry.substr(i, 2) == "»")) {
            if (countNested == 0) {
                string prg = entry.substr(idx + 2, i - idx - 2);
                Trim(prg);
                elements.push_back({.type = kProgram, .value = prg});
                next_idx = i + 2;
                return true;
            } else {
                countNested--;
            }
        }
    }
    string prg = entry.substr(idx + 2, entry.size() - idx - 2);
    Trim(prg);
    elements.push_back({.type = kProgram, .value = prg});
    next_idx = entry.size();
    return true;
}

int Lexer::GetBaseAt(const string& entry, size_t& next_idx, bool& positive) {
    // a regex could be "([+-])?((0[xX])|([0-9][0-9]?[bB]))"
    // regex is not use because dramatically slow
    // entry is scanned from idxStart, searching for [s]abc (sign and 3 first chars)
    size_t scan = 0;
    next_idx = 0;
    positive = true;
    if (scan >= entry.size()) return 10;
    if (entry[scan] == '+') {
        scan++;
        next_idx = scan;
    } else if (entry[scan] == '-') {
        scan++;
        next_idx = scan;
        positive = false;
    }
    if (scan + 2 >= entry.size()) return 10;
    char a = entry[scan];
    char b = entry[scan + 1];
    char c = 0;
    if (scan + 2 < entry.size()) c = entry[scan + 2];
    if (a == '0') {
        if (b == 'x' || b == 'X') {
            next_idx = scan + 2;
            return 16;
        }
        if (b == 'b' || b == 'B') {
            next_idx = scan + 2;
            return 2;
        }
    } else if (isdigit(a)) {
        if (b == 'b' || b == 'B') {
            next_idx = scan + 2;
            return static_cast<int>(a - '0');
        }
        if (isdigit(b) && (c == 'b' || c == 'B')) {
            next_idx = scan + 3;
            return 10 * static_cast<int>(a - '0') + static_cast<int>(b - '0');
        }
    }
    return 10;
}

bool Lexer::GetNumberAt(const string& entry, size_t idx, size_t& next_idx, int& base, mpreal** r, char delim) {
    stringstream ss;
    string token;
    bool positive = true;

    next_idx = idx;

    ss.str(entry.substr(idx));
    if (getline(ss, token, delim)) {
        size_t numberIdx;
        next_idx = token.size() + idx + 1;
        Trim(token);
        base = GetBaseAt(token, numberIdx, positive);
        if (base < 2 || base > 62) return false;
        if (numberIdx != 0) token = token.substr(numberIdx);
        *r = new mpreal;
        if (mpfr_set_str((*r)->mpfr_ptr(), token.c_str(), base, mpreal::get_default_rnd()) == 0) {
            if (!positive) *(*r) = -*(*r);
            return true;
        } else {
            delete *r;
            return false;
        }
    }
    next_idx = token.size() + idx + 1;
    return false;
}

bool Lexer::ParseNumber(const string& entry, size_t idx, size_t& next_idx, vector<SynError>& errors,
                        vector<SynElement>& elements) {
    mpreal* r = nullptr;
    int base = 10;
    if (GetNumberAt(entry, idx, next_idx, base, &r)) {
        elements.push_back({.type = kNumber, .re = r, .re_base = base});
        return true;
    } else {
        errors.push_back({entry.size(), "unterminated number"});
        return false;
    }
}

bool Lexer::ParseComplex(const string& entry, size_t idx, size_t& next_idx, vector<SynError>& errors __attribute__((unused)),
                         vector<SynElement>& elements) {
    mpreal* re = nullptr;
    mpreal* im = nullptr;
    int re_base, im_base = 10;
    if (idx + 1 == entry.size()) {
        elements.push_back({.type = kSymbol, .value = entry.substr(idx, entry.size() - idx)});
        next_idx = entry.size();
        return true;  // complex format error, return a symbol
    }
    if (!GetNumberAt(entry, idx + 1, next_idx, re_base, &re, ',')) {
        elements.push_back({.type = kSymbol, .value = entry.substr(idx, entry.size() - idx)});
        next_idx = entry.size();
        return true;  // complex format error, return a symbol
    }

    size_t i = next_idx;
    if (i >= entry.size()) {
        elements.push_back({.type = kSymbol, .value = entry.substr(idx, entry.size() - idx)});
        next_idx = entry.size();
        if (re != nullptr) delete re;
        if (im != nullptr) delete im;
        return true;  // complex format error, return a symbol
    }

    if (!GetNumberAt(entry, i, next_idx, im_base, &im, ')')) {
        elements.push_back({.type = kSymbol, .value = entry.substr(idx, entry.size() - idx)});
        next_idx = entry.size();
        if (re != nullptr) delete re;
        if (im != nullptr) delete im;
        return true;  // complex format error, return a symbol
    }
    elements.push_back({.type = kComplex, .re = re, .im = im, .re_base = re_base, .im_base = im_base});
    next_idx++;
    return true;
}

bool Lexer::ParseReserved(const string& entry, size_t idx, size_t& next_idx, vector<SynElement>& elements,
                          map<string, ReservedWord>& keywords) {
    stringstream ss(entry.substr(idx));
    string token;
    ss >> token;

    auto resa = keywords.find(token);
    if (resa != keywords.end()) {
        elements.push_back({.type = resa->second.type, .value = token, .fn = resa->second.fn});
        next_idx = token.size() + idx;
        return true;
    }
    return false;
}

bool Lexer::ParseUnknown(const string& entry, size_t idx, size_t& next_idx, vector<SynElement>& elements) {
    stringstream ss(entry.substr(idx));
    string token;
    ss >> token;
    elements.push_back({.type = kSymbol, .value = token, .auto_eval = true});
    next_idx = token.size() + idx;
    return true;
}
