#include "nosj-test.hpp"


namespace /*unnamed*/ {

	template <typename Tested, typename ExpectedValue>
	void assert_value_object(Tested& v, ExpectedValue expectedValue) {
		assert_value(v, expectedValue);
		assert_neq(v, nosj::null);
		assert_neq(v, true);
		assert_neq(v, nosj::Value(true));
		assert_neq(v, 7);
		assert_neq(v, nosj::Value(7));
		assert_neq(v, "nosj");
		assert_neq(v, nosj::Value("nosj"));
		assert_neq(v, nosj::Array({ true, nosj::null }));
		assert_neq(v, nosj::Value(nosj::Array{ true, nosj::null }));

		assert(v.type() == nosj::Value::Type::ObjectValue);

		assert(!v.isNull());
		assert(!v.isBoolean());
		assert(!v.isNumber());
		assert(!v.isString());
		assert(!v.isArray());
		assert(v.isObject());

		assert_throws(v.asNull(),    nosj::InvalidConversion);
		assert_throws(v.asBoolean(), nosj::InvalidConversion);
		assert_throws(v.asNumber(),  nosj::InvalidConversion);
		assert_throws(v.asString(),   nosj::InvalidConversion);
		assert_throws(v.asArray(),  nosj::InvalidConversion);
		assert(expectedValue == v.asObject());
	}

	const nosj::Object O1 = {
		std::make_pair("age", 47),
		std::make_pair("name", "John"),
		std::make_pair("married", true),
		std::make_pair("children", nosj::Array{"Mary", "Peter"}),
	};
	const nosj::Object O2 = {
		std::make_pair("number", 7),
		std::make_pair("string", "XyZ"),
		std::make_pair("boolean", false),
		std::make_pair("null", nosj::null),
		std::make_pair("object", nosj::Object{
				std::make_pair("a", 1),
				std::make_pair("b", true),
		}),
	};

	void test_value_object_constructor() {
		nosj::Value o1 = O1;
		assert_value_object(o1, O1);

		nosj::Value o2 = O2;
		assert_value_object(o2, O2);

		assert_neq(o1, o2);
	}

	void test_value_object_assignment() {
		nosj::Value v;

		v = O1;
		assert_value_object(v, O1);

		v = O2;
		assert_value_object(v, O2);
	}

	void test_value_object_reference() {
		nosj::Value v = O1;

		nosj::Object& o = v.asObject();

		o = O2;
		assert_value_object(v, O2);
	}

	void test_value_object_copy() {
		nosj::Value v1 = O1;
		nosj::Value v2 = v1;

		assert_eq(v1, O1);
		assert_eq(v2, O1);

		v2 = O2;

		assert_eq(v1, O1);
		assert_eq(v2, O2);
	}

}

namespace tests {
	void value_object() {
		TEST(value_object_constructor);
		TEST(value_object_assignment);
		TEST(value_object_reference);
		TEST(value_object_copy);
	}
}
