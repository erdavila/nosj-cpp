#include <cassert>
#include "nosj.hpp"
#include <exception>

#define assert_eq(A, B)  assert(A == B); assert(B == A); assert(!(A != B)); assert(!(B != A))
#define assert_neq(A, B) assert(A != B); assert(B != A); assert(!(A == B)); assert(!(B == A))
#define assert_throws(CODE, EX) try { CODE; assert(false); } catch(EX&) { assert(true); }
#define UNUSED __attribute__((unused))


namespace { // Unnamed namespace

template <typename T>
void assert_null(T v) {
	assert_eq(v, v);
	assert_eq(v, nosj::null);

	assert(v.type() == nosj::Type::NullType);

	assert(v.isNull());
	assert(!v.isBoolean());
	assert(!v.isNumber());
	assert(!v.isIntegerNumber());
	assert(!v.isFloatNumber());
	assert(!v.isString());
	assert(!v.isArray());
	assert(!v.isObject());

	v.asNull();
	assert_throws(v.asBoolean(),       nosj::InvalidConversion);
	assert_throws(v.asNumber(),        nosj::InvalidConversion);
	assert_throws(v.asIntegerNumber(), nosj::InvalidConversion);
	assert_throws(v.asFloatNumber(),   nosj::InvalidConversion);
	assert_throws(v.asString(),        nosj::InvalidConversion);
	assert_throws(v.asArray(),         nosj::InvalidConversion);
	assert_throws(v.asObject(),        nosj::InvalidConversion);
}

void test_null() {
	{
		nosj::Value v;
		assert_null(v);
	}

	{
		nosj::Value v = nosj::null;
		assert_null(v);
	}
}

} /* Unnamed namespace */


#include <iostream>
#include <cstdlib>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;


namespace { // Unnamed namespace

size_t passedCount = 0;
size_t failedCount = 0;

#define GREEN "\e[32m"
#define RED "\e[31m"
#define CLEAR "\e[0m"

#ifndef NOFORK
# define TEST(NAME) {                             \
	cerr << #NAME "... " << flush;                \
	pid_t pid = fork();                           \
	if(pid) {                                     \
		/* parent */                              \
		int status;                               \
		waitpid(pid, &status, 0);                 \
		if(WIFEXITED(status)) {                   \
			passedCount++;                        \
			cerr << GREEN "PASSED" CLEAR << endl; \
		} else {                                  \
			failedCount++;                        \
			cerr << RED "FAILED" CLEAR << endl;   \
		}                                         \
	} else {                                      \
		/* child */                               \
		test_ ## NAME();                          \
		exit(0);                                  \
	}                                             \
}
#else
# define TEST(NAME) {                     \
	cerr << #NAME "... " << flush;        \
	test_ ## NAME();                      \
	cerr << GREEN "PASSED" CLEAR << endl; \
	passedCount++;                        \
}
#endif

string coloredCount(size_t count, const char* color) {
	string countString = to_string(count);
	if(count > 0) {
		return color + countString + CLEAR;
	} else {
		return countString;
	}
}

} /* Unnamed namespace */


int main() {
	struct rlimit rlim;
	rlim.rlim_cur = rlim.rlim_max = 128 * 1024 * 1024;
	assert(setrlimit(RLIMIT_AS, &rlim) == 0);

	TEST(null);

	cout << endl;
	cout << "PASSED: " << coloredCount(passedCount, GREEN) << endl;
	cout << "FAILED: " << coloredCount(failedCount, RED)   << endl;
	return 0;
}
