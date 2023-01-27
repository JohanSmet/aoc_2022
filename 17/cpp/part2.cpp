#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

using namespace std;

constexpr int32_t BLOCK_SIZE = 4;
constexpr int32_t BLOCK_COUNT = 5;
constexpr int32_t WORLD_WIDTH = 7;

struct Coord {
	int32_t		x;
	int32_t		y;
};

using Block = std::vector<Coord>;

static const Block blocks[BLOCK_COUNT] = {
	{ {0, 0}, {1, 0}, {2, 0}, {3, 0} },
	{ {1, 0}, {0, 1}, {1, 1}, {2, 1}, {1, 2} },
	{ {0, 0}, {1, 0}, {2, 0}, {2, 1}, {2, 2} },
	{ {0, 0}, {0, 1}, {0, 2}, {0, 3} },
	{ {0, 0}, {1, 0}, {0, 1}, {1, 1} },
};

class World {
public:
	std::string &jet_pattern() {return m_jet_pattern;}

	int64_t top_position() const {
		for (int64_t top = m_lines.size() - 1; top >= 0; top -= 1) {
			if (std::ranges::count(m_lines[top], true) > 0) {
				return top;
			}
		}

		return -1;
	}

	int64_t skipped() const {
		return m_skipped;
	}

	void expand_world(int64_t max_y) {
		if (max_y >= m_lines.size()) {
			m_lines.resize(max_y + 1, {});
		}
	}

	bool overlap(Block const &block, int64_t x, int64_t y) {
		if (x < 0 || y < 0) {
			return true;
		}

		for (auto const &c : block) {
			if (x + c.x >= WORLD_WIDTH || m_lines[y + c.y][x + c.x]) {
				return true;
			}
		}

		return false;
	}

	void place_block(Block const &block, int64_t x, int64_t y) {
		for (auto const &c : block) {
			m_lines[y + c.y][x + c.x] = true;
		}
	}

	void simulate_block() {

		// start position of the block
		int64_t x = 2;
		int64_t y = top_position() + 4;

		// make sure the block fits in the world
		expand_world(y + BLOCK_SIZE);

		// drop the block
		while (true) {

			// lateral movement
			int64_t new_x = x + ((m_jet_pattern[m_jet_index] == '<') ? -1 : 1);
			m_jet_index = (m_jet_index + 1) % m_jet_pattern.size();
			if (!overlap(blocks[m_block_index], new_x, y)) {
				x = new_x;
			}

			// downward movement
			int64_t new_y = y - 1;
			if (!overlap(blocks[m_block_index], x, new_y)) {
				y = new_y;
			} else {
				break;
			}
		}

		place_block(blocks[m_block_index], x, y);
		m_block_index = (m_block_index + 1) % BLOCK_COUNT;
	}

	void print() {
		for (int64_t top = m_lines.size() - 1; top >= 0; top -= 1) {
			cout << "|";
			for (auto filled : m_lines[top]) {
				cout << (filled ? '#' : '.');
			}
			cout << "|\n";
		}
		cout << "+-------+\n";
	}

	int64_t check_cache(int64_t cycle, int64_t max_cycles) {
		if (m_skipped > 0) {
			return 0;
		}

		auto key = cache_key(m_block_index, m_jet_index);
		auto found = m_cache.find(key);

		if (found == end(m_cache)) {
			auto &entry = m_cache[key];
			entry.prev_cycle = cycle;
			entry.prev_top = top_position();
			entry.prev_delta = 0;
			return 0;
		}

		int64_t top = top_position();
		int64_t delta = top - found->second.prev_top;
		if (delta == found->second.prev_delta) {
			int64_t delta_cycles = cycle - found->second.prev_cycle;
			int64_t num_times = (max_cycles - cycle) / delta_cycles;
			m_skipped = delta * num_times;
			return delta_cycles * num_times;
		}

		found->second.prev_cycle = cycle;
		found->second.prev_delta = delta;
		found->second.prev_top = top;
		return 0;
	}

private:
	uint64_t cache_key(int32_t block, int32_t jet) const {
		return uint64_t(block) << 32 | jet;
	}

	struct cache_entry_t {
		int64_t		prev_cycle;
		int64_t		prev_top;
		int64_t		prev_delta;
	};

private:
	vector<array<bool, WORLD_WIDTH>>	m_lines;
	std::string							m_jet_pattern = "";
	int32_t								m_jet_index = 0;
	int32_t								m_block_index = 0;
	int64_t								m_skipped = 0;

	unordered_map<uint64_t, cache_entry_t>	m_cache;
};

int main() {

	World world;

	// read input
	getline(cin, world.jet_pattern());

	constexpr int64_t MAX_CYCLES = 1000000000000ll;

	for (int64_t cycles = 0; cycles < MAX_CYCLES; ++cycles) {
		world.simulate_block();
		cycles += world.check_cache(cycles, MAX_CYCLES);
	}

	cout << world.top_position() + world.skipped() + 1 << endl;

	return 0;
}
