#ifndef __TESTING_TESTGENS_SPLAY_TESTS_H_
#define __TESTING_TESTGENS_SPLAY_TESTS_H_

#include "./../test_utils.h"

void genSplayTests(bool create_tests, const std::string& base_path);
TestGroup getSplayTests(bool perf_tests, const std::string& base_path);

#endif // __TESTING_TESTGENS_SPLAY_TESTS_H_
