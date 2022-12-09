#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

struct Vec2 {
	int32_t x;
	int32_t	y;
};

void process_knot(Vec2 const &prev, Vec2 &knot) {
	int32_t delta_x = prev.x - knot.x;
	int32_t delta_y = prev.y - knot.y;

	if (prev.y == knot.y && abs(delta_x) == 2) {
		knot.x += delta_x / 2;
	} else if (prev.x == knot.x && abs(delta_y) == 2) {
		knot.y += delta_y / 2;
	} else if (abs(delta_x) > 1 || abs(delta_y) > 1) {
		knot.x += delta_x / abs(delta_x);
		knot.y += delta_y / abs(delta_y);
	}
}

int main() {

	array<Vec2, 10> rope;
	fill(begin(rope), end(rope), Vec2{0,0});

	unordered_set<int64_t>	tail_positions;
	auto save_pos = [&](Vec2 const& v) {
		tail_positions.insert((static_cast<int64_t>(v.x) << 32) + v.y);
	};

	char direction;
	int count;

	save_pos(rope.back());

	while (cin >> direction >> count) {

		for (int i = 0; i < count; ++i) {

			// move head
			switch (direction) {
				case 'R':
					rope.front().x += 1;
					break;
				case 'L':
					rope.front().x -= 1;
					break;
				case 'U':
					rope.front().y -= 1;
					break;
				case 'D':
					rope.front().y += 1;
					break;
			}

			// move knots
			for (int32_t i = 1; i < rope.size(); ++i) {
				process_knot(rope[i-1], rope[i]);
			}

			save_pos(rope.back());
		}
	}

	cout << tail_positions.size() << endl;

	return 0;
}
