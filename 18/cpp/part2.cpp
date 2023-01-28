#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

static constexpr int32_t X = 0;
static constexpr int32_t Y = 1;
static constexpr int32_t Z = 2;

struct Coord {
	int32_t v[3];

	int32_t &operator[](int32_t i) { return v[i]; }
	int32_t operator[](int32_t i) const { return v[i]; }
};

Coord operator+(Coord const &a, Coord const &b) {
	return {
		a[X] + b[X],
		a[Y] + b[Y],
		a[Z] + b[Z]
	};
}

struct World {
	int32_t			size[3];
	vector<int8_t>	values;

	void init(Coord const &min_coords, Coord const &max_coords) {
		size[0] = max_coords[0] + 2;
		size[1] = max_coords[1] + 2;
		size[2] = max_coords[2] + 2;
		values.resize(size[0] * size[1] * size[2], 0);
	}

	int8_t &value_at(Coord const &c) {
		int32_t offset = (c[Z] * size[1] * size[0]) + (c[Y] * size[0]) + c[X];
		assert(offset < values.size());
		return values[offset];
	}

	bool coord_valid(Coord const &coord) {
		for (int i = 0; i < 3; ++i) {
			if (coord[i] < 0 || coord[i] >= size[i]) {
				return false;
			}
		}

		return true;
	}

	int32_t floodfill(Coord const &coord) {
		static constexpr Coord DELTA[] = {
			{1, 0, 0}, {-1, 0, 0},
			{0, 1, 0}, {0, -1, 0},
			{0, 0, 1}, {0, 0, -1}
		};

		vector<Coord> todo;
		todo.push_back(coord);
		int32_t outside_edges = 0;

		while (!todo.empty()) {
			auto c = todo.back();
			todo.pop_back();

			value_at(c) = 2;

			for (auto const &delta : DELTA) {
				Coord next = c + delta;
				if (!coord_valid(next)) {
					continue;
				}
				auto next_value = value_at(next);

				if (next_value == 1) {
					outside_edges += 1;
				} else if (next_value == 0) {
					value_at(next) = 3;
					todo.push_back(next);
				}
			}
		}

		return outside_edges;
	}

};

static constexpr int32_t OFFSET = 1;

int main() {

	vector<Coord>	cubes;

	// read input
	Coord c;
	char d1, d2;

	Coord min_coords = {INT32_MAX, INT32_MAX, INT32_MAX};
	Coord max_coords = {INT32_MIN, INT32_MIN, INT32_MIN};

	while (cin >> c[X] >> d1 >> c[Y] >> d2 >> c[Z]) {
		cubes.push_back(Coord{c[X] + OFFSET, c[Y] + OFFSET, c[Z] + OFFSET});
		for (int i = 0; i < 3; ++i) {
			min_coords[i] = min(min_coords[i], c[i] + OFFSET);
			max_coords[i] = max(max_coords[i], c[i] + OFFSET);
		}
	}

	World world;
	world.init(min_coords, max_coords);

	for (auto const &c : cubes) {
		world.value_at(c) = 1;
	}

	// flood fill from the outside and count outside edges
	auto result = world.floodfill({0, 0, 0});
	cout << result << endl;
	return 0;
}
