#include <algorithm>
#include <array>
#include <execution>
#include <iostream>
#include <numeric>
#include <regex>
#include <string>
#include <string_view>
#include <vector>

#include "dijkstra.h"

// I'm not too satisfied with this solution; the intention was to add dynamic programming to reduce the runtime.
// But I already spent more time on this than I'd like to admit. This brute-force approach finished in about 15 minutes
// on my machine (6-core ryzen).

using namespace std;

struct Valve {
	string			name;
	int32_t			flow_rate;
	int32_t			significant;
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
	return static_cast<int32_t>(valves.size() - 1);
}

int32_t r_calculate(
	vector<Valve>& valves,
	int32_t pawn,
	array<int32_t, 2> time_remaining,
	array<int32_t, 2> current,
	int32_t opened = 0,
	int32_t level = 0) {

	auto update_current = [=](int32_t next, int32_t pawn) -> std::array<int32_t, 2> {
		auto result = current;
		result[pawn] = next;
		return result;
	};

	auto mark_open = [&](int32_t idx) {
		opened |= (1 << idx);
	};

	auto check_open = [&](int32_t idx) -> bool {
		return (opened & (1 << idx));
	};

	int32_t turn_flow = 0;

	// open the valve with the current pawn
	if (valves[current[pawn]].flow_rate > 0 && time_remaining[pawn] > 0) {
		time_remaining[pawn] -= 1;
		mark_open(current[pawn]);
		turn_flow = time_remaining[pawn] * valves[current[pawn]].flow_rate;
	}

	// move the other pawn to another valve
	int32_t  best_result = 0;

	for (int32_t next_pawn = 0; next_pawn < 2; ++next_pawn) {
		for (int32_t next = 0; next < valves.size(); ++next) {

			if (check_open(next) || valves[next].flow_rate <= 0) {
				continue;
			}

			auto remaining = time_remaining;
			remaining[next_pawn] -= valves[current[next_pawn]].costs[next];

			if (remaining[next_pawn] > 0) {
				auto result = r_calculate(valves, next_pawn, remaining, update_current(next, next_pawn), opened, level + 1);
				if (result > best_result) {
					best_result = result;
				}
			}
		}
	}

	return best_result + turn_flow;
}

int main() {

	vector<Valve>	valves;

	// input
	regex r_data("Valve ([A-Z][A-Z]) has flow rate=([[:digit:]]+); [a-z ]+(.*)");

	int32_t count_significant = 0;
	int32_t start_index = -1;

	string line;
	while (getline(cin, line)) {
		smatch	sm;

		if (regex_match(line, sm, r_data)) {
			int32_t idx = index_of_valve(valves, sm[1].str());

			valves[idx].flow_rate = stoi(sm[2]);
			if (valves[idx].flow_rate > 0 || valves[idx].name == "AA") {
				valves[idx].significant = count_significant++;
				if (valves[idx].name == "AA") {
					start_index = valves[idx].significant;
				}
			} else {
				valves[idx].significant = -1;
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

	for (int32_t idx = 0; idx < valves.size(); ++idx) {
		if (valves[idx].significant < 0) {
			continue;
		}

		shortest_path.initialize(static_cast<int32_t>(valves.size()));
		shortest_path.execute(idx, [&](int32_t index) -> std::vector<int32_t> & {return valves[index].tunnels;});

		valves[idx].costs = shortest_path.node_costs();
	}

	// build a reduced set of valves
	vector<Valve> significant_valves(count_significant);

	for (auto &valve: valves) {
		if (valve.significant < 0) {
			continue;
		}

		auto &sig_valve = significant_valves[valve.significant];
		sig_valve.name = valve.name;
		sig_valve.flow_rate = valve.flow_rate;
		sig_valve.significant = valve.significant;
		sig_valve.costs.resize(count_significant);

		for (int32_t idx = 0; idx < valves.size(); ++idx) {
			auto &next_valve = valves[idx];
			if (next_valve.significant >= 0) {
				sig_valve.costs[next_valve.significant] = valve.costs[idx];
			}
		}
	}

	// calculate
	vector<int32_t> results(count_significant, 0);

	std::for_each(std::execution::par_unseq, std::begin(significant_valves), std::end(significant_valves), [&](auto&& valve) {
		if (valve.flow_rate <= 0) {
			return;
		}

		results[valve.significant] = r_calculate(
											significant_valves, 0,
											{ 26 - significant_valves[start_index].costs[valve.significant], 26 },
											{ valve.significant, start_index });
	});

	cout << *std::ranges::max_element(results) << endl;

	return 0;
}
