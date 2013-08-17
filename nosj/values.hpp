#ifndef NOSJ_HPP_
#define NOSJ_HPP_

#include <deque>
#include <exception>
#include <string>
#include <unordered_map>


#include <cassert> // TEMPORARY!!!
#define NOT_IMPLEMENTED { assert(! "implemented"); } // TEMPORARY!!!
#define NOT_TESTED { assert(! "tested"); } // TEMPORARY!!!


namespace nosj {


class Exception : public std::exception {};
class InvalidConversion : public Exception {};


class Value;

class Null {};
using Boolean = bool;
class Number;
using String = std::string; // UTF-8 bytes
using Array = std::deque<Value>;
using Object = std::unordered_map<String, Value>;


inline constexpr bool operator==(const Null&, const Null&) { return true; }
inline constexpr bool operator!=(const Null&, const Null&) { return false; }


namespace _details {
	class Impl;
} // namespace _details


class Number {
public:
	using Integer = long long int;
	using Float = long double;

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

	const Integer& integerRef() const;
	const Float&   floatRef()   const;

	// Convert to any compatible type
	template <typename T>
	operator T() const noexcept;

private:
	Type type_;
	union {
		Number::Integer integerValue;
		Number::Float   floatValue;
	};

	template <typename T>
	static T& checkAndGetRef(Type, Type, T&);

	friend class Value;
	friend bool operator==(const Number&, const Number&) noexcept;
};


class Visitor;
class ConstVisitor;


class Value {
public:
	enum Type {
		NullValue, BooleanValue, NumberValue,
		StringValue, ArrayValue, ObjectValue
	};

	Value(const Value&) noexcept;
	Value(Value&&)      noexcept;

	Value()     noexcept : Value(Null()) {}
	Value(Null) noexcept;

	Value(bool) noexcept;

	Value(const Number&) noexcept;
	Value(Number&&)      noexcept;

	Value(int)             noexcept;
	Value(long int)        noexcept;
	Value(Number::Integer) noexcept;

	Value(double)        noexcept;
	Value(Number::Float) noexcept;

	Value(const char*)   noexcept;
	Value(const String&) noexcept;
	Value(String&&)      noexcept;

	Value(const Array&) noexcept;
	Value(Array&&)      noexcept;

	Value(const Object&) noexcept;
	Value(Object&&)      noexcept;

	~Value() noexcept;

	Value& operator=(Value);

	void accept(Visitor& visitor);
	void accept(ConstVisitor& visitor) const;

	Type type() const noexcept;

	bool isNull()    const noexcept { return type() == NullValue; }
	bool isBoolean() const noexcept { return type() == BooleanValue; }
	bool isNumber()  const noexcept { return type() == NumberValue; }
	bool isString()  const noexcept { return type() == StringValue; }
	bool isArray()   const noexcept { return type() == ArrayValue; }
	bool isObject()  const noexcept { return type() == ObjectValue; }

	Null&    asNull();
	Boolean& asBoolean();
	Number&  asNumber();
	String&  asString();
	Array&   asArray();
	Object&  asObject();

	const Null&    asNull()    const NOT_IMPLEMENTED;
	const Boolean& asBoolean() const;
	const Number&  asNumber()  const;
	const String&  asString()  const NOT_IMPLEMENTED;
	const Array&   asArray()   const NOT_IMPLEMENTED;
	const Object&  asObject()  const NOT_IMPLEMENTED;

private:
	_details::Impl* impl;

	friend bool operator==(const Value&, const Value&) noexcept;
};

inline bool operator!=(const Value& lhs, const Value& rhs) noexcept { return !(lhs == rhs); }


namespace { // Unnamed namespace
	constexpr Null null;
	const Array emptyArray;
	const Object emptyObject;
}


struct Visitor {
	virtual ~Visitor() = default;
	virtual void visit(Null&)    = 0;
	virtual void visit(Boolean&) = 0;
	virtual void visit(Number&)  = 0;
	virtual void visit(String&)  = 0;
	virtual void visit(Array&)   = 0;
	virtual void visit(Object&)  = 0;
};

struct ConstVisitor {
	virtual ~ConstVisitor() = default;
	virtual void visit(const Null&)    = 0;
	virtual void visit(const Boolean&) = 0;
	virtual void visit(const Number&)  = 0;
	virtual void visit(const String&)  = 0;
	virtual void visit(const Array&)   = 0;
	virtual void visit(const Object&)  = 0;
};


} /* namespace nosj */

#include "values.inl"

#endif /* NOSJ_HPP_ */
