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
	
	Error(std::string error_name, std::string details, Position start, Position end, std::string file_name, std::string function_name) {
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

		while (this->currentChar != NULL && ( isDigit(currentChar) || this->currentChar == '.' ) ) {

			if (this->currentChar == '.') dot_count++;

			num_str += this->currentChar;

			//std::cout << "??? (" << currentChar << ") ";
			advance();

		}

		if (dot_count > 0) {

			if (dot_count == 1) {
				return Token(DOUBLE, num_str);
			}
			else {
				return Token(STRING, num_str);
			}

		}

		return Token(INT, num_str);

	}

};