// Copyright (c) 2014-2022 Louis Rubet

#ifndef SRC_LEXER_H_
#define SRC_LEXER_H_

#include <mpreal.h>
using mpfr::mpreal;

#include <map>
#include <string>
#include <vector>
using std::map, std::string, std::vector;

#include "object.h"

class Lexer {
 public:
    // a structure to describe a syntaxical element and its value
    struct SynElement {
        ObjectType type;
        string value;
        mpreal* re;
        mpreal* im;
        int re_base;
        int im_base;
        object_cb_t fn;
        bool auto_eval;
    };

    struct SynError {
        size_t indx;
        string err;
    };

    struct ReservedWord {
        ObjectType type;
        object_cb_t fn;
    };

    Lexer() {}

    /// @brief lexical analysis of an entry string
    ///
    /// @param[in] entry the entry to lex
    /// @param[out] elements syntax elements vector
    /// @param[out] errors errors vector
    /// @param[in] keywords reserved keywords
    /// @return false=errors, the lexer must stop
    ///
    bool Analyse(const string& entry, map<string, ReservedWord>& keywords, vector<SynElement>& elements,
                 vector<SynError>& errors);

 private:
    bool ParseString(const string& entry, size_t idx, size_t& next_idx, vector<SynError>& errors,
                     vector<SynElement>& elements);
    bool ParseSymbol(const string& entry, size_t idx, size_t& next_idx, vector<SynError>& errors,
                     vector<SynElement>& elements);
    bool ParseProgram(const string& entry, size_t idx, size_t& next_idx, vector<SynError>& errors,
                      vector<SynElement>& elements);
    bool ParseNumber(const string& entry, size_t idx, size_t& next_idx, vector<SynError>& errors,
                     vector<SynElement>& elements);
    bool ParseComplex(const string& entry, size_t idx, size_t& next_idx, vector<SynError>& errors,
                      vector<SynElement>& elements);
    bool ParseReserved(const string& entry, size_t idx, size_t& next_idx, vector<SynElement>& elements,
                       map<string, ReservedWord>& keywords);
    bool ParseUnknown(const string& entry, size_t idx, size_t& next_idx, vector<SynElement>& elements);

    void Trim(string& s);
    int GetBaseAt(const string& entry, size_t& next_idx, bool& positive);
    bool GetNumberAt(const string& entry, size_t idx, size_t& next_idx, int& base, mpreal** r, char delim = ' ');
};

#endif  // SRC_LEXER_H_
