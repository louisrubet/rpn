// Copyright (c) 2014-2022 Louis Rubet

#ifndef SRC_INPUT_HPP_
#define SRC_INPUT_HPP_

#include <cstdio>
#include <string>
#include <vector>
using std::string;
using std::vector;

#include "linenoise.h"

class Input {
 public:
    Input(string& entry, vector<string>& autocompletion_list, string prompt = "rpn> ", string multiline_prompt = ">");
    enum class InputStatus { kOk, kContinue, kAbort, kCtrlc, kError } status;

    static void preload(const char* preloadText);

 private:
    static void entry_completion_generator(const char* text, linenoiseCompletions* lc);
    static vector<string>* ac_list_;
};

#endif  // SRC_INPUT_HPP_
