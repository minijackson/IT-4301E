#pragma once

#include <iostream>
#include <limits>
#include <list>
#include <string>
#include <vector>

template<typename T>
using matrix = std::vector<std::vector<T>>;

template<typename value_type>
struct Modification {
	enum { ADD, DEL, SUB } type;
	value_type const &lhs, &rhs;

	void print() {
		switch(type) {
			case ADD:
				std::cout << "Add '" << lhs << "'";
				break;
			case DEL:
				std::cout << "Del '" << lhs << "'";
				break;
			case SUB:
				std::cout << "Substitute '" << lhs << "' by '" << rhs << "'";
				break;
		}
		std::cout << std::endl;
	}
};

size_t min(size_t lhs, size_t rhs);

void print_tab(matrix<size_t> const& mat);

template<typename... T>
size_t min(size_t lhs, size_t rhs, T... values) {
	return (lhs < rhs) ? min(lhs, values...) : min(rhs, values...);
}

template<typename Strategy, typename Container>
matrix<size_t> fill_tab(Container const& input1, Container const& input2) {
	auto input1_len = input1.size(), input2_len = input2.size();
	matrix<size_t> res(input1_len + 1, std::vector<size_t>(input2_len + 1));

	res[0][0] = 0;

	for(size_t i = 1; i < input1_len + 1; ++i) {
		res[i][0] = res[i - 1][0] + Strategy::add(input1[i - 1]);
	}

	for(size_t i = 1; i < input2_len + 1; ++i) {
		res[0][i] = res[0][i - 1] + Strategy::add(input2[i - 1]);
	}

	for(size_t i = 1; i < input1_len + 1; ++i) {
		for(size_t j = 1; j < input2_len + 1; ++j) {
			res[i][j] = min(res[i - 1][j - 1] + Strategy::sub(input1[i - 1], input2[j - 1]),
			                res[i - 1][j] + Strategy::del(input1[i - 1]),
			                res[i][j - 1] + Strategy::add(input2[j - 1]));
		}
	}

	return res;
}

template<typename Strategy, typename Container>
std::list<Modification<typename Container::value_type>> backtrack(matrix<size_t> const& mat,
                                                                  Container& input1,
                                                                  Container& input2) {
	using Mod = Modification<typename Container::value_type>;
	std::list<Mod> res;

	std::pair<size_t, size_t> position{mat.size() - 1, mat[0].size() - 1};

	while(position != std::pair<size_t, size_t>{0, 0}) {
		constexpr size_t max_size_t = std::numeric_limits<size_t>::max();

		size_t cost_add = (position.second) ? (mat[position.first][position.second - 1] +
		                                       Strategy::add(input2[position.second - 1]))
		                                    : max_size_t,
		       cost_sub = (position.first && position.second)
		                          ? (mat[position.first - 1][position.second - 1] +
		                             Strategy::sub(input1[position.first - 1],
		                                           input2[position.second - 1]))
		                          : max_size_t,
		       cost_del = (position.first) ? (mat[position.first - 1][position.second] +
		                                      Strategy::del(input1[position.first - 1]))
		                                   : max_size_t,
		       minimum = min(cost_add, cost_sub, cost_del);

		if(minimum == cost_add) {
			res.push_front(Mod{Mod::ADD, input2[position.second - 1], Strategy::empty_value()});
			--position.second;
		} else if(minimum == cost_sub) {
			res.push_front(Mod{Mod::SUB, input1[position.first - 1], input2[position.second - 1]});
			--position.first;
			--position.second;
		} else {
			res.push_front(Mod{Mod::DEL, input1[position.first - 1], Strategy::empty_value()});
			--position.first;
		}
	}

	return res;
}

template<typename Strategy, typename Container>
void make_padded(std::list<Modification<typename Container::value_type>> mods,
                 Container& input1,
                 Container& input2) {

	using Mod = Modification<typename Container::value_type>;

	size_t pos1 = 0, pos2 = 0;

	for(auto mod : mods) {
		// re-obtain the begin iterator because of iterator invalidation induced by the "insert"
		// method
		auto begin1 = input1.begin(), begin2 = input2.begin();

		if(mod.type == Mod::ADD) {
			input1.insert(begin1 + pos1, Strategy::empty_value());

		} else if(mod.type == Mod::DEL) {
			input2.insert(begin2 + pos2, Strategy::empty_value());

		} else {
			// Substitution
			Strategy::modify_subelements(*(begin1 + pos1), *(begin2 + pos2));
		}

		++pos1;
		++pos2;
	}
}

template<typename Strategy, typename Container>
size_t do_the_thing(Container& input1, Container& input2) {
	auto tab      = fill_tab<Strategy>(input1, input2);
	auto mods     = backtrack<Strategy>(tab, input1, input2);
	auto last_col = tab[tab.size() - 1];
	make_padded<Strategy>(mods, input1, input2);
	return last_col[last_col.size() - 1];
}
