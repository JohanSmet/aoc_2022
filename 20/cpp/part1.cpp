#include <algorithm>
#include <iostream>
#include <vector>

// solving this took a while - it sure didn't help that I did in several 30-60 minute sessions spread over a few nights
//	finally it clicked that the number of moves had to be limited by one less than the total number of values because
//	the value that is being shifted isn't in the list of numbers it can be positioned after.

using namespace std;

int32_t normalize(int32_t idx, int32_t size) {
	while (idx < 0) {
		idx += size;
	}

	return idx % size;
}

vector<int32_t> sort_values(std::vector<int32_t> const &values, std::vector<int32_t> const &next) {
	vector<int32_t> result;
	result.reserve(values.size());

	int32_t idx = distance(begin(values), ranges::find(values, 0));
	for (int32_t count = 0; count < values.size(); ++count) {
		result.push_back(values[idx]);
		idx = next[idx];
	}

	return result;
}

int main() {
	std::vector<int32_t>	values;
	std::vector<int32_t>	next;
	std::vector<int32_t>	prev;

	// input
	int16_t v;
	while (cin >> v) {
		values.push_back(v);
	}

	for (int32_t i = 0; i < values.size(); ++i) {
		prev.push_back(normalize(i - 1, values.size()));
		next.push_back(normalize(i + 1, values.size()));
	}

	// process
	for (int32_t pos = 0; pos < values.size(); pos += 1) {

		// find new position
		int32_t total = abs(values[pos]) % (values.size() - 1);
		auto &link = (values[pos] > 0) ? next : prev;
		int32_t new_pos = pos;

		for (int32_t i = 0; i < total; ++i) {
			new_pos = link[new_pos];
		}

		if (new_pos == pos) {
			continue;
		}

		// remove from linked list
		next[prev[pos]] = next[pos];
		prev[next[pos]] = prev[pos];

		// insert at new position
		int32_t new_prev = (values[pos] > 0) ? new_pos : prev[new_pos];
		int32_t new_next = (values[pos] > 0) ? next[new_pos] : new_pos;

		next[new_prev] = pos;
		prev[new_next] = pos;
		prev[pos] = new_prev;
		next[pos] = new_next;
	}


	auto sorted = sort_values(values, next);

	int32_t result =
		sorted[normalize(1000, values.size())] +
		sorted[normalize(2000, values.size())] +
		sorted[normalize(3000, values.size())];

	cout << "Part 1: " << result << endl;


	return 0;
}
