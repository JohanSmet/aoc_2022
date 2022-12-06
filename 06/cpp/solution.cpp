#include <array>
#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>

using namespace std;

int find_unique_sequence(string_view input, int len) {

	array<bool, 26>	occurs;

	for (int i = len - 1; i < input.size(); ++i) {
		fill(begin(occurs), end(occurs), false);
		bool unique = true;

		for (int j = i - len + 1; unique && j <= i; ++j) {
			int pos = input[j] - 'a';
			unique = !occurs[pos];
			occurs[pos] = true;
		}

		if (unique) {
			return i + 1;
		}
	}

	return 0;
}

int main() {

	// read input
	string input;
	getline(cin, input);

	cout << "Part 1: " << find_unique_sequence(input, 4) << endl;
	cout << "Part 2: " << find_unique_sequence(input, 14) << endl;


	return 0;
}
