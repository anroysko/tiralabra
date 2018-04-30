#include <vector>
#include <utility>

#include "tests/splay_tests.h"
#include "tests/linkcut_tests.h"
#include "tests/dinic_tests.h"
#include "test_util.h"

int main(int argc, char* argv[]) {
	std::vector<TestGroup> test_groups;
	if (argc == 1) {
		TestGroup splay_tests = getSplayTests();
		TestGroup linkcut_tests = getLinkCutTests();
		TestGroup dinic_tests = getDinicTests();
		test_groups = {splay_tests, linkcut_tests, dinic_tests};
	} else {
		TestGroup splay_time_tests = getSplayTimeTests();
		TestGroup linkcut_time_tests = getLinkCutTimeTests();
		TestGroup dinic_time_tests = getDinicTimeTests();
		test_groups = {splay_time_tests, linkcut_time_tests, dinic_time_tests};
	}
	testTestGroups(test_groups);
}
