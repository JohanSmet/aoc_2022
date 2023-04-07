#include <algorithm>
#include <iostream>
#include <optional>
#include <regex>
#include <unordered_map>
#include <vector>

using namespace std;

union Coord {
	struct {
		int16_t	m_x;
		int16_t m_y;
	};
	int32_t		m_key;
};

Coord operator+(Coord const &lhs, Coord const &rhs) {
	Coord result;
	result.m_x = lhs.m_x + rhs.m_x;
	result.m_y = lhs.m_y + rhs.m_y;
	return result;
}

struct Elf {
	Coord	m_position;
	Coord	m_next_position;
};

struct Proposal {
	Coord	m_checks[3];
	Coord	m_delta;
};

static constexpr Coord N  = { 0, -1};
static constexpr Coord NE = { 1, -1};
static constexpr Coord E  = { 1,  0};
static constexpr Coord SE = { 1,  1};
static constexpr Coord S  = { 0,  1};
static constexpr Coord SW = {-1,  1};
static constexpr Coord W  = {-1,  0};
static constexpr Coord NW = {-1, -1};

static constexpr Proposal proposals[4] = {
	{{N, NE, NW}, {N}},
	{{S, SE, SW}, {S}},
	{{W, NW, SW}, {W}},
	{{E, NE, SE}, {E}}
};

vector<Elf>	g_elfs;
unordered_map<int32_t, int32_t>	g_map;
unordered_map<int32_t, int32_t> g_potential_map;

void build_map() {
	g_map.clear();
	for (auto const &elf : g_elfs) {
		g_map[elf.m_position.m_key] = 1;
	}
}

bool elf_at_position(Coord const &pos, Coord const &delta) {
	auto check = pos + delta;
	return g_map.find(check.m_key) != end(g_map);
}

int32_t count_empty() {
	auto min_c = g_elfs.front().m_position;
	auto max_c = min_c;

	for (auto &elf : g_elfs) {
		min_c.m_x = min(min_c.m_x, elf.m_position.m_x);
		min_c.m_y = min(min_c.m_y, elf.m_position.m_y);
		max_c.m_x = max(max_c.m_x, elf.m_position.m_x);
		max_c.m_y = max(max_c.m_y, elf.m_position.m_y);
	}

	int32_t empty_tiles = (max_c.m_x - min_c.m_y) * (max_c.m_y - min_c.m_y);

	for (auto &elf : g_elfs) {
		if (elf.m_position.m_x >= min_c.m_x && elf.m_position.m_x <= max_c.m_x &&
		    elf.m_position.m_y >= min_c.m_y && elf.m_position.m_y <= max_c.m_y) {
			empty_tiles -= 1;
		}
	}

	return empty_tiles;
}

int main() {

	// input
	string line;
	int32_t y = 0;
	while (getline(cin, line)) {
		for (int32_t x = 0; x < line.size(); ++x) {
			if (line[x] == '#') {
				auto &elf = g_elfs.emplace_back();
				elf.m_position.m_x = x;
				elf.m_position.m_y = y;
			}
		}

		y += 1;
	}

	// process
	bool sw_continue = true;
	int32_t proposal_start = 0;

	auto round = 0;

	while (sw_continue) {
		g_potential_map.clear();

		round += 1;
		if (round == 10) {
			cout << "Empty tiles at round 10: " << count_empty() << endl;
		}

		// create map with current positions
		build_map();

		// determine proposals
		for (auto &elf : g_elfs) {
			elf.m_next_position = elf.m_position;

			bool do_move =  elf_at_position(elf.m_position, N) ||
							elf_at_position(elf.m_position, NE) ||
							elf_at_position(elf.m_position, E) ||
							elf_at_position(elf.m_position, SE) ||
							elf_at_position(elf.m_position, S) ||
							elf_at_position(elf.m_position, SW) ||
							elf_at_position(elf.m_position, W) ||
							elf_at_position(elf.m_position, NW);

			if (do_move) {
				for (int32_t i = 0; i < 4; ++i) {
					auto const &proposal = proposals[(proposal_start + i) % 4];

					if (!elf_at_position(elf.m_position, proposal.m_checks[0]) &&
						!elf_at_position(elf.m_position, proposal.m_checks[1]) &&
						!elf_at_position(elf.m_position, proposal.m_checks[2])) {
						elf.m_next_position = elf.m_position + proposal.m_delta;
						break;
					}
				}
			}

			g_potential_map[elf.m_next_position.m_key] += 1;
		}

		// move elfs
		sw_continue = false;

		for (auto &elf : g_elfs) {
			if (elf.m_next_position.m_key != elf.m_position.m_key && g_potential_map[elf.m_next_position.m_key] <= 1) {
				elf.m_position = elf.m_next_position;
				sw_continue = true;
			}
		}

		proposal_start = (proposal_start + 1) % 4;
	}

	// print result
	cout << "End at round " << round << endl;

	return 0;
}
