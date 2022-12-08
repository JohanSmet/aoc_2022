#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {

	vector<string> 	forest;

	// read input
	string input;

	while (getline(cin, input)) {
		forest.push_back(input);
	}

	// check visibility
	int h = forest.size();
	int w = forest[0].size();
	vector<bool> visibility(w * h, false);

	char max = '0';

	auto check_pos = [&](int x, int y) {
		if (forest[y][x] > max) {
			visibility[y * w + x] = true;
			max = forest[y][x];
		}
	};

	// rows
	for (int y = 0; y < h; ++y) {
		max = '0' - 1;
		for (int x = 0; x < w; ++x) {
			check_pos(x,y);
		}
		max = '0' - 1;
		for (int x = w-1; x >= 0; --x) {
			check_pos(x,y);
		}
	}

	// columns
	for (int x = 0; x < w; ++x) {
		max = '0' - 1;
		for (int y = 0; y < h; ++y) {
			check_pos(x,y);
		}
		max = '0' - 1;
		for (int y = h-1; y >= 0; --y) {
			check_pos(x,y);
		}
	}

	// total
	auto total = count(begin(visibility), end(visibility), true);
	cout << total << endl;

	return 0;
}
