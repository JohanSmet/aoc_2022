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

static std::vector<Monkey>	monkeys;
static std::unordered_map<std::string, int32_t> monkey_map;

int32_t monkey_index(std::string const &name) {
	auto found = monkey_map.find(name);
	if (found != end(monkey_map)) {
		return found->second;
	}

	monkeys.push_back(Monkey{' ', 0, 0, {}});
	monkey_map[name] = monkeys.size() - 1;
	return monkeys.size() - 1;
}

int64_t solve(int32_t index) {

	if (monkeys[index].m_value.has_value()) {
		return *monkeys[index].m_value;
	}

	int64_t op1 = solve(monkeys[index].m_operand_1);
	int64_t op2 = solve(monkeys[index].m_operand_2);

	switch (monkeys[index].m_operator) {
		case '+': return op1 + op2;
		case '-': return op1 - op2;
		case '*': return op1 * op2;
		case '/': return op1 / op2;
		default:
			cerr << "Wait, what? [" << monkeys[index].m_operator << "]\n";
	}

	return 0;
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
	int32_t monkey = monkey_index("root");
	cout << solve(monkey) << endl;

	return 0;
}
