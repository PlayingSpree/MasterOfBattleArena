#pragma once
#include "Game.h"

class Effect : GameObject
{
public:
	bool havelife;
	float Fading;
	float PosOffset;
	enum EffectEnum
	{
		DeadEnemy,
		FrogExplosion,
		ItemExplosion,
		ItemHeal,
		Map2Spawnpoint
	}EE;
	Game* game;
	AnimatedSprite AS;
	bool UI;
	float Time;
	Effect(Game * g, const float x, const float y,const EffectEnum, const int arg);
	sf::Vector2f GetPosition();
	bool Update(const float deltatime);
	void Draw(sf::RenderWindow* w, const bool UI);
	~Effect();
};

