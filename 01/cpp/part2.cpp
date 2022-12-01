#include <array>
#include <iostream>
#include <string>
#include <numeric>

using namespace std;

int main() {

	std::string	line;


	array<size_t, 3> max_energy { 0, 0, 0 };
	auto process_total = [&](size_t total) {
		if (total > max_energy[0]) {
			max_energy[2] = max_energy[1];
			max_energy[1] = max_energy[0];
			max_energy[0] = total;
		} else if (total > max_energy[1]) {
			max_energy[2] = max_energy[1];
			max_energy[1] = total;
		} else if (total > max_energy[2]) {
			max_energy[2] = total;
		}
	};

	size_t total = 0;

	while (getline(cin, line)) {
		if (!line.empty()) {
			total += stoi(line);
		} else {
			process_total(total);
			total = 0;
		}
	}

	process_total(total);
	cout << accumulate(begin(max_energy), end(max_energy), 0ull) << endl;

	return 0;
}
