#ifndef DIFFICULTY_H
#define DIFFICULTY_H
class Difficulty
{
public:
	Difficulty();
	Difficulty(int);
	enum DIFFICULTY {EASY,HARD};
	float timeLimit;
	int type;
	int branchInterval;
	int bonusRate;
};
#endif