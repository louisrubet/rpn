#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include <map>
#include <vector>
using namespace std;

#define MPFR_USE_NO_MACRO
#include <mpfr.h>
#include <mpreal.h>
using namespace mpfr;

#include "constant.h"
#include "object.hpp"

class Lexer {
   public:
    // a structure to describe a syntaxical element and its value
    struct SynElement {
        cmd_type_t type;
        string value;
        mpreal* re;
        mpreal* im;
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
    bool parseString(string& entry, size_t idx, size_t& nextIdx, vector<SynError>& errors,
                     vector<SynElement>& elements);
    bool parseSymbol(string& entry, size_t idx, size_t& nextIdx, vector<SynError>& errors,
                     vector<SynElement>& elements);
    bool parseProgram(string& entry, size_t idx, size_t& nextIdx, vector<SynError>& errors,
                      vector<SynElement>& elements);
    bool parseNumber(string& entry, size_t idx, size_t& nextIdx, vector<SynError>& errors,
                     vector<SynElement>& elements);
    bool parseComplex(string& entry, size_t idx, size_t& nextIdx, vector<SynError>& errors,
                      vector<SynElement>& elements);
    bool parseReserved(string& entry, size_t idx, size_t& nextIdx, vector<SynElement>& elements,
                       map<string, ReservedWord>& keywords);
    bool parseUnknown(string& entry, size_t idx, size_t& nextIdx, vector<SynElement>& elements);

    void trim(string& s);
    int getBaseAt(string& entry, int idxStart, bool& positive);
    bool getNumberAt(string& entry, size_t idx, size_t& nextIdx, int& base, mpreal** r, char delim = ' ');
};

#endif
