// std c
#include <errno.h>
#include <linux/limits.h>
#include <pwd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

// internal includes
#include "program.hpp"

static heap s_global_heap;
static rpnstack s_global_stack;
static program* s_prog_to_interrupt = NULL;

/// @brief actions to be done at rpn exit
///
void exit_interactive_rpn() {
    struct passwd* pw = getpwuid(getuid());
    if (pw != NULL) {
        char history_path[PATH_MAX];
        sprintf(history_path, "%s/%s", pw->pw_dir, HISTORY_FILE);

        // trunc current history
        ofstream history(history_path, ios_base::out | ios_base::trunc);
        history.close();

        // save it
        if (linenoiseHistorySave(history_path) != 0)
            fprintf(stderr, "warning, could not save %s (errno=%d, '%s')\n", history_path, errno, strerror(errno));
        linenoiseHistoryFree();
    }
}

/// @brief actions to be done at rpn exit
///
void init_interactive_rpn() {
    struct passwd* pw = getpwuid(getuid());
    if (pw != NULL) {
        char history_path[PATH_MAX];
        sprintf(history_path, "%s/%s", pw->pw_dir, HISTORY_FILE);

        // don't care about errors
        linenoiseHistorySetMaxLen(HISTORY_FILE_MAX_LINES);
        linenoiseHistoryLoad(history_path);
    }
}

/// @brief handle CtrlC signal (sigaction handler): exit rpn
///
/// @param sig signal, see POSIX sigaction
/// @param siginfo signal info, see POSIX sigaction
/// @param context see POSIX sigaction
///
static void ctrlc_handler(int sig, siginfo_t* siginfo, void* context) {
    if (s_prog_to_interrupt != NULL) {
        s_prog_to_interrupt->stop();
        s_prog_to_interrupt = NULL;
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
    fprintf(stderr, "Internal error\n");
    s_prog_to_interrupt->stop();
    s_prog_to_interrupt = NULL;
}

/// @brief setup signals handlers to stop with honours
///
/// @param prog the prog to catch the signals to, must be checked not NULL by user
///
static void catch_signals(program* prog) {
    struct sigaction act = {0};

    s_prog_to_interrupt = prog;

    act.sa_sigaction = &ctrlc_handler;
    act.sa_flags = SA_SIGINFO;
    if (sigaction(SIGINT, &act, NULL) < 0)
        (void)fprintf(stderr, "Warning, Ctrl-C cannot be catched [errno=%d '%s']", errno, strerror(errno));

    act.sa_sigaction = &segv_handler;
    act.sa_flags = SA_SIGINFO;
    if (sigaction(SIGSEGV, &act, NULL) < 0)
        (void)fprintf(stderr, "Warning, SIGSEGV cannot be catched [errno=%d '%s']", errno, strerror(errno));
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
            program prog;
            switch (program::entry(prog)) {
                case ret_good_bye:
                    go_on = false;
                    break;
                case ret_abort_current_entry:
                    break;
                default:
                    // user could stop prog with CtrlC
                    catch_signals(&prog);

                    // run it
                    if (prog.run(s_global_stack, s_global_heap) == ret_good_bye)
                        go_on = false;
                    else
                        program::show_stack(s_global_stack);
                    break;
            }
        }

        // manage history and exit
        exit_interactive_rpn();
    }
    // run with cmd line arguments
    else {
        program prog;
        string entry;
        int i;

        // make one string from entry
        for (i = 1; i < argc; i++) {
            entry += argv[i];
            entry += ' ';
        }

        // make program
        ret = program::parse(entry, prog);
        if (ret == ret_ok) {
            string separator = "";

            // user could stop prog with CtrlC
            catch_signals(&prog);

            // run it
            ret = prog.run(s_global_stack, s_global_heap);
            program::show_stack(s_global_stack, false);
        }
    }

    mpfr_free_cache();

    return ret;
}
