#include "algorithms.hpp"
#include "strategies.hpp"

char CharacterStrategy::empty_value() {
	return ' ';
}

size_t CharacterStrategy::add(char /* input */) {
	return 1;
}

size_t CharacterStrategy::del(char /* input */) {
	return 1;
}

size_t CharacterStrategy::sub(char input1, char input2) {
	return static_cast<size_t>(not(input1 == input2));
}

void CharacterStrategy::modify_subelements(char& /* input1 */, char& /* input2 */) {}

std::map<std::pair<std::string, std::string>, std::vector<std::vector<size_t>>>
        LineStrategy::tab_cache =
                std::map<std::pair<std::string, std::string>, std::vector<std::vector<size_t>>>();

std::string LineStrategy::empty_value() {
	return "";
}

size_t LineStrategy::add(std::string const& input) {
	return input.length();
}

size_t LineStrategy::del(std::string const& input) {
	return input.length();
}

size_t LineStrategy::sub(std::string const& input1, std::string const& input2) {
	auto tab      = get_score_tab(input1, input2);
	auto last_col = tab[tab.size() - 1];
	return last_col[last_col.size() - 1];
}

void LineStrategy::modify_subelements(std::string& input1, std::string& input2) {
	do_the_thing<CharacterStrategy>(input1, input2, get_score_tab(input1, input2));
}

LineStrategy::ScoreTab& LineStrategy::get_score_tab(std::string const& input1,
                                                    std::string const& input2) {
	auto const cache_key = LineStrategy::CacheKey{input1, input2};
	if(tab_cache.count(cache_key) > 0) {
		return tab_cache[cache_key];
	}

	auto tab = fill_tab<CharacterStrategy>(input1, input2);

	tab_cache[cache_key] = tab;
	return tab_cache[cache_key];
}
