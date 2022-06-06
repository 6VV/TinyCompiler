#include "TinyCompiler.h"
#include <iostream>

std::vector<TinyToken> TinyCompiler::tokenizer(const std::string& text)
{
	// `current` 变量就像一个指光标一样让我们可以在代码中追踪我们的位置
	int current = 0;

	// `tokens` 数组用来存我们的标记
	std::vector<TinyToken> tokens{};

	// 首先创建一个`while`循环，在循环里面我们可以将`current`变量增加为我们想要的值
	// 做这一步的原因是因为我们想要在一个循环里多次增加`current`变量的值，因为我们的标记的长度不确定
	while (current < text.size()) {

		// 我们还将在 `text` 中存储 `current` 字符
		char chr = text[current];

		// 我们要检查的第一件事是一个左括号。 这稍后将用于 `CallExpression` 但现在我们只关心字符。
		// 检查是否有一个左括号:
		if (chr == '(') {
			tokens.push_back({ TinyToken::TypeParen,"(" });
			current++;
			continue;
		}

		// 接下去检查右括号
		if (chr == ')') {
			tokens.push_back({ TinyToken::TypeParen,")" });
			current++;
			continue;
		}

		// 接下去我们检查空格，这对于我们来说就是为了知道字符的分割，但是并不需要存储为标记。
		// 所以我们来检查是否有空格的存在，如果存在，就继续下一次循环，做除了存储到标记数组之外的其他操作即可
		if (isspace(chr)) {
			current++;
			continue;
		}

		// **首先我们从表达式的值中来入手，这里简单举例子就只检查了两种类型的值
		// **一种是数字类型，一种是字符串类型

		// 接下去的标记类型是数字. 这和我们之前看到的不太一样
		// 因为数字可以是任何数量的字符，我们希望将整个字符序列捕获称为一个标记
		//
		//   (add 123 456)
		//        ^^^ ^^^
		//        只有两个单独的标记
		//
		// 因此，当我们遇到序列中的第一个数字时，我们就开始了
		if (isdigit(chr)) {

			// 我们将创建一个`value`字符串，并把字符推送给他
			std::string value = "";

			// 然后我们将遍历序列中的每个字符，直到遇到一个不是数字的字符
			// 将每个作为数字的字符推到我们的 `value` 并随着我们去增加 `current`
			// 这样我们就能拿到一个完整的数字字符串，例如上面的 123 和 456，而不是单独的 1 2 3 4 5 6
			while (isdigit(chr)) {
				value.push_back(chr);
				chr = text[++current];
			}

			// 接着我们把数字放到标记数组中，用数字类型来描述区分它
			tokens.push_back({ TinyToken::TypeNumber, value });

			// 继续外层的下一次循环
			continue;
		}

		// 我们还将在我们的语言中添加对字符串的支持，可以是任何
		// 用双引号 (") 括起来的文本。
		//
		//   (concat "foo" "bar")
		//            ^^^   ^^^ 字符串标记
		//
		// 从检查开头的双引号开始:
		if (chr == '"') {
			// 保留一个 `value` 变量来构建我们的字符串标记。
			std::string value = "";

			// 我们将跳过编辑中开头的双引号
			chr = text[++current];

			// 然后我们将遍历每个字符，直到我们到达另一个双引号
			while (chr != '"') {
				value.push_back(chr);
				chr = text[++current];
			}

			// 跳过相对应闭合的双引号.
			chr = text[++current];

			// 把我们的字符串标记添加到标记数组中
			tokens.push_back({ TinyToken::TypeString, value });

			continue;
		}

		// 最后一种标记的类型是名称。这是一个字母序列而不是数字，这是我们 lisp 语法中的函数名称
		//
		//   (add 2 4)
		//    ^^^
		//    名称标记
		//
		if (isalpha(chr)) {
			std::string value = "";

			// 同样，我们遍历所有，并将它们完整的存到`value`变量中
			while (isalpha(chr)) {
				value.push_back(chr);
				chr = text[++current];
			}

			// 并把这种名称类型的标记存到标记数组中，继续循环
			tokens.push_back({ TinyToken::TypeName, value });

			continue;
		}

		// 如果到最后我们还没有匹配到一个字符，那就抛出一个异常错误并完全退出
		throw std::exception("I dont know what this character is : " + chr);
	}

	// 在标记器执行结束后，直接返回标记数组即可
	return tokens;
}

