#include "nosj-test.hpp"
#include "nosj/stringify.hpp"
#include <initializer_list>
#include <set>
#include <sstream>

namespace /*unnamed*/ {

typedef std::set<std::string> ExpectedSet;

void assert_stringify(const nosj::Value& v, const ExpectedSet& expected, const ExpectedSet& expectedPretty_ = {}) {
	const ExpectedSet& expectedPretty = expectedPretty_.empty() ? expected : expectedPretty_;
	std::ostringstream os;

	os << v;
	assert(expected.count(os.str()) == 1);
	os.str("");

	os << pretty(v);
	assert(expectedPretty.count(os.str()) == 1);
	os.str("");

	os << nopretty(v);
	assert(expected.count(os.str()) == 1);
	os.str("");

	nosj::writeTo(os, v);
	assert(expected.count(os.str()) == 1);
	os.str("");

	nosj::writeTo(os, v, false);
	assert(expected.count(os.str()) == 1);
	os.str("");

	nosj::writeTo(os, v, true);
	assert(expectedPretty.count(os.str()) == 1);

	std::string str = nosj::stringify(v);
	assert(expected.count(str) == 1);

	str = nosj::stringify(v, false);
	assert(expected.count(str) == 1);

	str = nosj::stringify(v, true);
	assert(expectedPretty.count(str) == 1);
}

void assert_stringify(const nosj::Value& v, const std::string& expected, const std::string& expectedPretty_ = "") {
	const std::string& expectedPretty = expectedPretty_.empty() ? expected : expectedPretty_;
	assert_stringify(v, ExpectedSet{expected}, ExpectedSet{expectedPretty});
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

std::string join_lines(std::initializer_list<std::string> lines) {
	std::string joinedLines = "";

	bool first = true;
	for(auto& line : lines) {
		if(!first) {
			joinedLines += "\n";
		}
		joinedLines += line;

		first = false;
	}

	return joinedLines;
}

void test_stringify_array() {
	assert_stringify(nosj::emptyArray, "[]");

	assert_stringify(nosj::Array{"nosj"}, R"(["nosj"])");

	assert_stringify(
			nosj::Array {
					true,
					nosj::null,
					7,
					1.25
			},
			"[true,null,7,1.25]",
			join_lines({
				R"([)",
				R"(   true,)",
				R"(   null,)",
				R"(   7,)",
				R"(   1.25)",
				R"(])"
			})
	);

	assert_stringify(
			nosj::Array {
					"nosj",
					nosj::Array {
							nosj::null,
							false,
							34
					},
					7.0,
					nosj::Array { 1.25 }
			},
			R"(["nosj",[null,false,34],7.0,[1.25]])",
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
			})
	);

	assert_stringify(
			nosj::Array {
				nosj::Object {
					{"name", "John"},
					{"age", 34}
				},
				nosj::emptyObject,
				nosj::Object {
					{"nosj", true}
				}
			},
			ExpectedSet {
				R"([{"name":"John","age":34},{},{"nosj":true}])",
				R"([{"age":34,"name":"John"},{},{"nosj":true}])",
			},
			ExpectedSet {
				join_lines({
					R"([)",
					R"(   {)",
					R"(      "name" : "John",)",
					R"(      "age" : 34)",
					R"(   },)",
					R"(   {},)",
					R"(   {)",
					R"(      "nosj" : true)",
					R"(   })",
					R"(])",
				}),
				join_lines({
					R"([)",
					R"(   {)",
					R"(      "age" : 34,)",
					R"(      "name" : "John")",
					R"(   },)",
					R"(   {},)",
					R"(   {)",
					R"(      "nosj" : true)",
					R"(   })",
					R"(])",
				})
			}
	);
}

void test_stringify_object() {
	assert_stringify(nosj::emptyObject, "{}");

	assert_stringify(
			nosj::Object {
					{ "name", "John" }
			},
			R"({"name":"John"})",
			join_lines({
				R"({)",
				R"(   "name" : "John")",
				R"(})"
			})
	);

	assert_stringify(
			nosj::Object {
					{ "name", "John" },
					{ "age", 34 }
			},
			ExpectedSet {
					R"({"name":"John","age":34})",
					R"({"age":34,"name":"John"})",
			},
			ExpectedSet {
				join_lines({
					R"({)",
					R"(   "name" : "John",)",
					R"(   "age" : 34)",
					R"(})"
				}),
				join_lines({
					R"({)",
					R"(   "age" : 34,)",
					R"(   "name" : "John")",
					R"(})"
				})
			}
	);

	assert_stringify(
			nosj::Object {
					{ "person", nosj::Object {
							{ "name", "John" },
							{ "children", nosj::Array{12, 7} }
					}}
			},
			ExpectedSet {
					R"({"person":{"name":"John","children":[12,7]}})",
					R"({"person":{"children":[12,7],"name":"John"}})",
			},
			ExpectedSet {
				join_lines({
					R"({)"
					R"(   "person" : {)",
					R"(      "name" : "John",)",
					R"(      "children" : [)",
					R"(         12,)",
					R"(         7)",
					R"(      ])",
					R"(   })",
					R"(})"
				}),
				join_lines({
					R"({)",
					R"(   "person" : {)",
					R"(      "children" : [)",
					R"(         12,)",
					R"(         7)",
					R"(      ],)",
					R"(      "name" : "John")",
					R"(   })",
					R"(})"
				})
			}
	);
}

}

namespace tests {
	void stringify() {
		TEST(stringify_null);
		TEST(stringify_boolean);
		TEST(stringify_number);
		TEST(stringify_string);
		TEST(stringify_array);
		TEST(stringify_object);
	}
}
