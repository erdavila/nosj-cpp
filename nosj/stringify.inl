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

struct WriterVisitor : nosj::ConstVisitor {
	std::ostream& os;
	bool pretty;

	WriterVisitor(std::ostream& os, bool pretty) : os(os), pretty(pretty) {}

	void visit(const Null&)            override { os << "null"; }

	void visit(const Boolean& boolean) override { os << (boolean ? "true" : "false"); }

	void visit(const Number& number)   override {
		if(number.type() == Number::Type::IntegerNumber) {
			os << number.integerRef();
		} else {
			std::ostringstream oss;
			oss.precision(110);
			oss.unsetf(oss.floatfield);
			oss << number.floatRef();

			const std::string& formatted = oss.str();
			os << formatted;
			if(formatted.find(L'.') == std::wstring::npos) {
				os << ".0";
			}
		}
	}

	void visit(const String&) override NOT_IMPLEMENTED;

	void visit(const Array&) override NOT_IMPLEMENTED;

	void visit(const Object&) override NOT_IMPLEMENTED;
};

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
	_details::WriterVisitor visitor(os, pretty);
	value.accept(visitor);
}

}
