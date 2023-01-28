#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

struct Coord {
	int32_t		x;
	int32_t		y;
	int32_t		z;
};

int32_t key(int32_t x, int32_t y, int32_t z) {
	return x + (y << 8) + (z << 16);
}

int main() {

	vector<Coord>	cubes;

	// read input
	int32_t x, y, z;
	char d1, d2;

	while (cin >> x >> d1 >> y >> d2 >> z) {
		cubes.push_back(Coord{x*2, y*2, z*2});
	}

	// solution
	unordered_map<int32_t, int32_t>		map;
	for (auto const &cube : cubes) {
		map[key(cube.x - 1, cube.y, cube.z)] += 1;
		map[key(cube.x + 1, cube.y, cube.z)] += 1;
		map[key(cube.x, cube.y - 1, cube.z)] += 1;
		map[key(cube.x, cube.y + 1, cube.z)] += 1;
		map[key(cube.x, cube.y, cube.z - 1)] += 1;
		map[key(cube.x, cube.y, cube.z + 1)] += 1;
	}

	int32_t result = 0;
	for (auto const &[key, count] : map) {
		if (count == 1) {
			result += 1;
		}
	}

	cout << result << endl;


	return 0;
}
