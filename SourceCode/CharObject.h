#pragma once
#include "Game.h"
class CharObject
{
public:
	int Team;
	AnimatedSprite AS;

	struct CharStatusStruct {
		float MaxHP;
		float HP;
		int dir;
		struct StatusStruct
		{
			bool walking;
			bool attacking;
		}Status;
		float movespeed;
		float movex;
		float movey;
		sf::Vector2f speed;
		void Update(const float);
		void DoHP(const float dmg);
	};
	CharStatusStruct* CharStatus;
};