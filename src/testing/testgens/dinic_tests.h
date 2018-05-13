#ifndef __TESTING_TESTGENS_DINIC_TESTS
#define __TESTING_TESTGENS_DINIC_TESTS

#include "./../test_utils.h"

void genDinicTests(bool create_tests, const std::string& base_path);
TestGroup getDinicTests(bool perf_tests, const std::string& base_path);

#endif // __TESTING_TESTGENS_DINIC_TESTS
