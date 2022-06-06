#pragma once
#include <memory>
#include <vector>
#include <string>

/*
* 解释器语法树
*/

class TinyAstNode
{
public:
	enum AstType {
		TypeNull, //空
		TypeProgram, //程序入口
		TypeNumberLiteral, //数字常量
		TypeStringLiteral, //字符串常量
		TypeCallExpression, //函数调用
		TypeExpressionStatement, //语句
		TypeIdentifier, //函数标志
	};

public:
	TinyAstNode(AstType type, const std::string& value = "", const std::vector<std::shared_ptr<TinyAstNode>> body = {});

	std::string typeString() const; //类型字符串

public:
	AstType type = TypeNull;
	std::vector<std::shared_ptr<TinyAstNode>> body{}; //子节点
	std::string value; //值
};

