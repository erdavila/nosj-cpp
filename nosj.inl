namespace nosj {



inline Number::Number(int value)           noexcept : Number(static_cast<Number::Integer>(value)) {}
inline Number::Number(long int value)      noexcept : Number(static_cast<Number::Integer>(value)) {}
inline Number::Number(Number::Integer value) noexcept : type_(IntegerNumberType), integerValue(value) {}

inline Number::Number(double value)      noexcept : Number(static_cast<Number::Float>(value)) {}
inline Number::Number(Number::Float value) noexcept : type_(FloatNumberType), floatValue(value) {}

inline Type Number::type() const noexcept { return type_; }

template <typename T>
Number::operator T() const noexcept {
	if(type_ == IntegerNumberType) {
		return integerValue;
	} else {
		return floatValue;
	}
}

inline bool operator==(const Number& lhs, const Number& rhs) noexcept {
	if(lhs.type_ == IntegerNumberType) {
		if(rhs.type_ == IntegerNumberType) {
			return lhs.integerValue == rhs.integerValue;
		} else {
			NOT_IMPLEMENTED
		}
	} else {
		if(rhs.type_ == IntegerNumberType) {
			NOT_IMPLEMENTED
		} else {
			return lhs.floatValue == rhs.floatValue;
		}
	}
}

inline bool operator!=(const Number& lhs, const Number& rhs) noexcept { return !(lhs == rhs); }


namespace _details {

	template <typename T> struct TypeTag;
	template <> struct TypeTag<Null>          { static const Type value = NullType; };
	template <> struct TypeTag<Boolean>       { static const Type value = BooleanType; };
	template <> struct TypeTag<Number>        { static const Type value = NumberType; };
	template <> struct TypeTag<Number::Integer> { static const Type value = IntegerNumberType; };
	template <> struct TypeTag<Number::Float>   { static const Type value = FloatNumberType; };
	template <> struct TypeTag<String>        { static const Type value = StringType; };
	template <> struct TypeTag<Array>         { static const Type value = ArrayType; };
	template <> struct TypeTag<Object>        { static const Type value = ObjectType; };

	struct Impl {
		virtual ~Impl() noexcept = default;
		virtual Impl* clone() const noexcept = 0;
		virtual Type type() const noexcept = 0;
		virtual std::pair<Type, void*> typeAndPointer() const noexcept = 0;
		virtual bool eq(const Impl*) const noexcept = 0;

		template <typename T>
		T& as() const {
			std::pair<Type, void*> typeAndPointer = this->typeAndPointer();
			if(typeAndPointer.first != TypeTag<T>::value) throw InvalidConversion();
			return *static_cast<T*>(typeAndPointer.second);
		}
	};

	template <typename T>
	struct BasicImpl : Impl {
		T value;
		BasicImpl(const T& value) : value(value) {}
		virtual Impl* clone() const noexcept override { return new BasicImpl(value); }
		virtual Type type() const noexcept override { return TypeTag<T>::value; }
		virtual std::pair<Type, void*> typeAndPointer() const noexcept override {
			T* pointer = const_cast<T*>(&value);
			Type type = TypeTag<T>::value;
			return std::pair<Type, void*>(type, pointer);
		}
		virtual bool eq(const Impl* impl) const noexcept override {
			typedef BasicImpl<T> MyType;
			const MyType* otherImpl = dynamic_cast<const MyType*>(impl);
			if(otherImpl) {
				return value == otherImpl->value;
			}
			return false;
		}
	};

	typedef BasicImpl<Null>    NullImpl;
	typedef BasicImpl<Boolean> BooleanImpl;
	typedef BasicImpl<Number>  NumberImpl;

} // namespace _details


inline Value::Value(const Value& value) noexcept : impl(value.impl->clone()) {}

inline Value::Value(Null value) noexcept : impl(new _details::NullImpl(value)) {}
inline Value::Value(bool value) noexcept : impl(new _details::BooleanImpl(value)) {}

inline Value::Value(int value)           noexcept : Value(static_cast<Number::Integer>(value)) {}
inline Value::Value(long int value)      noexcept : Value(static_cast<Number::Integer>(value)) {}
inline Value::Value(Number::Integer value) noexcept : impl(new _details::NumberImpl(value)) {}

inline Value::Value(double value)      noexcept : Value(static_cast<Number::Float>(value)) {}
inline Value::Value(Number::Float value) noexcept : impl(new _details::NumberImpl(value)) {}

inline Value::~Value() noexcept { delete impl; }

inline Value& Value::operator=(Value value) { std::swap(impl, value.impl); return *this; }

inline Type Value::type() const noexcept { return impl->type(); }

inline bool Value::isIntegerNumber() const noexcept {
	return isNumber() && asNumber().type() == IntegerNumberType;
}

inline bool Value::isFloatNumber() const noexcept {
	return isNumber() && asNumber().type() == FloatNumberType;
}

inline Null&          Value::asNull()          { return impl->as<Null>(); }
inline Boolean&       Value::asBoolean()       { return impl->as<Boolean>(); }
inline Number&        Value::asNumber()        { return impl->as<Number>(); }

inline Number& Value::asNumberIfTypeIs(Type type) {
	Number& number = asNumber();
	if(number.type() == type) {
		return number;
	}
	throw InvalidConversion();
}
inline Number::Integer& Value::asIntegerNumber() { return asNumberIfTypeIs(IntegerNumberType).integerValue; }
inline Number::Float&   Value::asFloatNumber()   { return asNumberIfTypeIs(FloatNumberType).floatValue; }

inline String&        Value::asString()        { return impl->as<String>(); }
inline Array&         Value::asArray()         { return impl->as<Array>(); }
inline Object&        Value::asObject()        { return impl->as<Object>(); }

inline const Number& Value::asNumber() const { return impl->as<Number>(); }


inline bool operator==(const Value& lhs, const Value& rhs) noexcept {
	return lhs.impl->eq(rhs.impl);
}


} // namespace nosj
