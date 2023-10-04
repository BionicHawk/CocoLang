#pragma once
#include "legacy.h"
#include <iostream>
#include <string>
#include <vector>

class NumberNode {
public:
	Token token;
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

};
