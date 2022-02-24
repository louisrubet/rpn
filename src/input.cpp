// Copyright (c) 2014-2022 Louis Rubet

#include "input.hpp"

#include <cstring>

vector<string>* Input::_acWords = nullptr;

Input::Input(string& entry, vector<string>& autocompletionWords, string prompt, string mlPrompt) : status(cont) {
    char* c_entry = nullptr;
    bool multiline = false;
    int entry_len;

    _acWords = &autocompletionWords;

    // linenoise for entry
    linenoiseSetCompletionCallback(entry_completion_generator);
    while (status == cont) {
        // get user entry
        if (multiline)
            c_entry = linenoise(mlPrompt.c_str(), &entry_len);
        else
            c_entry = linenoise(prompt.c_str(), &entry_len);

        // Ctrl-C
        if (linenoiseKeyType() == 1) {
            if (entry_len > 0 || multiline)
                status = abort;
            else
                status = ctrlc;
        } else if (linenoiseKeyType() == 3) {
            multiline = true;
            if (c_entry != nullptr) entry += c_entry;
            entry += " ";
        } else {
            if (c_entry != nullptr) {
                entry += c_entry;
                // keep history
                if (c_entry[0] != 0) (void)linenoiseHistoryAdd(entry.c_str());
                status = ok;
            } else {
                status = error;
            }
        }
    }
    free(c_entry);
}

/// @brief completion callback as asked by linenoise-ng
/// this is called by linenoise-ng whenever the user enters TAB
///
/// @param text the text after wich the user entered TAB
/// @param lc the completion object to add strings with linenoiseAddCompletion()
///
void Input::entry_completion_generator(const char* text, linenoiseCompletions* lc) {
    if (Input::_acWords == nullptr || text == nullptr) return;

    int text_len = strnlen(text, 6);

    if (text_len == 0)
        // propose all keywords
        for (string& ac : *Input::_acWords) linenoiseAddCompletion(lc, ac.c_str());
    else
        // propose only keywords matching to text begining
        for (string& ac : *Input::_acWords)
            // compare list entry with text, return if match
            if (ac.compare(0, text_len, text) == 0) linenoiseAddCompletion(lc, ac.c_str());
}

void Input::preload(const char* preloadText) {
    if (preloadText != nullptr) linenoisePreloadBuffer(preloadText);
}
