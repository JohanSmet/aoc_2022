#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

struct Vec2 {
	int32_t x;
	int32_t	y;
};

int main() {

	Vec2 head = {0, 0};
	Vec2 tail = {0, 0};

	unordered_set<int64_t>	tail_positions;
	auto save_pos = [&](Vec2 const& v) {
		tail_positions.insert((static_cast<int64_t>(v.x) << 32) + v.y);
	};

	char direction;
	int count;

	save_pos(tail);

	while (cin >> direction >> count) {

		for (int i = 0; i < count; ++i) {

			// move head
			switch (direction) {
				case 'R':
					head.x += 1;
					break;
				case 'L':
					head.x -= 1;
					break;
				case 'U':
					head.y -= 1;
					break;
				case 'D':
					head.y += 1;
					break;
			}

			// move tail
			int32_t delta_x = head.x - tail.x;
			int32_t delta_y = head.y - tail.y;

			if (head.y == tail.y && abs(delta_x) == 2) {
				tail.x += delta_x / 2;
			} else if (head.x == tail.x && abs(delta_y) == 2) {
				tail.y += delta_y / 2;
			} else if (abs(delta_x) > 1 || abs(delta_y) > 1) {
				tail.x += delta_x / abs(delta_x);
				tail.y += delta_y / abs(delta_y);
			}

			save_pos(tail);
		}
	}

	cout << tail_positions.size() << endl;

	return 0;
}
