#include <algorithm>
#include <array>
#include <functional>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

using namespace std;

struct El {
	bool				is_integer;
	int32_t				int_value;
	std::vector<El>		list_value;
};

El parse_list(string_view line, int &idx) {
	El result;

	result.is_integer = false;
	result.int_value = 0;

	while (line[idx] != ']') {
		if (line[idx] == '[') {
			idx += 1;
			result.list_value.push_back(parse_list(line, idx));
		} else if (line[idx] == ',') {
			idx += 1;
		} else {
			int v = 0;
			while (isdigit(line[idx])) {
				v = (v * 10) + (line[idx] - '0');
				idx += 1;
			}
			result.list_value.push_back({true, v, {}});
		}
	}

	idx += 1;
	return result;
}

El parse_packet(string_view line) {
	int index = 1;
	return parse_list(line, index);
}

int r_compare_packets(El const& packet_1, El const& packet_2);

int compare_lists(vector<El> const &list_1, vector<El> const &list_2) {

	for (int32_t idx = 0; idx < list_1.size() && idx < list_2.size(); ++idx) {
		auto comp = r_compare_packets(list_1[idx], list_2[idx]);
		if (comp != 0) {
			return comp;
		}
	}

	return list_2.size() - list_1.size();
}

int r_compare_packets(El const& packet_1, El const& packet_2) {

	if (packet_1.is_integer && packet_2.is_integer) {
		return packet_2.int_value - packet_1.int_value;
	} else if (!packet_1.is_integer && !packet_2.is_integer) {
		return compare_lists(packet_1.list_value, packet_2.list_value);
	} else if (packet_1.is_integer && !packet_2.is_integer) {
		return compare_lists({packet_1}, packet_2.list_value);
	} else {
		return compare_lists(packet_1.list_value, {packet_2});
	}
}

bool compare_packets(El const& packet_1, El const& packet_2) {
	return r_compare_packets(packet_1, packet_2) > 0;
}

int main() {

	bool do_input = true;
	string line;
	int pair_idx = 1;
	int sum = 0;

	while (do_input) {
		getline(cin, line);
		auto packet_1 = parse_packet(line);

		getline(cin, line);
		auto packet_2 = parse_packet(line);

		bool in_order = compare_packets(packet_1, packet_2);
		if (in_order) {
			sum += pair_idx;
		}

		do_input = !!getline(cin, line);
		pair_idx += 1;
	}

	cout << sum << endl;

	return 0;
}
