#include <array>

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

	istream& is;
	unsigned int positionNextChar = 0;

	Reader(istream& is) : is(is) {}

	Value readValue() {
		skipWhitespaces();
		istream::int_type ch = readNextChar();
		switch(ch) {
			case 'n':
				return readNull();
			default:
				throwUnexpectedNextChar();
		}
	}

	Value readNull() {
		const std::array<istream::char_type, 4> nullToken = {{ 'n', 'u', 'l', 'l' }};

		for(istream::int_type expectedCh : nullToken) {
			istream::int_type ch = extractNextChar();

			if(ch != expectedCh) {
				throwUnexpectedPreviousChar(ch);
			}
		}

		istream::int_type ch = readNextChar();
		if(ch != istream::traits_type::eof()  &&  !isWhitespace(ch)) {
			throwUnexpectedNextChar();
		}

		return null;
	}

	void skipWhitespaces() {
		do {
			auto ch = readNextChar();
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

	istream::int_type readNextChar() {
		return is.peek();
	}

	static bool isWhitespace(istream::char_type ch) {
		return ch == '\u0020'  // Space
		    || ch == '\u0009'  // Tab
		    || ch == '\u000A'  // Line feed
		    || ch == '\u000D'  // Carriage return
		    ;
	}

	__attribute__((noreturn))
	void throwUnexpectedNextChar() {
		auto ch = readNextChar();
		throwUnexpectedChar(ch, positionNextChar);
	}

	__attribute__((noreturn))
	void throwUnexpectedPreviousChar(istream::int_type ch) {
		throwUnexpectedChar(ch, positionNextChar-1);
	}

	__attribute__((noreturn))
	static void throwUnexpectedChar(istream::int_type ch, unsigned int position) {
		if(ch == istream::traits_type::eof()) {
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
