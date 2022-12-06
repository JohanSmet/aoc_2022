#include <array>
#include <algorithm>
#include <iostream>
#include <string>

using namespace std;

constexpr int SIZE = 14 - 1;

int main() {

	// read input
	string input;
	getline(cin, input);

	array<bool, 26>	occurs;

	for (int i = SIZE; i < input.size(); ++i) {
		fill(begin(occurs), end(occurs), false);
		bool unique = true;

		for (int j = i - SIZE; unique && j <= i; ++j) {
			int pos = input[j] - 'a';
			unique = !occurs[pos];
			occurs[pos] = true;
		}

		if (unique) {
			cout << "First unique sequence at " << i + 1 << endl;
			break;
		}
	}

	return 0;
}
