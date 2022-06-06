#pragma once

/*
* 解释器标记
*/

#include <string>

class TinyToken
{
public:
	enum TokenType {
		TypeNull,
		TypeParen,
		TypeNumber,
		TypeString,
		TypeName,
	};

public:
	TinyToken(TokenType type, const std::string& value);

	std::string toText() const; //转换为文本

public:
	TokenType type = TypeNull;
	std::string value = "";
};

