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

/// @brief actions to be done at rpn exit
///
static void ExitInteractive() {
    struct passwd* pw = getpwuid(getuid());
    if (pw != nullptr) {
        stringstream history_path;
        history_path << pw->pw_dir << "/.rpn_history";

        // trunc current history
        ofstream history(history_path.str(), ios_base::out | ios_base::trunc);
        history.close();

        // save it
        if (linenoiseHistorySave(history_path.str().c_str()) != 0)
            cerr << "warning, could not save " << history_path.str() << " [errno=" << errno << ' ' << strerror(errno)
                 << "']" << endl;
        linenoiseHistoryFree();
    }
}

/// @brief actions to be done at rpn exit
///
static void EnterInteractive() {
    struct passwd* pw = getpwuid(getuid());
    if (pw != nullptr) {
        stringstream history_path;
        history_path << pw->pw_dir << "/.rpn_history";

        // don't care about errors
        linenoiseHistorySetMaxLen(100);
        linenoiseHistoryLoad(history_path.str().c_str());
    }
}

/// @brief handle CtrlC signal (sigaction handler): exit rpn
///
/// @param sig signal, see POSIX sigaction
/// @param siginfo signal info, see POSIX sigaction
/// @param context see POSIX sigaction
///
static void CtrlHandler(int sig __attribute__((unused)), siginfo_t* siginfo __attribute__((unused)),
                        void* context __attribute__((unused))) {
    ExitInteractive();
}

/// @brief setup signals handlers to stop with honours
///
/// @param prog the prog to catch the signals to, must be checked not nullptr by user
///
static void CatchSignals() {
    struct sigaction act = {0};

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
    bool go_on = true;

    // apply default configuration
    program::ApplyDefault();

    // run with interactive prompt
    if (argc == 1) {
        program::Welcome();

        // init history
        EnterInteractive();

        // user could stop prog with CtrlC
        CatchSignals();

        // entry loop
        heap heap;
        rpnstack stack;
        while (go_on) {
            //
            // make program from interactive entry
            program prog(stack, heap);
            string entry;
            switch (Input(entry, program::GetAutocompletionWords()).status) {
                case Input::InputStatus::kOk:
                    // run it
                    if (prog.Parse(entry) == kOk && prog.Run() == kGoodbye)
                        go_on = false;
                    else
                        prog.ShowStack();
                    break;
                case Input::InputStatus::kCtrlc:
                    go_on = false;
                    break;
                default:
                    break;
            }
        }

        // manage history and exit
        ExitInteractive();
    } else {  // run with cmd line arguments
        heap heap;
        rpnstack stack;
        program prog(stack, heap);
        string entry;
        int i;

        // make one string from entry
        for (i = 1; i < argc; i++) entry += string(argv[i]) + ' ';

        // make program
        ret = prog.Parse(entry);
        if (ret == kOk) {
            // user could stop prog with CtrlC
            CatchSignals();

            // run it
            ret = prog.Run();
            prog.ShowStack(false);
        }
    }

    mpfr_free_cache();

    if (ret != kOk) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
