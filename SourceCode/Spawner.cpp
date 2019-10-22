#include "Spawner.h"
#include "SceneGame.h"

Spawner::Spawner(Game * g, Scene * sg, float x, float y)
{
	ObjectType = GameObject::SpawnerObj;
	ID = SceneGame::UID++;
	game = g;
	scenegame = sg;
	Transfrom.left = x;
	Transfrom.top = y;
}

bool Spawner::Update(const float deltatime) {
	return false;
}

void Spawner::Spawn(Enemy* e) {
	e->SetPosition(GetPosition());
	((SceneGame*)scenegame)->newobjectlist.push_back(e);
}

Spawner::~Spawner()
{
}
