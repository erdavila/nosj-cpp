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


class Value;

class Null {};
typedef bool Boolean;
class Number;
typedef std::string String; // UTF-8 bytes
typedef std::deque<Value> Array;
typedef std::unordered_map<String, Value> Object;


namespace { // Unnamed namespace
	constexpr Null null;
}

inline constexpr bool operator==(const Null&, const Null&) { return true; }
inline constexpr bool operator!=(const Null&, const Null&) { return false; }


namespace _details {
	class Impl;
} // namespace _details


class Number {
public:
	typedef long long int Integer;
	typedef long double   Float;

	enum Type {
		IntegerNumber, FloatNumber
	};

	class InvalidType : public std::exception {};

	Number() noexcept : Number(0) {}

	Number(const Number&) noexcept = default;
	Number(Number&&)      noexcept = default;

	Number(int value)             noexcept;
	Number(long int value)        noexcept;
	Number(Number::Integer value) noexcept;

	Number(double value)        noexcept;
	Number(Number::Float value) noexcept;

	~Number() noexcept = default;

	Number& operator=(const Number&) noexcept = default;
	Number& operator=(Number&&)      noexcept = default;

	Type type() const noexcept;

	Integer& integerRef();
	Float&   floatRef();

	const Integer& integerRef() const NOT_IMPLEMENTED;
	const Float&   floatRef()   const NOT_IMPLEMENTED;

	// Convert to any compatible type
	template <typename T>
	operator T() const noexcept;

private:
	Type type_;
	union {
		Number::Integer integerValue;
		Number::Float   floatValue;
	};

	friend class Value;
	friend bool operator==(const Number&, const Number&) noexcept;
};


class Value {
public:
	enum Type {
		NullValue, BooleanValue, NumberValue,
		StringValue, ArrayValue, ObjectValue
	};

	Value() noexcept : Value(null) {}

	Value(Null) noexcept;

	Value(const Value&) noexcept;
	Value(Value&&)      noexcept NOT_IMPLEMENTED;

	Value(bool) noexcept;

	Value(const Number&) noexcept NOT_IMPLEMENTED;
	Value(Number&&)      noexcept NOT_IMPLEMENTED;

	Value(int)             noexcept;
	Value(long int)        noexcept;
	Value(Number::Integer) noexcept;

	Value(double)        noexcept;
	Value(Number::Float) noexcept;

	~Value() noexcept;

	Value& operator=(Value);

	Type type() const noexcept;

	bool isNull()    const noexcept { return type() == NullValue; }
	bool isBoolean() const noexcept { return type() == BooleanValue; }
	bool isNumber()  const noexcept { return type() == NumberValue; }
	bool isString()  const noexcept { return false; /* TEMPORARY */ }
	bool isArray()   const noexcept { return false; /* TEMPORARY */ }
	bool isObject()  const noexcept { return false; /* TEMPORARY */ }

	Null&    asNull();
	Boolean& asBoolean();
	Number&  asNumber();
	String&  asString();
	Array&   asArray();
	Object&  asObject();

	const Null&    asNull()    const NOT_IMPLEMENTED;
	const Boolean& asBoolean() const NOT_IMPLEMENTED;
	const Number&  asNumber()  const;
	const String&  asString()  const NOT_IMPLEMENTED;
	const Array&   asArray()   const NOT_IMPLEMENTED;
	const Object&  asObject()  const NOT_IMPLEMENTED;

private:
	_details::Impl* impl;

	friend bool operator==(const Value&, const Value&) noexcept;
};

inline bool operator!=(const Value& lhs, const Value& rhs) noexcept { return !(lhs == rhs); }


} /* namespace nosj */

#include "nosj.inl"

#endif /* NOSJ_HPP_ */
