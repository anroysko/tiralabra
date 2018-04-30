#ifndef __TESTING_TESTS_DINIC_TESTS
#define __TESTING_TESTS_DINIC_TESTS

#include <vector>
#include "./../test_util.h"

TestGroup getDinicTests() {
	std::vector<Test> none;
	return makeTestGroup(none, "dinic tests", false);
}
TestGroup getDinicTimeTests() {
	std::vector<Test> none;
	return makeTestGroup(none, "dinic performance tests", false);
}

#endif // __TESTING_TESTS_DINIC_TESTS
