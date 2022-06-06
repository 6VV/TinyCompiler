#include "TinyCompiler.h"
#include <iostream>

int main(int argc, char* argv[])
{
	std::string text = "(add 2 (subtract 4 2))";

	try {
		auto tokens = TinyCompiler::tokenizer(text);
		auto ast = TinyCompiler::parse(tokens);
		auto newAst = TinyCompiler::traverser(ast);
		auto output = TinyCompiler::codeGenerator(newAst);

		TinyCompiler::print(tokens);
		std::cout << std::endl;
		TinyCompiler::print(ast);
		std::cout << std::endl;
		TinyCompiler::print(newAst);
		std::cout << std::endl;
		std::cout << output << std::endl;
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}

	return 0;
}
