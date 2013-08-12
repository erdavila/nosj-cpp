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

	Value(bool) noexcept;

	Value(const Number&) noexcept NOT_IMPLEMENTED;
	Value(Number&&)      noexcept NOT_IMPLEMENTED;

	Value(int)           noexcept;
	Value(long int)      noexcept;
	Value(IntegerNumber) noexcept;

	Value(FloatNumber) noexcept NOT_IMPLEMENTED;

	~Value() noexcept;

	Value& operator=(Value);

	Type type() const noexcept;

	bool isNull()          const noexcept { return type() == NullType; }
	bool isBoolean()       const noexcept { return type() == BooleanType; }
	bool isNumber()        const noexcept { return type() == NumberType; }
	bool isIntegerNumber() const noexcept;
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

	const Null&          asNull()          const NOT_IMPLEMENTED;
	const Boolean&       asBoolean()       const NOT_IMPLEMENTED;
	const Number&        asNumber()        const;
	const IntegerNumber& asIntegerNumber() const NOT_IMPLEMENTED;
	const FloatNumber&   asFloatNumber()   const NOT_IMPLEMENTED;
	const String&        asString()        const NOT_IMPLEMENTED;
	const Array&         asArray()         const NOT_IMPLEMENTED;
	const Object&        asObject()        const NOT_IMPLEMENTED;

private:
	_details::Impl* impl;

	friend bool operator==(const Value&, const Value&) noexcept;
};

inline bool operator!=(const Value& lhs, const Value& rhs) noexcept { return !(lhs == rhs); }


} /* namespace nosj */

#include "nosj.inl"

#endif /* NOSJ_HPP_ */
