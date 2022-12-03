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
	array<int, 53>	occurence = {0};

	int total = 0;
	int flag = 1;

	while (getline(cin, input)) {
		for (auto c : input) {
			int prio = get_priority(c);
			occurence[prio] |= flag;
		}

		if (flag == 4) {
			auto common = find(begin(occurence), end(occurence), 7);
			total += distance(begin(occurence), common);

			fill(begin(occurence), end(occurence), 0);
			flag = 1;
		} else {
			flag <<= 1;
		}
	}

	cout << total << endl;

	return 0;
}
