#include "nosj-test.hpp"
#include "nosj/stringify.hpp"
#include <sstream>

namespace /*unnamed*/ {


void assert_stringify(const nosj::Value& v, const std::string& expected, const std::string& expectedPretty_ = "") {
	const std::string& expectedPretty = expectedPretty_.empty() ? expected : expectedPretty_;
	std::ostringstream os;

	os << v;
	assert(os.str() == expected);
	os.str("");

	os << pretty(v);
	assert(os.str() == expectedPretty);
	os.str("");

	os << nopretty(v);
	assert(os.str() == expected);
	os.str("");

	nosj::writeTo(os, v);
	assert(os.str() == expected);
	os.str("");

	nosj::writeTo(os, v, false);
	assert(os.str() == expected);
	os.str("");

	nosj::writeTo(os, v, true);
	assert(os.str() == expectedPretty);

	std::string str = nosj::stringify(v);
	assert(str == expected);

	str = nosj::stringify(v, false);
	assert(str == expected);

	str = nosj::stringify(v, true);
	assert(str == expectedPretty);
}

void test_stringify_null() {
	assert_stringify(nosj::null, "null");
}

void test_stringify_boolean() {
	assert_stringify(false, "false");
	assert_stringify(true, "true");
}

}

namespace tests {
	void stringify() {
		TEST(stringify_null);
		TEST(stringify_boolean);
	}
}
