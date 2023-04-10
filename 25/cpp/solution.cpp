#include <algorithm>
#include <iostream>
#include <optional>
#include <regex>
#include <unordered_map>
#include <vector>

using namespace std;

int64_t snafu_value(char c) {
	if (c == '2') {
		return 2;
	} else if (c == '1') {
		return 1;
	} else if (c == '0') {
		return 0;
	} else if (c == '-') {
		return -1;
	} else if (c == '=') {
		return -2;
	} else {
		cerr << "Error !" << endl;
		return -10000;
	}
}

char snafu_digit(int32_t v) {
	switch (v) {
		case -2: return '=';
		case -1: return '-';
		case 0:  return '0';
		case 1:  return '1';
		case 2:  return '2';
		default: return '?';
	}
}

int64_t snafu_to_decimal(string const &snafu) {

	int64_t result = 0;
	int64_t place = 1;

	for (auto iter = snafu.rbegin(); iter != snafu.rend(); ++iter) {
		result += place * snafu_value(*iter);
		place *= 5;
	}

	return result;
}

string decimal_to_snafu(int64_t decimal) {

	string snafu = "";
	int64_t place = 1;
	int64_t next_place = place * 5;

	while (decimal > 0) {
		int64_t v = decimal % next_place;
		v /= place;

		if (v > 2) {
			v = v - 5;
		}
		decimal -= v * place;

		snafu.push_back(snafu_digit(v));

		place = next_place;
		next_place *= 5;
	}

	reverse(begin(snafu), end(snafu));
	return snafu;
}

int main() {

	// input
	string line;
	int64_t total = 0;

	while(getline(cin, line)) {
		total += snafu_to_decimal(line);
	}

	cout << total << endl;
	cout << decimal_to_snafu(total);
	return 0;
}
