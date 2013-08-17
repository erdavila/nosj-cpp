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

void test_stringify_number() {
	assert_stringify(0, "0");
	assert_stringify(468312354LL, "468312354");
	assert_stringify(-468312354LL, "-468312354");
	assert_stringify(0.0, "0.0");
	assert_stringify(156.015625, "156.015625");
	assert_stringify(0.0001220703125, "0.0001220703125");
	assert_stringify(-156.015625, "-156.015625");
	assert_stringify(-0.0001220703125, "-0.0001220703125");
	assert_stringify(0.0000152587890625, "1.52587890625e-05");
}

std::string quoted(const std::string& str) {
	return '"' + str + '"';
}

void test_stringify_string() {
	assert_stringify("", quoted(""));
	assert_stringify(std::string({'\x0'}), R"("\u0000")");
	assert_stringify("\x01", R"("\u0001")");
	assert_stringify("\x02", R"("\u0002")");
	assert_stringify("\x08", R"("\b")");
	assert_stringify("\x09", R"("\t")");
	assert_stringify("\x0A", R"("\n")");
	assert_stringify("\x0B", R"("\u000B")");
	assert_stringify("\x0C", R"("\f")");
	assert_stringify("\x0D", R"("\r")");
	assert_stringify("\x1F", R"("\u001F")");
	assert_stringify("\x20", R"(" ")");
	assert_stringify("\x22", R"("\"")");
	assert_stringify("\x5C", R"("\\")");
	assert_stringify("\x7A", R"("z")");
	assert_stringify("\x7E", R"("~")");
	assert_stringify("\x7F", quoted(u8"\u007F"));
	assert_stringify(u8"\u0080", quoted(u8"\u0080"));
	assert_stringify(u8"\u6C34", quoted(u8"\u6C34"));
	assert_stringify(u8"\uFFFF", quoted(u8"\uFFFF"));
	assert_stringify(u8"\U00010000", quoted(u8"\U00010000"));
	assert_stringify(u8"\U0001D11E", quoted(u8"\U0001D11E"));
	assert_stringify(u8"\U00064321", quoted(u8"\U00064321"));
	assert_stringify(u8"\U0010FFFD", quoted(u8"\U0010FFFD"));
	assert_stringify("Hello\nGood bye!", R"("Hello\nGood bye!")");
}

}

namespace tests {
	void stringify() {
		TEST(stringify_null);
		TEST(stringify_boolean);
		TEST(stringify_number);
		TEST(stringify_string);
	}
}
