#include <algorithm>
#include <iostream>
#include <optional>
#include <regex>
#include <unordered_map>
#include <vector>

using namespace std;

struct Monkey {
	char				m_operator;
	int32_t				m_operand_1;
	int32_t				m_operand_2;
	optional<int64_t>	m_value;
};

struct Node {
	char				m_operator;
	int64_t				m_value;
	bool				m_leads_to_humn;
	unique_ptr<Node>	m_left_child;
	unique_ptr<Node>	m_right_child;
};

static std::vector<Monkey>	monkeys;
static std::unordered_map<std::string, int32_t> monkey_map;
static int32_t humn_node = 0;

int32_t monkey_index(std::string const &name) {
	auto found = monkey_map.find(name);
	if (found != end(monkey_map)) {
		return found->second;
	}

	monkeys.push_back(Monkey{' ', 0, 0, {}});
	monkey_map[name] = monkeys.size() - 1;
	return monkeys.size() - 1;
}

unique_ptr<Node> solve(int32_t index) {

	auto node = make_unique<Node>();

	if (monkeys[index].m_value.has_value()) {
		node->m_operator = ' ';
		node->m_value = *monkeys[index].m_value;
		node->m_leads_to_humn = (index == humn_node);
		node->m_left_child = nullptr;
		node->m_right_child = nullptr;
		return node;
	}

	node->m_operator = monkeys[index].m_operator;
	node->m_left_child = solve(monkeys[index].m_operand_1);
	node->m_right_child = solve(monkeys[index].m_operand_2);
	node->m_leads_to_humn = node->m_left_child->m_leads_to_humn || node->m_right_child->m_leads_to_humn;

	switch (monkeys[index].m_operator) {
		case '+':
			node->m_value = node->m_left_child->m_value + node->m_right_child->m_value;
			break;
		case '-':
			node->m_value = node->m_left_child->m_value - node->m_right_child->m_value;
			break;
		case '*':
			node->m_value = node->m_left_child->m_value * node->m_right_child->m_value;
			break;
		case '/':
			node->m_value = node->m_left_child->m_value / node->m_right_child->m_value;
			break;
		default:
			cerr << "Wait, what? [" << monkeys[index].m_operator << "]\n";
	}

	return node;
}

int64_t solve_for_humn(Node *node, int64_t target) {

	if (node->m_left_child == nullptr && node->m_right_child == nullptr) {
		return target;
	}

	auto *next = node->m_left_child.get();
	auto *value = node->m_right_child.get();
	bool left = true;

	if (node->m_right_child->m_leads_to_humn) {
		next = node->m_right_child.get();
		value = node->m_left_child.get();
		left = false;
	}

	switch (node->m_operator) {
		case '+': return solve_for_humn(next, target - value->m_value);
		case '-': return solve_for_humn(next, left ? (target + value->m_value) : (value->m_value - target));
		case '*': return solve_for_humn(next, target / value->m_value);
		case '/': return solve_for_humn(next, left ? (target * value->m_value) : (value->m_value / target));
		default:  return 0;
	}
}

int main() {

	// input
	regex r_operation("([^:]+): ([^:]+) (.) ([^:]+)");
	regex r_shouter("([^:]+): ([[:digit:]]+)");

	string line;
	while (getline(cin, line)) {
		smatch	sm;

		if (regex_match(line, sm, r_operation)) {
			// sm[1]
			int32_t idx = monkey_index(sm[1]);
			monkeys[idx].m_operator = sm[3].str()[0];
			monkeys[idx].m_operand_1 = monkey_index(sm[2]);
			monkeys[idx].m_operand_2 = monkey_index(sm[4]);
		} else if (regex_match(line, sm, r_shouter)) {
			auto &monkey = monkeys[monkey_index(sm[1])];
			monkey.m_value = stoi(sm[2]);
		} else {
			cerr << "Parsing failed\n";
		}
	}

	// solve
	humn_node = monkey_index("humn");
	int32_t monkey = monkey_index("root");

	auto left = solve(monkeys[monkey].m_operand_1);
	auto right = solve(monkeys[monkey].m_operand_2);

	if (left->m_leads_to_humn) {
		cout << solve_for_humn(left.get(), right->m_value) << endl;
	}

	if (right->m_leads_to_humn) {
		cout << solve_for_humn(right.get(), left->m_value) << endl;
	}

	return 0;
}
