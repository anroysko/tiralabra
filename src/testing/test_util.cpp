#include <chrono> // For timing
#include <string> // std::string
#include <iostream> // std::cout
#include <iomanip> // std::setprecision
#include <vector> // std::vector
#include <utility> // std::pair

#include "test_util.h" // Implement functions for 

using TestFunc = bool (*)(); // Function that takes nothing and returns a boolean

Test makeTest(TestFunc test_function, std::string test_name, bool should_time) {
	Test res;
	res.function = test_function;
	res.name = test_name;
	res.should_time = should_time;
	return res;
}

TestGroup makeTestGroup(std::vector<Test> tests, std::string test_group_name, bool should_time) {
	TestGroup res;
	res.tests = tests;
	res.name = test_group_name;
	res.should_time = should_time;
	return res;
}

int getLongestName(const TestGroup& group) {
	int res = 0;
	for (int i = 0; i < group.tests.size(); ++i) res = std::max(res, (int)group.tests[i].name.size());
	return res;
}

std::string makeChar(int count, char c) {
	std::string res;
	for (int i = 0; i < count; ++i) res += c;
	return res;
}

bool testTest(const Test& test, int indent_len, int indents, int add_width) {
	int total_indent = indent_len * indents;
	if (add_width != 0) add_width = add_width - test.name.size();

	auto start_time = std::chrono::high_resolution_clock::now();
	bool res = test.function();
	if (res == false) {
		std::cout << '!' << makeChar(total_indent-1, '-') << test.name;
		std::cout << ": failed!" << "\n";
	}
	auto finish_time = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = finish_time - start_time;
	if (test.should_time) {
		std::cout << makeChar(total_indent, ' ') << test.name;
		std::cout << ": " << makeChar(add_width, ' ') << std::fixed << std::setprecision(3) << duration.count() << '\n';
	}
	return res;
}

int testTestGroup(const TestGroup& group, int indent_len, int indents) {
	int total_indent = indent_len * indents;
	std::cout << makeChar(total_indent, '-') << group.name << ":\n";
	
	int total = group.tests.size();
	int correct = 0;
	int max_test_name_len = getLongestName(group);
	
	auto start_time = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < total; ++i) {
		correct += testTest(group.tests[i], indent_len, indents + 1, max_test_name_len);
	}
	auto finish_time = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = finish_time - start_time;

	if (correct == total) {
		// std::cout << std::setw(set_width + indent_len * indents) << group.name;
		std::cout << makeChar(total_indent, ' ') << group.name << ": All tests correct\n";
	} else {
		// std::cout << std::setw(set_width + indent_len * indents) << group.name;
		std::cout << makeChar(total_indent, ' ') << group.name << ": " << correct << "/" << total << " tests correct\n";
	}
	if (group.should_time) {
		// std::cout << std::setw(set_width + indent_len * indents) << group.name;
		std::cout << makeChar(total_indent, ' ') << group.name << ": " << std::fixed << std::setprecision(3) << duration.count() << '\n';
	}
	return total - correct;
}

int testTestGroups(const std::vector<TestGroup>& test_groups, int indent_len, int indents) {
	std::cout << makeChar(20, '*') << "   TESTING   " << makeChar(20, '*') << "\n";

	int errors = 0;	
	auto start_time = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < test_groups.size(); ++i) {
		errors += testTestGroup(test_groups[i], indent_len, indents + 1);
	}
	auto finish_time = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = finish_time - start_time;
	
	std::cout << makeChar(20, '*') << " END TESTING " << makeChar(20, '*') << "\n";
	std::cout << "Errors: " << errors << "\n";
	std::cout << "Time: " << std::fixed << std::setprecision(3) << duration.count() << '\n';
}
