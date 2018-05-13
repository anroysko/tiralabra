#ifndef __TESTING_TESTGENS_LINK_CUT_TESTS
#define __TESTING_TESTGENS_LINK_CUT_TESTS

#include <string> // std::string
#include "./../test_utils.h"

void genLinkCutTests(bool create_tests, const std::string& base_path);
TestGroup getLinkCutTests(bool perf_tests, const std::string& base_path);

#endif // __TESTING_TESTGENS_DINIC_TESTS
