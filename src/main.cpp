#include "program.hpp"

static heap s_global_heap;
static stack s_global_stack;

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

            // run it
            ret = prog.run(s_global_stack, s_global_heap);
            program::show_stack(s_global_stack, separator);
        }
    }
 
    mpfr_free_cache();
 
    return ret;
}
