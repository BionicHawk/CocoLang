#include <iostream>
#include "legacy.h"
#include <string>
#include <vector>
#include "Arithmetics.h"
#include <fstream>

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

void interpret_mode() {
	bool running = true;

	while (running) {

		std::string response = "";
		std::cout << ">> ";
		std::getline(std::cin, response);

		if (response == "exit()") {
			running = false;
		} else {

			std::vector<Token> tokens = run(&response);

			if (!tokens.empty()) {
				Parser parser = Parser(&tokens);
				BinOperationNode response = parser.parse();
				std::string result = Operate::Eval(&response);
				std::cout << result << "\n";
			}

		}

	}
}

std::string get_source_code(char** file_name) {
	std::ifstream file(*file_name);
	std::string source_code = "";

	if (!file.is_open()) {
		std::cout << "Failed to open the file '" <<
			*file_name << "'!\n";
		std::cout << "The file could be missing or you are not given a valid route\n";
		return "";
	}

	std::string line;
	while (std::getline(file, line)) {
		source_code += line + "\n";
	}

	file.close();
	return source_code;

}

void read_code(char** file_name) {
	std::string code = get_source_code(file_name);
	if (!code.empty()) {
		std::cout << code << "\n";
	}
}

/*
	This is the entry point of the program, it will read the code and then interpet it
*/
int main(int argc, char *argv[]) {

	if (argc > 1) {

		char* first_arg = argv[1];
		read_code(&first_arg);

	} else {
		interpret_mode();
	}

	return 0;

}