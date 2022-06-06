#include "TinyVisitor.h"

std::map<TinyAstNode::AstType, std::shared_ptr<AbstractTinyVisitor>> TinyVisitorManager::visitorMap = {
	{TinyAstNode::TypeNumberLiteral,std::make_shared<NumberTinyVisitor>()},
	{TinyAstNode::TypeStringLiteral,std::make_shared<StringTinyVisitor>()},
	{TinyAstNode::TypeCallExpression,std::make_shared<CallExpressionTinyVisitor>()},
};

std::shared_ptr<TinyAstNode> TinyVisitorManager::enter(std::shared_ptr<TinyAstNode> node, std::shared_ptr<TinyAstNode> parent, std::shared_ptr<TinyAstNode> newParent)
{
	auto itor = visitorMap.find(node->type);
	if (itor == visitorMap.end()) {
		std::string errMsg("unknown visitor: " + std::to_string(node->type));
		throw std::exception(errMsg.c_str());
	}
	return itor->second->enter(node, parent, newParent);
}

std::shared_ptr<TinyAstNode> TinyVisitorManager::exit(std::shared_ptr<TinyAstNode> node, std::shared_ptr<TinyAstNode> parent, std::shared_ptr<TinyAstNode> newParent)
{
	auto itor = visitorMap.find(node->type);
	if (itor == visitorMap.end()) {
		std::string errMsg("unknown visitor: " + std::to_string(node->type));
		throw std::exception(errMsg.c_str());
	}
	return itor->second->exit(node, parent, newParent);
}

std::shared_ptr<TinyAstNode> AbstractTinyVisitor::enter(std::shared_ptr<TinyAstNode> node, std::shared_ptr<TinyAstNode> parent, std::shared_ptr<TinyAstNode> newParent)
{
	return nullptr;
}

std::shared_ptr<TinyAstNode> AbstractTinyVisitor::exit(std::shared_ptr<TinyAstNode> node, std::shared_ptr<TinyAstNode> parent, std::shared_ptr<TinyAstNode> newParent)
{
	return nullptr;
}

std::shared_ptr<TinyAstNode> NumberTinyVisitor::enter(std::shared_ptr<TinyAstNode> node, std::shared_ptr<TinyAstNode> parent, std::shared_ptr<TinyAstNode> newParent)
{
	auto newNode = std::make_shared<TinyAstNode>(node->type, node->value);
	newParent->body.push_back(newNode);
	return newNode;
}

std::shared_ptr<TinyAstNode> StringTinyVisitor::enter(std::shared_ptr<TinyAstNode> node, std::shared_ptr<TinyAstNode> parent, std::shared_ptr<TinyAstNode> newParent)
{
	auto newNode = std::make_shared<TinyAstNode>(node->type, node->value);
	newParent->body.push_back(newNode);
	return newNode;
}


std::shared_ptr<TinyAstNode> CallExpressionTinyVisitor::enter(std::shared_ptr<TinyAstNode> node, std::shared_ptr<TinyAstNode> parent, std::shared_ptr<TinyAstNode> newParent)
{
	auto callNode = std::make_shared<TinyAstNode>(node->type, node->value);
	auto newNode = callNode;

	if (parent->type != TinyAstNode::AstType::TypeCallExpression) {
		newNode = std::make_shared<TinyAstNode>(TinyAstNode::TypeExpressionStatement, "", std::vector<std::shared_ptr<TinyAstNode>>{callNode});
	}

	newParent->body.push_back(newNode);

	return callNode;
}