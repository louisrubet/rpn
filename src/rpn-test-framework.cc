// Copyright (c) 2014-2022 Louis Rubet

#include <string>
using std::string;

#include "program.h"
#include "version.h"

// foreground colors
static const char FG_RED[] = "\33[31m";
static const char FG_GREEN[] = "\33[32m";
static const char COLOR_OFF[] = "\33[m";

static void FindAndReplaceAll(string& data, string toSearch, string replaceStr) {
    // Get the first occurrence
    size_t pos = data.find(toSearch);
    // Repeat till end is reached
    while (pos != string::npos) {
        // Replace this occurrence of Sub String
        data.replace(pos, toSearch.size(), replaceStr);
        // Get the next occurrence from the current position
        pos = data.find(toSearch, pos + replaceStr.size());
    }
}

/// @brief write stack in a string, each entry separated between commas
///
/// @param stack_is the output string
/// @param stk the stack
///
static void GetStackAsString(string& stack_is, rpnstack& stk) {
    ostringstream st;
    if (stk.empty()) {
        stack_is.clear();
        return;
    }
    stk[stk.size() - 1]->Show(st);
    stack_is += st.str();
    for (int i = static_cast<int>(stk.size()) - 2; i >= 0; i--) {
        ostringstream st;
        stk[i]->Show(st);
        stack_is += ", " + st.str();
    }
}

/// @brief show the tests results
///
/// @param title test title
/// @param tests tests nb
/// @param tests_failed failed tests nb
/// @param steps steps nb
/// @param steps_failed failed steps nb
///
static void ShowTestResult(string title, int tests, int tests_failed, int steps, int steps_failed) {
    if (!title.empty()) cout << title << ": ";
    cout << "run " << tests << " tests: " << tests - tests_failed << " passed, ";
    if (tests_failed > 0) cout << FG_RED;
    cout << tests_failed << " failed";
    if (tests_failed > 0) cout << COLOR_OFF;

    cout << " (" << steps << " steps: " << steps - steps_failed << " passed, ";
    if (steps_failed > 0) cout << FG_RED;
    cout << steps_failed << " failed";
    if (steps_failed > 0) cout << COLOR_OFF;
    cout << ")" << endl;
}

/// @brief test keyword implementation
///
void Program::RpnTest() {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kString);

    int total_tests = 0;
    int total_tests_failed = 0;
    int total_steps = 0;
    int total_steps_failed = 0;

    string test_filename = stack_.value<String>(0);
    stack_.pop();
    cout << endl << "rpn version is " << RPN_VERSION << endl;
    RunTestFile(test_filename, total_tests, total_tests_failed, total_steps, total_steps_failed);
    ShowTestResult("\nTotal", total_tests, total_tests_failed, total_steps, total_steps_failed);

    // notify to caller that test succeeded or not
    if (total_tests_failed > 0) {
        err_ = kTestFailed;
        err_context_ = string("rpn version ") + RPN_VERSION + ", test file " + test_filename;
    }
}

