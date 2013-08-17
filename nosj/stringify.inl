#include <sstream>

namespace nosj {

namespace _details {

struct StringifiableValue {
	const Value& value;
	bool pretty;
	StringifiableValue(const Value& value, bool pretty) : value(value), pretty(pretty) {}
};

inline std::ostream& operator<<(std::ostream& os, const StringifiableValue& sv) {
	writeTo(os, sv.value);
	return os;
}

}


inline _details::StringifiableValue pretty(const Value& value) {
	return _details::StringifiableValue(value, true);
}

inline _details::StringifiableValue nopretty(const Value& value) {
	return _details::StringifiableValue(value, false);
}

inline std::ostream& operator<<(std::ostream& os, const Value& value) {
	writeTo(os, value);
	return os;
}

inline std::string stringify(const Value& value, bool pretty) {
	std::ostringstream os;
	writeTo(os, value, pretty);
	return os.str();
}

inline void writeTo(std::ostream& os, const Value& value, bool pretty) {
	switch(value.type()) {
	case Value::Type::NullValue:
		os << "null";
		break;
	case Value::Type::BooleanValue:
		os << (value.asBoolean() ? "true" : "false");
		break;
	case Value::Type::NumberValue: {
		const Number& number = value.asNumber();
		switch(number.type()) {
		case Number::Type::IntegerNumber:
			os << number.integerRef();
			break;
		case Number::Type::FloatNumber:
			std::ostringstream oss;
			oss.precision(110);
			oss.unsetf(oss.floatfield);
			oss << number.floatRef();

			const std::string& formatted = oss.str();
			os << formatted;
			if(formatted.find(L'.') == std::string::npos) {
				os << ".0";
			}
			break;
		}
		break;
	}
	default:
		NOT_IMPLEMENTED
	}
}

}
