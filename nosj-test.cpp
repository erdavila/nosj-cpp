#include <cassert>
#include "nosj.hpp"
#include <exception>

#define assert_eq(A, B)  assert(A == B); assert(B == A); assert(!(A != B)); assert(!(B != A))
#define assert_neq(A, B) assert(A != B); assert(B != A); assert(!(A == B)); assert(!(B == A))
#define assert_throws(CODE, EX) try { CODE; assert(false); } catch(EX&) { assert(true); }
#define UNUSED __attribute__((unused))


namespace { // Unnamed namespace

template <typename T>
void assert_null(T& v) {
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

template <typename T, typename U>
void assert_value(T& v, U value) {
	assert_eq(v, v);
	assert_eq(v, value);
	assert_eq(v, nosj::Value(value));
}

template <typename T>
void assert_boolean(T& v, bool b) {
	assert_value(v, b);
	assert_neq(v, nosj::null);

	assert(v.type() == nosj::Type::BooleanType);

	assert(!v.isNull());
	assert(v.isBoolean());
	assert(!v.isNumber());
	assert(!v.isIntegerNumber());
	assert(!v.isFloatNumber());
	assert(!v.isString());
	assert(!v.isArray());
	assert(!v.isObject());

	assert_throws(v.asNull(),          nosj::InvalidConversion);
	assert(b == v.asBoolean());
	assert_throws(v.asNumber(),        nosj::InvalidConversion);
	assert_throws(v.asIntegerNumber(), nosj::InvalidConversion);
	assert_throws(v.asFloatNumber(),   nosj::InvalidConversion);
	assert_throws(v.asString(),        nosj::InvalidConversion);
	assert_throws(v.asArray(),         nosj::InvalidConversion);
	assert_throws(v.asObject(),        nosj::InvalidConversion);
}

void test_boolean_constructor() {
	nosj::Value t = true;
	assert_boolean(t, true);

	nosj::Value f = false;
	assert_boolean(f, false);

	assert_neq(t, f);
}

void test_boolean_assignment() {
	nosj::Value v;

	v = true;

	assert_boolean(v, true);
}

void test_boolean_reference() {
	nosj::Value v = true;

	bool& b = v.asBoolean();
	b = false;

	assert_eq(v, false);
}

void test_boolean_copy() {
	nosj::Value t = true;
	nosj::Value v = t;

	assert_eq(v, true);
	assert_eq(t, true);

	v = false;

	assert_eq(v, false);
	assert_eq(t, true);
}

template <typename T>
void assert_integer(T& v, long long int i) {
	assert_value(v, i);
	assert_neq(v, nosj::null);
	assert_neq(v, true);
	assert_neq(v, nosj::Value(true));

	assert(v.type() == nosj::Type::NumberType);
	assert(v.asNumber().type() == nosj::Type::IntegerNumberType);

	assert(!v.isNull());
	assert(!v.isBoolean());
	assert(v.isNumber());
	assert(v.isIntegerNumber());
	assert(!v.isFloatNumber());
	assert(!v.isString());
	assert(!v.isArray());
	assert(!v.isObject());

	assert_throws(v.asNull(),          nosj::InvalidConversion);
	assert_throws(v.asBoolean(),       nosj::InvalidConversion);
	assert(i == v.asNumber());
	assert(i == v.asIntegerNumber());
	assert_throws(v.asFloatNumber(),   nosj::InvalidConversion);
	assert_throws(v.asString(),        nosj::InvalidConversion);
	assert_throws(v.asArray(),         nosj::InvalidConversion);
	assert_throws(v.asObject(),        nosj::InvalidConversion);
}

void test_integer_constructor() {
	nosj::Value i1 = static_cast<char>(1);
	assert_integer(i1, 1);

	nosj::Value i2 = static_cast<short>(2);
	assert_integer(i2, 2);

	nosj::Value i3 = static_cast<int>(3);
	assert_integer(i3, 3);

	nosj::Value i4 = static_cast<long>(4);
	assert_integer(i4, 4);

	nosj::Value i5 = static_cast<long long>(5);
	assert_integer(i5, 5);

	assert_neq(i1, i2);
	assert_neq(i1, i3);
	assert_neq(i1, i4);
	assert_neq(i1, i5);

	assert_neq(i2, i3);
	assert_neq(i2, i4);
	assert_neq(i2, i5);

	assert_neq(i3, i4);
	assert_neq(i3, i5);

	assert_neq(i4, i5);
}

void test_integer_assignment() {
	nosj::Value v;

	v = static_cast<char>(1);
	assert_integer(v, 1);

	v = static_cast<short>(2);
	assert_integer(v, 2);

	v = static_cast<int>(3);
	assert_integer(v, 3);

	v = static_cast<long>(4);
	assert_integer(v, 4);

	v = static_cast<long long>(5);
	assert_integer(v, 5);
}

void test_integer_number_reference() {
	nosj::Value v = 7;

	nosj::Number& n = v.asNumber();

	n = static_cast<char>(1);
	assert(n == 1);
	assert_integer(v, 1);

	n = static_cast<short>(2);
	assert(n == 2);
	assert_integer(v, 2);

	n = static_cast<int>(3);
	assert(n == 3);
	assert_integer(v, 3);

	n = static_cast<long>(4);
	assert(n == 4);
	assert_integer(v, 4);

	n = static_cast<long long>(5);
	assert(n == 5);
	assert_integer(v, 5);
}

void test_integer_reference() {
	nosj::Value v = 7;

	nosj::IntegerNumber& i = v.asIntegerNumber();

	i = static_cast<char>(1);
	assert_integer(v, 1);

	i = static_cast<short>(2);
	assert_integer(v, 2);

	i = static_cast<int>(3);
	assert_integer(v, 3);

	i = static_cast<long>(4);
	assert_integer(v, 4);

	i = static_cast<long long>(5);
	assert_integer(v, 5);
}

void test_integer_copy() {
	nosj::Value i = 3;
	nosj::Value i2 = i;

	assert_eq(i, 3);
	assert_eq(i2, 3);

	i2 = 4;

	assert_integer(i, 3);
	assert_integer(i2, 4);
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

	TEST(boolean_constructor);
	TEST(boolean_assignment);
	TEST(boolean_reference);
	TEST(boolean_copy);

	TEST(integer_constructor);
	TEST(integer_assignment);
	TEST(integer_number_reference);
	TEST(integer_reference);
	TEST(integer_copy);

	cout << endl;
	cout << "PASSED: " << coloredCount(passedCount, GREEN) << endl;
	cout << "FAILED: " << coloredCount(failedCount, RED)   << endl;
	return 0;
}