std::shared_ptr<TinyAstNode> TinyCompiler::parse(const std::vector<TinyToken>& tokens)
{
	// 同样的我们保持一个`current`变量当作光标来供我们使用
	int current = 0;

	// 但是这一次，我们我们将使用递归来替代之前使用的 whie 循环
	// 先定义一个`walk`函数

	// 现在，我们将要创建我们的 AST 树，他有一个根节点叫做`Program`节点
	auto ast = std::make_shared<TinyAstNode>(TinyAstNode::TypeProgram);

	// 并且启动我们的`walk`函数，把节点推到我们的`ast.body`数组
	// 我们之所以在一个循环里做这些事是因为我们的程序可以有一个接一个的`CallExpression`而不是嵌套的
	//
	//   (add 2 2)
	//   (subtract 4 2)
	//
	while (current < tokens.size()) {
		ast->body.push_back(walk(tokens, current));
	}

	// 最后返回我们得到的 AST 结构
	return ast;
}

std::shared_ptr<TinyAstNode> TinyCompiler::traverser(std::shared_ptr<TinyAstNode> node)
{
	std::shared_ptr<TinyAstNode> newNode = std::make_shared<TinyAstNode>(TinyAstNode::TypeProgram);
	for (auto child : node->body) {
		traverseNode(child, node, newNode);
	}
	return newNode;
}

std::string TinyCompiler::codeGenerator(std::shared_ptr<TinyAstNode> node)
{
	// 还是按照节点的类型来进行分解操作
	switch (node->type) {

		// 如果我们有`Program`节点，我们将映射`body`中的每个节点
		// 并且通过代码生成器来运行他们，用换行符将他们连接起来
	case TinyAstNode::TypeProgram: {
		std::string text = "";
		for (auto child : node->body) {
			if (!text.empty()) {
				text += "\n";
			}
			text += codeGenerator(child);
		}

		return text;
	}

		// 对于`ExpressionStatement`，我们将在嵌套表达式上调用代码生成器，并添加一个分号
	case TinyAstNode::TypeExpressionStatement: {
		if (node->body.size() == 1) {
			return codeGenerator(node->body[0]) + ';';// << 这是因为保持代码的统一性（用正确的方式编写代码）
		}
		else {
			throw std::exception("error expression statement size");
		}
	}

		// 对于`CallExpression`我们将打印`callee`, 新增一个左括号
		// 然后映射每一个`arguments`数组的节点，并用代码生成器执行，每一个节点运行完之后加上逗号
		// 最后增加一个右括号
	case TinyAstNode::TypeCallExpression: {
		std::string text = "";
		for (auto child : node->body) {
			if (!text.empty()) {
				text += ",";
			}
			text += codeGenerator(child);
		}

		return node->value + "(" + text + ")";
	}

		// 对于`NumberLiteral`直接返回`node`的值就好.
	case TinyAstNode::TypeNumberLiteral: {
		return node->value;
	}

		// 对于`StringLiteral`，在`node`的值的周围添加双引号.
	case TinyAstNode::TypeStringLiteral: {
		return '"' + node->value + '"';
	}

		// 如果没有匹配到节点的类型，就抛出异常
	default: {
		std::string errMsg("unknown type: " + std::to_string(node->type));
		throw std::exception(errMsg.c_str());
	}
	}
}

void TinyCompiler::print(const std::vector<TinyToken>& tokens)
{
	for (auto& token : tokens)
	{
		std::cout << token.toText() << std::endl;
	}
}

