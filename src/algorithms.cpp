#include "algorithms.hpp"

size_t min(size_t lhs, size_t rhs) {
	return (lhs < rhs) ? lhs : rhs;
}

void print_tab(matrix<size_t> const& mat) {
	for(auto const& subtab : mat) {
		for(auto const& value : subtab) {
			std::cout << value << " ";
		}
		std::cout << std::endl;
	}
}
