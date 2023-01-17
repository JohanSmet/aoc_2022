#include <algorithm>
#include <iostream>
#include <numeric>
#include <regex>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

#include "dijkstra.h"

using namespace std;

struct Valve {
	string			name;
	int32_t			flow_rate;
	vector<int32_t> tunnels;
	vector<int32_t> costs;
};

int32_t index_of_valve(vector<Valve> &valves, string_view name) {

	for (int32_t i = 0; i < valves.size(); ++i) {
		if (valves[i].name == name) {
			return i;
		}
	}

	auto &v = valves.emplace_back();
	v.name = name;
	return valves.size() - 1;
}

int32_t r_calculate(vector<Valve> const &valves, vector<int32_t> const &working,
					int32_t source, int32_t time_remaining, unordered_set<int32_t> opened) {

	if (time_remaining < 0) {
		return 0;
	}

	// open the valve
	time_remaining -= 1;
	opened.insert(source);
	int32_t this_flow = time_remaining * valves[source].flow_rate;

	// go to the next open valve
	int32_t max_flow = 0;

	for (auto next: working) {
		if (opened.contains(next)) {
			continue;
		}

		max_flow = max(max_flow, r_calculate(valves, working, next, time_remaining - valves[source].costs[next], opened));
	}

	return this_flow + max_flow;

}

int main() {

	vector<Valve>	valves;

	// input
	regex r_data("Valve ([A-Z][A-Z]) has flow rate=([[:digit:]]+); [a-z ]+(.*)");

	int32_t max_open = 0;

	string line;
	while (getline(cin, line)) {
		smatch	sm;

		if (regex_match(line, sm, r_data)) {
			int32_t idx = index_of_valve(valves, sm[1].str());

			valves[idx].flow_rate = stoi(sm[2]);
			if (valves[idx].flow_rate > 0) {
				max_open += 1;
			}

			auto tunnels = sm[3].str();
			for (int32_t i = 0; i < tunnels.size(); i += 4) {
				int32_t connection = index_of_valve(valves, tunnels.substr(i, 2));
				valves[idx].tunnels.push_back(connection);
			}
		} else {
			cerr << "Oepsie" << endl;
		}
	}

	// precompute the cost to move to each valve from each valve
	Dijkstra shortest_path;
	vector<int32_t>	working_valves;

	auto fetch_neighbour = [&](int32_t index) -> std::vector<int32_t> & {
		return valves[index].tunnels;
	};

	for (int32_t source = 0; source < valves.size(); ++source) {
		shortest_path.initialize(valves.size());
		shortest_path.execute(source, fetch_neighbour);

		valves[source].costs = shortest_path.node_costs();

		if (valves[source].flow_rate > 0) {
			working_valves.push_back(source);
		}
	}

	// calculate
	int32_t current = index_of_valve(valves, "AA");
	int32_t max_flow = 0;

	for (auto valve : working_valves) {
		int32_t flow = r_calculate(valves, working_valves, valve, 30 - valves[current].costs[valve], {});
		max_flow = max(flow, max_flow);
	}

	cout << max_flow << endl;

	return 0;
}
