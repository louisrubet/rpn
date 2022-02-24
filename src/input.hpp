#ifndef INPUT_HPP_
#define INPUT_HPP_

#include <cstdio>

#include <string>
#include <vector>
using namespace std;

#include "linenoise.h"

class Input {
   public:
    Input(string& entry, vector<string>& autocompletionWords, string prompt = "rpn> ", string mlPrompt = ">");
    enum {ok, cont, abort, ctrlc, error} status;

    static void preload(const char* preloadText);

   private:
    static void entry_completion_generator(const char* text, linenoiseCompletions* lc);
    static vector<string>* _acWords;
};

#endif
