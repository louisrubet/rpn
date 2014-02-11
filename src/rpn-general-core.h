void program::test()
{
	const string test_filename = "test.txt";
	ifstream test_file(test_filename);
	if (test_file.is_open())
	{
		const string stack_size("-> stack size should be ");
		const string stack_value("-> stack should be ");
		string test_title;
		string entry;
		ret_value ret;
		stack stk;
		heap hp;
		bool indicate_passed = false;
		bool failed = false;
		int count_tests = 0;

		while (!test_file.eof())
		{
			getline(test_file, entry);
			if (entry.substr(0,2)=="##")
			{
				if (indicate_passed)
					cout << "passed " << count_tests << " tests"<< endl;
				cout << entry << " .. ";
				indicate_passed = true;
				count_tests = 0;
			}
			else if (entry.substr(0,1)=="#")
			{
				test_title = entry;
				count_tests++;
			}
			else if (entry.find(stack_size, 0) == 0)
			{
				istringstream isub;
				int size;

				isub.str(entry.substr(stack_size.size()));
				isub>>size;
				if (stk.size() != size)
				{
					cout<<endl<<endl<<test_title<<endl<<entry<<endl<<"FAIL, ";
					cout<<"real stack size is "<<stk.size()<<endl;
					failed = true;
					indicate_passed = false;
				}
			}
			else if (entry.find(stack_value, 0) == 0)
			{
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
					cout<<"real stack is "<<stack_is<<endl;
					failed = true;
					indicate_passed = false;
				}
			}
			else if (entry.size() > 0)
			{
				program prog;
				ret = program::parse(entry, prog);
				if (ret == ret_ok)
				{
					// run it
					(void)prog.run(stk, hp);
				}
			}
		}
		if (indicate_passed)
			cout << "passed " << count_tests << " tests"<< endl;
		if (! failed)
			cout << "test file '"<<test_filename<<"' has passed" << endl;
	}
	else
		cerr << "test file '"<<test_filename<<"' not found" << endl;
}
