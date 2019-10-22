#pragma once
#include <SFML/Graphics.hpp>
#include "GameResources.h"
#include "GameObject.h"
#include "CharObject.h"
#include "HighScore.h"
#include "SoundEngine.h"

class Scene
{
public:
	virtual void loop() = 0 {};
};


struct PlayData
{
	int init = -1;
	void initPlayData(const int t);
	unsigned int Score = 0;
	bool Playing;
	enum PlayModeEnum
	{
		COOP
	}PlayMode;
	int map = 2;
	int playerCount = 1;
	struct PlayerInfo
	{
		int PlayerModel;
		int controlnum;
		int team;
		int weapon;
	}playerinfo[4];
	PlayData();
};

class Game {
public:
	enum SecneEnum
	{
		SGame,
		SMainMenu
	};

	sf::Clock* clock;
	sf::RenderWindow* window;
	GameResources* GameResource;
	HighScore highscore;
	SoundEngine sound;

	float deltatime;
	float time;
	float windowratio;
	PlayData playdata;

	Scene* scene;

	Game(sf::Clock*, sf::RenderWindow*, GameResources*);
	~Game();

	void loop();

	void ChangeScene(SecneEnum);

	void TimeUpdate();

	sf::Uint32 Char;
	void CharEnter(const sf::Uint32 i);
};

class GGEZ
{
public:
	static float LengthVector2f(const sf::Vector2f v) {
		return sqrt((v.x * v.x) + (v.y * v.y));
	}

	static sf::Vector2f NormalizeVector2f(const sf::Vector2f v) {
		float length = LengthVector2f(v);
		if (length != 0)
			return sf::Vector2f(v.x / length, v.y / length);
		return v;
	}

	static sf::Vector2f MoveToVector(const sf::Vector2f CurrentPos, const sf::Vector2f TargetPos, const float MoveSpeed) {
		return (NormalizeVector2f(TargetPos - CurrentPos)*MoveSpeed);
	}

	static sf::Vector2f MoveTo(const sf::Vector2f CurrentPos, const sf::Vector2f TargetPos, const float MoveSpeed) {
		return CurrentPos + (NormalizeVector2f(TargetPos - CurrentPos)*MoveSpeed);
	}

	static float Smooth(const float Old,const float New,const float Factor) {
		return Old + ((New - Old)*Factor);
	}

	static float Rand() {
		return (float)rand() / (float)RAND_MAX;
	}
	static int randInRange(int min, int max)
	{
		return min + (int)(rand() / (double)(RAND_MAX + 1) * (max - min + 1));
	}
};