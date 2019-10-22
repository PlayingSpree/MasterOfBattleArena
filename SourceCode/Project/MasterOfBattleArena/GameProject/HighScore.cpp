#include "HighScore.h"
#include <iostream>
#include <fstream>

void HighScore::LoadHighscore()
{
	//Load Setting File
	wchar_t line[32];
	std::wifstream file;
	file.open("highscore.noob", std::ios::in);

	if (!file)
	{
		SaveHighscore();
	}
	else
	{
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t i2 = 0; i2 < 5; i2++)
			{
				file.getline(line, 32);
				score[i][i2].name = line;
				file.getline(line, 32);
				score[i][i2].Score = stoul(std::wstring(line));
			}
		}
	}
	file.close();
}

void HighScore::SaveHighscore()
{
	std::wstring line;
	std::wofstream file;
	file.open("highscore.noob");
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t i2 = 0; i2 < 5; i2++)
		{
			line = score[i][i2].name;
			file << line << std::endl;
			line = std::to_wstring(score[i][i2].Score);
			file << line << std::endl;
		}
	}
	file.close();
}

int HighScore::IsHighScore(const unsigned int Score, const int mode)
{
	LoadHighscore();
	for (int i = 0; i < 5; i++)
	{
		if (Score > score[mode][i].Score) {
			return i;
		}
	}
	return -1;
}

void HighScore::SubmitHighScore(const std::wstring * ws, const unsigned int Score, const int mode)
{
	LoadHighscore();
	for (int i = 0; i < 5; i++)
	{
		if (Score > score[mode][i].Score) {
			for (int i2 = 3; i2 >= i; i2--)
			{
				score[mode][i2 + 1].name = score[mode][i2].name;
				score[mode][i2 + 1].Score = score[mode][i2].Score;
			}
			score[mode][i].name = *ws;
			score[mode][i].Score = Score;
			break;
		}
	}
	SaveHighscore();
}