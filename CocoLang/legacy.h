#pragma once
#include <string>
#include <vector>
#include <iostream>

const char DIGITS[] = "0123456789";

class Position {
public:

	int64_t index;
	int64_t line;
	int64_t column;
	std::string function_name;
	std::string file_name;

	Position() {
		setDefaultPos();
		this->function_name = "Main";
		this->file_name = "";
	}

	Position(std::string function_name, std::string file_name) {
		setDefaultPos();
		this->function_name = function_name;
		this->file_name = file_name;
	}

	Position(int64_t index, int64_t line, int64_t column, std::string function_name, std::string file_name) {
		this->index = index;
		this->line = line;
		this->column = column;
	}

	void advance(char currentChar) {
		this->index++;
		this->column++;

		if (currentChar == '\n') {
			this->column = 0;
			this->line++;
		}

	}

	Position copy() {
		return Position(this->index, this->line, this->column, this->function_name, this->file_name);
	}

private:
	void setDefaultPos() {
		this->index = -1;
		this->line = 0;
		this->column = -1;
	}

};

class Error {
public:
	std::string error_name;
	std::string details;
	std::string file_name;
	std::string function_name;

	Position start, end;
	
	Error(std::string error_name, 
		std::string details, 
		Position start, 
		Position end, 
		std::string file_name, 
		std::string function_name
	) {

		this->start = start;
		this->end = end;
		this->error_name = error_name;
		this->details = details;
		this->function_name = function_name;
		this->file_name = file_name;
	}

	std::string to_string() {
		return this->error_name + ": " + this->details + "\n Between (line: " +
			std::to_string(start.line) + ", column: " +
			std::to_string(start.column) + ") and (line: " +
			std::to_string(end.line) + ", column: " +
			std::to_string(end.column) + ")\n at '" + this->file_name + "' in function: '"
			+ this->function_name + "'";
	}

};

class IllegalCharState : public Error {

	IllegalCharState(std::string details, 
		Position start, 
		Position end,
		std::string file_name,
		std::string function_name)
		: Error("IllegalCharState", 
			details, 
			start, 
			end,
			file_name,
			function_name) {
	}
};


enum TokenType {
	INT,
	DOUBLE,
	STRING,
	BOOL,
	VOID,
	ANY,
	VAR,
	CONST,
	PLUS,
	MINUS,
	MULT,
	DIV,
	LPAREN,
	RPAREN,
	NULLPTR
};

class Token {
public:
	TokenType type;
	std::string value;

	Token() {
		this->type = NULLPTR;
		this->value = "";
	}

	Token(TokenType type, std::string value = "") {
		this->type = type;
		this->value = value;
	}

	std::string to_string() {
		if (this->value.empty()) {
			return std::to_string(this->type);
		}
		
		return std::to_string(this->type) + ": " + this->value;

	}
};

class Variable {
public:

	Variable() {
		this->token = Token(ANY, "undefined");
		this->value = "null";
	}

	Variable(Token* token, std::string value) {
		this->token = *token;
		this->value = value;

		if (token->type == ANY) {
			if (!infer_type()) {
				std::cout << "You should initialize your variable " << 
					token->value << 
					" otherwhise it isn't possible to infer the type of this variable.\n";
			}
			exit(-1);
		}

	}

	std::string get_type() {

		std::string type = "";

		switch (this->token.type) {
		case VOID:
			type = "void";
			break;
		case INT:
			type = "int";
			break;
		case DOUBLE:
			type = "double";
			break;
		case STRING:
			type = "string";
			break;
		}

		return type;

	}

	std::string get_name() {
		return this->token.value;
	}

	std::string get_value() {
		return this->value;
	}

private:
	Token token;
	std::string value;

