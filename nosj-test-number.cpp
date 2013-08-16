#include "nosj-test.hpp"


namespace /*unnamed*/ {

	template <typename Target, typename Tested, typename ExpectedValue>
	void assert_number_conversion(Tested& n, ExpectedValue expectedValue) {
		Target var = n;
		assert(var == static_cast<Target>(expectedValue));
	}

	template <typename Tested, typename ExpectedValue>
	void assert_number(Tested& n, ExpectedValue expectedValue) {
		assert_eq(n, n);
		assert_eq(n, expectedValue);
		assert_neq(n, expectedValue + 1);

		assert_number_conversion<char>(n, expectedValue);
		assert_number_conversion<short>(n, expectedValue);
		assert_number_conversion<int>(n, expectedValue);
		assert_number_conversion<long>(n, expectedValue);
		assert_number_conversion<long long>(n, expectedValue);
		assert_number_conversion<float>(n, expectedValue);
		assert_number_conversion<double>(n, expectedValue);
		assert_number_conversion<long double>(n, expectedValue);
	}

	template <typename Tested, typename ExpectedValue>
	void assert_integer_number(Tested& n, ExpectedValue expectedValue) {
		assert_number(n, expectedValue);
		assert(n.type() == nosj::Number::Type::IntegerNumber);
	}

	template <typename Tested, typename ExpectedValue>
	void assert_float_number(Tested& n, ExpectedValue expectedValue) {
		assert_number(n, expectedValue);
		assert(n.type() == nosj::Number::Type::FloatNumber);
	}

	void test_number_constructor() {
		nosj::Number n0;
		assert_integer_number(n0, 0);

		nosj::Number n1 = static_cast<char>(1);
		assert_integer_number(n1, 1);

		nosj::Number n2 = static_cast<short>(2);
		assert_integer_number(n2, 2);

		nosj::Number n3 = static_cast<int>(3);
		assert_integer_number(n3, 3);

		nosj::Number n4 = static_cast<long>(4);
		assert_integer_number(n4, 4);

		nosj::Number n5 = static_cast<long long>(5);
		assert_integer_number(n5, 5);

		nosj::Number n6 = static_cast<float>(6.5);
		assert_float_number(n6, 6.5);

		nosj::Number n7 = static_cast<double>(7.25);
		assert_float_number(n7, 7.25);

		nosj::Number n8 = static_cast<long double>(8.125);
		assert_float_number(n8, 8.125);

		assert_neq(n0, n1);
		assert_neq(n0, n2);
		assert_neq(n0, n3);
		assert_neq(n0, n4);
		assert_neq(n0, n5);
		assert_neq(n0, n6);
		assert_neq(n0, n7);
		assert_neq(n0, n8);

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

	void test_number_assignment() {
		nosj::Number n;

		n = static_cast<char>(1);
		assert_integer_number(n, 1);

		n = static_cast<short>(2);
		assert_integer_number(n, 2);

		n = static_cast<int>(3);
		assert_integer_number(n, 3);

		n = static_cast<long>(4);
		assert_integer_number(n, 4);

		n = static_cast<long long>(5);
		assert_integer_number(n, 5);

		n = static_cast<float>(6.5);
		assert_float_number(n, 6.5);

		n = static_cast<double>(7.25);
		assert_float_number(n, 7.25);

		n = static_cast<long double>(8.125);
		assert_float_number(n, 8.125);
	}

	void test_number_integer_reference() {
		nosj::Number n = 3;

		nosj::Number::Integer& i = n.integerRef();
		i += 4;

		assert_integer_number(n, 7);

		assert_throws(n.floatRef(), nosj::Number::InvalidType);
	}

	void test_number_float_reference() {
		nosj::Number n = 1.25;

		nosj::Number::Float& f = n.floatRef();
		f += 2.25;

		assert_float_number(n, 3.5);

		assert_throws(n.integerRef(), nosj::Number::InvalidType);
	}

}

namespace tests {
	void number() {
		TEST(number_constructor);
		TEST(number_assignment);
		TEST(number_integer_reference);
		TEST(number_float_reference);
	}
}
