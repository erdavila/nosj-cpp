#include "nosj-test.hpp"


namespace /*unnamed*/ {

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

}

namespace tests {
	void value_number() {
		TEST(value_number_constructor);
		TEST(value_number_assignment);
		TEST(value_number_reference);
		TEST(value_number_copy);
	}
}
