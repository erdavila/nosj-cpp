#include "nosj-test.hpp"


namespace /*unnamed*/ {

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

}

namespace tests {
	void value_boolean() {
		TEST(value_boolean_constructor);
		TEST(value_boolean_assignment);
		TEST(value_boolean_reference);
		TEST(value_boolean_copy);
	}
}
