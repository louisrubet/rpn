// std c
#include <signal.h>
#include <errno.h>
#include <string.h>

// internal includes 
#include "program.hpp"

static heap s_global_heap;
static stack s_global_stack;
static program* s_prog_to_interrupt = NULL;

// handle CtrlC signal
static void ctrlc_handler(int sig, siginfo_t* siginfo, void* context)
{
    if (s_prog_to_interrupt != NULL)
    {
        s_prog_to_interrupt->stop();
        s_prog_to_interrupt = NULL;
    }
}

// handle SIGSEGV signal
static void segv_handler(int sig, siginfo_t* siginfo, void* context)
{
    fprintf(stderr, "Internal error\n");
}

static void catch_signals(program* prog)
{
    struct sigaction act;

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

//
int main(int argc, char* argv[])
{
    int ret = 0;
    
    // apply default configuration
    program::apply_default();
    
    // run with interactive prompt
    if (argc == 1)
    {
        //
        for (;;)
        {
            // make program from interactive entry
            program prog;
            if (program::entry(prog) == ret_good_bye)
                break;
            else
            {
                // user could stop prog with CtrlC
                catch_signals(&prog);

                // run it
                if (prog.run(s_global_stack, s_global_heap) == ret_good_bye)
                    break;
                else
                    program::show_stack(s_global_stack);
            }
        }
    }
    // run with cmd line arguments
    else
    {
        program prog;
        string entry;
        int i;

        // make one string from entry
        for (i=1; i<argc; i++)
        {
            entry += argv[i];
            entry += ' ';
        }

        // make program
        ret = program::parse(entry.c_str(), prog);
        if (ret == ret_ok)
        {
            string separator = "";

            // user could stop prog with CtrlC
            catch_signals(&prog);

            // run it
            ret = prog.run(s_global_stack, s_global_heap);
            program::show_stack(s_global_stack, true);
        }
    }
 
    mpfr_free_cache();
 
    return ret;
}
