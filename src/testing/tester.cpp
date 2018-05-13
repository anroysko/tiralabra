#include <vector> // vector
#include <iostream> // ios_base, cout
#include <string.h> // strcmp

#include "./testgens/splay_tests.h" // getSplayTests
#include "./testgens/linkcut_tests.h" // getLinkCutTests
#include "./testgens/dinic_tests.h" // getDinicTests
#include "test_utils.h" // Test, TestGroup, testTestGroups()

/// Main-function.
/// Command line arguments:
/// "test": run tests.
/// "performance": run performance tests.
/// "generate": generate tests.
int main(int argc, char* argv[]) {
	std::string base_path = "./testing/tests/";

	bool should_gen = false;
	bool should_test = false;
	bool should_perf = false;

	if (argc == 1) should_test = true;
	for (int i = 1; i < argc; ++i) {
		if (strcmp(argv[i], "test") == 0) should_test = true;
		if (strcmp(argv[i], "performance") == 0) should_perf = true;
		if (strcmp(argv[i], "generate") == 0) should_gen = true;
	}

	if (should_gen) {
		std::cout << "Generating splay tests:\n";
		genSplayTests(true, base_path);
		std::cout << "Generating Link/Cut tests:\n";
		genLinkCutTests(true, base_path);
		std::cout << "Generating Dinic tests:\n";
		genDinicTests(true, base_path);
		std::cout << "Done generating tests!\n";
	}
	if (should_test) {
		std::vector<TestGroup> test_groups (3);
		test_groups[0] = getSplayTests(false, base_path);
		test_groups[1] = getLinkCutTests(false, base_path);
		test_groups[2] = getDinicTests(false, base_path);
		std::string start_msg = "RUN TESTS";
		std::string end_msg = "FINISH TESTS";
		testTestGroups(test_groups, start_msg, end_msg);
	}
	if (should_perf) {
		std::vector<TestGroup> test_groups (3);
		test_groups[0] = getSplayTests(true, base_path);
		test_groups[1] = getLinkCutTests(true, base_path);
		test_groups[2] = getDinicTests(true, base_path);
		std::string start_msg = "RUN PERFORMANCE TESTS";
		std::string end_msg = "FINISH PERFORMANCE TESTS";
		testTestGroups(test_groups, start_msg, end_msg);
	}
}
