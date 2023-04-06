#include <algorithm>
#include <array>
#include <iostream>
#include <optional>
#include <regex>
#include <unordered_map>
#include <vector>
#include <cstring>

using namespace std;

constexpr int32_t CUBE_SIZE = 50;

enum class Op {
	Forward,
	RotateLeft,
	RotateRight
};

namespace Heading {
	constexpr int32_t Right = 0;
	constexpr int32_t Down  = 1;
	constexpr int32_t Left  = 2;
	constexpr int32_t Up    = 3;
};

namespace Transit {
	constexpr int32_t Up = 0;
	constexpr int32_t Right = 1;
	constexpr int32_t Down = 2;
	constexpr int32_t Left = 3;
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

/* faces of the cube
    4
  3 0 1 2
    5
*/

constexpr static int32_t TRANSITS[6][4] = {
/* [0] = */	{ 4, 1, 5, 3 },
/* [1] = */	{ 4, 2, 5, 0 },
/* [2] = */	{ 4, 3, 5, 1 },
/* [3] = */	{ 4, 0, 5, 2 },
/* [4] = */	{ 2, 1, 0, 3 },
/* [5] = */	{ 0, 1, 2, 3 }
};

constexpr static int32_t TRANSIT_HEADING[4] = {
/* from top */		Heading::Down,
/* from right */	Heading::Left,
/* from bottom */	Heading::Up,
/* from left */		Heading::Right
};


constexpr Coord MOVEMENT_DELTA[4] = {
	{1, 0},
	{0, 1},
	{-1, 0},
	{0, -1}
};

vector<string>				g_map;
array<string, CUBE_SIZE>	g_cube_sides[6];
Coord						g_cube_offset[6];
int32_t						g_transits[6][4];
vector<Command>				g_commands;

bool is_valid_coordinate(Coord const& c) {

	if (c.m_x < 0 || c.m_y < 0) {
		return false;
	}

	if (c.m_y >= g_map.size() || c.m_x >= g_map[c.m_y].size()) {
		return false;
	}

	return g_map[c.m_y][c.m_x] != ' ';
}

bool parse_input() {

	string line;
	while(getline(cin, line)) {
		if (line.empty()) {
			break;
		}

		g_map.push_back(line);
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
				return false;
			}
		}
	}

	if (dist > 0) {
		g_commands.push_back({Op::Forward, dist});
		dist = 0;
	}

	return true;
}

void parse_cube_sides() {

	Coord c = {0, 0};
	static constexpr Coord NB_DELTA[4] = {
		{0, -CUBE_SIZE},
		{CUBE_SIZE, 0},
		{0, CUBE_SIZE},
		{-CUBE_SIZE, 0}
	};

	// find the start of the first side
	while (g_map[c.m_y][c.m_x] == ' ') {
		c.m_x += CUBE_SIZE;
		if (c.m_x >= g_map[c.m_y].size()) {
			c.m_x = 0;
			c.m_y += CUBE_SIZE;
		}
	}

	memcpy(g_transits[0], TRANSITS[0], 4 * sizeof(int32_t));

	vector<pair<int32_t, Coord>> todo = {make_pair(0, c)};

	while (!todo.empty()) {

		auto [idx, c] = todo.back();
		todo.pop_back();

		// copy the face data
		g_cube_offset[idx] = c;
		for (int32_t i = 0; i < CUBE_SIZE; ++i) {
			g_cube_sides[idx][i] = g_map[c.m_y+i].substr(c.m_x, CUBE_SIZE);
		}

		// check neighbors
		for (int32_t nb = 0; nb < 4; ++nb) {
			auto nb_c = c + NB_DELTA[nb];

			if (is_valid_coordinate(nb_c)) {
				int32_t nb_idx = g_transits[idx][nb];
				if (g_cube_sides[nb_idx][0].empty()) {
					int32_t anchor_idx = (nb + 2) % 4;

					memcpy(g_transits[nb_idx], TRANSITS[nb_idx], 4 * sizeof(int32_t));
					while (g_transits[nb_idx][anchor_idx] != idx) {
						rotate(begin(g_transits[nb_idx]), begin(g_transits[nb_idx]) + 1, end(g_transits[nb_idx]));
					}

					todo.push_back(make_pair(g_transits[idx][nb], nb_c));
				}
			}
		}
	}
}

