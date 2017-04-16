#pragma once

#include <cstddef>
#include <map>
#include <string>
#include <utility>

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

private:
	using CacheKey = std::pair<std::string, std::string>;
	using ScoreTab = std::vector<std::vector<size_t>>;

	static ScoreTab& get_score_tab(std::string const& input1, std::string const& input2);

	static std::map<CacheKey, ScoreTab> tab_cache;
};
