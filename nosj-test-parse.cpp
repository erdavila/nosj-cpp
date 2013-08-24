#include "nosj-test.hpp"
#include "nosj/parse.hpp"
#include <sstream>

namespace /*unnamed*/ {

void assert_stream_position(std::istringstream& is, std::istream::streampos expectedPosition) {
	if(expectedPosition == -1) {
		assert(is.eof()  ||  size_t(is.tellg()) == is.str().length());
	} else {
		auto pos = is.tellg();
		assert(pos == expectedPosition);
	}
}

void assert_parse_stream(const std::string& str, const nosj::Value& expectedValue, std::istream::streampos expectedPosition = -1) {
	nosj::Value v;
	std::istringstream is(str);

	is >> v;
	assert_eq(v, expectedValue);
	assert_stream_position(is, expectedPosition);
	if(expectedValue.isNumber()) {
		assert(expectedValue.asNumber().type() == v.asNumber().type());
	}

	is.clear(); is.seekg(0);
	v = nosj::readFrom(is);
	assert_eq(v, expectedValue);
	assert_stream_position(is, expectedPosition);
	if(expectedValue.isNumber()) {
		assert(expectedValue.asNumber().type() == v.asNumber().type());
	}
}

void assert_parse_string(const std::string& str, const nosj::Value& expectedValue) {
	nosj::Value v = nosj::parse(str);
	assert_eq(v, expectedValue);
	if(expectedValue.isNumber()) {
		assert(expectedValue.asNumber().type() == v.asNumber().type());
	}
}

void assert_parse_stream_unexpected(const std::string& str, char unexpectedChar, std::istream::streampos position) {
	nosj::Value v;
	std::istringstream is(str);

	try {
		is >> v;
		assert(false);
	} catch(nosj::UnexpectedCharacter& e) {
		assert(e.character == unexpectedChar);
		assert(e.position == position);
	}

	is.seekg(0);
	try {
		nosj::readFrom(is);
		assert(false);
	} catch(nosj::UnexpectedCharacter& e) {
		assert(e.character == unexpectedChar);
		assert(e.position == position);
	}
}

void assert_parse_string_unexpected(const std::string& str, char unexpectedChar, unsigned int position) {
	try {
		nosj::parse(str);
		assert(false);
	} catch(nosj::UnexpectedCharacter& e) {
		assert(e.character == unexpectedChar);
		assert(e.position == position);
	}
}

void assert_parse_stream_incomplete(const std::string& str) {
	nosj::Value v;
	std::istringstream is(str);

	try {
		is >> v;
		assert(false);
	} catch(nosj::IncompleteInput&) {
		assert(true);
	}

	is.seekg(0);
	try {
		nosj::readFrom(is);
		assert(false);
	} catch(nosj::IncompleteInput&) {
		assert(true);
	}
}

void assert_parse_string_incomplete(const std::string& str) {
	try {
		nosj::parse(str);
		assert(false);
	} catch(nosj::IncompleteInput&) {
		assert(true);
	}
}

void assert_parse(const std::string& str, const nosj::Value& expectedValue, std::istream::streampos expectedPosition = -1) {
	assert_parse_stream(str, expectedValue, expectedPosition);
	assert_parse_string(str, expectedValue);
}

void assert_parse_stream_ok_but_string_unexpected(const std::string& str,
                                    const nosj::Value expectedStreamValue, std::istream::streampos consumedChars,
                                    char unexpectedStringChar) {
	assert_parse_stream(str, expectedStreamValue, consumedChars);
	assert_parse_string_unexpected(str, unexpectedStringChar, consumedChars);
}

void assert_parse_unexpected(const std::string& str, char unexpectedChar, unsigned int unexpectedCharPosition) {
	assert_parse_stream_unexpected(str, unexpectedChar, unexpectedCharPosition);
	assert_parse_string_unexpected(str, unexpectedChar, unexpectedCharPosition);
}

void assert_parse_incomplete(const std::string& str) {
	assert_parse_stream_incomplete(str);
	assert_parse_string_incomplete(str);
}

void test_parse_null() {
	assert_parse("null", nosj::null);
	assert_parse(" null", nosj::null);
	assert_parse("null ", nosj::null, 4);
	assert_parse_stream_ok_but_string_unexpected("nullnull", nosj::null, 4, 'n');
	assert_parse_unexpected("noll", 'o', 1);
	assert_parse_incomplete("nul");
}

void test_parse_boolean() {
	assert_parse("false", false);
	assert_parse(" false", false);
	assert_parse("false ", false, 5);
	assert_parse_stream_ok_but_string_unexpected("falsenull", false, 5, 'n');
	assert_parse_unexpected("folse", 'o', 1);
	assert_parse_incomplete("fals");

	assert_parse("true", true);
	assert_parse(" true", true);
	assert_parse("true ", true, 4);
	assert_parse_stream_ok_but_string_unexpected("truenull", true, 4, 'n');
	assert_parse_unexpected("tlue", 'l', 1);
	assert_parse_incomplete("tru");
}

void test_parse_number() {
	assert_parse("0", 0);
	assert_parse(" 0", 0);
	assert_parse("0 ", 0, 1);
	assert_parse_stream_ok_but_string_unexpected("0null", 0, 1, 'n');
	assert_parse_stream_ok_but_string_unexpected("0123", 0, 1, '1');
	assert_parse("-0", -0);

	assert_parse("7", 7);
	assert_parse(" 7", 7);
	assert_parse("7 ", 7, 1);
	assert_parse_stream_ok_but_string_unexpected("7null", 7, 1, 'n');
	assert_parse_unexpected("7.null", 'n', 2);
	assert_parse_incomplete("7.");
	assert_parse("-7", -7);

	assert_parse("123", 123);
	assert_parse("-123", -123);
	assert_parse_unexpected("+123", '+', 0);

	assert_parse("-0.0", 0.0);
	assert_parse("0.0", 0.0);
	assert_parse("0e0", 0.0);
	assert_parse("0.0e0", 0.0);
	assert_parse_incomplete("0.");
	assert_parse_incomplete("0e");
	assert_parse_stream_ok_but_string_unexpected("0e0.0", 0.0, 3, '.');
	assert_parse_unexpected("0.e", 'e', 2);
	assert_parse_unexpected("0.null", 'n', 2);
	assert_parse_unexpected("0enull", 'n', 2);
	assert_parse_unexpected("0e-null", 'n', 3);
	assert_parse_stream_ok_but_string_unexpected("0.0null", 0.0, 3, 'n');

	assert_parse("1.25", 1.25);
	assert_parse("1e2", 100.0);
	assert_parse("12.5e-1", 1.25);
	assert_parse("0.125e+1", 1.25);
	assert_parse("0.125e1", 1.25);
	assert_parse("7e0", 7.0);
	assert_parse("7e13", 70000000000000.0);

	assert_parse_incomplete("-");
	assert_parse_unexpected("-.", '.', 1);
	assert_parse_unexpected("+", '+', 0);
}

void test_parse_invalid() {
	assert_parse_incomplete("");
	assert_parse_incomplete(" ");
}

}

namespace tests {
	void parse() {
		TEST(parse_null);
		TEST(parse_boolean);
		TEST(parse_number);
		TEST(parse_invalid);
	}
}
