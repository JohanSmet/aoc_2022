#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

using namespace std;

int main() {

	vector<string> 	forest;

	string input;

	while (getline(cin, input)) {
		forest.push_back(input);
	}

	const int h = forest.size();
	const int w = forest[0].size();

	int max_score = 0;

	for (int y = 0; y < h; ++y) {
		for (int x = 0; x < w; ++x) {

			const char tree = forest[y][x];
			int count[4] = {0};

			// left
			for (int i = x - 1; i >= 0; --i) {
				count[0] += 1;
				if (forest[y][i] >= tree) {
					break;
				}
			}

			// right
			for (int i = x + 1; i < w; ++i) {
				count[1] += 1;
				if (forest[y][i] >= tree) {
					break;
				}
			}

			// up
			for (int i = y - 1; i >= 0; --i) {
				count[2] += 1;
				if (forest[i][x] >= tree) {
					break;
				}
			}

			// up
			for (int i = y + 1; i < h; ++i) {
				count[3] += 1;
				if (forest[i][x] >= tree) {
					break;
				}
			}

			int score = accumulate(begin(count), end(count), 1, multiplies<int>());
			max_score = max(max_score, score);
		}
	}

	cout << max_score << endl;

	return 0;
}
