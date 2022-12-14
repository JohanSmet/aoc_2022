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

	vector<El>	packets;

	// read input
	while (do_input) {
		getline(cin, line);
		packets.push_back(parse_packet(line));

		getline(cin, line);
		packets.push_back(parse_packet(line));

		do_input = !!getline(cin, line);
	}

	// add dividers
	packets.push_back(parse_packet("[[2]]"));
	packets.push_back(parse_packet("[[6]]"));

	// sort packets
	sort(begin(packets), end(packets), compare_packets);

	// find dividers
	int32_t divider_2 = 0;
	int32_t divider_6 = 0;

	for (int32_t i = 0; i < packets.size() && (divider_2 == 0 || divider_6 == 0); ++i) {
		auto const &p = packets[i];

		if (!(!p.is_integer && p.list_value.size() == 1)) {
			continue;
		}

		auto const &sp = p.list_value[0];

		if (!sp.is_integer && sp.list_value.size() == 1 && sp.list_value[0].int_value) {
			switch (sp.list_value[0].int_value) {
				case 2:
					divider_2 = i + 1;
					break;
				case 6:
					divider_6 = i + 1;
					break;
			}
		}
	}

	cout << divider_2 * divider_6 << endl;

	return 0;
}
