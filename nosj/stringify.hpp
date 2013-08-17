#ifndef STRINGIFY_HPP_
#define STRINGIFY_HPP_


#include "values.hpp"
#include <ostream>


namespace nosj {


namespace _details {
	class StringifiableValue;
	std::ostream& operator<<(std::ostream&, const StringifiableValue&);
}


_details::StringifiableValue pretty(const Value&);
_details::StringifiableValue nopretty(const Value&);

std::ostream& operator<<(std::ostream&, const Value&);

std::string stringify(const Value&, bool pretty = false);
void writeTo(std::ostream&, const Value&, bool pretty = false);


}


#include "stringify.inl"


#endif /* STRINGIFY_HPP_ */
