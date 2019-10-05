#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <vector>
#include "total_pairing.h"

int main(int argc,char* argv[]) {
//Input reading an sanitization
	int n = 0;
	char mode = 'p';
	const char errormsg[] = "Invalid arguments. Expected: [int,>0] [char = \'p\',\'s\'||\'p\']\n";
	if (argc == 1) {
		std::cout << "Vertex count[2*n]: n = ";
		if (!(std::cin >> n) || n <= 0) {
			std::cerr << "Invalid input\n";
			return 1;
		}
		std::cin.ignore(1000, '\n');
		std::cout << "Print[p] or Save[s]: mode = ";
		std::cin >> mode;
		if (mode != 's' && mode != 'p') {
			std::cerr << "Invalid input.\n";
			return 1;
		}
	} else if (argc == 2) {
		std::istringstream input(argv[1]);
		if (!(input >> n) || n <= 0) {
			std::cerr << errormsg;
			return 1;
		}
	} else if (argc == 3) {
		std::istringstream input1(argv[1]);
		if (!(input1 >> n) || n <= 0) {
			std::cerr << errormsg;
			return 1;
		}
		std::istringstream input2(argv[2]);
		input2 >> mode;
		if (mode != 'p' && mode != 's') {
			std::cerr << errormsg;
			return 1;
		}
	} else {
		std::cout << errormsg;
		return 1;
	}
	std::fstream save_file;
	std::string file_name;
	if (mode == 's') {
		std::ostringstream name;
		name << "n = " << n << ".txt";
		file_name = std::move(name.str());
		save_file.open(file_name.c_str(), std::fstream::out | std::fstream::trunc);
		if (!save_file.good()) {
			std::cerr << "Unable to save to file.\n";
			return 1;
		}
	}
	std::ostream& output = (mode == 's' ? save_file : std::cout);
//Using sanitized input to generate table
	n *= 2;
	std::vector<int> symbols(n);
	std::vector<int> pairings(n);

	for (int i = 0; i < n; ++i) {
		symbols[i] = i+1;
	}

	int fill_count = 0;
	while (n) ++fill_count, n /= 10;

	for (auto itr : symbols) {
		output << std::setw(fill_count) << std::setfill(' ') << itr << ' ';
	}
	output << '\n';

	total_pairing alg(symbols.cbegin(), symbols.cend());

	while (alg(pairings.begin())) {
		for (auto itr : pairings) {
			output << std::setw(fill_count) << std::setfill(' ') << itr << ' ';
		}
		output << '\n';
	}
	output << std::flush;
}
