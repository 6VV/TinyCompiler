#include "TinyAstNode.h"
#include "TinyAstNode.h"

TinyAstNode::TinyAstNode(AstType type, const std::string& value, const std::vector<std::shared_ptr<TinyAstNode>> body)
	: type(type), value(value), body(body)
{
}

std::string TinyAstNode::typeString() const
{
	switch (type)
	{
	case TinyAstNode::TypeNull: return "Null";
	case TinyAstNode::TypeProgram: return "Program";
	case TinyAstNode::TypeNumberLiteral: return "NumberLiteral";
	case TinyAstNode::TypeStringLiteral: return "StringLiteral";
	case TinyAstNode::TypeCallExpression: return "CallExpression";
	case TinyAstNode::TypeExpressionStatement: return "ExpressionStatement";
	case TinyAstNode::TypeIdentifier: return "Identifier";
	default: return "Null";
	}
}
