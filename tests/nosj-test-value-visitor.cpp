#include "nosj-test.hpp"


namespace /*unnamed*/ {

class TestVisitor : public nosj::Visitor {
	void visit(nosj::Null&)    override { assert(false); }
	void visit(nosj::Boolean&) override { assert(false); }
	void visit(nosj::Number&)  override { assert(false); }
	void visit(nosj::String&)  override { assert(false); }
	void visit(nosj::Array&)   override { assert(false); }
	void visit(nosj::Object&)  override { assert(false); }
};

class TestConstVisitor : public nosj::ConstVisitor {
	void visit(const nosj::Null&)    override { assert(false); }
	void visit(const nosj::Boolean&) override { assert(false); }
	void visit(const nosj::Number&)  override { assert(false); }
	void visit(const nosj::String&)  override { assert(false); }
	void visit(const nosj::Array&)   override { assert(false); }
	void visit(const nosj::Object&)  override { assert(false); }
};

template <typename T>
void assert_visit(nosj::Value& v, const T& newValue) {
	struct TV : TestVisitor {
		bool visited = false;
		const T& newValue;
		TV(const T& newValue) : newValue(newValue) {}
		void visit(T& value) override { visited = true; value = newValue; }
	} visitor(newValue);
	v.accept(visitor);
	assert(visitor.visited);
	assert_eq(v, newValue);

	struct : TestConstVisitor {
		bool visited = false;
		void visit(const T&) override { visited = true; }
	} constVisitor;
	v.accept(constVisitor);
	assert(constVisitor.visited);
}

void test_value_visitor_null() {
	nosj::Value v = nosj::null;
	assert_visit<nosj::Null>(v, nosj::null);
}

void test_value_visitor_boolean() {
	nosj::Value v = true;
	assert_visit<nosj::Boolean>(v, false);
}

void test_value_visitor_number() {
	nosj::Value v = 7;
	assert_visit<nosj::Number>(v, 1.25);
}

void test_value_visitor_string() {
	nosj::Value v = "nosj";
	assert_visit<nosj::String>(v, "END");
}

void test_value_visitor_array() {
	nosj::Value v = nosj::emptyArray;
	assert_visit<nosj::Array>(v, {"nosj", 7});
}

void test_value_visitor_object() {
	nosj::Value v = nosj::emptyObject;
	assert_visit<nosj::Object>(v, { {"name","John"}, {"age", 34} });
}

}

namespace tests {

void value_visitor() {
	TEST(value_visitor_null);
	TEST(value_visitor_boolean);
	TEST(value_visitor_number);
	TEST(value_visitor_string);
	TEST(value_visitor_array);
	TEST(value_visitor_object);
}

}
