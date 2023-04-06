#include <algorithm>
#include <iostream>
#include <optional>
#include <regex>
#include <unordered_map>
#include <vector>

using namespace std;

enum class Op {
	Forward,
	RotateLeft,
	RotateRight
};

struct Command {
	Op		m_operation;
	int32_t m_distance;
};

struct Coord {
	int32_t	m_x;
	int32_t m_y;
};

Coord operator+(Coord const &lhs, Coord const &rhs) {
	return {lhs.m_x + rhs.m_x, lhs.m_y + rhs.m_y};
}

constexpr Coord MOVEMENT_DELTA[4] = {
	{1, 0},
	{0, 1},
	{-1, 0},
	{0, -1}
};

vector<string>	g_map;
vector<int32_t>	g_x_min;
vector<int32_t>	g_y_min;
vector<int32_t>	g_y_max;
vector<Command>	g_commands;

bool map_check_coordinate(Coord const &c) {
	return g_map[c.m_y][c.m_x] == '.';
}

bool map_move(Coord &coord, Coord const &delta) {
	Coord new_coord = coord + delta;

	if (delta.m_x != 0) {
		if (new_coord.m_x < g_x_min[new_coord.m_y]) {
			new_coord.m_x = g_map[new_coord.m_y].size() - 1;
		} else if (new_coord.m_x >= g_map[new_coord.m_y].size()) {
			new_coord.m_x = g_x_min[new_coord.m_y];
		}
	}

	if (delta.m_y != 0) {
		if (new_coord.m_y < g_y_min[new_coord.m_x]) {
			new_coord.m_y = g_y_max[new_coord.m_x];
		} else if (new_coord.m_y > g_y_max[new_coord.m_x]) {
			new_coord.m_y = g_y_min[new_coord.m_x];
		}
	}

	if (map_check_coordinate(new_coord)) {
		coord = new_coord;
		return true;
	} else {
		return false;
	}
}

int main() {

	// input
	string line;
	while(getline(cin, line)) {
		if (line.empty()) {
			break;
		}

		int32_t y = g_map.size();
		g_map.push_back(line);
		g_x_min.push_back(line.find_first_not_of(' '));

		g_y_min.resize(line.size(), 100000);
		g_y_max.resize(line.size(), -1);

		for (int32_t i = 0; i < line.size(); ++i) {
			if (line[i] != ' ') {
				g_y_min[i] = min(g_y_min[i], y);
				g_y_max[i] = max(g_y_max[i], y);
			}
		}
	}

	getline(cin, line);

	int32_t dist = 0;
	for (auto c : line) {
		if (isdigit(c)) {
			dist = (dist * 10) + (c - '0');
		} else {
			if (dist > 0) {
				g_commands.push_back({Op::Forward, dist});
				dist = 0;
			}

			if (c == 'L') {
				g_commands.push_back({Op::RotateLeft, 0});
			} else if (c == 'R') {
				g_commands.push_back({Op::RotateRight, 0});
			} else {
				cerr << "What?" << endl;
				return -1;
			}
		}
	}

	if (dist > 0) {
		g_commands.push_back({Op::Forward, dist});
		dist = 0;
	}

	// solve
	Coord pos {g_x_min.front(), 0};
	int32_t heading = 0;

	for (auto const &cmd : g_commands) {

		if (cmd.m_operation == Op::RotateRight) {
			heading = (heading + 1) % 4;
			continue;
		}

		if (cmd.m_operation == Op::RotateLeft) {
			heading = (heading - 1 + 4) % 4;
			continue;
		}

		for (int32_t i = 0; i < cmd.m_distance; ++i) {
			if (!map_move(pos, MOVEMENT_DELTA[heading])) {
				break;
			}
		}
	}

	cout << (1000 * (pos.m_y + 1)) + (4 * (pos.m_x + 1)) + heading << endl;
	return 0;
}
