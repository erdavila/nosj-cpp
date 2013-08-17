#ifdef __CYGWIN__

#include <iomanip>

namespace _details {

template <typename T>
T stoX(const std::string& str, size_t* idx, int base) {
	std::istringstream is(str);
	T result;
	if(base >= 0) {
		is >> std::setbase(base);
	}
	is >> result;
	if(idx) {
		if(is.eof()) {
			*idx = str.length();
		} else {
			*idx = is.tellg();
		}
	}
	return result;
}

}

namespace std {

inline long long stoll(const std::string& str, size_t* idx = 0, int base = 10) {
	return _details::stoX<long long>(str, idx, base);
}

inline long double stold(const std::string& str, size_t* idx = 0) {
	return _details::stoX<long double>(str, idx, -1);
}

}

#endif


namespace nosj {


inline UnexpectedCharacter::UnexpectedCharacter(char character, unsigned int position)
	: character(character), position(position)
{
	std::ostringstream ostr;
	ostr << "Unexpected character '\\u";
	ostr.fill('0');
	ostr.width(4);
	ostr << std::hex << int(character) << "' at position " << position;
	message = ostr.str();
}


namespace _details {

struct Reader {
	using istream = std::istream;
	enum { eof = istream::traits_type::eof() };

	istream& is;
	unsigned int positionNextChar = 0;

	Reader(istream& is) : is(is) {}

	Value readValue() {
		skipWhitespaces();
		istream::int_type nextCh = nextChar();
		switch(nextCh) {
			case 'n': return readNull();
			case 'f': return readBooleanFalse();
			case 't': return readBooleanTrue();
			case '"': return readString();
			case '[': return readArray();
			default:
				if(isDigit(nextCh)  ||  nextCh == '-') {
					return readNumber();
				}
				throwUnexpectedNextChar();
		}
	}

	Null readNull() {
		readToken("null");
		return null;
	}

	bool readBooleanFalse() {
		readToken("false");
		return false;
	}

	bool readBooleanTrue() {
		readToken("true");
		return true;
	}

	void readToken(const char* token) {
		for(; *token != '\0'; token++) {
			istream::int_type expectedCh = *token;
			istream::int_type ch = extractNextChar();
			if(ch != expectedCh) {
				throwUnexpectedPreviousChar(ch);
			}
		}
	}

	Number readNumber() {
		std::string numberString;
		Number::Type type = Number::Type::IntegerNumber;

		if(nextChar() == '-') {
			numberString += extractNextChar();
		}

		auto ch = readDigit();
		numberString += ch;
		if(ch != '0') {
			numberString += readOptionalDigits();
		}

		if(nextChar() == '.') {
			numberString += extractNextChar();
			type = Number::Type::FloatNumber;
			numberString += readDigits();
		}

		ch = nextChar();
		if(ch == 'e'  ||  ch == 'E') {
			numberString += extractNextChar();
			type = Number::Type::FloatNumber;

			ch = nextChar();
			if(ch == '-'  ||  ch == '+') {
				numberString += extractNextChar();
			}

			numberString += readDigits();
		}

		if(type == Number::Type::IntegerNumber) {
			return std::stoll(numberString);
		} else {
			return std::stold(numberString);
		}
	}

	std::string readDigits() {
		std::string firstDigit = { readDigit() };
		return firstDigit + readOptionalDigits();
	}

	istream::char_type readDigit() {
		istream::int_type ch = extractNextChar();
		if(!isDigit(ch)) {
			throwUnexpectedPreviousChar(ch);
		}
		return ch;
	}

	std::string readOptionalDigits() {
		std::string digits;
		while(isDigit(nextChar())) {
			digits += extractNextChar();
		}
		return digits;
	}

	std::string readString() {
		std::string str;

		auto ch = extractNextChar();
		if(ch != '"') {
			throwUnexpectedPreviousChar(ch);
		}

		bool finished = false;
		do {
			ch = extractNextChar();
			if(ch == '"') {
				finished = true;
			} else if(ch == '\\') {
				auto ch = readEscapedChar();
				if(isLeadSurrogate(ch)) {
					ch = completeUTF16Char(ch);
				}
				str += utf8Encode(ch);
			} else if(ch >= 0x20) {
				str += ch;
			} else {
				throwUnexpectedPreviousChar(ch);
			}
		} while(!finished);

		return str;
	}

	char32_t completeUTF16Char(unsigned int lead) {
		unsigned int position = positionNextChar;

		auto ch = extractNextChar();
		if(ch != '\\') {
			throw ExpectedTrailCodePoint(position);
		}

		auto trail = readEscapedChar();
		if(!isTrailSurrogate(trail)) {
			throw ExpectedTrailCodePoint(position);
		}

		ch  = (lead  - 0xD800) << 10;
		ch |= (trail - 0xDC00);
		ch += 0x010000;

		return ch;
	}

	static bool isLeadSurrogate(istream::int_type ch) {
		return ch >= 0xD800  &&  ch <= 0xDBFF;
	}

