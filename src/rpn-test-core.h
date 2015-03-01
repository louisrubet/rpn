void program::test()
{
	const string test_filename = "test.txt";
	const string stack_size("-> stack size should be ");
	const string stack_value("-> stack should be ");
	const string cmd_error("-> error should be ");
	const string cmd_exit("exit test");
	ifstream test_file(test_filename.c_str());

	if (test_file.is_open())
	{
		string test_title;
		string entry;
		ret_value ret;
		stack stk;
		heap hp;
		bool indicate_passed = false;
		bool failed = false;
		int count_tests = 0;
		int last_err;
		stringstream cerr_buffer;
		streambuf* cerr_old_buffer;

		// redirect cerr
		cerr_old_buffer = cerr.rdbuf(cerr_buffer.rdbuf());

		while ((!failed) && (!test_file.eof()))
		{
			getline(test_file, entry);
			if (entry.substr(0,2)=="##")
			{
				// read a test section
				if (indicate_passed)
				{
					if (g_verbose > 0)
						cout << "\t";
					cout << "passed " << count_tests << " tests"<< endl;
				}
				cout << entry;
				if (g_verbose == 0)
					cout << " .. ";
				else
					cout << endl;
				indicate_passed = true;
				count_tests = 0;
			}
			else if (entry.substr(0,1)=="#")
			{
				// read a test title
				test_title = entry;
				if (g_verbose > 0)
					cout << "\t" << test_title << endl;
				count_tests++;
			}
			else if (entry.find(stack_size, 0) == 0)
			{
				// check current stack size
				istringstream isub;
				int size;

				isub.str(entry.substr(stack_size.size()));
				isub>>size;
				if (size != (int)stk.size())
				{
					cout<<endl<<endl<<test_title<<endl<<entry<<endl<<"FAIL, ";
					cout<<"stack size is "<<stk.size()<<endl;
					failed = true;
					indicate_passed = false;
				}
			}
			else if (entry.find(stack_value, 0) == 0)
			{
				// check current stack value
				string stack_should_be = entry.substr(stack_value.size());
				string stack_is;
				string tmp;
				for (int i = (int)stk.size() - 1; i >= 0; i--)
				{
					stringstream os;
					if (i < (int)(stk.size() - 1))
						stack_is += ", ";
					((object*)stk.seq_obj(i))->show(os);
					getline(os, tmp);
					stack_is += tmp;
				}
				if (stack_is != stack_should_be)
				{
					cout<<endl<<endl<<test_title<<endl<<entry<<endl<<"FAIL, ";
					cout<<"real stack is '"<<stack_is<<endl;
					failed = true;
					indicate_passed = false;
				}
			}
			else if (entry.find(cmd_error, 0) == 0)
			{
				// check current error
				istringstream isub;
				int err_should_be;
				isub.str(entry.substr(cmd_error.size()));
				isub>>err_should_be;
				if (err_should_be != last_err)
				{
					cout<<endl<<endl<<test_title<<endl<<entry<<endl<<"FAIL, ";
					cout<<"last error is "<<last_err<<endl;
					failed = true;
					indicate_passed = false;
				}
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
		if (indicate_passed)
			cout << "passed " << count_tests << " tests"<< endl;
		if (! failed)
			cout << "test file '"<<test_filename<<"' has passed" << endl;

		// cerr back
		cerr.rdbuf(cerr_old_buffer);
	}
	else
		cerr << "test file '"<<test_filename<<"' not found" << endl;
}
