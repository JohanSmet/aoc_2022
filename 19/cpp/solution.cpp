#include <algorithm>
#include <array>
#include <execution>
#include <iostream>
#include <regex>
#include <unordered_map>
#include <vector>

using namespace std;

namespace Material {
	constexpr int32_t None = -1;
	constexpr int32_t Ore = 0;
	constexpr int32_t Clay = 1;
	constexpr int32_t Obsidian = 2;
	constexpr int32_t Geode = 3;
	constexpr int32_t COUNT = 4;
}

struct Cost {
	int32_t		m_material;
	int32_t		m_count;
};

using BuildRule = std::array<Cost, 2>;

struct Blueprint {
	int32_t									m_id;
	std::array<BuildRule, Material::COUNT>	m_rules;
	std::array<int32_t, Material::COUNT>	m_max_robots;
};

struct State {
	int32_t									m_time_remaining;
	std::array<int32_t, Material::COUNT>	m_robots;
	std::array<int32_t, Material::COUNT>	m_inventory;

	void run_production() {
		for (int32_t i = 0; i < Material::COUNT; i += 1) {
			m_inventory[i] += m_robots[i];
		}
	}

	bool check_build_rule(BuildRule const &rule) {
		for (auto const &cost : rule) {
			if (cost.m_material != Material::None && m_inventory[cost.m_material] < cost.m_count) {
				return false;
			}
		}

		return true;
	}

	void apply_build_rule(int32_t material, BuildRule const &rule) {
		m_robots[material] += 1;
		for (auto const &cost : rule) {
			if (cost.m_material != Material::None) {
				m_inventory[cost.m_material] -= cost.m_count;
			}
		}
	}
};

int32_t process(Blueprint const &bp, int32_t time) {

	int32_t max_geodes = 0;

	vector<State> states_todo;
	auto &state = states_todo.emplace_back();
	state.m_time_remaining = time;
	state.m_robots[Material::Ore] = 1;

	while (!states_todo.empty()) {
		auto state = states_todo.back();
		states_todo.pop_back();

		// end-game
		if (state.m_time_remaining <= 0) {
			max_geodes = max(max_geodes, state.m_inventory[Material::Geode]);
			continue;
		}

		// is it still possible to top the current best?
		int32_t max_additional = state.m_time_remaining * state.m_robots[Material::Geode] + state.m_time_remaining * (state.m_time_remaining - 1);
		if (state.m_inventory[Material::Geode] + max_additional < max_geodes) {
			continue;
		}

		// next state where we don't produce anything
		auto &next_nop = states_todo.emplace_back();
		next_nop.m_time_remaining = state.m_time_remaining - 1;
		next_nop.m_robots = state.m_robots;
		next_nop.m_inventory = state.m_inventory;
		next_nop.run_production();

		// try each rule
		for (int32_t mat = Material::COUNT - 1; mat >= 0; --mat) {
			if (!state.check_build_rule(bp.m_rules[mat])) {
				continue;
			}

			if (bp.m_max_robots[mat] > 0 && state.m_robots[mat] >= bp.m_max_robots[mat]) {
				continue;
			}

			auto &next_op = states_todo.emplace_back();
			next_op.m_time_remaining = state.m_time_remaining - 1;
			next_op.m_robots = state.m_robots;
			next_op.m_inventory = state.m_inventory;
			next_op.run_production();
			next_op.apply_build_rule(mat, bp.m_rules[mat]);

			// if we can be a geode robot: don't do the other options
			if (mat == Material::Geode) {
				break;
			}
		}
	}

	return max_geodes;
}

int main() {
	std::vector<Blueprint>	blueprints;

	// input
	regex r_data("Blueprint ([[:digit:]]+): "
				 "Each ore robot costs ([[:digit:]]+) ore. "
				 "Each clay robot costs ([[:digit:]]+) ore. "
				 "Each obsidian robot costs ([[:digit:]]+) ore and ([[:digit:]]+) clay. "
				 "Each geode robot costs ([[:digit:]]+) ore and ([[:digit:]]+) obsidian.");

	string line;
	while (getline(cin, line)) {
		smatch	sm;

		if (regex_match(line, sm, r_data)) {
			auto &bp = blueprints.emplace_back();
			int32_t i = 1;
			bp.m_id = stoi(sm[i++]);
			bp.m_rules[Material::Ore][0] = {Material::Ore, stoi(sm[i++])};
			bp.m_rules[Material::Ore][1] = {Material::None, 0};
			bp.m_rules[Material::Clay][0] = {Material::Ore, stoi(sm[i++])};
			bp.m_rules[Material::Clay][1] = {Material::None, 0};
			bp.m_rules[Material::Obsidian][0] = {Material::Ore, stoi(sm[i++])};
			bp.m_rules[Material::Obsidian][1] = {Material::Clay, stoi(sm[i++])};
			bp.m_rules[Material::Geode][0] = {Material::Ore, stoi(sm[i++])};
			bp.m_rules[Material::Geode][1] = {Material::Obsidian, stoi(sm[i++])};

			bp.m_max_robots[Material::Ore] = max(max(max(bp.m_rules[0][0].m_count, bp.m_rules[1][0].m_count),
											 bp.m_rules[2][0].m_count), bp.m_rules[3][0].m_count);
			bp.m_max_robots[Material::Clay] = bp.m_rules[Material::Obsidian][1].m_count;
			bp.m_max_robots[Material::Obsidian] = bp.m_rules[Material::Geode][1].m_count;
			bp.m_max_robots[Material::Geode] = 0;

		} else {
			cerr << "Parsing failed\n";
		}
	}

	// try each blueprint
	int32_t level = 0;

	std::for_each(std::execution::par_unseq, std::begin(blueprints), std::end(blueprints), [&](auto&& bp) {
		int32_t geodes = process(bp, 24);
		level += geodes * bp.m_id;
	});

	cout << "Part 1: " << level << endl;

	int32_t product = 1;

	std::for_each(std::execution::par_unseq, std::begin(blueprints), std::next(std::begin(blueprints), 3), [&](auto&& bp) {
		int32_t geodes = process(bp, 32);
		product *= geodes;
	});

	cout << "Part 2: " << product << endl;

	return 0;
}
