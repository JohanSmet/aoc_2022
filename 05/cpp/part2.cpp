#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {

	vector<vector<char>> 	stacks;

	// read input
	string input;

	while (getline(cin, input)) {
		if (input.size() < 2 || input[1] == '1') {
			break;
		}

		for (int i = 1, s = 0; i < input.size(); i += 4, s += 1) {
			if (input[i] != ' ') {
				if (s >= stacks.size()) {
					stacks.resize(s + 1);
				}

				stacks[s].push_back(input[i]);
			}
		}
	}

	for (auto &stack : stacks) {
		reverse(begin(stack), end(stack));
	}

	// skip empty line dividing input
	getline(cin, input);

	// move stacks
	int count, source, dest;
	string move, from, to;

	while (cin >> move >> count >> from >> source >> to >> dest) {
		auto &s_stack = stacks[source - 1];
		auto &d_stack = stacks[dest - 1];

		auto s = prev(end(s_stack), count);
		d_stack.insert(end(d_stack), s, end(s_stack));
		s_stack.erase(s, end(s_stack));
	}

	for (auto const &stack : stacks) {
		if (!stack.empty()) {
			cout << stack.back();
		}
	}
	cout << endl;

	return 0;
}
