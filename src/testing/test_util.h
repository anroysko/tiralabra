#ifndef __TESTING_TEST_UTIL_H_
#define __TESTING_TEST_UTIL_H_

#include <chrono> // For timing
#include <string> // std::string
#include <iostream> // std::cout
#include <iomanip> // std::setprecision
#include <vector> // std::vector
#include <utility> // std::pair

using TestFunc = bool (*)(); // Function that takes nothing and returns a boolean

struct Test {
	TestFunc function;
	std::string name;
	bool should_time;
};
struct TestGroup {
	std::vector<Test> tests;
	std::string name;
	bool should_time;
};

Test makeTest(TestFunc test_function, std::string test_name, bool should_time = false);
TestGroup makeTestGroup(std::vector<Test> tests, std::string test_group_name, bool should_time = false);

bool testTest(const Test& test, int indent_len = 4, int indents = 0, int add_width = 0);
int testTestGroup(const TestGroup& group, int indent_len = 4, int indents = 0);
int testTestGroups(const std::vector<TestGroup>& test_groups, int indent_len = 4, int indents = 0);

#endif // __TESTING_TEST_UTIL_H_