/// @brief load a test file and run its tests
///
/// @param test_filename the test file filename
/// @param total_tests the total tests nb
/// @param total_tests_failed the total failed tests nb
/// @param total_steps the total steps nb
/// @param total_steps_failed the total failed steps nb
///
void Program::RunTestFile(string test_filename, int& total_tests, int& total_tests_failed, int& total_steps,
                          int& total_steps_failed) {
    const string stack_size("-> stack size should be ");
    const string stack_value("-> stack should be ");
    const string cmd_error("-> error should be ");
    const string cmd_exit("exit test");

    ifstream test_file(test_filename.c_str());
    int tests = 0;
    int tests_failed = 0;
    int steps = 0;
    int steps_failed = 0;

    if (test_file.is_open()) {
        string test_title;
        string entry;
        RetValue ret;
        rpnstack stk;
        heap hp;
        bool failed = false;
        bool is_first_step = true;
        bool is_test_error_shown = false;
        int last_err = static_cast<int>(kOk);
        stringstream cerr_buffer;
        streambuf* cerr_old_buffer;

        // redirect cerr
        cerr_old_buffer = cerr.rdbuf(cerr_buffer.rdbuf());

        while (!test_file.eof()) {
            getline(test_file, entry);
            if (entry.empty()) continue;

            if (entry.substr(0, 8) == "@include") {
                RunTestFile(entry.substr(9), total_tests, total_tests_failed, total_steps, total_steps_failed);
            } else if (entry.substr(0, 2) == "# ") {
                cout << endl << test_filename << ": " << entry.substr(2) << endl;
            } else if (entry.substr(0, 3) == "## ") {
                // indicates the status of previous test
                if (failed == false && tests > 0) cout << FG_GREEN << " PASSED" << COLOR_OFF << endl;
                failed = false;

                // read a test title
                test_title = entry;
                is_first_step = true;
                is_test_error_shown = false;
                cout << test_title;
            } else if (entry.find(stack_size, 0) == 0) {  // treat "-> stack size should be "
                // count test and step
                if (is_first_step) tests++;
                steps++;

                // check current stack size
                istringstream isub;
                int size;

                isub.str(entry.substr(stack_size.size()));
                isub >> size;
                if (size != static_cast<int>(stk.size())) {
                    // count fail test and step
                    if (!is_test_error_shown) {
                        cout << FG_RED << " FAIL" << COLOR_OFF << endl;
                        tests_failed++;
                        is_test_error_shown = true;
                    }
                    steps_failed++;

                    // show failure
                    cout << '\t' << entry << endl;
                    cout << "\t  but real stack size is " << FG_RED << stk.size() << COLOR_OFF << endl;
                    failed = true;
                }
                is_first_step = false;
            } else if (entry.find(stack_value, 0) == 0) {  // treat "-> stack should be "
                // count test
                if (is_first_step) tests++;
                steps++;

                // check current stack value
                string stack_should_be = entry.substr(stack_value.size());
                string stack_is;

                GetStackAsString(stack_is, stk);

                if (stack_is != stack_should_be) {
                    // count fail test and step
                    if (!is_test_error_shown) {
                        cout << FG_RED << " FAIL" << COLOR_OFF << endl;
                        tests_failed++;
                        is_test_error_shown = true;
                    }
                    steps_failed++;

                    // show failure
                    cout << '\t' << entry << endl;
                    cout << "\t but real stack is " << FG_RED << stack_is << COLOR_OFF << endl;
                    failed = true;
                }
                is_first_step = false;
            } else if (entry.find(cmd_error, 0) == 0) {  // treat "-> error should be "
                // count test
                if (is_first_step) tests++;
                steps++;

                // check current error
                istringstream isub;
                int err_should_be;
                isub.str(entry.substr(cmd_error.size()));
                isub >> err_should_be;
                if (err_should_be != last_err) {
                    // count fail test and step
                    if (!is_test_error_shown) {
                        cout << FG_RED << " FAIL" << COLOR_OFF << endl;
                        tests_failed++;
                        is_test_error_shown = true;
                    }
                    steps_failed++;

                    // show failure
                    cout << '\t' << entry << endl;
                    cout << "\t but last error is " << FG_RED << last_err << COLOR_OFF << endl;
                    failed = true;
                }
                is_first_step = false;
            } else if (entry.find(cmd_exit, 0) == 0) {
                // forced test end
                break;
            } else if (entry.find("->", 0) == 0) {  // treat unknown "->"
                // count test
                if (is_first_step) tests++;
                steps++;

                // count fail test and step
                if (!is_test_error_shown) {
                    cout << FG_RED << " FAIL" << COLOR_OFF << endl;
                    tests_failed++;
                    is_test_error_shown = true;
                }
                steps_failed++;

                // show failure
                cout << FG_RED << "\tthis test '" << entry << "' is unknown" << COLOR_OFF << endl;
                failed = true;
            } else {
                // parse entry and run line
                FindAndReplaceAll(entry, "`", "");
                if (!entry.empty()) {
                    Program prog(stk, hp);
                    ret = prog.Parse(entry);
                    if (ret == kOk) (void)prog.Run();
                    last_err = static_cast<int>(prog.GetLastError());
                }
            }
        }

        // last test
        // indicates the status of previous test
        if (failed == false && tests > 0) cout << FG_GREEN << " PASSED" << COLOR_OFF << endl;

        // cerr back
        cerr.rdbuf(cerr_old_buffer);

        // conclusion: show and keep for total
        if (tests != 0) {
            ShowTestResult("", tests, tests_failed, steps, steps_failed);

            total_tests += tests;
            total_tests_failed += tests_failed;
            total_steps += steps;
            total_steps_failed += steps_failed;
        }
    } else {
        cerr << "test file '" << test_filename << "' not found" << endl;
    }
}
