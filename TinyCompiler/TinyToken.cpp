#include "TinyToken.h"

TinyToken::TinyToken(TokenType type, const std::string& value)
	:type(type), value(value)
{
}

std::string TinyToken::toText() const
{
	std::string typeText;

	switch (type)
	{
	case TinyToken::TypeNull: typeText = "Null";break;
	case TinyToken::TypeParen:typeText = "Paren"; break;
	case TinyToken::TypeNumber:typeText = "Number"; break;
	case TinyToken::TypeString:typeText = "String"; break;
	case TinyToken::TypeName:typeText = "Name"; break;
	default:typeText = ""; break;
	}

	return "Type: " + typeText + ";\t" + "Value: " + value + ";";
}
