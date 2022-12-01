#include <iostream>
#include <string>

using namespace std;

int main() {

	std::string	line;

	size_t total = 0;
	size_t max_energy = 0;

	while (getline(cin, line)) {
		if (!line.empty()) {
			total += stoi(line);
		} else {
			max_energy = max(max_energy, total);
			total = 0;
		}
	}
	max_energy = max(max_energy, total);

	cout << max_energy << endl;

	return 0;
}
