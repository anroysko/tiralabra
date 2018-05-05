
#include <vector> // vector

#include "tests/splay_tests.h" // getSplayTests
#include "tests/linkcut_tests.h" // getLinkCutTests
#include "tests/dinic_tests.h" // getDinicTests
#include "test_util.h" // Test, TestGroup, testTestGroups()

int main(int argc, char* argv[]) {
	std::vector<TestGroup> test_groups (3);
	if (argc == 1) {
		test_groups[0] = getSplayTests();
		test_groups[1] = getLinkCutTests();
		test_groups[2] = getDinicTests();
	} else {
		test_groups[0] = getSplayTimeTests();
		test_groups[1] = getLinkCutTimeTests();
		test_groups[2] = getDinicTimeTests();
	}
	testTestGroups(test_groups);
}
