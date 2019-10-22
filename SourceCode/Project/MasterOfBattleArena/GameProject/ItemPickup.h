#pragma once
#include "Game.h"
class ItemPickup :
	public GameObject
{
public:
	static const int CrystalPos = 2;
	enum PickUpEnum
	{
		IPE_Bomb,
		IPE_Health,
		IPE_CrystalRed,
		IPE_CrystalPurple,
		IPE_CrystalYellow,
		IPE_CrystalWhite,
		IPE_CrystalGreen,
		IPE_CrystalCyan,
		IPE_CrystalBlue,
		IPE_CrystalRainbow
	}Item;
	int SpriteOffset;
	Game* game;
	AnimatedSprite AS;
	float Time;
	ItemPickup(Game * g, float x, float y, PickUpEnum);
	sf::Vector2f GetPosition();
	bool Update(const float deltatime);
	void Draw(sf::RenderWindow* w, const bool UI);
	~ItemPickup();
};