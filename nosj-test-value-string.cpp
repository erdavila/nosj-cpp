#include "nosj-test.hpp"


namespace /*unnamed*/ {

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

}

namespace tests {
	void value_string() {
		TEST(value_string_constructor);
		TEST(value_string_assignment);
		TEST(value_string_reference);
		TEST(value_string_copy);
	}
}
