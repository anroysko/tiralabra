#ifndef __TESTING_TEST_UTIL_H_
#define __TESTING_TEST_UTIL_H_

#include <chrono> // For timing
#include <string> // std::string
#include <iostream> // std::cout
#include <iomanip> // std::setprecision
#include <vector> // std::vector
#include <utility> // std::pair

/// Type "TestFunc" is a function that takes two references to strings and returns a boolean.
using TestFunc = bool (*)(const std::string&, const std::string&);

/// Test class.
/// Contains a function pointer and the args for that function.
struct Test {
	TestFunc function;
	std::string in_file;
	std::string out_file;
	std::string name;
	bool should_time;
};
/// Group of tests.
struct TestGroup {
	std::vector<Test> tests;
	std::string name;
	bool should_time;
};
/// Factory function for tests.
Test makeTest(TestFunc test_function, const std::string& in_file, const std::string& out_file, const std::string& test_name, bool should_time = false);
/// Factory function for test groups.
TestGroup makeTestGroup(const std::vector<Test>& tests, const std::string& test_group_name, bool should_time = false);

/// Test a test.
bool testTest(const Test& test, int precision = 4, int indent_len = 4, int indents = 0, int add_width = 0);
/// Test a test group.
int testTestGroup(const TestGroup& group, int precision = 4, int indent_len = 4, int indents = 0);
/// Test many test groups.
/// start_msg and end_msg are the messages printed at the start and end of the tests.
int testTestGroups(const std::vector<TestGroup>& test_groups, const std::string& start_msg, const std::string& end_msg, int precision = 4, int indent_len = 4, int indents = 0);

#endif // __TESTING_TEST_UTIL_H_
