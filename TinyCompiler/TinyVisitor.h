#pragma once
#include <memory>
#include "TinyAstNode.h"
#include <map>

/*
* 语法树访问者
*/

class AbstractTinyVisitor
{
public:
	virtual	std::shared_ptr<TinyAstNode> enter(std::shared_ptr<TinyAstNode> node, std::shared_ptr<TinyAstNode> parent, std::shared_ptr<TinyAstNode> newParent);
	virtual std::shared_ptr<TinyAstNode> exit(std::shared_ptr<TinyAstNode> node, std::shared_ptr<TinyAstNode> parent, std::shared_ptr<TinyAstNode> newParent);
};

class NumberTinyVisitor :public AbstractTinyVisitor
{
public:
	virtual std::shared_ptr<TinyAstNode> enter(std::shared_ptr<TinyAstNode> node, std::shared_ptr<TinyAstNode> parent, std::shared_ptr<TinyAstNode> newParent) override;
};

class StringTinyVisitor :public AbstractTinyVisitor
{
public:
	virtual std::shared_ptr<TinyAstNode> enter(std::shared_ptr<TinyAstNode> node, std::shared_ptr<TinyAstNode> parent, std::shared_ptr<TinyAstNode> newParent) override;
};

class CallExpressionTinyVisitor :public AbstractTinyVisitor
{
public:
	virtual std::shared_ptr<TinyAstNode> enter(std::shared_ptr<TinyAstNode> node, std::shared_ptr<TinyAstNode> parent, std::shared_ptr<TinyAstNode> newParent) override;
};

class TinyVisitorManager
{
public:
	static std::shared_ptr<TinyAstNode> enter(std::shared_ptr<TinyAstNode> node, std::shared_ptr<TinyAstNode> parent, std::shared_ptr<TinyAstNode> newParent);
	static std::shared_ptr<TinyAstNode> exit(std::shared_ptr<TinyAstNode> node, std::shared_ptr<TinyAstNode> parent, std::shared_ptr<TinyAstNode> newParent);

private:
	static std::map<TinyAstNode::AstType, std::shared_ptr<AbstractTinyVisitor>> visitorMap;
};