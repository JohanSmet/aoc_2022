#include <algorithm>
#include <array>
#include <functional>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

using namespace std;

struct Coord {
	int32_t x;
	int32_t y;
};

static constexpr array<Coord,4> NEIGHBOUR_DELTA = {
	Coord{ 0, -1},
	Coord{ 1,  0},
	Coord{ 0,  1},
	Coord{-1,  0}
};

int main() {

	vector<vector<int>> map;

	Coord source;
	Coord target;

	// read map
	string line;
	while (getline(cin, line)) {
		auto &row = map.emplace_back();
		row.reserve(line.size());

		for (int i = 0; i < line.size(); ++i) {
			if (line[i] == 'S') {
				row.push_back(0);
				source = {i, int(map.size()) - 1};
			} else if (line[i] == 'E') {
				row.push_back('z' - 'a');
				target = {i, int(map.size()) - 1};
			} else {
				row.push_back(line[i] - 'a');
			}
		}
	}

	auto coord_to_index = [map](Coord const &c) -> int32_t {
		return c.y * map[0].size() + c.x;
	};

	auto index_to_coord = [map](int32_t index) -> Coord {
		return {
			int32_t(index % map[0].size()),
			int32_t(index / map[0].size())
		};
	};

	auto valid_cell = [map](Coord const &c) -> bool {
		return c.x >= 0 && c.x < map[0].size() && c.y >= 0 && c.y < map.size();
	};

	auto coordinate_add = [](Coord const &c, Coord const &delta) -> Coord {
		return {c.x + delta.x, c.y + delta.y};
	};

	// dijkstra
	using prio_t = pair<int32_t, int32_t>;
	priority_queue<prio_t, vector<prio_t>, std::less<prio_t>> todo;
	todo.push(make_pair(0, coord_to_index(target)));

	vector<int32_t> previous(map.size() * map[0].size(), -1);
	vector<int32_t> cost(map.size() * map[0].size(), 100000);

	while (!todo.empty()) {
		auto [curr_cost, curr_index] = todo.top();
		todo.pop();

		auto curr_coord = index_to_coord(curr_index);

		// check if this is a cheaper way to reach our neighbouring cells
		for (auto const &nb_delta : NEIGHBOUR_DELTA) {
			auto next_coord = coordinate_add(curr_coord, nb_delta);

			if (!valid_cell(next_coord)) {
				continue;
			}

			if (map[next_coord.y][next_coord.x] - map[curr_coord.y][curr_coord.x] < -1) {
				continue;
			}

			auto next_index = coord_to_index(next_coord);
			auto next_cost  = curr_cost + 1;

			if (next_cost < cost[next_index]) {
				todo.push(make_pair(next_cost, next_index));
				cost[next_index] = next_cost;
				previous[next_index] = curr_index;
			}
		}
	}

	cout << "Part 1: " << cost[coord_to_index(source)] << endl;

	auto min_index = coord_to_index(source);
	for (int32_t idx = 0; idx < cost.size(); ++idx) {
		auto coord = index_to_coord(idx);
		if (map[coord.y][coord.x] == 0 && cost[idx] < cost[min_index]) {
			min_index = idx;
		}
	}
	cout << "Part 2: " << cost[min_index] << endl;


	return 0;
}
