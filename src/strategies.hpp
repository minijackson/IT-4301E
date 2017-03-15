#pragma once

#include <cstddef>
#include <string>

struct CharacterStrategy {
	static char empty_value();
	static size_t add(char input);
	static size_t del(char input);
	static size_t sub(char input1, char input2);
	static void modify_subelements(char& input1, char& input2);
};

struct LineStrategy {
	static std::string empty_value();
	static size_t add(std::string const& input);
	static size_t del(std::string const& input);
	static size_t sub(std::string const& input1, std::string const& input2);
	static void modify_subelements(std::string& input1, std::string& input2);
};
