#include <iostream>
#include <string>

using namespace std;

constexpr int PLAY_ROCK = 0;
constexpr int PLAY_PAPER = 1;
constexpr int PLAY_SCISSORS = 2;

constexpr int SCORE_WIN  = 6;
constexpr int SCORE_DRAW = 3;
constexpr int SCORE_LOSE = 0;

int play_rock_papper_scissors(int you, int opponent) {

	int card_score = you + 1;

	if (you == opponent) {
		return card_score + SCORE_DRAW;
	}

	if ((you == PLAY_ROCK && opponent == PLAY_SCISSORS) ||
	    (you == PLAY_SCISSORS && opponent == PLAY_PAPER) ||
	    (you == PLAY_PAPER && opponent == PLAY_ROCK)) {
		return card_score + SCORE_WIN;
	} else {
		return card_score + SCORE_LOSE;
	}
}

int main() {

	char input_1, input_2;

	int total_score = 0;

	while (cin >> input_1 >> input_2) {
		int play_them = input_1 - 'A';
		int play_you = input_2 - 'X';

		total_score += play_rock_papper_scissors(play_you, play_them);
	}

	cout << total_score << endl;

	return 0;
}
