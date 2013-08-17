#include "nosj-test.hpp"
#include "nosj/parse.hpp"
#include <sstream>

namespace /*unnamed*/ {

void assert_stream_position(std::istream& is, std::istream::streampos expectedPosition) {
	if(expectedPosition == -1) {
		assert(is.eof());
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

	is.seekg(0);
	v = nosj::readFrom(is);
	assert_eq(v, expectedValue);
	assert_stream_position(is, expectedPosition);
}

void assert_parse_string(const std::string& str, const nosj::Value& expectedValue) {
	nosj::Value v = nosj::parse(str);
	assert_eq(v, expectedValue);
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
                                    const nosj::Value expectedStreamValue, std::istream::streampos expectedStreamPosition,
                                    char unexpectedStringChar, unsigned int unexpectedStringCharPosition) {
	assert_parse_stream(str, expectedStreamValue, expectedStreamPosition);
	assert_parse_string_unexpected(str, unexpectedStringChar, unexpectedStringCharPosition);
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
	assert_parse_stream_ok_but_string_unexpected("null null", nosj::null, 4, 'n', 5);
	assert_parse_unexpected("noll", 'o', 1);
	assert_parse_unexpected("nullo", 'o', 4);
	assert_parse_incomplete("nul");
}

void test_parse_boolean() {
	assert_parse("false", false);
	assert_parse(" false", false);
	assert_parse("false ", false, 5);
	assert_parse_stream_ok_but_string_unexpected("false null", false, 5, 'n', 6);
	assert_parse_unexpected("folse", 'o', 1);
	assert_parse_unexpected("falsee", 'e', 5);
	assert_parse_incomplete("fals");

	assert_parse("true", true);
	assert_parse(" true", true);
	assert_parse("true ", true, 4);
	assert_parse_stream_ok_but_string_unexpected("true null", true, 4, 'n', 5);
	assert_parse_unexpected("tlue", 'l', 1);
	assert_parse_unexpected("truee", 'e', 4);
	assert_parse_incomplete("tru");
}

}

namespace tests {
	void parse() {
		TEST(parse_null);
		TEST(parse_boolean);
	}
}
