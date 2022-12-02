#include <iostream>
#include <string>

using namespace std;

constexpr int PLAY_ROCK = 0;
constexpr int PLAY_PAPER = 1;
constexpr int PLAY_SCISSORS = 2;

constexpr int ACTION_LOSE = 0;
constexpr int ACTION_DRAW = 1;
constexpr int ACTION_WIN = 2;

constexpr int PLAYBOOK[3][3] = {
	{PLAY_SCISSORS, PLAY_ROCK, PLAY_PAPER},
	{PLAY_ROCK, PLAY_PAPER, PLAY_SCISSORS},
	{PLAY_PAPER, PLAY_SCISSORS, PLAY_ROCK}
};


int main() {

	char input_1, input_2;

	int total_score = 0;

	while (cin >> input_1 >> input_2) {
		int play_them = input_1 - 'A';
		int play_action = input_2 - 'X';

		int score = (play_action * 3) + (PLAYBOOK[play_them][play_action] + 1);
		total_score += score;
	}

	cout << total_score << endl;

	return 0;
}
