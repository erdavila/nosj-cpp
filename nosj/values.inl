#include <utility>


namespace nosj {


inline Number::Number(int value)             noexcept : Number(static_cast<Number::Integer>(value)) {}
inline Number::Number(long int value)        noexcept : Number(static_cast<Number::Integer>(value)) {}
inline Number::Number(Number::Integer value) noexcept : type_(IntegerNumber), integerValue(value) {}

inline Number::Number(double value)        noexcept : Number(static_cast<Number::Float>(value)) {}
inline Number::Number(Number::Float value) noexcept : type_(FloatNumber), floatValue(value) {}

inline Number::Type Number::type() const noexcept { return type_; }

inline Number::Integer& Number::integerRef() { return checkAndGetRef(type_, IntegerNumber, integerValue); }
inline Number::Float&   Number::floatRef()   { return checkAndGetRef(type_, FloatNumber, floatValue); }

inline const Number::Integer& Number::integerRef() const { return checkAndGetRef(type_, IntegerNumber, integerValue); }
inline const Number::Float&   Number::floatRef()   const { return checkAndGetRef(type_, FloatNumber, floatValue); }

template <typename T>
T& Number::checkAndGetRef(Type type, Type expectedType, T& ref) {
	if(type == expectedType) {
		return ref;
	}
	throw InvalidType();
}

template <typename T>
Number::operator T() const noexcept {
	if(type_ == IntegerNumber) {
		return integerValue;
	} else {
		return floatValue;
	}
}

inline bool operator==(const Number& lhs, const Number& rhs) noexcept {
	if(lhs.type_ == Number::Type::IntegerNumber) {
		if(rhs.type_ == Number::Type::IntegerNumber) {
			return lhs.integerValue == rhs.integerValue;
		} else {
			return lhs.integerValue == rhs.floatValue;
		}
	} else {
		if(rhs.type_ == Number::Type::IntegerNumber) {
			return lhs.floatValue == rhs.integerValue;
		} else {
			return lhs.floatValue == rhs.floatValue;
		}
	}
}

inline bool operator!=(const Number& lhs, const Number& rhs) noexcept { return !(lhs == rhs); }


namespace _details {

	template <typename T> struct TypeTag;
	template <> struct TypeTag<Null>    { static constexpr Value::Type value = Value::NullValue; };
	template <> struct TypeTag<Boolean> { static constexpr Value::Type value = Value::BooleanValue; };
	template <> struct TypeTag<Number>  { static constexpr Value::Type value = Value::NumberValue; };
	template <> struct TypeTag<String>  { static constexpr Value::Type value = Value::StringValue; };
	template <> struct TypeTag<Array>   { static constexpr Value::Type value = Value::ArrayValue; };
	template <> struct TypeTag<Object>  { static constexpr Value::Type value = Value::ObjectValue; };

	struct Impl {
		virtual ~Impl() noexcept = default;
		virtual Impl* clone() const noexcept = 0;
		virtual void visit(Visitor&) = 0;
		virtual void visit(ConstVisitor&) const = 0;
		virtual Value::Type type() const noexcept = 0;
		virtual std::pair<Value::Type, void*> typeAndPointer() const noexcept = 0;
		virtual bool eq(const Impl*) const noexcept = 0;

		template <typename T>
		T& as() const {
			std::pair<Value::Type, void*> typeAndPointer = this->typeAndPointer();
			if(typeAndPointer.first != TypeTag<T>::value) throw InvalidConversion();
			return *static_cast<T*>(typeAndPointer.second);
		}
	};

	template <typename T>
	struct BasicImpl : Impl {
		T value;
		BasicImpl(const T& value) : value(value) {}
		BasicImpl(T&& value)      : value(std::forward<T>(value)) {}
		virtual Impl* clone() const noexcept override { return new BasicImpl(value); }
		virtual void visit(     Visitor& visitor)       override { visitor.visit(value); }
		virtual void visit(ConstVisitor& visitor) const override { visitor.visit(value); }
		virtual Value::Type type() const noexcept override { return TypeTag<T>::value; }
		virtual std::pair<Value::Type, void*> typeAndPointer() const noexcept override {
			T* pointer = const_cast<T*>(&value);
			Value::Type type = TypeTag<T>::value;
			return std::pair<Value::Type, void*>(type, pointer);
		}
		virtual bool eq(const Impl* impl) const noexcept override {
			using MyType = BasicImpl<T>;
			const MyType* otherImpl = dynamic_cast<const MyType*>(impl);
			if(otherImpl) {
				return value == otherImpl->value;
			}
			return false;
		}
	};

