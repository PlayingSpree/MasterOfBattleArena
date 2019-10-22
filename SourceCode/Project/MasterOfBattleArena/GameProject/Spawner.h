#pragma once
#include "Game.h"
#include "Enemy.h"
class Spawner : public GameObject
{
public:
	Spawner(Game * g, Scene * sg,float x,float y);
	bool Update(const float);
	void Spawn(Enemy * e);
	void Draw(sf::RenderWindow* w, const bool UI) {};
	sf::Vector2f GetPosition() { return sf::Vector2f(Transfrom.left, Transfrom.top); };

	~Spawner();

	Game* game;
	Scene* scenegame;
};

