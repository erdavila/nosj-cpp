#include <cassert>
#include "nosj.hpp"
#include <exception>

#define assert_eq(A, B)  assert(A == B); assert(B == A); assert(!(A != B)); assert(!(B != A))
#define assert_neq(A, B) assert(A != B); assert(B != A); assert(!(A == B)); assert(!(B == A))
#define assert_throws(CODE, EX) try { CODE; assert(false); } catch(EX&) { assert(true); }
#define UNUSED __attribute__((unused))


namespace { // Unnamed namespace

template <typename Tested, typename ExpectedValue>
void assert_value(Tested& v, ExpectedValue expectedValue) {
	assert_eq(v, v);
	assert_eq(v, expectedValue);
	assert_eq(v, nosj::Value(expectedValue));
}

template <typename Tested>
void assert_value_null(Tested& v) {
	assert_value(v, nosj::null);

	assert(v.type() == nosj::Value::Type::NullValue);

	assert(v.isNull());
	assert(!v.isBoolean());
	assert(!v.isNumber());
	assert(!v.isString());
	assert(!v.isArray());
	assert(!v.isObject());

	v.asNull();
	assert_throws(v.asBoolean(), nosj::InvalidConversion);
	assert_throws(v.asNumber(),  nosj::InvalidConversion);
	assert_throws(v.asString(),  nosj::InvalidConversion);
	assert_throws(v.asArray(),   nosj::InvalidConversion);
	assert_throws(v.asObject(),  nosj::InvalidConversion);
}

void test_value_null() {
	{
		nosj::Value v;
		assert_value_null(v);
	}

	{
		nosj::Value v = nosj::null;
		assert_value_null(v);
	}
}

template <typename Tested>
void assert_value_boolean(Tested& v, bool b) {
	assert_value(v, b);
	assert_neq(v, nosj::null);

	assert(v.type() == nosj::Value::Type::BooleanValue);

	assert(!v.isNull());
	assert(v.isBoolean());
	assert(!v.isNumber());
	assert(!v.isString());
	assert(!v.isArray());
	assert(!v.isObject());

	assert_throws(v.asNull(),   nosj::InvalidConversion);
	assert(b == v.asBoolean());
	assert_throws(v.asNumber(), nosj::InvalidConversion);
	assert_throws(v.asString(), nosj::InvalidConversion);
	assert_throws(v.asArray(),  nosj::InvalidConversion);
	assert_throws(v.asObject(), nosj::InvalidConversion);
}

void test_value_boolean_constructor() {
	nosj::Value t = true;
	assert_value_boolean(t, true);

	nosj::Value f = false;
	assert_value_boolean(f, false);

	assert_neq(t, f);
}

void test_value_boolean_assignment() {
	nosj::Value v;

	v = true;

	assert_value_boolean(v, true);
}

void test_value_boolean_reference() {
	nosj::Value v = true;

	bool& b = v.asBoolean();
	b = false;

	assert_eq(v, false);
}

void test_value_boolean_copy() {
	nosj::Value t = true;
	nosj::Value v = t;

	assert_eq(v, true);
	assert_eq(t, true);

	v = false;

	assert_eq(v, false);
	assert_eq(t, true);
}

template <typename Target, typename Tested, typename ExpectedValue>
void assert_number_conversion(Tested& n, ExpectedValue expectedValue) {
	Target var = n;
	assert(var == static_cast<Target>(expectedValue));
}

template <typename Tested, typename ExpectedValue>
void assert_number(Tested& n, ExpectedValue expectedValue) {
	assert_eq(n, n);
	assert_eq(n, expectedValue);
	assert_neq(n, expectedValue + 1);

	assert_number_conversion<char>(n, expectedValue);
	assert_number_conversion<short>(n, expectedValue);
	assert_number_conversion<int>(n, expectedValue);
	assert_number_conversion<long>(n, expectedValue);
	assert_number_conversion<long long>(n, expectedValue);
	assert_number_conversion<float>(n, expectedValue);
	assert_number_conversion<double>(n, expectedValue);
	assert_number_conversion<long double>(n, expectedValue);
}

template <typename Tested, typename ExpectedValue>
void assert_integer_number(Tested& n, ExpectedValue expectedValue) {
	assert_number(n, expectedValue);
	assert(n.type() == nosj::Number::Type::IntegerNumber);
}

template <typename Tested, typename ExpectedValue>
void assert_float_number(Tested& n, ExpectedValue expectedValue) {
	assert_number(n, expectedValue);
	assert(n.type() == nosj::Number::Type::FloatNumber);
}

void test_number_constructor() {
	nosj::Number n0;
	assert_integer_number(n0, 0);

	nosj::Number n1 = static_cast<char>(1);
	assert_integer_number(n1, 1);

	nosj::Number n2 = static_cast<short>(2);
	assert_integer_number(n2, 2);

	nosj::Number n3 = static_cast<int>(3);
	assert_integer_number(n3, 3);

	nosj::Number n4 = static_cast<long>(4);
	assert_integer_number(n4, 4);

	nosj::Number n5 = static_cast<long long>(5);
	assert_integer_number(n5, 5);

	nosj::Number n6 = static_cast<float>(6.5);
	assert_float_number(n6, 6.5);

	nosj::Number n7 = static_cast<double>(7.25);
	assert_float_number(n7, 7.25);

	nosj::Number n8 = static_cast<long double>(8.125);
	assert_float_number(n8, 8.125);

	assert_neq(n0, n1);
	assert_neq(n0, n2);
	assert_neq(n0, n3);
	assert_neq(n0, n4);
	assert_neq(n0, n5);
	assert_neq(n0, n6);
	assert_neq(n0, n7);
	assert_neq(n0, n8);

	assert_neq(n1, n2);
	assert_neq(n1, n3);
	assert_neq(n1, n4);
	assert_neq(n1, n5);
	assert_neq(n1, n6);
	assert_neq(n1, n7);
	assert_neq(n1, n8);

	assert_neq(n2, n3);
	assert_neq(n2, n4);
	assert_neq(n2, n5);
	assert_neq(n2, n6);
	assert_neq(n2, n7);
	assert_neq(n2, n8);

	assert_neq(n3, n4);
	assert_neq(n3, n5);
	assert_neq(n3, n6);
	assert_neq(n3, n7);
	assert_neq(n3, n8);

	assert_neq(n4, n5);
	assert_neq(n4, n6);
	assert_neq(n4, n7);
	assert_neq(n4, n8);

	assert_neq(n5, n6);
	assert_neq(n5, n7);
	assert_neq(n5, n8);

	assert_neq(n6, n7);
	assert_neq(n6, n8);

	assert_neq(n7, n8);
}

void test_number_assignment() {
	nosj::Number n;

	n = static_cast<char>(1);
	assert_integer_number(n, 1);

	n = static_cast<short>(2);
	assert_integer_number(n, 2);

	n = static_cast<int>(3);
	assert_integer_number(n, 3);

	n = static_cast<long>(4);
	assert_integer_number(n, 4);

	n = static_cast<long long>(5);
	assert_integer_number(n, 5);

	n = static_cast<float>(6.5);
	assert_float_number(n, 6.5);

	n = static_cast<double>(7.25);
	assert_float_number(n, 7.25);

	n = static_cast<long double>(8.125);
	assert_float_number(n, 8.125);
}

void test_number_integer_reference() {
	nosj::Number n = 3;

	nosj::Number::Integer& i = n.integerRef();
	i += 4;

	assert_integer_number(n, 7);

	assert_throws(n.floatRef(), nosj::Number::InvalidType);
}

void test_number_float_reference() {
	nosj::Number n = 1.25;

	nosj::Number::Float& f = n.floatRef();
	f += 2.25;

	assert_float_number(n, 3.5);

	assert_throws(n.integerRef(), nosj::Number::InvalidType);
}

template <typename Tested, typename ExpectedValue>
void assert_value_number(Tested& v, ExpectedValue expectedValue) {
	assert_value(v, expectedValue);
	assert_neq(v, nosj::null);
	assert_neq(v, true);
	assert_neq(v, nosj::Value(true));

	assert(v.type() == nosj::Value::Type::NumberValue);

	assert(!v.isNull());
	assert(!v.isBoolean());
	assert(v.isNumber());
	assert(!v.isString());
	assert(!v.isArray());
	assert(!v.isObject());

	assert_throws(v.asNull(),    nosj::InvalidConversion);
	assert_throws(v.asBoolean(), nosj::InvalidConversion);
	assert(expectedValue == v.asNumber());
	assert_throws(v.asString(),  nosj::InvalidConversion);
	assert_throws(v.asArray(),   nosj::InvalidConversion);
	assert_throws(v.asObject(),  nosj::InvalidConversion);
}

void test_value_number_constructor() {
	nosj::Value n1 = static_cast<char>(1);
	assert_value_number(n1, 1);

	nosj::Value n2 = static_cast<short>(2);
	assert_value_number(n2, 2);

	nosj::Value n3 = static_cast<int>(3);
	assert_value_number(n3, 3);

	nosj::Value n4 = static_cast<long>(4);
	assert_value_number(n4, 4);

	nosj::Value n5 = static_cast<long long>(5);
	assert_value_number(n5, 5);

	nosj::Value n6 = static_cast<float>(6.5);
	assert_value_number(n6, 6.5);

	nosj::Value n7 = static_cast<double>(7.25);
	assert_value_number(n7, 7.25);

	nosj::Value n8 = static_cast<long double>(8.125);
	assert_value_number(n8, 8.125);

	assert_neq(n1, n2);
	assert_neq(n1, n3);
	assert_neq(n1, n4);
	assert_neq(n1, n5);
	assert_neq(n1, n6);
	assert_neq(n1, n7);
	assert_neq(n1, n8);

	assert_neq(n2, n3);
	assert_neq(n2, n4);
	assert_neq(n2, n5);
	assert_neq(n2, n6);
	assert_neq(n2, n7);
	assert_neq(n2, n8);

	assert_neq(n3, n4);
	assert_neq(n3, n5);
	assert_neq(n3, n6);
	assert_neq(n3, n7);
	assert_neq(n3, n8);

	assert_neq(n4, n5);
	assert_neq(n4, n6);
	assert_neq(n4, n7);
	assert_neq(n4, n8);

	assert_neq(n5, n6);
	assert_neq(n5, n7);
	assert_neq(n5, n8);

	assert_neq(n6, n7);
	assert_neq(n6, n8);

	assert_neq(n7, n8);
}

void test_value_number_assignment() {
	nosj::Value v;

	v = static_cast<char>(1);
	assert_value_number(v, 1);

	v = static_cast<short>(2);
	assert_value_number(v, 2);

	v = static_cast<int>(3);
	assert_value_number(v, 3);

	v = static_cast<long>(4);
	assert_value_number(v, 4);

	v = static_cast<long long>(5);
	assert_value_number(v, 5);

	v = static_cast<float>(6.5);
	assert_value_number(v, 6.5);

	v = static_cast<double>(7.25);
	assert_value_number(v, 7.25);

	v = static_cast<long double>(8.125);
	assert_value_number(v, 8.125);
}

void test_value_number_reference() {
	nosj::Value v = 7;

	nosj::Number& n = v.asNumber();

	n = static_cast<char>(1);
	assert_value_number(v, 1);

	n = static_cast<short>(2);
	assert_value_number(v, 2);

	n = static_cast<int>(3);
	assert_value_number(v, 3);

	n = static_cast<long>(4);
	assert_value_number(v, 4);

	n = static_cast<long long>(5);
	assert_value_number(v, 5);

	n = static_cast<float>(6.5);
	assert_value_number(v, 6.5);

	n = static_cast<double>(7.25);
	assert_value_number(v, 7.25);

	n = static_cast<long double>(8.125);
	assert_value_number(v, 8.125);
}

void test_value_number_copy() {
	nosj::Value v1 = 3;
	nosj::Value v2 = v1;

	assert_eq(v1, 3);
	assert_eq(v2, 3);

	v2 = 4;

	assert_eq(v1, 3);
	assert_eq(v2, 4);
}

template <typename Tested, typename ExpectedValue>
void assert_value_string(Tested& v, ExpectedValue expectedValue) {
	assert_value(v, expectedValue);
	assert_neq(v, nosj::null);
	assert_neq(v, true);
	assert_neq(v, nosj::Value(true));
	assert_neq(v, 7);
	assert_neq(v, nosj::Value(7));

	assert(v.type() == nosj::Value::Type::StringValue);

	assert(!v.isNull());
	assert(!v.isBoolean());
	assert(!v.isNumber());
	assert(v.isString());
	assert(!v.isArray());
	assert(!v.isObject());

	assert_throws(v.asNull(),    nosj::InvalidConversion);
	assert_throws(v.asBoolean(), nosj::InvalidConversion);
	assert_throws(v.asNumber(),  nosj::InvalidConversion);
	assert(expectedValue == v.asString());
	assert_throws(v.asArray(),   nosj::InvalidConversion);
	assert_throws(v.asObject(),  nosj::InvalidConversion);
}

void test_value_string_constructor() {
	nosj::Value s1 = "nosj";
	assert_value_string(s1, "nosj");

	nosj::Value s2 = std::string("XyZ");
	assert_value_string(s2, "XyZ");

	assert_neq(s1, s2);
}

void test_value_string_assignment() {
	nosj::Value v;

	v = "nosj";
	assert_value_string(v, "nosj");

	v = std::string("XyZ");
	assert_value_string(v, std::string("XyZ"));
}

void test_value_string_reference() {
	nosj::Value v = "";

	nosj::String& s = v.asString();

	s = "nosj";
	assert_value_string(v, "nosj");

	s = std::string("XyZ");
	assert_value_string(v, std::string("XyZ"));
}

void test_value_string_copy() {
	nosj::Value v1 = "nosj";
	nosj::Value v2 = v1;

	assert_eq(v1, "nosj");
	assert_eq(v2, "nosj");

	v2 = std::string("XyZ");

	assert_eq(v1, "nosj");
	assert_eq(v2, std::string("XyZ"));
}


template <typename Tested, typename ExpectedValue>
void assert_value_array(Tested& v, ExpectedValue expectedValue) {
	assert_value(v, expectedValue);
	assert_neq(v, nosj::null);
	assert_neq(v, true);
	assert_neq(v, nosj::Value(true));
	assert_neq(v, 7);
	assert_neq(v, nosj::Value(7));
	assert_neq(v, "nosj");
	assert_neq(v, nosj::Value("nosj"));

	assert(v.type() == nosj::Value::Type::ArrayValue);

	assert(!v.isNull());
	assert(!v.isBoolean());
	assert(!v.isNumber());
	assert(!v.isString());
	assert(v.isArray());
	assert(!v.isObject());

	assert_throws(v.asNull(),    nosj::InvalidConversion);
	assert_throws(v.asBoolean(), nosj::InvalidConversion);
	assert_throws(v.asNumber(),  nosj::InvalidConversion);
	assert_throws(v.asString(),   nosj::InvalidConversion);
	assert(expectedValue == v.asArray());
	assert_throws(v.asObject(),  nosj::InvalidConversion);
}

const nosj::Array A1 = { 7, "nosj", true, nosj::null };
const nosj::Array A2 = { "XyZ", false, nosj::null, A1, 2.25 };

void test_value_array_constructor() {
	nosj::Value a1 = A1;
	assert_value_array(a1, A1);

	nosj::Value a2 = A2;
	assert_value_array(a2, A2);

	assert_neq(a1, a2);
}

void test_value_array_assignment() {
	nosj::Value v;

	v = A1;
	assert_value_array(v, A1);

	v = A2;
	assert_value_array(v, A2);
}

void test_value_array_reference() {
	nosj::Value v = A1;

	nosj::Array& a = v.asArray();

	a = A2;
	assert_value_array(v, A2);
}

void test_value_array_copy() {
	nosj::Value v1 = A1;
	nosj::Value v2 = v1;

	assert_eq(v1, A1);
	assert_eq(v2, A1);

	v2 = A2;

	assert_eq(v1, A1);
	assert_eq(v2, A2);
}

template <typename Tested, typename ExpectedValue>
void assert_value_object(Tested& v, ExpectedValue expectedValue) {
	assert_value(v, expectedValue);
	assert_neq(v, nosj::null);
	assert_neq(v, true);
	assert_neq(v, nosj::Value(true));
	assert_neq(v, 7);
	assert_neq(v, nosj::Value(7));
	assert_neq(v, "nosj");
	assert_neq(v, nosj::Value("nosj"));
	assert_neq(v, nosj::Array({ true, nosj::null }));
	assert_neq(v, nosj::Value(nosj::Array{ true, nosj::null }));

	assert(v.type() == nosj::Value::Type::ObjectValue);

	assert(!v.isNull());
	assert(!v.isBoolean());
	assert(!v.isNumber());
	assert(!v.isString());
	assert(!v.isArray());
	assert(v.isObject());

	assert_throws(v.asNull(),    nosj::InvalidConversion);
	assert_throws(v.asBoolean(), nosj::InvalidConversion);
	assert_throws(v.asNumber(),  nosj::InvalidConversion);
	assert_throws(v.asString(),   nosj::InvalidConversion);
	assert_throws(v.asArray(),  nosj::InvalidConversion);
	assert(expectedValue == v.asObject());
}

const nosj::Object O1 = {
	std::make_pair("age", 47),
	std::make_pair("name", "John"),
	std::make_pair("married", true),
	std::make_pair("children", nosj::Array{"Mary", "Peter"}),
};
const nosj::Object O2 = {
	std::make_pair("number", 7),
	std::make_pair("string", "XyZ"),
	std::make_pair("boolean", false),
	std::make_pair("null", nosj::null),
	std::make_pair("object", nosj::Object{
			std::make_pair("a", 1),
			std::make_pair("b", true),
	}),
};

void test_value_object_constructor() {
	nosj::Value o1 = O1;
	assert_value_object(o1, O1);

	nosj::Value o2 = O2;
	assert_value_object(o2, O2);

	assert_neq(o1, o2);
}

void test_value_object_assignment() {
	nosj::Value v;

	v = O1;
	assert_value_object(v, O1);

	v = O2;
	assert_value_object(v, O2);
}

void test_value_object_reference() {
	nosj::Value v = O1;

	nosj::Object& o = v.asObject();

	o = O2;
	assert_value_object(v, O2);
}

void test_value_object_copy() {
	nosj::Value v1 = O1;
	nosj::Value v2 = v1;

	assert_eq(v1, O1);
	assert_eq(v2, O1);

	v2 = O2;

	assert_eq(v1, O1);
	assert_eq(v2, O2);
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

	TEST(value_null);

	TEST(value_boolean_constructor);
	TEST(value_boolean_assignment);
	TEST(value_boolean_reference);
	TEST(value_boolean_copy);

	TEST(number_constructor);
	TEST(number_assignment);
	TEST(number_integer_reference);
	TEST(number_float_reference);

	TEST(value_number_constructor);
	TEST(value_number_assignment);
	TEST(value_number_reference);
	TEST(value_number_copy);

	TEST(value_string_constructor);
	TEST(value_string_assignment);
	TEST(value_string_reference);
	TEST(value_string_copy);

	TEST(value_array_constructor);
	TEST(value_array_assignment);
	TEST(value_array_reference);
	TEST(value_array_copy);

	TEST(value_object_constructor);
	TEST(value_object_assignment);
	TEST(value_object_reference);
	TEST(value_object_copy);

	cout << endl;
	cout << "PASSED: " << coloredCount(passedCount, GREEN) << endl;
	cout << "FAILED: " << coloredCount(failedCount, RED)   << endl;
	return 0;
}
