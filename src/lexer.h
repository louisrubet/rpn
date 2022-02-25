// Copyright (c) 2014-2022 Louis Rubet

#ifndef SRC_LEXER_HPP_
#define SRC_LEXER_HPP_

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
        program_fn_t fn;
        bool auto_eval;
    };

    struct SynError {
        size_t indx;
        string err;
    };

    struct ReservedWord {
        ObjectType type;
        program_fn_t fn;
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
    bool Analyse(string& entry, map<string, ReservedWord>& keywords, vector<SynElement>& elements,
                 vector<SynError>& errors);

 private:
    bool ParseString(string& entry, size_t idx, size_t& next_idx, vector<SynError>& errors,
                     vector<SynElement>& elements);
    bool ParseSymbol(string& entry, size_t idx, size_t& next_idx, vector<SynError>& errors,
                     vector<SynElement>& elements);
    bool ParseProgram(string& entry, size_t idx, size_t& next_idx, vector<SynError>& errors,
                      vector<SynElement>& elements);
    bool ParseNumber(string& entry, size_t idx, size_t& next_idx, vector<SynError>& errors,
                     vector<SynElement>& elements);
    bool ParseComplex(string& entry, size_t idx, size_t& next_idx, vector<SynError>& errors,
                      vector<SynElement>& elements);
    bool ParseReserved(string& entry, size_t idx, size_t& next_idx, vector<SynElement>& elements,
                       map<string, ReservedWord>& keywords);
    bool ParseUnknown(string& entry, size_t idx, size_t& next_idx, vector<SynElement>& elements);

    void Trim(string& s);
    int GetBaseAt(string& entry, size_t& next_idx, bool& positive);
    bool GetNUmberAt(string& entry, size_t idx, size_t& next_idx, int& base, mpreal** r, char delim = ' ');
};

#endif  // SRC_LEXER_HPP_
