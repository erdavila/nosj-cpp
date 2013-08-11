#ifndef NOSJ_HPP_
#define NOSJ_HPP_

#include <deque>
#include <exception>
#include <string>
#include <unordered_map>
#include <utility>


#define NOT_IMPLEMENTED { assert(! "implemented"); }


namespace nosj {


class Exception : public std::exception {};
class InvalidConversion : public Exception {};


class Null {};
inline bool operator==(const Null&, const Null&) { return true; }
inline bool operator!=(const Null&, const Null&) { return false; }

class Value;

typedef bool Boolean;
typedef long long int IntegerNumber;
typedef long double FloatNumber;
class Number;
typedef std::string String; // UTF-8 bytes
typedef std::deque<Value> Array;
typedef std::unordered_map<String, Value> Object;


namespace { // Unnamed namespace
	const Null null;
}


namespace _details {
	class Impl;
} // namespace _details


enum Type {
	NullType, BooleanType, NumberType,
	IntegerNumberType, FloatNumberType,
	StringType, ArrayType, ObjectType
};


class Value {
public:
	Value(Null=null) noexcept;
	Value(const Value&) noexcept;
	Value(Value&&) noexcept NOT_IMPLEMENTED;
	~Value() noexcept;

	Type type() const noexcept;

	bool isNull()          const noexcept { return true; }
	bool isBoolean()       const noexcept { return false; /* TEMPORARY */ }
	bool isNumber()        const noexcept { return false; /* TEMPORARY */ }
	bool isIntegerNumber() const noexcept { return false; /* TEMPORARY */ }
	bool isFloatNumber()   const noexcept { return false; /* TEMPORARY */ }
	bool isString()        const noexcept { return false; /* TEMPORARY */ }
	bool isArray()         const noexcept { return false; /* TEMPORARY */ }
	bool isObject()        const noexcept { return false; /* TEMPORARY */ }

	Null&          asNull();
	Boolean&       asBoolean();
	Number&        asNumber();
	IntegerNumber& asIntegerNumber();
	FloatNumber&   asFloatNumber();
	String&        asString();
	Array&         asArray();
	Object&        asObject();

	const Null&          asNull()          const;
	const Boolean&       asBoolean()       const;
	const Number&        asNumber()        const;
	const IntegerNumber& asIntegerNumber() const;
	const FloatNumber&   asFloatNumber()   const;
	const String&        asString()        const;
	const Array&         asArray()         const;
	const Object&        asObject()        const;

private:
	_details::Impl* impl;
};

bool operator==(const Value&, const Value&) noexcept;
inline bool operator!=(const Value& lhs, const Value& rhs) noexcept { return !(lhs == rhs); }


} /* namespace nosj */

#include "nosj.inl"

#endif /* NOSJ_HPP_ */
