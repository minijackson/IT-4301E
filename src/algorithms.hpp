#pragma once

#include <iostream>
#include <limits>
#include <list>
#include <string>
#include <tuple>
#include <vector>

template<typename T>
using matrix = std::vector<std::vector<T>>;

template<typename Container>
struct Modification {
	enum { ADD, DEL, SUB } type;

	void print(typename Container::iterator& i, typename Container::iterator& j) {
		switch(type) {
			case ADD:
				std::cout << "Add '" << *j << "'";
				++j;
				break;
			case DEL:
				std::cout << "Del '" << *i << "'";
				++i;
				break;
			case SUB:
				std::cout << "Substitute '" << *i << "' by '" << *j << "'";
				++i;
				++j;
				break;
		}
		std::cout << std::endl;
	}
};

size_t min(size_t lhs, size_t rhs);

void print_tab(matrix<size_t> const& mat);

void print_sbs(std::vector<std::string> const& input1, std::vector<std::string> const& input2);

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
std::list<Modification<Container>> backtrack(matrix<size_t> const& mat,
                                                                  Container& input1,
                                                                  Container& input2) {
	using Mod = Modification<Container>;
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
			res.push_front(Mod{Mod::ADD});
			--position.second;
		} else if(minimum == cost_sub) {
			res.push_front(Mod{Mod::SUB});
			--position.first;
			--position.second;
		} else {
			res.push_front(Mod{Mod::DEL});
			--position.first;
		}
	}

	return res;
}

template<typename Strategy, typename Container>
std::pair<Container, Container> make_padded(
        std::list<Modification<Container>> mods,
        Container const& input1,
        Container const& input2) {

	using Mod = Modification<Container>;

	Container res1, res2;

	size_t pos1 = 0, pos2 = 0;

	for(auto mod : mods) {
		if(mod.type == Mod::ADD) {
			res1.push_back(Strategy::empty_value());
			res2.push_back(input2[pos2]);

			++pos2;
		} else if(mod.type == Mod::DEL) {
			res1.push_back(input1[pos1]);
			res2.push_back(Strategy::empty_value());

			++pos1;
		} else {
			// Substitution
			auto sub_element1 = input1[pos1], sub_element2 = input2[pos2];

			Strategy::modify_subelements(sub_element1, sub_element2);

			res1.push_back(sub_element1);
			res2.push_back(sub_element2);

			++pos2;
			++pos1;
		}
	}

	return {res1, res2};
}


template<typename Strategy, typename Container>
std::tuple<size_t, Container, Container> do_the_thing(Container& input1, Container& input2, matrix<size_t> tab) {
	auto mods     = backtrack<Strategy>(tab, input1, input2);
	auto last_col = tab[tab.size() - 1];

	Container res1, res2;
	std::tie(res1, res2) = make_padded<Strategy>(mods, input1, input2);

	return {last_col[last_col.size() - 1], res1, res2};
}

template<typename Strategy, typename Container>
std::tuple<size_t, Container, Container> do_the_thing(Container& input1, Container& input2) {
	auto tab      = fill_tab<Strategy>(input1, input2);
	return do_the_thing<Strategy>(input1, input2, tab);
}
