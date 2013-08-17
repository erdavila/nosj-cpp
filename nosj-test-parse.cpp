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

void assert_parse_stream_expected_trail(const std::string& str, unsigned int expectedPosition) {
	nosj::Value v;
	std::istringstream is(str);

	try {
		is >> v;
		assert(false);
	} catch(nosj::ExpectedTrailCodePoint& e) {
		assert(e.position == expectedPosition);
	}

	is.seekg(0);
	try {
		nosj::readFrom(is);
		assert(false);
	} catch(nosj::ExpectedTrailCodePoint& e) {
		assert(e.position == expectedPosition);
	}
}

void assert_parse_string_expected_trail(const std::string& str, unsigned int expectedPosition) {
	try {
		nosj::parse(str);
		assert(false);
	} catch(nosj::ExpectedTrailCodePoint& e) {
		assert(e.position == expectedPosition);
	}
}

void assert_parse_expected_trail(const std::string& str, unsigned int expectedPosition) {
	assert_parse_stream_expected_trail(str, expectedPosition);
	assert_parse_string_expected_trail(str, expectedPosition);
}

void test_parse_string() {
	assert_parse(R"("")", "");

	assert_parse(R"("\u0000")", std::string{'\x0'});
	assert_parse_unexpected(std::string{'"', '\x0', '"'}, '\x0', 1);

	assert_parse(R"("\u0001")", "\x1");
	assert_parse(R"("\u0002")", "\x2");

	assert_parse(R"("\b")",     "\b");
	assert_parse(R"("\u0008")", "\b");
	assert_parse_unexpected(std::string{'"', '\b', '"'}, '\b', 1);

	assert_parse(R"("\t")",     "\t");
	assert_parse(R"("\u0009")", "\t");
	assert_parse_unexpected(std::string{'"', '\t', '"'}, '\t', 1);

	assert_parse(R"("\n")",     "\n");
	assert_parse(R"("\u000A")", "\n");
	assert_parse(R"("\u000a")", "\n");
	assert_parse_unexpected(std::string{'"', '\n', '"'}, '\n', 1);

	assert_parse(R"("\f")",     "\f");
	assert_parse(R"("\u000C")", "\f");
	assert_parse(R"("\u000c")", "\f");
	assert_parse_unexpected(std::string{'"', '\f', '"'}, '\f', 1);

	assert_parse(R"("\r")",     "\r");
	assert_parse(R"("\u000D")", "\r");
	assert_parse(R"("\u000d")", "\r");
	assert_parse_unexpected(std::string{'"', '\r', '"'}, '\r', 1);

	assert_parse(R"("\u001F")", "\x1F");
	assert_parse(R"("\u001f")", "\x1F");
	assert_parse_unexpected(std::string{'"', '\x1F', '"'}, '\x1F', 1);

	assert_parse(R"(" ")",      " ");
	assert_parse(R"("\u0020")", " ");

	assert_parse(R"("\"")",     R"(")");
	assert_parse(R"("\u0022")", R"(")");

	assert_parse(R"("\/")",     R"(/)");
	assert_parse(R"("/")",      R"(/)");
	assert_parse(R"("\u002F")", R"(/)");
	assert_parse(R"("\u002f")", R"(/)");

	assert_parse(R"("\\")",     R"(\)");
	assert_parse(R"("\u005C")", R"(\)");
	assert_parse(R"("\u005c")", R"(\)");

	assert_parse(R"("z")",      "z");
	assert_parse(R"("\u007A")", "z");
	assert_parse(R"("\u007a")", "z");

	assert_parse(R"("~")",      "~");
	assert_parse(R"("\u007E")", "~");
	assert_parse(R"("\u007e")", "~");

	assert_parse(std::string{'"', '\x7F', '"'}, "\x7F");
	assert_parse(R"("\u007F")", "\x7F");
	assert_parse(R"("\u007f")", "\x7F");

	assert_parse(R"("\u0080")", u8"\u0080");
	assert_parse(R"("\u6C34")", u8"\u6C34");
	assert_parse(R"("\uFFFE")", u8"\uFFFE");

	//assert_parse(R"("\uFFFF")", u8"\uFFFF");   See http://gcc.gnu.org/bugzilla/show_bug.cgi?id=41698
	assert_parse(R"("\uFFFF")", "\xEF\xBF\xBF");

	assert_parse(R"("\uD800\uDC00")", u8"\U00010000");

	assert_parse(R"("\uD834\uDD1E")", u8"\U0001D11E");
	assert_parse(u8"\"\U0001D11E\"",  u8"\U0001D11E");
	assert_parse(u8"\"\U0001d11e\"",  u8"\U0001D11E");

	assert_parse(R"("\uD950\uDF21")", u8"\U00064321");
	assert_parse(u8"\"\U00064321\"",  u8"\U00064321");

	assert_parse(R"("\uDBFF\uDFFD")", u8"\U0010FFFD");
	assert_parse(u8"\"\U0010FFFD\"",  u8"\U0010FFFD");

	assert_parse(R"("Hello\nGood bye!")", "Hello\nGood bye!");

	assert_parse_expected_trail(R"("\uD800\uD800")", 7);
	assert_parse_expected_trail(R"("\uD800x")", 7);
	assert_parse_expected_trail(R"("\uD800")", 7);

	assert_parse_unexpected(R"("\j0000")", 'j', 2);
	assert_parse_unexpected(R"("\x0000")", 'x', 2);
	assert_parse_unexpected(R"("\u000")", '"', 6);
	assert_parse_incomplete(R"("\u000)");
}

void test_parse_array() {
	assert_parse("[]",    nosj::emptyArray);
	assert_parse(" [ ] ", nosj::emptyArray, 4);

	assert_parse("[null]",     nosj::Array{nosj::null});
	assert_parse(" [ null ] ", nosj::Array{nosj::null}, 9);

	assert_parse(R"([false,1.25,"nosj"])",         nosj::Array{false, 1.25, "nosj"});
	assert_parse(R"( [ false , 1.25 , "nosj" ] )", nosj::Array{false, 1.25, "nosj"}, 26);

	assert_parse(
			R"(["nosj",[null,false,34],7.0,[1.25]])",
			nosj::Array {
					"nosj",
					nosj::Array {
							nosj::null,
							false,
							34
					},
					7.0,
					nosj::Array { 1.25 }
			}
	);

	assert_parse(
			join_lines({
				R"([)",
				R"(   "nosj",)",
				R"(   [)",
				R"(      null,)",
				R"(      false,)",
				R"(      34)",
				R"(   ],)",
				R"(   7.0,)",
				R"(   [1.25])",
				R"(])"
			}),
			nosj::Array {
					"nosj",
					nosj::Array {
							nosj::null,
							false,
							34
					},
					7.0,
					nosj::Array { 1.25 }
			}
	);

	assert_parse_incomplete("[");
	assert_parse_incomplete("[null,");
	assert_parse_unexpected("[,", ',', 1);
	assert_parse_unexpected("[null,]", ']', 6);
	assert_parse_unexpected("[null,,]", ',', 6);
	assert_parse_unexpected("[null null]", 'n', 6);
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
		TEST(parse_string);
		TEST(parse_array);
		TEST(parse_invalid);
	}
}
