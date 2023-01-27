#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <string_view>
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
	int64_t top_position() const {
		for (int64_t top = m_lines.size() - 1; top >= 0; top -= 1) {
			if (std::ranges::count(m_lines[top], true) > 0) {
				return top;
			}
		}

		return -1;
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

private:
	vector<array<bool, WORLD_WIDTH>>	m_lines;
};

int64_t run_simulation(std::string const &jet_pattern, int64_t cycles) {

	int64_t jet_index = 0;
	int64_t block_index = 0;

	// drop blocks
	World world;

	for (int64_t i = 0; i < cycles; ++i) {

		// start position of the block
		int64_t x = 2;
		int64_t y = world.top_position() + 4;

		// make sure the block fits in the world
		world.expand_world(y + BLOCK_SIZE);

		// drop the block
		while (true) {

			// lateral movement
			int64_t new_x = x + ((jet_pattern[jet_index] == '<') ? -1 : 1);
			jet_index = (jet_index + 1) % jet_pattern.size();
			if (!world.overlap(blocks[block_index], new_x, y)) {
				x = new_x;
			}

			// downward movement
			int64_t new_y = y - 1;
			if (!world.overlap(blocks[block_index], x, new_y)) {
				y = new_y;
			} else {
				break;
			}
		}

		world.place_block(blocks[block_index], x, y);
		block_index = (block_index + 1) % BLOCK_COUNT;
	}

	return world.top_position() + 1;
}

int main() {

	// read input
	string jet_pattern = "";
	getline(cin, jet_pattern);

	cout << "Pattern Length = " << jet_pattern.size() << endl;
	cout << "Part 1: " << run_simulation(jet_pattern, 2022) << endl;

	return 0;
}
