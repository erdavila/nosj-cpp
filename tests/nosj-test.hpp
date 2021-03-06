#ifndef NOSJ_TEST_HPP_
#define NOSJ_TEST_HPP_


#include "nosj/values.hpp"

#include <cassert>
#define assert_eq(A, B)  assert(A == B); assert(B == A); assert(!(A != B)); assert(!(B != A))
#define assert_neq(A, B) assert(A != B); assert(B != A); assert(!(A == B)); assert(!(B == A))
#define assert_throws(CODE, EX) try { CODE; assert(false); } catch(EX&) { assert(true); }

namespace tests {
	void run_test_function(const char*, void(*)());
}

#define TEST(NAME) tests::run_test_function(#NAME, test_##NAME);


template <typename Tested, typename ExpectedValue>
void assert_value(Tested& v, ExpectedValue expectedValue) {
	assert_eq(v, v);
	assert_eq(v, expectedValue);
	assert_eq(v, nosj::Value(expectedValue));
}

inline std::string join_lines(std::initializer_list<std::string> lines) {
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


#endif /* NOSJ_TEST_HPP_ */
