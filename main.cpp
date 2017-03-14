#include <iostream>
#include <string>
#include <vector>

template <typename T>
using matrix = std::vector<std::vector<T>>;

size_t add(char /* i */) {
	return 1;
}

size_t del(char /* i */) {
	return 1;
}

size_t sub(char i, char j) {
	return static_cast<size_t>(not(i == j));
}

template <typename... T>
size_t min(size_t lhs, size_t rhs, T... values) {
	return (lhs < rhs) ? min(lhs, values...) : min(rhs, values...);
}

template <>
size_t min(size_t lhs, size_t rhs) {
	return (lhs < rhs) ? lhs : rhs;
}

std::vector<std::vector<size_t>> fill_tab(std::string const& x, std::string const& y) {
	auto x_len = x.length(), y_len = y.length(); // , max = (x_len > y_len) ? x_len : y_len;
	matrix<size_t> res(x_len + 1, std::vector<size_t>(y_len + 1));

	res[0][0] = 0;

	for(size_t i = 0; i < x_len + 1; ++i) {
		res[i][0] = i;
	}

	for(size_t i = 0; i < y_len + 1; ++i) {
		res[0][i] = i;
	}

	for(size_t i = 1; i < x_len + 1; ++i) {
		for(size_t j = 1; j < y_len + 1; ++j) {
			res[i][j] = min(res[i - 1][j - 1] + sub(x[i - 1], y[j - 1]),
			                res[i - 1][j] + del(x[i - 1]),
			                res[i][j - 1] + add(y[j - 1]));
		}
	}

	return res;
}

void print_tab(matrix<size_t> mat) {
	for(auto const& subtab : mat) {
		for(auto const& value : subtab) {
			std::cout << value << " ";
		}
		std::cout << std::endl;
	}
}

int main(int argc, char const* argv[]) {
	std::string x = "abcd", y = "aaccd";

	auto res = fill_tab(x, y);

	std::cout << std::endl;
	print_tab(res);
	std::cout << std::endl;

	std::cout << res[x.length()][y.length()] << std::endl;
}
