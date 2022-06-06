#pragma once
#include <string>
#include "TinyToken.h"
#include <vector>
#include "TinyAstNode.h"
#include <memory>
#include "TinyVisitor.h"
#include <map>

/*
* 迷你解释器
*/

class TinyCompiler
{
public:
	static std::vector<TinyToken> tokenizer(const std::string& text); //解析文本标记
	static std::shared_ptr<TinyAstNode> parse(const std::vector<TinyToken>& tokens); //生成语法树
	static std::shared_ptr<TinyAstNode> traverser(std::shared_ptr<TinyAstNode> node); //遍历语法树
	static std::string codeGenerator(std::shared_ptr<TinyAstNode> node); //生成新代码

	static void print(const std::vector<TinyToken>& tokens); //打印标记
	static void print(std::shared_ptr<TinyAstNode> node, int indent = 0); //打印语法树

private:
	static std::shared_ptr<TinyAstNode> walk(const std::vector<TinyToken>& tokens, int& current); //解析为树节点

	static void traverseNode(std::shared_ptr<TinyAstNode> node, std::shared_ptr<TinyAstNode> parent, std::shared_ptr<TinyAstNode> newParent); //接受两个参数：node 节点和他的 parent 节点
	static void traverseArray(std::vector<std::shared_ptr<TinyAstNode>> nodes, std::shared_ptr<TinyAstNode> parent, std::shared_ptr<TinyAstNode> newParent);

	static void printEmptyChar(int indent); //打印空格

};

