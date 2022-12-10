#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

using namespace std;

enum class Opcode : uint8_t {
	NoOp = 0,
	AddX = 1
};

struct Instruction {
	Opcode		m_opcode;
	int32_t		m_param1;
};

class VirtualMachine {
public:
	VirtualMachine() = default;

	void AddInstruction(Instruction const& instr) {
		m_program.push_back(instr);
	}

	int32_t	Cycle() const {return m_cycle;}
	int32_t RegX() const {return m_reg_x;}

	void ExecuteClockCycle() {

		switch (m_program[m_pc].m_opcode) {
			case Opcode::NoOp:
				GotoNextInstruction();
				break;

			case Opcode::AddX:
				if (m_phase == 1) {
					m_reg_x += m_program[m_pc].m_param1;
					GotoNextInstruction();
				} else {
					m_phase += 1;
				}
				break;
		}

		m_cycle += 1;
	}

	void GotoNextInstruction() {
		m_pc += 1;
		m_phase = 0;
	}

	bool ProgramEnded() {
		return m_pc >= m_program.size();
	}

private:
	int32_t					m_reg_x = 1;
	int32_t					m_pc = 0;
	int32_t					m_cycle = 0;
	int32_t					m_phase = 0;
	vector<Instruction>		m_program;
};

int main() {

	VirtualMachine vm;

	string	instr;
	int32_t param;

	int32_t strength = 0;

	while (cin >> instr) {
		if (instr == "noop") {
			vm.AddInstruction({Opcode::NoOp, 0});
		} else if (instr == "addx") {
			cin >> param;
			vm.AddInstruction({Opcode::AddX, param});
		}
	}

	while (!vm.ProgramEnded()) {
		vm.ExecuteClockCycle();

		if (vm.Cycle() == 19 || vm.Cycle() == 59 ||
			vm.Cycle() == 99 || vm.Cycle() == 139 ||
			vm.Cycle() == 179 || vm.Cycle() == 219) {
			strength += (vm.Cycle() + 1) * vm.RegX();
		}
	}

	cout << strength << endl;

	return 0;
}