	bool try_infer_number() {
		bool number = true;
		bool is_it_double = false;
		int dot_count = 0;

		for (char e : this->value) {
			bool is_digit = false;

			for (char number : DIGITS) {

				if (e == number) is_digit = true;

			}

			if (e == '.') dot_count++;

			if (!is_digit && e != '.' && e != 'f') number = false;

		}

		if (number) {

			if (dot_count == 1 || this->value[this->value.length() - 1] == 'f') {
				this->token.type = DOUBLE;
				return true;
			}
			if (dot_count == 0 && this->value[this->value.length() - 1] != 'f') {
				this->token.type = INT;
				return true;
			}
			return true;

		}

		return false;
	}

	bool infer_type() {
		bool could_infer = false;
		if (!this->value.empty()) {
			if ((this->value[0] == '\"' && this->value[this->value.length() - 1] == '\"') ||
				(this->value[0] == '\'' && this->value[this->value.length() - 1] == '\'')) {
				this->token.type = STRING;
				could_infer = true;
			}
			else if (this->value == "true" || this->value == "false") {
				this->token.type = BOOL;
				could_infer = true;
			}
			else if (!try_infer_number()) {
				this->token.type = NULLPTR;
			} else could_infer = true;
		}
		else {
			 
		}

		return could_infer;

	}

};

class Lexer {
public:
	std::string function_name;
	std::string file_name;
	std::string text;
	Position position;
	char currentChar;

	Lexer(std::string* text, std::string* function_name, std::string* file_name) {
		this->text = *text;
		this->function_name = *function_name;
		this->file_name = *file_name;
		this->position = Position(*function_name, *file_name);
		this->currentChar = NULL;
		advance();
	}

	void advance() {
		this->position.advance(currentChar);
		this->currentChar = (position.index < text.length()) ? text[position.index] : NULL;
	}

	std::vector<Token> infer_tokens() {
		
		std::vector<Token> tokens = std::vector<Token>();

		while (this->currentChar != NULL) {

			//std::cout << "... ";

			if (this->currentChar == ' ' || this->currentChar == '\n') {
				advance();
			}
			else {

				switch (this->currentChar) {
				case '+':
					tokens.push_back(Token(PLUS));
					advance();
					break;
				case '-':
					tokens.push_back(Token(MINUS));
					advance();
					break;
				case '*':
					tokens.push_back(Token(MULT));
					advance();
					break;
				case '/':
					tokens.push_back(Token(DIV));
					advance();
					break;
				case '(':
					tokens.push_back(Token(LPAREN));
					advance();
					break;
				case ')': 
					tokens.push_back(Token(RPAREN));
					advance();
				default:

					// Checking for numbers
					if (isDigit(currentChar)) {
						tokens.push_back(infer_number());
						break;
					}
					else {
						Position start = position.copy();
						advance();
						std::string details = "Char '";
						details += currentChar;
						details += "' is not a valid char";
						Error exception = Error("IllegalCharException", details,
							start, this->position, this->file_name, this->function_name);
						std::cout << exception.to_string() << "\n";
						return std::vector<Token>();
					}

					break;
				}
			}

		}

		return tokens;

	}

	bool isDigit(char letter) {

		for (char digit : DIGITS) {

			//std::cout << "/// (" << letter << ", " << digit <<") ";

			if (letter == digit) return true;
		
		}

		return false;
	}

	Token infer_number() {
		std::string num_str = "";
		uint64_t dot_count = 0;

		while (this->currentChar != NULL && ( isDigit(currentChar) || this->currentChar == '.'  || this->currentChar == 'f')) {

			if (this->currentChar == '.') dot_count++;

			num_str += this->currentChar;

			//std::cout << "??? (" << currentChar << ") ";
			advance();

		}

		if (dot_count > 0) {

			if (dot_count == 1 || num_str[num_str.length() -1] == 'f') {
				return Token(DOUBLE, num_str.erase(num_str.length() - size_t(1)));
			}
			else {
				return Token(NULLPTR);
			}

		}

		return Token(INT, num_str);

	}

};