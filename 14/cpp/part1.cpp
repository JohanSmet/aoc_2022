#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

using namespace std;

struct Coord {
	int32_t x;
	int32_t y;
};

using Path = vector<Coord>;

class Grid {
public:
	Grid(int32_t x, int32_t y, int32_t w, int32_t h) : data(w *h, '.'), origin({x,y}), size({w,h}) {}

	char GetCell(int32_t x, int32_t y) const {
		return data[index(x,y)];
	}

	void SetCell(int32_t x, int32_t y, char v) {
		data[index(x,y)] = v;
	}

	bool IsValidCell(int32_t x, int32_t y) const {
		return	(x > origin.x) &&
				(x < origin.x + size.x) &&
				(y > origin.y) &&
				(y < origin.y + size.y);
	}

	void DrawLine(Coord const &c0, Coord const &c1) {
		if (c0.x == c1.x) {
			DrawLineVertical(c0.x, c0.y, c1.y);
		} else {
			DrawLineHorizontal(c0.x, c1.x, c0.y);
		}
	}

	void DrawLineHorizontal(int x0, int x1, int y) {
		if (x0 > x1) swap(x0, x1);
		for (int32_t x = x0; x <= x1; ++x) {
			SetCell(x, y, '#');
		}
	}

	void DrawLineVertical(int x, int y0, int y1) {
		if (y0 > y1) swap(y0, y1);
		for (int32_t y = y0; y <= y1; ++y) {
			SetCell(x, y, '#');
		}
	}

	void Print() const {
		for (int32_t y = 0; y < size.y; ++y) {
			for (int32_t x = 0; x < size.x; ++x) {
				cout << data[y * size.x + x];
			}
			cout << endl;
		}
	}

	bool DropSand() {
		Coord pos{500, 0};

		while (true) {
			// down
			if (!IsValidCell(pos.x, pos.y + 1)) {
				return false;
			}

			if (GetCell(pos.x, pos.y + 1) == '.') {
				pos.y += 1;
				continue;
			}

			// down left
			if (!IsValidCell(pos.x - 1, pos.y + 1)) {
				return false;
			}

			if (GetCell(pos.x - 1, pos.y + 1) == '.') {
				pos.x -= 1;
				pos.y += 1;
				continue;
			}

			// down right
			if (!IsValidCell(pos.x + 1, pos.y + 1)) {
				return false;
			}

			if (GetCell(pos.x + 1, pos.y + 1) == '.') {
				pos.x += 1;
				pos.y += 1;
				continue;
			} else {
				break;
			}
		}

		SetCell(pos.x, pos.y, 'o');

		return true;
	}

private:
	inline int32_t index(int32_t x, int32_t y) const {
		return ((y - origin.y) * size.x) + (x - origin.x);
	}

private:
	vector<char>	data;
	Coord			origin;
	Coord			size;
};


Path parse_path_line(string const &line) {

	Path result;
	string dummy;
	char c;
	Coord coord;

	istringstream is(line);
	do {
		is >> coord.x >> c >> coord.y;
		result.push_back(coord);
	} while (is >> dummy);


	return result;
}


int main() {

	vector<Path> cave;

	Coord min_c = {1000, 0};
	Coord max_c = {0};

	// read input
	string line;
	while (getline(cin, line)) {
		cave.push_back(parse_path_line(line));

		for (auto const &c : cave.back()) {
			min_c.x = min(min_c.x, c.x);
			min_c.y = min(min_c.y, c.y);
			max_c.x = max(max_c.x, c.x);
			max_c.y = max(max_c.y, c.y);
		}
	}

	// initialize the grid
	Grid grid(min_c.x - 1, min_c.y - 1, max_c.x - min_c.x + 3, max_c.y - min_c.y + 3);

	// fill in the paths
	for (auto const &path : cave) {
		for (int32_t i = 1; i < path.size(); ++i) {
			grid.DrawLine(path[i-1], path[i]);
		}
	}

	// drop the sand
	int count = 0;
	while (grid.DropSand()) {
		++count;
	}

	cout << count << endl;

	return 0;
}
