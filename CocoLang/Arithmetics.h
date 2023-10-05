#pragma once
#include "legacy.h"
#include <iostream>
#include <string>
#include <vector>

class NumberNode {
public:
	Token token;

	NumberNode() {
		this->token = NULLPTR;
	}

	NumberNode(Token token) {
		this->token = token;
	}

	std::string to_string() {
		if (token.value.empty()) {
			return std::to_string(token.type);
		}
		return std::to_string(token.type) + ":" + token.value;
	}

};

class BinOperationNode {
public:
	NumberNode left_node;
	NumberNode right_node;
	Token operator_token;

	BinOperationNode() {
		this->left_node = NumberNode();
		this->right_node = NumberNode();
		this->operator_token = Token(NULLPTR);
	}

	BinOperationNode(NumberNode left_node, NumberNode right_node, Token operator_token) {
		this->left_node = left_node;
		this->right_node = right_node;
		this->operator_token = operator_token;
	}

	std::string to_string() {
		return "{Value: " +
			left_node.token.value +
			"}, {Operator: " +
			operator_token.to_string() +
			"}, {Value: " +
			right_node.token.value +
			"}";
	}
};

class Parser {
public:

	std::vector<Token> tokens;
	int64_t token_index;
	Token currentToken;
	BinOperationNode node;

	Parser(std::vector<Token>* tokens) {

		this->tokens = *tokens;
		this->token_index = -1;
		advance();
	}

	Token advance() {

		this->token_index++;

		if (this->token_index < this->tokens.size()) {
			currentToken = this->tokens[this->token_index];
		}

		return currentToken;

	}

	BinOperationNode parse() {
		BinOperationNode res = expr();
		return res;
	}

	NumberNode factor() {

		Token token = this->currentToken;

		if (token.type == INT || token.type == DOUBLE) {
			advance();
			return NumberNode(token);
		}

		return NumberNode();

	}

	BinOperationNode term() {

		NumberNode left = factor();

		while (currentToken.type == MULT || currentToken.type == DIV) {

			Token operator_token = currentToken;
			advance();
			NumberNode right = factor();
			node = BinOperationNode(left, right, operator_token);

		}

		return node;
	}

	BinOperationNode expr() {

		NumberNode left = factor();

		while (currentToken.type == PLUS || currentToken.type == MINUS || 
			currentToken.type == MULT || currentToken.type == DIV) {

			Token operator_token = currentToken;
			advance();
			NumberNode right = factor();
			node = BinOperationNode(left, right, operator_token);

		}

		return node;

	}

};

class Operate {
public:
	static std::string Eval(BinOperationNode* operation) {
		
		std::string result = "";

		TokenType type = operation->operator_token.type;

		switch (type) {
		case PLUS:
			result = add(*operation);
			break;
		case MINUS:
			result = substract(*operation);
			break;
		case MULT:
			result = multiply(*operation);
			break;
		case DIV:
			result = divide(*operation);
			break;
		}

		return result;

	}
private:
	static std::string add(BinOperationNode node) {
		NumberNode left = node.left_node;
		NumberNode right = node.right_node;
		std::string result = "";

		if (left.token.type == DOUBLE || right.token.type == DOUBLE) {
			double leftNum = std::stod(left.token.value);
			double rightNum = std::stod(right.token.value);
			double res = leftNum + rightNum;
			result = std::to_string(res);
		}
		else {
			int32_t leftNum = std::stoi(left.token.value);
			int32_t rightNum = std::stoi(right.token.value);
			int32_t res = leftNum + rightNum;
			result = std::to_string(res);
		}

		return result;

	}

	static std::string substract(BinOperationNode node) {
		NumberNode left = node.left_node;
		NumberNode right = node.right_node;
		std::string result = "";

		if (left.token.type == DOUBLE || right.token.type == DOUBLE) {
			double leftNum = std::stod(left.token.value);
			double rightNum = std::stod(right.token.value);
			double res = leftNum - rightNum;
			result = std::to_string(res);
		}
		else {
			int32_t leftNum = std::stoi(left.token.value);
			int32_t rightNum = std::stoi(right.token.value);
			int32_t res = leftNum - rightNum;
			result = std::to_string(res);
		}

		return result;

	}

	static std::string multiply(BinOperationNode node) {
		NumberNode left = node.left_node;
		NumberNode right = node.right_node;
		std::string result = "";

		if (left.token.type == DOUBLE || right.token.type == DOUBLE) {
			double leftNum = std::stod(left.token.value);
			double rightNum = std::stod(right.token.value);
			double res = leftNum * rightNum;
			result = std::to_string(res);
		}
		else {
			int32_t leftNum = std::stoi(left.token.value);
			int32_t rightNum = std::stoi(right.token.value);
			int32_t res = leftNum * rightNum;
			result = std::to_string(res);
		}

		return result;
	
	}
	static std::string divide(BinOperationNode node) {
		NumberNode left = node.left_node;
		NumberNode right = node.right_node;
		std::string result = "";

		if (left.token.type == DOUBLE || right.token.type == DOUBLE) {
			double leftNum = std::stod(left.token.value);
			double rightNum = std::stod(right.token.value);
			double res = leftNum / rightNum;
			result = std::to_string(res);
		}
		else {
			int32_t leftNum = std::stoi(left.token.value);
			int32_t rightNum = std::stoi(right.token.value);
			int32_t res = leftNum / rightNum;
			result = std::to_string(res);
		}

		return result;

	}

};