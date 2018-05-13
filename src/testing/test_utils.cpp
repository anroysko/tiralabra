#include <chrono> // For timing
#include <string> // std::string
#include <iostream> // std::cout
#include <iomanip> // std::setprecision
#include <vector> // std::vector
#include <utility> // std::pair

#include "test_utils.h" // Implement functions for 

Test makeTest(TestFunc test_function, const std::string& in_file, const std::string& out_file, const std::string& test_name, bool should_time) {
	Test res;
	res.function = test_function;
	res.in_file = in_file;
	res.out_file = out_file;
	res.name = test_name;
	res.should_time = should_time;
	return res;
}

TestGroup makeTestGroup(const std::vector<Test>& tests, const std::string& test_group_name, bool should_time) {
	TestGroup res;
	res.tests = tests;
	res.name = test_group_name;
	res.should_time = should_time;
	return res;
}

/// Returns longest name that any of the tests in the testgroup has.
/// Used for aligning runtimes of tests.
int getLongestName(const TestGroup& group) {
	int res = 0;
	for (int i = 0; i < group.tests.size(); ++i) res = std::max(res, (int)group.tests[i].name.size());
	return res;
}

/// Make a string consisting of "count" copies of char "c".
std::string makeChar(int count, char c) {
	std::string res;
	for (int i = 0; i < count; ++i) res += c;
	return res;
}

bool testTest(const Test& test, int precision, int indent_len, int indents, int add_width) {
	int total_indent = indent_len * indents;
	if (add_width != 0) add_width = add_width - test.name.size();

	auto start_time = std::chrono::high_resolution_clock::now();
	bool res = test.function(test.in_file, test.out_file);
	if (res == false) {
		std::cout << '!' << makeChar(total_indent-1, '-') << test.name;
		std::cout << ": " << makeChar(add_width, ' ') << "failed!" << "\n";
	}
	auto finish_time = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = finish_time - start_time;
	if (test.should_time) {
		std::cout << makeChar(total_indent, ' ') << test.name;
		std::cout << ": " << makeChar(add_width, ' ') << std::fixed << std::setprecision(precision) << duration.count() << '\n';
	}
	return res;
}

int testTestGroup(const TestGroup& group, int precision, int indent_len, int indents) {
	int total_indent = indent_len * indents;
	std::cout << makeChar(total_indent, '-') << group.name << ":\n";
	
	int total = group.tests.size();
	int correct = 0;
	int max_test_name_len = getLongestName(group);
	
	auto start_time = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < total; ++i) {
		correct += testTest(group.tests[i], precision, indent_len, indents + 1, max_test_name_len);
	}
	auto finish_time = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = finish_time - start_time;

	if (correct == total) {
		std::cout << makeChar(total_indent, ' ') << group.name << ": All tests correct\n";
	} else {
		std::cout << makeChar(total_indent, ' ') << group.name << ": " << correct << "/" << total << " tests correct\n";
	}
	if (group.should_time) {
		std::cout << makeChar(total_indent, ' ') << group.name << ": " << std::fixed << std::setprecision(precision) << duration.count() << '\n';
	}
	return total - correct;
}

int testTestGroups(const std::vector<TestGroup>& test_groups, const std::string& start_msg, const std::string& end_msg, int precision, int indent_len, int indents) {
	// Print start msg
	int diff = (int)end_msg.size() - (int)start_msg.size();
	if (diff >= 0) {
		std::cout << makeChar(20, '*') << makeChar(diff / 2 + 1, ' ') << start_msg << makeChar((diff+1) / 2 + 1, ' ') << makeChar(20, '*') << "\n";
	} else {
		std::cout << makeChar(20, '*') << ' ' << end_msg << ' ' << makeChar(20, '*') << "\n";
	}

	// Run tests
	int errors = 0;	
	auto start_time = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < test_groups.size(); ++i) {
		errors += testTestGroup(test_groups[i], precision, indent_len, indents + 1);
	}
	auto finish_time = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = finish_time - start_time;
	
	// Print end msg
	diff *= -1;
	if (diff >= 0) {
		std::cout << makeChar(20, '*') << makeChar(diff / 2 + 1, ' ') << end_msg << makeChar((diff+1) / 2 + 1, ' ') << makeChar(20, '*') << "\n";
	} else {
		std::cout << makeChar(20, '*') << ' ' << end_msg << ' ' << makeChar(20, '*') << "\n";
	}

	// Print errors and time.
	std::cout << "Errors: " << errors << "\n";
	std::cout << "Time: " << std::fixed << std::setprecision(precision) << duration.count() << '\n';
}
