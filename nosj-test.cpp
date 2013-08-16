#include <iostream>
#include <cassert>
#include <cstdlib>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

#define GREEN "\e[32m"
#define RED "\e[31m"
#define CLEAR "\e[0m"

namespace /*unnamed*/ {
	size_t passedCount = 0;
	size_t failedCount = 0;

	string coloredCount(size_t count, const char* color) {
		string countString = to_string(count);
		if(count > 0) {
			return color + countString + CLEAR;
		} else {
			return countString;
		}
	}
}

namespace tests {
	void run_test_function(const char* testName, void(*testFunction)()) {
		cerr << testName << "... " << flush;

#ifndef NOFORK
		pid_t pid = fork();
		if(pid) {
			// parent
			int status;
			waitpid(pid, &status, 0);
			if(WIFEXITED(status)) {
				passedCount++;
				cerr << GREEN "PASSED" CLEAR << endl;
			} else {
				failedCount++;
				cerr << RED "FAILED" CLEAR << endl;
			}
		} else {
			// child
			testFunction();
			exit(0);
		}
#else
		testFunction();
		cerr << GREEN "PASSED" CLEAR << endl;
		passedCount++;
#endif

	}


	void number();
	void value_null();
	void value_boolean();
	void value_number();
	void value_string();
	void value_array();
	void value_object();
}


int main() {
	struct rlimit rlim;
	rlim.rlim_cur = rlim.rlim_max = 128 * 1024 * 1024;
	assert(setrlimit(RLIMIT_AS, &rlim) == 0);

	tests::number();
	tests::value_null();
	tests::value_boolean();
	tests::value_number();
	tests::value_string();
	tests::value_array();
	tests::value_object();

	cout << endl;
	cout << "PASSED: " << coloredCount(passedCount, GREEN) << endl;
	cout << "FAILED: " << coloredCount(failedCount, RED)   << endl;
	return 0;
}
