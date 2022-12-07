#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <string_view>
#include <vector>

using namespace std;

struct Node {
	string 			name = "";
	bool 			is_dir = false;
	vector<int>		children;
	int 			parent = 0;
	size_t 			size = 0;
};

vector<Node> dirtree = {Node{"/", true, {}, 0, 0}};
int current_dir = 0;

void print_node(int node, int depth) {

	cout << string(depth, '-') << " ";

	if (dirtree[node].is_dir) {
		cout << "directory: " << dirtree[node].name << " [" << dirtree[node].size << "]\n";
		for (auto sub: dirtree[node].children) {
			print_node(sub, depth + 1);
		}
	} else {
		cout << dirtree[node].name << " [" << dirtree[node].size << "]\n";
	}
}

int calculate_directory_sizes(int node) {

	if (dirtree[node].is_dir) {
		dirtree[node].size = 0;
		for (auto sub: dirtree[node].children) {
			dirtree[node].size += calculate_directory_sizes(sub);
		}
	}

	return dirtree[node].size;
}

void process_command(string_view cmd) {

	if (cmd.substr(2, 2) == "cd") {
		auto subdir_name = cmd.substr(5);
		auto &dir = dirtree[current_dir];

		if (subdir_name == "/") {
			current_dir = 0;
		} else if (subdir_name == "..") {
			current_dir = dir.parent;
		} else {
			auto subdir = find_if(begin(dir.children), end(dir.children), [=](auto const &e) { return dirtree[e].name == subdir_name;});
			if (subdir == end(dir.children)) {
				cerr << "Unknown subdirectory" << cmd << endl;
				return;
			}

			current_dir = *subdir;
		}
	} else if (cmd.substr(2, 2) == "ls") {

	} else {
		cerr << "Unknown command in line: " << cmd << endl;
	}
}

void process_listing_line(string_view line) {
	if (line.starts_with("dir")) {
		dirtree[current_dir].children.push_back(dirtree.size());
		auto &node = dirtree.emplace_back();
		node.name = line.substr(4);
		node.is_dir = true;
		node.parent = current_dir;
		node.size = 0;
	} else {
		auto split = line.find(' ');
		dirtree[current_dir].children.push_back(dirtree.size());
		auto &node = dirtree.emplace_back();
		node.name = line.substr(split + 1);
		node.is_dir = false;
		node.parent = current_dir;
		node.size = atoi(line.substr(0, split).data());
	}
}

int main() {

	string input;

	while (getline(cin, input)) {
		if (input.empty()) {
			continue;
		}

		if (input[0] == '$') {
			process_command(input);
		} else {
			process_listing_line(input);
		}
	}

	calculate_directory_sizes(0);

	auto sum = accumulate(begin(dirtree), end(dirtree), 0, [] (int total, Node const &node) {
		return (node.is_dir && node.size < 100000) ? total += node.size : total;
	});

	cout << "Part 1: " << sum << endl;

	int required = 30000000 - (70000000 - dirtree[0].size);
	int smallest = 70000000;

	for (auto const& dir : dirtree) {
		if (dir.is_dir && dir.size >= required && dir.size < smallest) {
			smallest = dir.size;
		}
	}

	cout << "Part 2: " << smallest << endl;
	return 0;
}
