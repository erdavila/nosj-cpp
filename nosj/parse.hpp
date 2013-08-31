#ifndef PARSE_HPP_
#define PARSE_HPP_


#include "values.hpp"
#include <istream>
#include <sstream>


namespace nosj {


class ParseException : public Exception {};

class IncompleteInput : public ParseException {
	virtual const char* what() const noexcept override { return "Incomplete input"; }
};

class UnexpectedCharacter : public ParseException {
public:
	char character;
	unsigned int position;
	UnexpectedCharacter(char character, unsigned int position);
	virtual const char* what() const noexcept override { return message.c_str(); }
private:
	std::string message;
};

class ExpectedTrailCodePoint : public ParseException {
public:
	unsigned int position;
	ExpectedTrailCodePoint(unsigned int position) : position(position) {}
};

class InvalidCodePoint : ParseException {};


std::istream& operator>>(std::istream&, Value&);

Value parse(const std::string&);
Value readFrom(std::istream&);


}


#include "parse.inl"


#endif /* PARSE_HPP_ */
