#include "algorithms.hpp"
#include "strategies.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

int main(int argc, char const* argv[]) {
	std::string x = "abbacb", y = "cbbbacab";

	auto res = fill_tab<CharacterStrategy>(x, y);

	std::cout
	        << "================================================================================\n"
	           "==                            Edition distances matrix                        ==\n"
	           "================================================================================"
	        << std::endl;

	print_tab(res);
	std::cout << "\nMinimum total edition distance: " << res[x.length()][y.length()] << std::endl;

	std::cout
	        << "================================================================================\n"
	           "==                                List of editions                            ==\n"
	           "================================================================================"
	        << std::endl;

	auto mods = backtrack<CharacterStrategy>(res, x, y);

	{
		auto i = x.begin(), j = y.begin();
		for(auto mod : mods) {
			mod.print(i, j);
		}
	}

	std::cout
	        << "================================================================================\n"
	           "==                              Padding by character                          ==\n"
	           "================================================================================"
	        << std::endl;

	std::string xp, yp;
	std::tie(xp, yp) = make_padded<CharacterStrategy>(mods, x, y);
	std::cout << xp << std::endl;
	std::cout << yp << std::endl;

	if(argc != 3) {
		std::cout << std::endl
		          << "*** To activate the line by line padding, pass two files as command-line "
		             "arguments ***"
		          << std::endl;
		return 0;
	}

	std::cout
	        << "================================================================================\n"
	           "==                                Padding by lines                            ==\n"
	           "================================================================================"
	        << std::endl;

	std::ifstream input_stream1((argv[1])), input_stream2((argv[2]));
	std::vector<std::string> lines1, lines2;

	for(std::string line; std::getline(input_stream1, line);) {
		lines1.push_back(line);
	}

	for(std::string line; std::getline(input_stream2, line);) {
		lines2.push_back(line);
	}

	size_t cost;
	std::vector<std::string> padded_lines1, padded_lines2;
	std::tie(cost, padded_lines1, padded_lines2) = do_the_thing<LineStrategy>(lines1, lines2);

	std::cout << "Total cost: " << cost << std::endl;

	print_sbs(padded_lines1, padded_lines2);
}
