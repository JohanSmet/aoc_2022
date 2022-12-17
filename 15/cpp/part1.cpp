#include <algorithm>
#include <iostream>
#include <numeric>
#include <regex>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

using namespace std;

struct Coord {
	int32_t x;
	int32_t y;
};

struct Line {
	int32_t x0;
	int32_t x1;
};

struct SensorData {
	Coord	sensor;
	Coord	beacon;
	int32_t distance;
};

bool segments_overlap(Line const &l1, Line const &l2) {
	return (l1.x0 <= l2.x1) && (l1.x1 >= l2.x0);
}

void segments_merge(Line &l, Line const &m) {
	l.x0 = min(l.x0, m.x0);
	l.x1 = max(l.x1, m.x1);
}

bool merge_or_insert(vector<Line> &segments, int32_t x0, int32_t x1) {
	Line l {min(x0, x1), max(x0, x1)};

	for (auto &s : segments) {
		if (segments_overlap(s, l)) {
			segments_merge(s, l);
			return true;
		}
	}

	segments.push_back(l);
	return false;
}

constexpr static int32_t TARGET = 2000000;

int main() {

	vector<SensorData>	data;

	// input
	regex r_data("Sensor at x=([+-]*[[:digit:]]+), y=([+-]*[[:digit:]]+): closest beacon is at x=([+-]*[[:digit:]]+), y=([+-]*[[:digit:]]+)");

	string line;
	while (getline(cin, line)) {
		smatch	sm;

		if (regex_match(line, sm, r_data)) {
			auto &sd = data.emplace_back();
			sd.sensor.x = stoi(sm[1]);
			sd.sensor.y = stoi(sm[2]);
			sd.beacon.x = stoi(sm[3]);
			sd.beacon.y = stoi(sm[4]);
			sd.distance = abs(sd.beacon.x - sd.sensor.x) + abs(sd.beacon.y - sd.sensor.y);
		} else {
			cerr << "Oepsie" << endl;
		}
	}

	// process
	vector<Line> segments;
	unordered_set<int32_t> beacons;

	for (auto const &sb : data) {
		if (sb.beacon.y == TARGET) {
			beacons.insert(sb.beacon.x);
		}

		int32_t delta = abs(sb.sensor.y - TARGET);

		if (delta <= sb.distance) {
			merge_or_insert(segments, sb.sensor.x - (sb.distance - delta), sb.sensor.x + (sb.distance - delta));
		}
	}

	bool keep_merging = true;
	while (keep_merging) {

		vector<Line> merged;
		keep_merging = false;

		for (auto const &l : segments) {
			if (merge_or_insert(merged, l.x0, l.x1)) {
				keep_merging = true;
			}
		}

		segments.clear();
		swap(merged, segments);
	}

	auto count = std::accumulate(begin(segments), end(segments), 0, [](int32_t sum, Line const &l) {return sum + (l.x1 - l.x0 + 1);});
	cout << count - beacons.size() << endl;

	return 0;
}
