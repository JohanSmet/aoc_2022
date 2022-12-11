#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

using namespace std;

enum class ArgumentType {
	Literal,
	CurrentValue
};

enum class Op {
	Add,
	Mul
};

struct Formula {
	ArgumentType	m_type_1;
	int64_t			m_value_1;
	Op				m_operation;
	ArgumentType	m_type_2;
	int64_t			m_value_2;
};


struct Monkey {
	vector<int64_t>		m_items;
	Formula				m_formula;
	int64_t				m_test_value;
	int64_t				m_true_target;
	int64_t				m_false_target;
	int64_t				m_inspect_count;
};

Monkey read_monkey() {
	// cautionary tale: this is what happens when you think: "nah, this input look simple ... I don't need a real parser"

	Monkey result;
	string line;

	// Monkey x:
	getline(cin, line);

	// Starting items: x, x, x
	getline(cin, line);

	int start = line.find(':') + 2;
	while (start != string::npos) {
		int end = line.find(',', start + 1);
		result.m_items.push_back(stoi(line.substr(start, end - start)));
		start = end;
		if (start != string::npos) {
			start += 2;
		}
	}

	// Operation: new = old * x
	getline(cin, line);
	result.m_formula.m_type_1 = ArgumentType::CurrentValue;
	result.m_formula.m_value_1 = 0;

	int op_pos = line.find_first_of("*+");
	result.m_formula.m_operation = (line[op_pos] == '+') ? Op::Add : Op::Mul;
	auto value2 = line.substr(op_pos+2);
	if (value2 == "old") {
		result.m_formula.m_type_2 = ArgumentType::CurrentValue;
		result.m_formula.m_value_2 = 0;
	} else {
		result.m_formula.m_type_2 = ArgumentType::Literal;
		result.m_formula.m_value_2 = stoi(value2);
	}

	// Test: divisible by x
	getline(cin, line);
	int sepa = line.rfind(' ');
	result.m_test_value = stoi(line.substr(sepa+1));

	// If true: throw to monkey x
	getline(cin, line);
	sepa = line.rfind(' ');
	result.m_true_target = stoi(line.substr(sepa+1));

	// If false: throw to monkey x
	getline(cin, line);
	sepa = line.rfind(' ');
	result.m_false_target = stoi(line.substr(sepa+1));

	result.m_inspect_count = 0;

	return result;
}

int64_t evaluate_formula(Formula const &formula, int64_t old) {

	int64_t arg2 = (formula.m_type_2 == ArgumentType::CurrentValue) ? old : formula.m_value_2;

	switch (formula.m_operation) {
		case Op::Add: return old + arg2;
		case Op::Mul: return old * arg2;
	}

	return 0;
}

void run_monkey_turns(vector<Monkey> &monkeys, int64_t limit) {

	for (auto &monkey: monkeys) {

		for (auto item: monkey.m_items) {
			int64_t new_value = evaluate_formula(monkey.m_formula, item) % limit;

			if (new_value % monkey.m_test_value == 0) {
				monkeys[monkey.m_true_target].m_items.push_back(new_value);
			} else {
				monkeys[monkey.m_false_target].m_items.push_back(new_value);
			}
		}

		monkey.m_inspect_count += monkey.m_items.size();
		monkey.m_items.clear();
	}
}

int main() {

	vector<Monkey> monkeys;

	// input
	bool do_input = true;
	while (do_input) {
		monkeys.push_back(read_monkey());

		string line;
		do_input = !!getline(cin, line);
	}

	int64_t limit = 1;
	for (auto const &m : monkeys) {
		limit *= m.m_test_value;
	}

	cout << limit << endl;


	// run 10000 turns
	for (int i = 0; i < 10000; ++i) {
		run_monkey_turns(monkeys, limit);
	}

	// find 2 most active monkey
	int64_t top[2] = {0, 0};

	for (auto const &m : monkeys) {
		if (m.m_inspect_count > top[0]) {
			top[1] = top[0];
			top[0] = m.m_inspect_count;
		} else if (m.m_inspect_count > top[1]) {
			top[1] = m.m_inspect_count;
		}
	}

	cout << top[0] * top[1] << endl;

	return 0;
}
