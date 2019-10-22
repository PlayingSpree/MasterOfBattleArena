#pragma once
#include "Game.h"
#include "Player.h"
#include "Enemy.h"

class Bullet :
	public GameObject
{
public:
	sf::Sprite sprite;
	sf::Vector2f Dir;
	sf::FloatRect rect;
	float Offset = 1.f;
	float Distance;
	float Time;
	float Damage,Falloff;
	float Range;
	float KnockBack;
	int pen;
	std::vector<int> ObjectHit;
	const GameObject* Owner;
	int Team;
	Game* game;
	GameObject::ObjectTypeEnum OwnerType;

	Bullet(Game * game,const GameObject* owner, const int team, const sf::Vector2f pos, const sf::Vector2f dir, const WeaponStruct * ws, const sf::Texture * tx,const float Offset = 1.f);
	Bullet(Game * g, const GameObject * owner, const int team, const sf::Vector2f pos, const sf::Vector2f dir, const Enemy::BaseStatStruct * ebs, const sf::Texture * tx);
	~Bullet();
	bool checkcoll(const std::vector<GameObject*>* objectvector);
	bool Update(const float deltatime);
	void Draw(sf::RenderWindow* w, const bool UI);
	sf::Vector2f GetPosition();
};

