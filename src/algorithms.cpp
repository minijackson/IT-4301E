#include "algorithms.hpp"

#include <iomanip>
#include <iostream>

#include <sys/ioctl.h>
#include <unistd.h>

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

std::string repeat(std::string str, const std::size_t n) {
	if(n == 0) {
		str.clear();
		str.shrink_to_fit();
		return str;
	}

	if(n == 1 || str.empty())
		return str;

	const auto period = str.size();

	if(period == 1) {
		str.append(n - 1, str.front());
		return str;
	}

	str.reserve(period * n);

	std::size_t m{2};

	for(; m < n; m *= 2)
		str += str;

	str.append(str.c_str(), (n - (m / 2)) * period);

	return str;
}

void print_wrapped_lines(std::string const& line1, std::string const& line2, size_t width) {
	size_t line1_len = line1.size(), line2_len = line2.size();
	for(size_t offset = 0; offset < line1_len or offset < line2_len; offset += width) {
		size_t line1_off = (offset > line1_len) ? line1_len : offset,
		       line2_off = (offset > line2_len) ? line2_len : offset;
		auto sub_line1 = line1.substr(line1_off, width), sub_line2 = line2.substr(line2_off, width);
		std::cout << "║";
		std::cout << std::left << std::setw(width) << sub_line1;
		std::cout << "║";
		std::cout << std::left << std::setw(width) << sub_line2;
		std::cout << "║" << std::endl;
	}
}

void print_sbs(std::vector<std::string> const& input1, std::vector<std::string> const& input2) {
	struct winsize win;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &win);

	size_t cols = win.ws_col, col_size = (cols - 3) / 2;

	std::string row_sep = repeat("═", col_size);

	std::cout << "╔" << row_sep << "╦" << row_sep << "╗" << std::endl;

	print_wrapped_lines(input1[0], input2[0], col_size);

	for(size_t i = 1, count = input1.size(); i < count; ++i) {

		std::cout << "╠" << row_sep << "╬" << row_sep << "╣" << std::endl;

		print_wrapped_lines(input1[i], input2[i], col_size);
	}

	std::cout << "╚" << row_sep << "╩" << row_sep << "╝" << std::endl;
}
