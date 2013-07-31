#include "nosj.hpp"
#include <cassert>


static void test_passes() {
	assert(true);
}

static void test_fails() {
	assert(false);
}


#include <iostream>
#include <cstdlib>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

#define TEST(NAME) {                              \
	cerr << #NAME "... " << flush;                \
	pid_t pid = fork();                           \
	if(pid) {                                     \
		/* parent */                              \
		int status;                               \
		waitpid(pid, &status, 0);                 \
		if(WIFEXITED(status)) {                   \
			cerr << "\e[32mPASSED\e[0m" << endl;  \
		} else {                                  \
			cerr << "\e[31mFAILED\e[0m" << endl;  \
		}                                         \
	} else {                                      \
		/* child */                               \
		test_ ## NAME();                          \
		exit(0);                                  \
	}                                             \
}


int main() {
	struct rlimit rlim;
	rlim.rlim_cur = rlim.rlim_max = 128 * 1024 * 1024;
	assert(setrlimit(RLIMIT_AS, &rlim) == 0);

	TEST(passes);
	TEST(fails);

	cout << "END" << endl;
	return 0;
}
