#include <pwd.h>
#include <unistd.h>

#include <csignal>
#include <cerrno>
#include <iostream>
using namespace std;

// internal includes
#include "input.hpp"
#include "program.hpp"

static heap _global_heap;
static rpnstack _global_stack;
static program* _prog_to_interrupt = nullptr;

/// @brief actions to be done at rpn exit
///
static void exit_interactive_rpn() {
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
static void init_interactive_rpn() {
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
static void ctrlc_handler(int sig, siginfo_t* siginfo, void* context) {
    if (_prog_to_interrupt != nullptr) {
        _prog_to_interrupt->stop();
        _prog_to_interrupt = nullptr;
    }

    exit_interactive_rpn();
}

/// @brief handle SIGSEGV signal (sigaction handler): stop and exit rpn
///
/// @param sig signal, see POSIX sigaction
/// @param siginfo signal info, see POSIX sigaction
/// @param context see POSIX sigaction
///
static void segv_handler(int sig, siginfo_t* siginfo, void* context) {
    cerr << "Internal error" << endl;
    _prog_to_interrupt->stop();
    _prog_to_interrupt = nullptr;
}

/// @brief setup signals handlers to stop with honours
///
/// @param prog the prog to catch the signals to, must be checked not nullptr by user
///
static void catch_signals(program* prog) {
    struct sigaction act = {0};

    _prog_to_interrupt = prog;

    act.sa_sigaction = &ctrlc_handler;
    act.sa_flags = SA_SIGINFO;
    if (sigaction(SIGINT, &act, nullptr) < 0)
        cerr << "Warning, Ctrl-C cannot be caught [errno=" << errno << ' ' << strerror(errno) << "']" << endl;

    act.sa_sigaction = &segv_handler;
    act.sa_flags = SA_SIGINFO;
    if (sigaction(SIGSEGV, &act, nullptr) < 0)
        cerr << "Warning, SIGSEGV cannot be caught [errno=" << errno << ' ' << strerror(errno) << "']" << endl;
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
    program::apply_default();

    // run with interactive prompt
    if (argc == 1) {
        // init history
        init_interactive_rpn();

        // entry loop
        while (go_on) {
            // make program from interactive entry
            program prog(_global_stack, _global_heap);
            string entry;
            switch (Input(entry, program::getAutocompletionWords()).status) {
                case Input::ok:
                    // user could stop prog with CtrlC
                    catch_signals(&prog);
                    // run it
                    if (prog.parse(entry) == ret_ok && prog.run() == ret_good_bye)
                        go_on = false;
                    else
                        program::show_stack(_global_stack);
                    break;
                case Input::ctrlc:
                    go_on = false;
                    break;
                default:
                    break;
            }
        }

        // manage history and exit
        exit_interactive_rpn();
    } else {  // run with cmd line arguments
        program prog(_global_stack, _global_heap);
        string entry;
        int i;

        // make one string from entry
        for (i = 1; i < argc; i++) {
            entry += argv[i];
            entry += ' ';
        }

        // make program
        ret = prog.parse(entry);
        if (ret == ret_ok) {
            // user could stop prog with CtrlC
            catch_signals(&prog);

            // run it
            ret = prog.run();
            program::show_stack(_global_stack, false);
        }
    }

    mpfr_free_cache();

    if (ret != ret_ok) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
