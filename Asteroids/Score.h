#pragma once
#ifndef SCORE_H
#define SCORE_H

class Score
{
public:
	Score();
	Score(long count);
	void draw();
	bool addScore(long num);
	long getScore();
private:
	long score;
	long toBonus;
};

#endif