	using    NullImpl = BasicImpl<Null>;
	using BooleanImpl = BasicImpl<Boolean>;
	using  NumberImpl = BasicImpl<Number>;
	using  StringImpl = BasicImpl<String>;
	using   ArrayImpl = BasicImpl<Array>;
	using  ObjectImpl = BasicImpl<Object>;

} // namespace _details


inline Value::Value(const Value& value) noexcept : impl(value.impl->clone()) {}
inline Value::Value(Value&& value)      noexcept : Value() { std::swap(impl, value.impl); }

inline Value::Value(Null value) noexcept : impl(new _details::NullImpl(value)) {}

inline Value::Value(bool value) noexcept : impl(new _details::BooleanImpl(value)) {}

inline Value::Value(const Number& value) noexcept : impl(new _details::NumberImpl(value)) {}
inline Value::Value(Number&& value)      noexcept : impl(new _details::NumberImpl(std::forward<Number>(value))) {}

inline Value::Value(int value)             noexcept : impl(new _details::NumberImpl(value)) {}
inline Value::Value(long int value)        noexcept : impl(new _details::NumberImpl(value)) {}
inline Value::Value(Number::Integer value) noexcept : impl(new _details::NumberImpl(value)) {}

inline Value::Value(double value)        noexcept : impl(new _details::NumberImpl(value)) {}
inline Value::Value(Number::Float value) noexcept : impl(new _details::NumberImpl(value)) {}

inline Value::Value(const char* value)   noexcept : impl(new _details::StringImpl(value)) {}
inline Value::Value(const String& value) noexcept : impl(new _details::StringImpl(value)) {}
inline Value::Value(String&& value)      noexcept : impl(new _details::StringImpl(std::forward<String>(value))) {}

inline Value::Value(const Array& value) noexcept : impl(new _details::ArrayImpl(value)) {}
inline Value::Value(Array&& value)      noexcept : impl(new _details::ArrayImpl(std::forward<Array>(value))) {}

inline Value::Value(const Object& value) noexcept : impl(new _details::ObjectImpl(value)) {}
inline Value::Value(Object&& value)      noexcept : impl(new _details::ObjectImpl(std::forward<Object>(value))) {}

inline Value::~Value() noexcept { delete impl; }

inline Value& Value::operator=(Value value) { std::swap(impl, value.impl); return *this; }

inline void Value::accept(     Visitor& visitor)       { impl->visit(visitor); }
inline void Value::accept(ConstVisitor& visitor) const { impl->visit(visitor); }

inline Value::Type Value::type() const noexcept { return impl->type(); }

inline Null&    Value::asNull()    { return impl->as<Null>(); }
inline Boolean& Value::asBoolean() { return impl->as<Boolean>(); }
inline Number&  Value::asNumber()  { return impl->as<Number>(); }
inline String&  Value::asString()  { return impl->as<String>(); }
inline Array&   Value::asArray()   { return impl->as<Array>(); }
inline Object&  Value::asObject()  { return impl->as<Object>(); }

inline const Null&    Value::asNull()    const { return impl->as<Null>(); }
inline const Boolean& Value::asBoolean() const { return impl->as<Boolean>(); }
inline const Number&  Value::asNumber()  const { return impl->as<Number>(); }
inline const String&  Value::asString()  const { return impl->as<String>(); }
inline const Array&   Value::asArray()   const { return impl->as<Array>(); }
inline const Object&  Value::asObject()  const { return impl->as<Object>(); }


inline bool operator==(const Value& lhs, const Value& rhs) noexcept {
	return lhs.impl->eq(rhs.impl);
}


} // namespace nosj
