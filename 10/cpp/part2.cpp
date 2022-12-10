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

class Display {
public:
	void ExecuteClockCycle(int32_t sprite_x) {
		char pixel = (abs(sprite_x - m_x) <= 1) ? '#' : ' ';
		m_display[m_y][m_x] = pixel;

		m_x += 1;
		if (m_x == 40) {
			m_x = 0;
			m_y += 1;
		}
	}

	void PrintDisplay() const {
		for (int y = 0; y < 6; ++y) {
			cout << m_display[y] << endl;
		 }
	}

private:
	char m_display[6][41] = {0};
	int  m_x = 0;
	int  m_y = 0;
};

int main() {

	VirtualMachine vm;
	Display display;

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
		display.ExecuteClockCycle(vm.RegX());
		vm.ExecuteClockCycle();

		if (vm.Cycle() == 19 || vm.Cycle() == 59 ||
			vm.Cycle() == 99 || vm.Cycle() == 139 ||
			vm.Cycle() == 179 || vm.Cycle() == 219) {
			strength += (vm.Cycle() + 1) * vm.RegX();
		}
	}

	display.PrintDisplay();
	return 0;
}
