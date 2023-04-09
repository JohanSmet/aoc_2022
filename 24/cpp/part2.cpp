#include <algorithm>
#include <iostream>
#include <optional>
#include <regex>
#include <unordered_map>
#include <vector>

using namespace std;

struct Coord {
	int32_t	m_x;
	int32_t m_y;
};

inline Coord operator+(Coord const &lhs, Coord const &rhs) {
	return {lhs.m_x + rhs.m_x, lhs.m_y + rhs.m_y};
}

inline bool operator==(Coord const &lhs, Coord const &rhs) {
	return lhs.m_x == rhs.m_x && lhs.m_y == rhs.m_y;
}

constexpr Coord MOVEMENT_DELTA[4] = {
	{1, 0},
	{0, 1},
	{-1, 0},
	{0, -1}
};

struct Blizzard {
	Coord	m_position;
	int32_t m_delta;
};

Coord				g_source{};
Coord				g_exit{};
vector<Blizzard>	g_blizzards;

struct Map {
	Map(int32_t w, int32_t h) : m_width(w), m_height(h), m_map(w * h, false) {};

	int8_t &at(int32_t x, int32_t y) {
		return m_map[(y * m_width) + x];
	}

	bool valid_position(int32_t x, int32_t y) const {
		return x >= 0 && x < m_width &&
			   y >= 0 && y < m_height;
	}

	void clear() {
		fill(begin(m_map), end(m_map), false);
	}

	void build() {
		// do walls
		for (int32_t x = 0; x < m_width; ++x) {
			at(x, 0) = 1;
			at(x, m_height - 1) = 1;
		}

		for (int32_t y = 0; y < m_height; ++y) {
			at(0, y) = 1;
			at(m_width - 1, y) = 1;
		}

		// do source & target
		at(g_source.m_x, g_source.m_y) = 0;
		at(g_exit.m_x, g_exit.m_y) = 0;

		// do blizzards
		for (auto const &blizzard: g_blizzards) {
			at(blizzard.m_position.m_x, blizzard.m_position.m_y) = 3;
		}
	}

	void print() {
		int32_t i = 0;
		for (int32_t y = 0; y < m_height; ++y) {
			for (int32_t x = 0; x < m_width; ++x) {
				char c = '.';
				if (m_map[i] == 1) {
					c = '#';
				} else if (m_map[i] == 2) {
					c = 'p';
				} else if (m_map[i] == 3) {
					c = 'B';
				} else if (m_map[i] > 0) {
					c = x;
				}
				i += 1;
				cout << c;
			}
			cout << endl;
		}

	}

public:
	int32_t			m_width;
	int32_t			m_height;
	vector<int8_t>	m_map;
};

void move_blizzards(int32_t map_w, int32_t map_h) {

	for (auto &blizzard : g_blizzards) {
		blizzard.m_position = blizzard.m_position + MOVEMENT_DELTA[blizzard.m_delta];

		if (blizzard.m_position.m_x <= 0) {
			blizzard.m_position.m_x = map_w - 2;
		} else if (blizzard.m_position.m_x >= map_w - 1) {
			blizzard.m_position.m_x = 1;
		} else if (blizzard.m_position.m_y <= 0) {
			blizzard.m_position.m_y = map_h - 2;
		} else if (blizzard.m_position.m_y >= map_h - 1) {
			blizzard.m_position.m_y = 1;
		}
	}
}

int32_t solve(Map &map, Coord const &from, Coord const &target) {

	vector<Coord>	current_state;
	vector<Coord>	new_state;
	int32_t			steps = 0;
	bool			reached_exit = false;

	current_state.push_back(from);

	while (!reached_exit && !current_state.empty()) {

		steps += 1;

		// move blizzards
		move_blizzards(map.m_width, map.m_height);

		// recreate the map
		map.clear();
		map.build();

		// do all potential moves
		while (!current_state.empty() && !reached_exit) {

			auto pos = current_state.back();
			current_state.pop_back();

			if (map.at(pos.m_x, pos.m_y) == 0) {
				new_state.push_back(pos);
				map.at(pos.m_x, pos.m_y) = 2;
			}

			for (int32_t i = 0; i < 4; ++i) {
				auto next_pos = pos + MOVEMENT_DELTA[i];
				if (!map.valid_position(next_pos.m_x, next_pos.m_y)) {
					continue;
				}

				if (next_pos == target) {
					reached_exit = true;
					break;
				}

				if (map.at(next_pos.m_x, next_pos.m_y) == 0) {
					new_state.push_back(next_pos);
					map.at(next_pos.m_x, next_pos.m_y) = 2;
				}
			}
		}

		swap(current_state, new_state);
		new_state.clear();
	}

	return steps;
}

int main() {

	// input
	string line;

	// >> first line is only wall and start position
	getline(cin, line);
	g_source.m_x = line.find('.');
	g_source.m_y = 0;

	const int32_t map_width = line.size();
	int32_t map_y = 1;

	while(getline(cin, line)) {

		// check if last line
		if (line[1] == '#') {
			g_exit.m_x = line.find('.');
			g_exit.m_y = map_y;
			map_y += 1;
			break;
		}

		for (int32_t x = 1; x < line.size() - 1; ++x) {
			if (line[x] == '.') {
				continue;
			}

			auto &blizzard = g_blizzards.emplace_back();
			blizzard.m_position.m_x = x;
			blizzard.m_position.m_y = map_y;

			if (line[x] == '^') {
				blizzard.m_delta = 3;
			} else if (line[x] == 'v') {
				blizzard.m_delta = 1;
			} else if (line[x] == '<') {
				blizzard.m_delta = 2;
			} else {
				blizzard.m_delta = 0;
			}
		}

		map_y += 1;
	}

	// solve
	Map map{map_width, map_y};

	int32_t trip_1 = solve(map, g_source, g_exit);
	cout << trip_1 << "\n";

	int32_t trip_2 = solve(map, g_exit, g_source);
	cout << trip_2 << "\n";

	int32_t trip_3 = solve(map, g_source, g_exit);
	cout << trip_3 << "\n";

	cout << "Total = " << trip_1 + trip_2 + trip_3 << endl;

	return 0;
}
