#include "nosj-test.hpp"


namespace /*unnamed*/ {

	template <typename Tested>
	void assert_value_null(Tested& v) {
		assert_value(v, nosj::null);

		assert(v.type() == nosj::Value::Type::NullValue);

		assert(v.isNull());
		assert(!v.isBoolean());
		assert(!v.isNumber());
		assert(!v.isString());
		assert(!v.isArray());
		assert(!v.isObject());

		v.asNull();
		assert_throws(v.asBoolean(), nosj::InvalidConversion);
		assert_throws(v.asNumber(),  nosj::InvalidConversion);
		assert_throws(v.asString(),  nosj::InvalidConversion);
		assert_throws(v.asArray(),   nosj::InvalidConversion);
		assert_throws(v.asObject(),  nosj::InvalidConversion);

		const Tested& constV = v;
		constV.asNull();
		assert_throws(constV.asBoolean(), nosj::InvalidConversion);
		assert_throws(constV.asNumber(),  nosj::InvalidConversion);
		assert_throws(constV.asString(),  nosj::InvalidConversion);
		assert_throws(constV.asArray(),   nosj::InvalidConversion);
		assert_throws(constV.asObject(),  nosj::InvalidConversion);
	}

	void test_value_null() {
		{
			nosj::Value v;
			assert_value_null(v);
		}

		{
			nosj::Value v = nosj::null;
			assert_value_null(v);
		}
	}

}

namespace tests {
	void value_null() {
		TEST(value_null);
	}
}
