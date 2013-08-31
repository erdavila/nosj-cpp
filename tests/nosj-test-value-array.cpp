#include "nosj-test.hpp"


namespace /*unnamed*/ {

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

}

namespace tests {
	void value_array() {
		TEST(value_array_constructor);
		TEST(value_array_assignment);
		TEST(value_array_reference);
		TEST(value_array_copy);
	}
}
