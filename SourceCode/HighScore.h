#pragma once
#include <string>
#include <map>
#include "SFML\Graphics.hpp"
class HighScore
{
public:
	struct ScoreInfo
	{
		unsigned int Score;
		std::wstring name;
	}score[4][5];
	void LoadHighscore();
	void SaveHighscore();
	int IsHighScore(const unsigned int Score, const int mode);
	void SubmitHighScore(const std::wstring* ws,const unsigned int Score, const int mode);
};