int32_t index_of_transit(int32_t from_side, int32_t to_side) {
	for (int i = 0; i < 4; ++i) {
		if (g_transits[to_side][i] == from_side) {
			return i;
		}
	}

	cerr << "Wait, what?" << endl;
	return -1;
}

bool map_check_coordinate(int32_t side, Coord const &c) {
	return g_cube_sides[side][c.m_y][c.m_x] == '.';
}

int main() {

	// input
	if (!parse_input()) {
		return -1;
	}

	// extract sides of the cube
	parse_cube_sides();

	// solve
	int32_t side = 0;
	int32_t heading = Heading::Right;
	int32_t new_heading = Heading::Right;
	Coord pos = {0, 0};

	auto do_transit = [&](int32_t transit, Coord &new_pos) {
		int32_t from_side = side;
		int32_t new_side = g_transits[side][transit];
		int32_t target_transit = index_of_transit(from_side, new_side);
		new_heading = TRANSIT_HEADING[target_transit];

		int32_t diff = (target_transit - transit + 4) % 4;
		switch (diff) {
			case 0:
				if (new_pos.m_x >= 0 && new_pos.m_x < CUBE_SIZE) {
					new_pos.m_x = CUBE_SIZE - 1 - new_pos.m_x;
					new_pos.m_y = (new_heading == Heading::Down || new_heading == Heading::Right) ? 0 : CUBE_SIZE - 1;
				} else {
					new_pos.m_x = (new_heading == Heading::Down || new_heading == Heading::Right) ? 0 : CUBE_SIZE - 1;
					new_pos.m_y = CUBE_SIZE - 1 - new_pos.m_y;
				}
				break;
			case 1:
				if (new_pos.m_x >= 0 && new_pos.m_x < CUBE_SIZE) {
					new_pos.m_y = CUBE_SIZE - 1 - new_pos.m_x;
					new_pos.m_x = (heading == Heading::Down || new_heading == Heading::Right) ? 0 : CUBE_SIZE - 1;
				} else {
					new_pos.m_x = new_pos.m_y;
					new_pos.m_y = (new_heading == Heading::Down || new_heading == Heading::Right) ? 0 : CUBE_SIZE - 1;
				}
				break;
			case 2:
				if (new_pos.m_x >= 0 && new_pos.m_x < CUBE_SIZE) {
					new_pos.m_y = (new_heading == Heading::Down || new_heading == Heading::Right) ? 0 : CUBE_SIZE - 1;
				} else {
					new_pos.m_x = (new_heading == Heading::Down || new_heading == Heading::Right) ? 0 : CUBE_SIZE - 1;
				}
				break;
			case 3:
				if (new_pos.m_x >= 0 && new_pos.m_x < CUBE_SIZE) {
					new_pos.m_y = new_pos.m_x;
					new_pos.m_x = (new_heading == Heading::Down || new_heading == Heading::Right) ? 0 : CUBE_SIZE - 1;
				} else {
					new_pos.m_x = CUBE_SIZE - 1 - new_pos.m_y;
					new_pos.m_y = (new_heading == Heading::Down || new_heading == Heading::Right) ? 0 : CUBE_SIZE - 1;
				}
				break;
		}

		return new_side;
	};

	for (auto const &cmd : g_commands) {
		if (cmd.m_operation == Op::RotateRight) {
			heading = (heading + 1) % 4;
			new_heading = heading;
			continue;
		}

		if (cmd.m_operation == Op::RotateLeft) {
			heading = (heading - 1 + 4) % 4;
			new_heading = heading;
			continue;
		}

		for (int32_t i = 0; i < cmd.m_distance; ++i) {
			auto new_pos = pos + MOVEMENT_DELTA[heading];
			int32_t new_side = side;

			if (new_pos.m_x < 0) {
				new_side = do_transit(Transit::Left, new_pos);
			} else if (new_pos.m_x >= CUBE_SIZE) {
				new_side = do_transit(Transit::Right, new_pos);
			} else if (new_pos.m_y < 0) {
				new_side = do_transit(Transit::Up, new_pos);
			} else if (new_pos.m_y >= CUBE_SIZE) {
				new_side = do_transit(Transit::Down, new_pos);
			}

			if (map_check_coordinate(new_side, new_pos)) {
				side = new_side;
				pos = new_pos;
				heading = new_heading;
			} else {
				break;
			}

		}
	}

	cout << (1000 * (g_cube_offset[side].m_y + pos.m_y + 1)) + (4 * (g_cube_offset[side].m_x + pos.m_x + 1)) + heading << endl;

	return 0;
}
