// Copyright (c) 2014-2022 Louis Rubet

#ifndef SRC_LEXER_HPP_
#define SRC_LEXER_HPP_

#include <mpreal.h>
using mpfr::mpreal;

#include <map>
#include <vector>
#include <string>
using std::map;
using std::string;
using std::vector;

#include "object.hpp"

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
    bool lexer(string& entry, map<string, ReservedWord>& keywords, vector<SynElement>& elements,
               vector<SynError>& errors);

 private:
    bool parseString(string& entry, size_t idx, size_t& next_idx, vector<SynError>& errors,
                     vector<SynElement>& elements);
    bool parseSymbol(string& entry, size_t idx, size_t& next_idx, vector<SynError>& errors,
                     vector<SynElement>& elements);
    bool parseProgram(string& entry, size_t idx, size_t& next_idx, vector<SynError>& errors,
                      vector<SynElement>& elements);
    bool parseNumber(string& entry, size_t idx, size_t& next_idx, vector<SynError>& errors,
                     vector<SynElement>& elements);
    bool parseComplex(string& entry, size_t idx, size_t& next_idx, vector<SynError>& errors,
                      vector<SynElement>& elements);
    bool parseReserved(string& entry, size_t idx, size_t& next_idx, vector<SynElement>& elements,
                       map<string, ReservedWord>& keywords);
    bool parseUnknown(string& entry, size_t idx, size_t& next_idx, vector<SynElement>& elements);

    void trim(string& s);
    int getBaseAt(string& entry, size_t& next_idx, bool& positive);
    bool getNumberAt(string& entry, size_t idx, size_t& next_idx, int& base, mpreal** r, char delim = ' ');
};

#endif  // SRC_LEXER_HPP_