	static bool isTrailSurrogate(istream::int_type ch) {
		return ch >= 0xDC00  &&  ch <= 0xDFFF;
	}

	char32_t readEscapedChar() {
		auto ch = extractNextChar();
		switch(ch) {

		case '"':
		case '/':
		case '\\':
			return ch;

		case 'b': return 0x08;
		case 'f': return 0x0C;
		case 'n': return 0x0A;
		case 'r': return 0x0D;
		case 't': return 0x09;
		case 'u': return readHexCodePoint();

		default:
			throwUnexpectedPreviousChar(ch);
		}
	}

	char32_t readHexCodePoint() {
		char32_t codePoint = 0;
		for(int i = 0; i < 4; i++) {
			int hexDigitValue;

			auto hexDigit = extractNextChar();
			if(hexDigit >= '0'  &&  hexDigit <= '9') {
				hexDigitValue = hexDigit - '0';
			} else if(hexDigit >= 'a'  &&  hexDigit <= 'f') {
				hexDigitValue = hexDigit - 'a' + 10;
			} else if(hexDigit >= 'A'  &&  hexDigit <= 'F') {
				hexDigitValue = hexDigit - 'A' + 10;
			} else {
				throwUnexpectedPreviousChar(hexDigit);
			}

			codePoint = (codePoint << 4) + hexDigitValue;
		}
		return codePoint;
	}

	static std::string utf8Encode(char32_t ch) {
		if(ch <= 0x7F) {
			return std::string{(char)ch};
		} else {
			int leadingIndicator;
			int continuationBytes;

			if(ch <= 0x07FF) {
				leadingIndicator = 0xC0; // 110xxxxx
				continuationBytes = 1;
			} else if(ch <= 0xFFFF) {
				leadingIndicator = 0xE0; // 1110xxxx
				continuationBytes = 2;
			} else if(ch <= 0x10FFFF) {
				leadingIndicator = 0xF0; // 11110xxx
				continuationBytes = 3;
			} else {
				NOT_IMPLEMENTED
			}

			enum { continuationIndicator = 0x80 }; // 10xxxxxx
			char bytes[] = {0, 0, 0, 0, 0};
			for(; continuationBytes > 0; continuationBytes--) {
				bytes[continuationBytes] = (0x3F/*00111111*/ & ch) | continuationIndicator;
				ch >>= 6;
			}
			bytes[0] = ch | leadingIndicator;

			return bytes;
		}
	}

	Array readArray() {
		auto ch = extractNextChar();
		if(ch != '[') {
			throwUnexpectedPreviousChar(ch);
		}

		skipWhitespaces();
		ch = nextChar();
		if(ch == ']') {
			extractNextChar();
			return emptyArray;
		}

		Array array;
		while(true) {
			Value value = readValue();
			array.push_back(std::move(value));

			skipWhitespaces();
			ch = extractNextChar();
			if(ch == ']') {
				break;
			} else if(ch != ',') {
				throwUnexpectedPreviousChar(ch);
			}
		}
		return array;
	}

	void skipWhitespaces() {
		do {
			auto ch = nextChar();
			if(isWhitespace(ch)) {
				extractNextChar();
			} else {
				break;
			}
		} while(true);
	}

	istream::int_type extractNextChar() {
		istream::int_type ch = is.get();
		positionNextChar++;
		return ch;
	}

	istream::int_type nextChar() {
		return is.peek();
	}

	static bool isValueFinalizer(istream::int_type ch) {
		return isWhitespace(ch)  ||  ch == eof;
	}

	static bool isWhitespace(istream::int_type ch) {
		return ch == '\u0020'  // Space
		    || ch == '\u0009'  // Tab
		    || ch == '\u000A'  // Line feed
		    || ch == '\u000D'  // Carriage return
		    ;
	}

	static bool isDigit(istream::int_type ch) {
		return ch >= '0'  &&  ch <= '9';
	}

	__attribute__((noreturn))
	void throwUnexpectedNextChar() {
		auto ch = nextChar();
		throwUnexpectedChar(ch, positionNextChar);
	}

	__attribute__((noreturn))
	void throwUnexpectedPreviousChar(istream::int_type ch) {
		throwUnexpectedChar(ch, positionNextChar-1);
	}

	__attribute__((noreturn))
	static void throwUnexpectedChar(istream::int_type ch, unsigned int position) {
		if(ch == eof) {
			throw IncompleteInput();
		} else {
			throw UnexpectedCharacter(ch, position);
		}
	}

};

}


inline std::istream& operator>>(std::istream& is, Value& value) {
	value = readFrom(is);
	return is;
}

inline Value parse(const std::string& str) {
	std::istringstream is(str);
	_details::Reader reader(is);
	Value result = reader.readValue();

	reader.skipWhitespaces();
	if(!is.eof()) {
		reader.throwUnexpectedNextChar();
	}

	return result;
}

inline Value readFrom(std::istream& is) {
	_details::Reader reader(is);
	return reader.readValue();
}

}
