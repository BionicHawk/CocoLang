#include <iostream>
#include "legacy.h"
#include <string>
#include <vector>
#include "Arithmetics.h"

void print_tokens(std::vector<Token>* tokens) {
	for (auto token : *tokens) {
		std::cout << "{" << token.to_string() << "}\n";
	}
}

/*
	This part of the program will interpret the function code given by tokenizing the code and returning the tokens
	readen
*/
std::vector<Token> run(std::string* text, std::string function_name = "Main", std::string file_name = "Coco Interpreter") {

	std::string function_n = function_name;
	std::string file_n = file_name;

	Lexer lexer = Lexer(text, &function_n, &file_n);
	std::vector<Token> tokens = lexer.infer_tokens();

	return tokens;

}

/*
	This is the entry point of the program, it will read the code and then interpet it
*/
int main() {

	while (true) {

		std::string response = "";
		std::cout << ">> ";
		std::getline(std::cin, response);

		std::vector<Token> tokens =  run(&response);

		if (!tokens.empty()) {
			Parser parser = Parser(&tokens);
			BinOperationNode response = parser.parse();
			std::string result = Operate::Eval(&response);
			std::cout << result << "\n";
		}

	}

	return 0;

}