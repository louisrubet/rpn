void program::test()
{
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, cmd_string);

    const string stack_size("-> stack size should be ");
    const string stack_value("-> stack should be ");
    const string cmd_error("-> error should be ");
    const string cmd_exit("exit test");

    string test_filename = ((ostring*)_stack->pop_back())->_value;
    ifstream test_file(test_filename.c_str());
    int tests = 0;
    int tests_failed = 0;
    int steps = 0;
    int steps_failed = 0;

    if (test_file.is_open())
    {
        string test_title;
        string entry;
        ret_value ret;
        stack stk;
        heap hp;
        bool failed = false;
        bool is_first_step;
        bool is_test_error_shown;
        int last_err;
        stringstream cerr_buffer;
        streambuf* cerr_old_buffer;

        // redirect cerr
        cerr_old_buffer = cerr.rdbuf(cerr_buffer.rdbuf());

        while (!test_file.eof())
        {
            getline(test_file, entry);
            if (entry.substr(0,2)=="##")
            {
                cout << entry;
                cout << endl;
            }
            else if (entry.substr(0,2)=="# ")
            {
                // indicates the status of previous test
                if (failed == false && tests > 0)
                    cout<<FG_GREEN<<" PASSED"<<COLOR_OFF<<endl;
                failed = false;

                // read a test title
                test_title = entry;
                is_first_step = true;
                is_test_error_shown = false;
                cout << test_title;
            }
            // treat "-> stack size should be "
            else if (entry.find(stack_size, 0) == 0)
            {
                // count test and step
                if (is_first_step)
                    tests++;
                steps++;

                // check current stack size
                istringstream isub;
                int size;

                isub.str(entry.substr(stack_size.size()));
                isub>>size;
                if (size != (int)stk.size())
                {
                    // count fail test and step
                    if (!is_test_error_shown)
                    {
                        cout<<FG_RED<<" FAIL"<<COLOR_OFF<<endl;
                        tests_failed++;
                        is_test_error_shown = true;
                    }
                    steps_failed++;

                    // show failure
                    cout<<"\t"<<entry<<endl;
                    cout<<"\tbut real stack size is "<<FG_RED<<stk.size()<<COLOR_OFF<<endl;
                    failed = true;
                }
                is_first_step = false;
            }
            // treat "-> stack should be "
            else if (entry.find(stack_value, 0) == 0)
            {
                // count test
                if (is_first_step)
                    tests++;
                steps++;

                // check current stack value
                string stack_should_be = entry.substr(stack_value.size());
                string stack_is;
                string tmp;
                for (int i = 0; i < (int)stk.size(); i++)
                {
                    stringstream os;
                    if (i > 0)
                        stack_is += ", ";
                    ((object*)stk.seq_obj(i))->show(os);
                    getline(os, tmp);
                    stack_is += tmp;
                }
                if (stack_is != stack_should_be)
                {
                    // count fail test and step
                    if (!is_test_error_shown)
                    {
                        cout<<FG_RED<<" FAIL"<<COLOR_OFF<<endl;
                        tests_failed++;
                        is_test_error_shown = true;
                    }
                    steps_failed++;

                    // show failure
                    cout<<"\t"<<entry<<endl;
                    cout<<"\tbut real stack is "<<FG_RED<<stack_is<<COLOR_OFF<<endl;
                    failed = true;
                }
                is_first_step = false;
            }
            // treat "-> error should be "
            else if (entry.find(cmd_error, 0) == 0)
            {
                // count test
                if (is_first_step)
                    tests++;
                steps++;

                // check current error
                istringstream isub;
                int err_should_be;
                isub.str(entry.substr(cmd_error.size()));
                isub>>err_should_be;
                if (err_should_be != last_err)
                {
                    // count fail test and step
                    if (!is_test_error_shown)
                    {
                        cout<<FG_RED<<" FAIL"<<COLOR_OFF<<endl;
                        tests_failed++;
                        is_test_error_shown = true;
                    }
                    steps_failed++;

                    // show failure
                    cout<<"\t"<<entry<<endl;
                    cout<<"\tbut last error is "<<FG_RED<<last_err<<COLOR_OFF<<endl;
                    failed = true;
                }
                is_first_step = false;
            }
            else if (entry.find(cmd_exit, 0) == 0)
            {
                // forced test end
                break;
            }
            else if (entry.size() > 0)
            {
                // parse entry and run line
                program prog;
                ret = program::parse(entry.c_str(), prog);
                if (ret == ret_ok)
                {
                    // run it
                    (void)prog.run(stk, hp);
                    last_err = (int)prog.get_err();
                }
            }
        }

        // last test
        // indicates the status of previous test
        if (failed == false && tests > 0)
            cout << FG_GREEN << " PASSED" << COLOR_OFF << endl;

        // cerr back
        cerr.rdbuf(cerr_old_buffer);
        
        // conclusion
        cout<<endl<<"Run "<<tests<<" tests: "<<tests-tests_failed<<" passed, ";
        if(tests_failed>0)
            cout<<FG_RED;
        cout<<tests_failed<<" failed";
        if(tests_failed>0)
            cout<<COLOR_OFF;
        
        cout<<" ("<<steps<<" steps: "<<steps-steps_failed<<" passed, ";
        if(steps_failed>0)
            cout<<FG_RED;
        cout<<steps_failed<<" failed";
        if(steps_failed>0)
            cout<<COLOR_OFF;
        cout<<")"<<endl;
    }
    else
        cerr << "test file '"<<test_filename<<"' not found" << endl;
}
