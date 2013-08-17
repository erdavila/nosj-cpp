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
		istream::int_type nextChar = readNextChar();
		switch(nextChar) {
			case 'n': return readNull();
			case 'f': return readBooleanFalse();
			case 't': return readBooleanTrue();
			default:
				if(isDigit(nextChar)  ||  nextChar == '-') {
					return readNumber();
				}
				throwUnexpectedNextChar();
		}
	}

	Value readNull() {
		readToken("null");
		return null;
	}

	Value readBooleanFalse() {
		readToken("false");
		return false;
	}

	Value readBooleanTrue() {
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

		istream::int_type nextCh = readNextChar();
		if(!isValueFinalizer(nextCh)) {
			throwUnexpectedNextChar();
		}
	}

	Value readNumber() {
		enum State {
			UNSET,
			//                Expects:
			BEGIN,         // '-', DIGIT
			SIGN,          // DIGIT
			INT,           // DIGIT, '.', 'e', END
			ZERO,          // '.', 'e', END
			DECIMAL_POINT, // DIGIT
			FRAC,          // DIGIT, 'e', END
			E,             // '-', '+', DIGIT
			E_SIGN,        // DIGIT
			EXP,           // DIGIT, END
			END
		} state = BEGIN;

		std::string numberString;
		Number::Type type = Number::Type::IntegerNumber;

		while(state != END) {
			auto nextChar = readNextChar();

			State newState = UNSET;
			switch(state) {
			case BEGIN:
				if(nextChar == '0') {
					newState = ZERO;
				} else if(isDigit(nextChar)) {
					newState = INT;
				} else if(nextChar == '-') {
					newState = SIGN;
				}
				break;
			case SIGN:
				if(nextChar == '0') {
					newState = ZERO;
				} else if(isDigit(nextChar)) {
					newState = INT;
				}
				break;
			case INT:
				if(isDigit(nextChar)) {
					newState = INT;
				} else if(nextChar == '.') {
					newState = DECIMAL_POINT;
				} else if(nextChar == 'e'  ||  nextChar == 'E') {
					newState = E;
				} else if(isValueFinalizer(nextChar)) {
					newState = END;
				}
				break;
			case ZERO:
				if(nextChar == '.') {
					newState = DECIMAL_POINT;
				} else if(nextChar == 'e'  ||  nextChar == 'E') {
					newState = E;
				} else if(isValueFinalizer(nextChar)) {
					newState = END;
				}
				break;
			case DECIMAL_POINT:
				if(isDigit(nextChar)) {
					newState = FRAC;
				}
				break;
			case FRAC:
				if(isDigit(nextChar)) {
					newState = FRAC;
				} else if(nextChar == 'e'  ||  nextChar == 'E') {
					newState = E;
				} else if(isValueFinalizer(nextChar)) {
					newState = END;
				}
				break;
			case E:
				if(nextChar == '+'  ||  nextChar == '-') {
					newState = E_SIGN;
				} else if(isDigit(nextChar)) {
					newState = EXP;
				}
				break;
			case E_SIGN:
				if(isDigit(nextChar)) {
					newState = EXP;
				}
				break;
			case EXP:
				if(isDigit(nextChar)) {
					newState = EXP;
				} else if(isValueFinalizer(nextChar)) {
					newState = END;
				}
				break;
			default:
				assert(false);
			}

			if(newState == UNSET) {
				throwUnexpectedNextChar();
			}
			if(newState == DECIMAL_POINT  ||  newState == E) {
				type = Number::Type::FloatNumber;
			}
			if(newState != END) {
				numberString += extractNextChar();
			}
			state = newState;
		}

		if(type == Number::Type::IntegerNumber) {
			return Number(std::stoll(numberString));
		} else {
			return Number(std::stold(numberString));
		}
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
		auto ch = readNextChar();
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
