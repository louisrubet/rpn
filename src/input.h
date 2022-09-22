// Copyright (c) 2014-2022 Louis Rubet

#ifndef SRC_INPUT_H_
#define SRC_INPUT_H_

#include <cstdio>
#include <string>
#include <vector>
using std::string, std::vector;

#include "linenoise.h"

class Input {
 public:
    Input(string& entry, vector<string>& autocompletion_list, string prompt = "rpn> ", string multiline_prompt = ">");
    enum class InputStatus {
        kOk,     // user entered a good string
        kCtrlc,  // user pressed CtrlC
        kAbort,  // user aborted current line
        kError   // entry internal error
    } status;

    static void Preload(const char* preloadText);

 private:
    static void EntryCompletionGenerator(const char* text, linenoiseCompletions* lc);
    static vector<string>* ac_list_;
};

#endif  // SRC_INPUT_H_
