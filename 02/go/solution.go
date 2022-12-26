package main

import (
	"fmt"
)

type Move struct {
	field_1		int
	field_2		int
}

const PLAY_ROCK = 0
const PLAY_PAPER = 1
const PLAY_SCISSORS = 2

const SCORE_WIN  = 6
const SCORE_DRAW = 3
const SCORE_LOSE = 0

const ACTION_LOSE = 0
const ACTION_DRAW = 1
const ACTION_WIN = 2

var PLAYBOOK = [3][3]int {
	{PLAY_SCISSORS, PLAY_ROCK, PLAY_PAPER},
	{PLAY_ROCK, PLAY_PAPER, PLAY_SCISSORS},
	{PLAY_PAPER, PLAY_SCISSORS, PLAY_ROCK},
};

func play_rock_paper_scissors(you int, opponent int) int {

	card_score := you + 1

	if you == opponent {
		return card_score + SCORE_DRAW
	}

	if (you == PLAY_ROCK && opponent == PLAY_SCISSORS) ||
	   (you == PLAY_SCISSORS && opponent == PLAY_PAPER) ||
	   (you == PLAY_PAPER && opponent == PLAY_ROCK) {
		return card_score + SCORE_WIN
	} else {
		return card_score + SCORE_LOSE
	}
}

func main() {

	var moves []Move

	// read input
	for {
		var c1, c2 rune

		var count, _ = fmt.Scanf("%c %c\n", &c1, &c2)
		if count == 0 {
			break
		}

		moves = append(moves, Move { int(c1 - 'A'), int(c2 - 'X') })
	}

	// part 1
	total_score := 0
	for i := 0; i < len(moves); i++ {
		total_score += play_rock_paper_scissors(moves[i].field_2, moves[i].field_1)
	}
	fmt.Printf("Part 1: %d\n", total_score)

	// part 2
	total_score = 0

	for i := 0; i < len(moves); i++ {
		score := (moves[i].field_2 * 3) + (PLAYBOOK[moves[i].field_1][moves[i].field_2] + 1)
		total_score += score;
	}
	fmt.Printf("Part 2: %d\n", total_score)

}
