// Copyright (c) 2014-2022 Louis Rubet

#include <pwd.h>
#include <unistd.h>

#include <cerrno>
#include <csignal>
#include <iostream>
using std::cerr;

// internal includes
#include "input.h"
#include "program.h"

/// @brief lines history management: load file
///
static void StartHistory() {
    struct passwd* pw = getpwuid(getuid());
    if (pw != nullptr) {
        stringstream history_path;
        history_path << pw->pw_dir << "/.rpn_history";
        linenoiseHistorySetMaxLen(100);
        linenoiseHistoryLoad(history_path.str().c_str());
    }
}

/// @brief lines history management: save
///
static void StopHistory() {
    struct passwd* pw = getpwuid(getuid());
    if (pw != nullptr) {
        stringstream history_path;
        history_path << pw->pw_dir << "/.rpn_history";

        // trunc current history
        ofstream history(history_path.str(), ios_base::out | ios_base::trunc);
        history.close();

        if (linenoiseHistorySave(history_path.str().c_str()) != 0)
            cerr << "warning, could not save " << history_path.str() << " [errno=" << errno << ' ' << strerror(errno)
                 << "']" << endl;
        linenoiseHistoryFree();
    }
}

static void CtrlHandler(int sig __attribute__((unused)), siginfo_t* siginfo __attribute__((unused)),
                        void* context __attribute__((unused))) {
    StopHistory();  // don't put an entry line canceled with CtrlC in history
}

static void CatchCtrlC() {
    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_sigaction = &CtrlHandler;
    act.sa_flags = SA_SIGINFO;
    if (sigaction(SIGINT, &act, nullptr) < 0)
        cerr << "Warning, Ctrl-C cannot be caught [errno=" << errno << ' ' << strerror(errno) << "']" << endl;
}

/// @brief rpn entry point
///
/// @param argc command line args nb
/// @param argv  command line args nb
/// @return int 0=ok
///
int main(int argc, char* argv[]) {
    int ret = 0;
    bool goodbye = false;

    // apply default configuration
    Program::ApplyDefault();

    // run with interactive prompt
    if (argc == 1) {
        Program::Welcome();

        StartHistory();
        CatchCtrlC();

        heap heap;
        rpnstack stack;
        while (!goodbye) {
            Program prog(stack, heap);
            string entry;
            switch (Input(entry, Program::GetAutocompletionWords()).status) {
                case Input::InputStatus::kOk:
                    if (prog.Parse(entry) == kOk) {
                        if (prog.Run() != kGoodbye)
                            prog.ShowStack();
                        else
                            goodbye = true;
                        continue;
                    }
                    break;
                case Input::InputStatus::kCtrlc:
                    goodbye = true;
                    continue;
                default:
                    break;
            }
        }
        StopHistory();
    } else {
        // run with cmd line arguments
        heap heap;
        rpnstack stack;
        Program prog(stack, heap);
        string entry;
        int i;

        // make one string from entry
        for (i = 1; i < argc; i++) entry += string(argv[i]) + ' ';

        // make program
        ret = prog.Parse(entry);
        if (ret == kOk) {
            ret = prog.Run();
            prog.ShowStack(false);
        }
    }

    mpfr_free_cache();

    if (ret != kOk) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
