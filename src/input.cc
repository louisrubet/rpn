// Copyright (c) 2014-2022 Louis Rubet

#include "input.h"

#include <cstring>

vector<string>* Input::ac_list_ = nullptr;

Input::Input(string& entry, vector<string>& autocompletion_list, string prompt, string multiline_prompt)
    : status(InputStatus::kContinue) {
    char* c_entry = nullptr;
    bool multiline = false;
    int entry_len;

    ac_list_ = &autocompletion_list;

    // linenoise for entry
    linenoiseSetCompletionCallback(EntryCompletionGenerator);
    while (status == InputStatus::kContinue) {
        // get user entry
        if (multiline)
            c_entry = linenoise(multiline_prompt.c_str(), &entry_len);
        else
            c_entry = linenoise(prompt.c_str(), &entry_len);

        // Ctrl-C
        if (linenoiseKeyType() == 1) {
            if (entry_len > 0 || multiline)
                status = InputStatus::kAbort;
            else
                status = InputStatus::kCtrlc;
        } else if (linenoiseKeyType() == 3) {
            multiline = true;
            if (c_entry != nullptr) entry += c_entry;
            entry += " ";
        } else {
            if (c_entry != nullptr) {
                entry += c_entry;
                // keep history
                if (c_entry[0] != 0) (void)linenoiseHistoryAdd(entry.c_str());
                status = InputStatus::kOk;
            } else {
                status = InputStatus::kError;
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
void Input::EntryCompletionGenerator(const char* text, linenoiseCompletions* lc) {
    if (Input::ac_list_ == nullptr || text == nullptr) return;

    int text_len = strnlen(text, 6);

    if (text_len == 0)
        // propose all keywords
        for (string& ac : *Input::ac_list_) linenoiseAddCompletion(lc, ac.c_str());
    else
        // propose only keywords matching to text begining
        for (string& ac : *Input::ac_list_)
            // compare list entry with text, return if match
            if (ac.compare(0, text_len, text) == 0) linenoiseAddCompletion(lc, ac.c_str());
}

void Input::Preload(const char* preloadText) {
    if (preloadText != nullptr) linenoisePreloadBuffer(preloadText);
}
