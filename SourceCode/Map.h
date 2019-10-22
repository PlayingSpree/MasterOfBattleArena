#pragma once
#include "Game.h"
#include "Bullet.h"
#include "Spawner.h"
#include "Enemy.h"
class MapClass
{
public:
	enum MapEnum
	{
		Map1 = 1,
		Map2
	}map;
	struct SpawnQueue
	{
		Enemy::BaseStatStruct Enemy;
		float WaitTime;
		SpawnQueue(Enemy::BaseStatStruct e, float f) : Enemy(e) , WaitTime(f){}
	}; 
	std::queue<GameObject*> MapObject;

	enum EnemyEnum
	{
		EE_Fighter,
		EE_DarkAssassin,
		EE_OldMan,
		EE_EvilKnight,
		EE_Mage,
		EE_Dragon,
		EE_Assassin,
		EE_Fat,
		EE_Bat,
		EE_ManBat,
		EE_Frog,
		EE_Skeleton,
		EE_Orc,
		EE_Ghost,
		EE_Cow,
		EE_Succ
	};
	void SpawnWave(int wave, bool endless, int playercount);
	void Spawn(EnemyEnum enemy, float Delay);
	MapClass(Game* game, Scene* sg, MapEnum map, std::queue<SpawnQueue>* EnemySpawnQueue);
	std::queue<SpawnQueue>* EnemySpawnQueue;
};