std::shared_ptr<TinyAstNode> TinyCompiler::walk(const std::vector<TinyToken>& tokens, int& current)
{
	// 在 walk 函数里面，我们首先获取`current`标记
	auto token = tokens[current];

	// 我们将把每种类型的标记分成不同的代码路径
	// 先从`number`类型的标记开始.

	// 首先先检查一下是否有`number`标签.
	if (token.type == TinyToken::TypeNumber) {
		current++;
		return std::make_shared<TinyAstNode>(TinyAstNode::TypeNumberLiteral, token.value);
	}

	// 对于字符串来说，也是和上面数字一样的操作。新增一个`StringLiteral`节点
	if (token.type == TinyToken::TypeString) {
		current++;
		return std::make_shared<TinyAstNode>(TinyAstNode::TypeStringLiteral, token.value);
	}

	// 接下去我们要寻找调用的表达式（CallExpressions）
	// 从找到第一个左括号开始
	if (token.type == TinyToken::TypeParen && token.value == "(") {

		// 我们将增加`current`来跳过这个插入语，因为在 AST 树中我们并不关心这个括号
		token = tokens[++current];

		// 我们创建一个类型为“CallExpression”的基本节点，并把当前标记的值设置到 name 字段上
		// 因为左括号的下一个标记就是这个函数的名字
		auto node = std::make_shared<TinyAstNode>(TinyAstNode::TypeCallExpression, token.value);
	
		// 继续增加`current`来跳过这个名称标记
		token = tokens[++current];

		// 现在我们要遍历每一个标记，找出其中是`CallExpression`的`params`，直到遇到右括号

		// 现在这就是递归发挥作用的地方。我们将使用递归来解决事情而不是尝试去解析可能无限嵌套的节点集
		//
		// 为了解释这个，我们先用 lisp 代码做演示
		// 
		// 可以看到`add` 的参数是一个数字，还有一个包含自己数字的嵌套`CallExpression`
		//   (add 2 (subtract 4 2))
		//
		// 你也将会注意到，在我们的标记数组里，我们有很多右括号
		//
		//   [
		//     { type: 'paren',  value: '('        },
		//     { type: 'name',   value: 'add'      },
		//     { type: 'number', value: '2'        },
		//     { type: 'paren',  value: '('        },
		//     { type: 'name',   value: 'subtract' },
		//     { type: 'number', value: '4'        },
		//     { type: 'number', value: '2'        },
		//     { type: 'paren',  value: ')'        }, <<< 右括号
		//     { type: 'paren',  value: ')'        }, <<< 右括号
		//   ]
		//
		// 我们将依赖嵌套的`walk`函数来增加我们的`current`变量来超过任何嵌套的`CallExpression`
		// 所以我们创建一个`while`循环持续到遇到一个`type`是'paren'并且`value`是右括号的标记
		while (
			(token.type != TinyToken::TypeParen) ||
			(token.type == TinyToken::TypeParen && token.value != ")")
			) {
			// 我们调用`walk`方法，这个方法会返回一个`node`节点
			// 我们把这个节点存到我们的`node.params`中去
			node->body.push_back(walk(tokens, current));
			token = tokens[current];
		}

		// 我们最后一次增加`current`变量来跳过右括号
		current++;

		// 返回node节点
		return node;
	}

	// 同样的，如果我们到这里都没有辨认出标记的类型，就抛出一个异常
	std::string errMsg("unknown type: " + std::to_string(token.type));
	throw std::exception(errMsg.c_str());
}

void TinyCompiler::traverseNode(std::shared_ptr<TinyAstNode> node, std::shared_ptr<TinyAstNode> parent, std::shared_ptr<TinyAstNode> newParent)
{
	auto enterNode = TinyVisitorManager::enter(node, parent, newParent);
	if (enterNode != nullptr) {
		
		// 接下去我们将按当前节点类型来进行拆分，以便于对子节点数组进行遍历，处理到每一个子节点
		switch (node->type) {

			// 首先从最高的`Program`层开始。因为 Program 节点有一个名叫 body 的属性，里面包含了节点数组
			// 我们调用`traverseArray`来向下遍历它们
		case TinyAstNode::TypeProgram:
		case TinyAstNode::TypeCallExpression:
		{
			traverseArray(node->body, node, enterNode);
			break;
		}

			// 对于`NumberLiteral`和`StringLiteral`的情况，由于没有子节点，所以直接 break 即可
		case TinyAstNode::TypeNumberLiteral:
		case TinyAstNode::TypeStringLiteral:
			break;

			// 接着，如果我们没有匹配到上面的节点类型，就抛出一个异常错误
		default: {
			std::string err("unknow type: " + std::to_string(node->type));
			throw std::exception(err.c_str());
		}
		}
	}

	// 如果这个节点类型里面有一个`exit`方法，我们就调用它，并且传入当前节点和他的父节点
	TinyVisitorManager::exit(node, parent, newParent);
}

void TinyCompiler::traverseArray(std::vector<std::shared_ptr<TinyAstNode>> nodes, std::shared_ptr<TinyAstNode> parent, std::shared_ptr<TinyAstNode> newParent)
{
	for (auto node : nodes)
	{
		traverseNode(node, parent,newParent);
	}
}

void TinyCompiler::printEmptyChar(int indent)
{
	for (int i = 0; i < indent; i++)
	{
		std::cout << " ";
	}
}

void TinyCompiler::print(std::shared_ptr<TinyAstNode> node, int indent)
{
	printEmptyChar(indent);
	std::cout << "{" << std::endl;

	printEmptyChar(indent);
	std::cout << "Type: " + node->typeString() + "," << std::endl;

	if (!node->value.empty())
	{
		printEmptyChar(indent);
		std::cout << "Value: " + node->value + "," << std::endl;
	}

	if (!node->body.empty())
	{
		printEmptyChar(indent);
		std::cout << "Body: [" << std::endl;

		for (std::shared_ptr<TinyAstNode> child : node->body)
		{
			print(child, indent + 4);
		}

		printEmptyChar(indent);
		std::cout << "]" << std::endl;
	}

	printEmptyChar(indent);
	std::cout << "}" << std::endl;
}
