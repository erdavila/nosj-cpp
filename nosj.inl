namespace nosj {


namespace _details {

	template <typename T> struct TypeTag;
	template <> struct TypeTag<Null>          { static const Type value = NullType; };
	template <> struct TypeTag<Boolean>       { static const Type value = BooleanType; };
	template <> struct TypeTag<Number>        { static const Type value = NumberType; };
	template <> struct TypeTag<IntegerNumber> { static const Type value = IntegerNumberType; };
	template <> struct TypeTag<FloatNumber>   { static const Type value = FloatNumberType; };
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
		virtual bool eq(const Impl* impl) const noexcept {
			typedef BasicImpl<T> MyType;
			const MyType* otherImpl = dynamic_cast<const MyType*>(impl);
			if(otherImpl) {
				return value == otherImpl->value;
			} else {
				return false;
			}
		}
	};

	typedef BasicImpl<Null>    NullImpl;
	typedef BasicImpl<Boolean> BooleanImpl;

} // namespace _details


class Number {

};


inline Value::Value(const Value& value) noexcept : impl(value.impl->clone()) {}

inline Value::Value(Null) noexcept   : impl(new _details::NullImpl(null)) {}
inline Value::Value(bool b) noexcept : impl(new _details::BooleanImpl(b)) {}

inline Value::~Value() noexcept { delete impl; }

inline Value& Value::operator=(Value value) { std::swap(impl, value.impl); return *this; }

inline Type Value::type() const noexcept { return impl->type(); }

inline Null&          Value::asNull()          { return impl->as<Null>(); }
inline Boolean&       Value::asBoolean()       { return impl->as<Boolean>(); }
inline Number&        Value::asNumber()        { return impl->as<Number>(); }
inline IntegerNumber& Value::asIntegerNumber() { return impl->as<IntegerNumber>(); }
inline FloatNumber&   Value::asFloatNumber()   { return impl->as<FloatNumber>(); }
inline String&        Value::asString()        { return impl->as<String>(); }
inline Array&         Value::asArray()         { return impl->as<Array>(); }
inline Object&        Value::asObject()        { return impl->as<Object>(); }


inline bool operator==(const Value& lhs, const Value& rhs) noexcept {
	return lhs.impl->eq(rhs.impl);
}


} // namespace nosj
