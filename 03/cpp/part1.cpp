#include <algorithm>
#include <array>
#include <iostream>
#include <string>

using namespace std;

int get_priority(char c) {
	if (c >= 'a' && c <= 'z') {
		return c - 'a' + 1;
	} else if (c >= 'A' && c <= 'Z') {
		return c - 'A' + 27;
	} else {
		return 0;
	}
}

int main() {

	string input;
	array<int, 53>	occurence;
	int total = 0;

	while (getline(cin, input)) {
		fill(begin(occurence), end(occurence), 0);
		int half = input.size() / 2;

		for (int i = 0; i < half; ++i) {
			occurence[get_priority(input[i])] += 1;
		}

		for (int i = half; i < input.size(); ++i) {
			int prio = get_priority(input[i]);
			if (occurence[prio] > 0) {
				total += prio;
				break;
			}
		}
	}

	cout << total << endl;

	return 0;
}
