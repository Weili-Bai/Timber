#include "Difficulty.h"
Difficulty::Difficulty() {
	type = EASY;
	timeLimit = 6.0f;
	branchInterval = 5;
	bonusRate = 1;
}
Difficulty::Difficulty(int difficulty) {
	if (difficulty == EASY) {
		timeLimit = 6.0f;
		branchInterval = 5;
		bonusRate = 1;
		type = EASY;
	}
	else {
		timeLimit = 4.0f;
		branchInterval = 2;
		bonusRate = 2;
		type = HARD;
	}
}