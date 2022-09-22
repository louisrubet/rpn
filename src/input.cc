// Copyright (c) 2014-2022 Louis Rubet

#include "input.h"

#include <cstring>

vector<string>* Input::ac_list_ = nullptr;

Input::Input(string& entry, vector<string>& autocompletion_list, string prompt, string multiline_prompt)
    : status(InputStatus::kOk) {
    char* c_entry = nullptr;
    bool multiline = false;
    int entry_len;
    bool goodbye;

    ac_list_ = &autocompletion_list;

    // linenoise for entry
    linenoiseSetCompletionCallback(EntryCompletionGenerator);
    do {
        // prompt
        goodbye = true;
        if (multiline)
            c_entry = linenoise(multiline_prompt.c_str(), &entry_len);
        else
            c_entry = linenoise(prompt.c_str(), &entry_len);

        if (linenoiseKeyType() == 1 || linenoiseKeyType() == 2) {
            if (entry_len > 0 || multiline)  // Ctrl-C or Ctrl-D
                status = InputStatus::kAbort;
            else
                status = InputStatus::kCtrlc;
        } else if (linenoiseKeyType() == 3) {
            multiline = true;  // Alt-Enter (multiline)
            if (c_entry != nullptr) entry += c_entry;
            entry += " ";
            goodbye = false;
        } else {
            status = InputStatus::kError;
            if (c_entry != nullptr) {
                entry += c_entry;                                                                // string entry
                if (entry_len > 0 && c_entry[0] != 0) (void)linenoiseHistoryAdd(entry.c_str());  // keep history
                status = InputStatus::kOk;
            }
        }
    } while (!goodbye);

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
