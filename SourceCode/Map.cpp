#include "Map.h"
#include "Effect.h"

void MapClass::SpawnWave(int wave,bool endless,int playercount) {
	if (endless) {
		int Power = 500 + 200 * wave * playercount;
		while (Power > 0)
		{
			float f = GGEZ::Rand();
			if (wave > 10) {
				if (Power*f > 2000) {
					if (GGEZ::Rand() > 0.5f)
						Spawn(EE_Succ, 1.0f);
					else
						Spawn(EE_Cow, 1.0f);
					Power -= 1000;
					continue;
				}
			}
			if (wave > 8) {
				if (Power*f > 1600) {
					if (GGEZ::Rand() > 0.5f)
						Spawn(EE_Orc, 1.0f);
					else
						Spawn(EE_Ghost, 1.0f);
					Power -= 800;
					continue;
				}
			}
			if (wave > 6) {
				if (Power*f > 1000) {
					float ff = GGEZ::Rand();
					if (ff > 0.75f)
						Spawn(EE_Bat, 1.0f);
					else if (ff > 0.5f)
						Spawn(EE_ManBat, 1.0f);
					else if (ff > 0.25f)
						Spawn(EE_Frog, 1.0f);
					else
						Spawn(EE_Skeleton, 1.0f);
					Power -= 600;
					continue;
				}
			}
			if (wave > 4) {
				if (Power*f > 500) {
					float ff = GGEZ::Rand();
					if (ff > 0.75f)
						Spawn(EE_Fat, 1.0f);
					else if (ff > 0.5f)
						Spawn(EE_Assassin, 1.0f);
					else if (ff > 0.25f)
						Spawn(EE_Dragon, 1.0f);
					else
						Spawn(EE_Mage, 1.0f);
					Power -= 400;
					continue;
				}
			}
			if (wave > 2) {
				if (Power*f > 200) {
					if (GGEZ::Rand() > 0.5f)
						Spawn(EE_EvilKnight, 1.0f);
					else
						Spawn(EE_OldMan, 1.0f);
					Power -= 200;
					continue;
				}
			}
			if (f > 0.5f)
				Spawn(EE_DarkAssassin, 1.0f);
			else
				Spawn(EE_Fighter, 1.0f);
			Power -= 100;
		}
	}
}

void MapClass::Spawn(EnemyEnum enemy, float Delay)
{
	if (enemy == EE_Fighter) {
		EnemySpawnQueue->push(SpawnQueue(Enemy::BaseStatStruct(Enemy::ESE_Fighter, 200, 50, 0.6f, 10, 1000, Enemy::TSTE_Closest, Enemy::ATE_Melee), Delay)); //Power 100 Balanced
	}
	else if (enemy == EE_DarkAssassin) {
		EnemySpawnQueue->push(SpawnQueue(Enemy::BaseStatStruct(Enemy::ESE_Assassin, 100, 80, 0.3f, 10, 700, Enemy::TSTE_Random, Enemy::ATE_Melee), Delay)); //Power 120 Fast Attack
	}
	else if (enemy == EE_OldMan) {
		EnemySpawnQueue->push(SpawnQueue(Enemy::BaseStatStruct(Enemy::ESE_OldMan, 400, 40, 1.0f, 25, 1000, Enemy::TSTE_Closest, Enemy::ATE_Melee), Delay)); //Power 150 Tank
	}
	else if (enemy == EE_EvilKnight) {
		EnemySpawnQueue->push(SpawnQueue(Enemy::BaseStatStruct(Enemy::ESE_EvilKnight, 500, 40, 1.2f, 25, 2000, Enemy::TSTE_Closest, Enemy::ATE_Melee), Delay)); // Power 200 Balanced
	}
	else if (enemy == EE_Mage) {
		EnemySpawnQueue->push(SpawnQueue(Enemy::BaseStatStruct(Enemy::ESE_Mage, 250, 70, 0.8f, 20, 500, Enemy::TSTE_Closest, Enemy::ATE_Ranged, 400.0f), Delay)); //Power 250 Ranged Attack
	}
	else if (enemy == EE_Dragon) {
		EnemySpawnQueue->push(SpawnQueue(Enemy::BaseStatStruct(Enemy::ESE_Dragon, 400, 60, 1.0f, 30, 1500, Enemy::TSTE_Closest, Enemy::ATE_Ranged, 200.0f, Enemy::EPE_FireBallRed), Delay)); //Power 300 Ranged Balanced
	}
	else if (enemy == EE_Assassin) {
		EnemySpawnQueue->push(SpawnQueue(Enemy::BaseStatStruct(Enemy::ESE_Assassin, 200, 100, 0.3f, 10, 700, Enemy::TSTE_Random, Enemy::ATE_Melee), Delay)); //Power 350 Fast Attack
	}
	else if (enemy == EE_Fat) {
		EnemySpawnQueue->push(SpawnQueue(Enemy::BaseStatStruct(Enemy::ESE_Fat, 1200, 30, 1.5f, 10, 5000, Enemy::TSTE_Closest, Enemy::ATE_Melee), Delay)); //Power 400 Tank
	}
	else if (enemy == EE_Bat) {
		EnemySpawnQueue->push(SpawnQueue(Enemy::BaseStatStruct(Enemy::ESE_Bat, 50, 120, 0.3f, 10, 700, Enemy::TSTE_Closest, Enemy::ATE_Melee), -1.0f)); //Power 500 Multi Fast Attack
		EnemySpawnQueue->push(SpawnQueue(Enemy::BaseStatStruct(Enemy::ESE_Bat, 50, 120, 0.3f, 10, 700, Enemy::TSTE_Closest, Enemy::ATE_Melee), -1.0f));
		EnemySpawnQueue->push(SpawnQueue(Enemy::BaseStatStruct(Enemy::ESE_Bat, 50, 120, 0.3f, 10, 700, Enemy::TSTE_Closest, Enemy::ATE_Melee), -1.0f));
		EnemySpawnQueue->push(SpawnQueue(Enemy::BaseStatStruct(Enemy::ESE_Bat, 50, 120, 0.3f, 10, 700, Enemy::TSTE_Closest, Enemy::ATE_Melee), -1.0f));
		EnemySpawnQueue->push(SpawnQueue(Enemy::BaseStatStruct(Enemy::ESE_Bat, 50, 120, 0.3f, 10, 700, Enemy::TSTE_Closest, Enemy::ATE_Melee), -1.0f));
		EnemySpawnQueue->push(SpawnQueue(Enemy::BaseStatStruct(Enemy::ESE_Bat, 50, 120, 0.3f, 10, 700, Enemy::TSTE_Closest, Enemy::ATE_Melee), Delay));
	}
	else if (enemy == EE_ManBat) {
		EnemySpawnQueue->push(SpawnQueue(Enemy::BaseStatStruct(Enemy::ESE_ManBat, 1000, 50, 0.6f, 35, 1000, Enemy::TSTE_Closest, Enemy::ATE_Melee), Delay)); // Power 500 Balanced
	}
	else if (enemy == EE_Frog) {
		EnemySpawnQueue->push(SpawnQueue(Enemy::BaseStatStruct(Enemy::ESE_Frog, 1000, 50, 0.2f, 50, 5000, Enemy::TSTE_Closest, Enemy::ATE_Explode, 300.0f), Delay)); // Power 500 Burst
	}
	else if (enemy == EE_Skeleton) {
		EnemySpawnQueue->push(SpawnQueue(Enemy::BaseStatStruct(Enemy::ESE_Skeleton, 500, 50, 0.6f, 50, 1000, Enemy::TSTE_Random, Enemy::ATE_Melee), Delay)); // Power 600 Damage
	}
	else if (enemy == EE_Orc) {
		EnemySpawnQueue->push(SpawnQueue(Enemy::BaseStatStruct(Enemy::ESE_Orc, 2500, 30, 2.0f, 30, 5000, Enemy::TSTE_Random, Enemy::ATE_Melee), Delay)); //Power 800 Tank
	}
	else if (enemy == EE_Ghost) {
		EnemySpawnQueue->push(SpawnQueue(Enemy::BaseStatStruct(Enemy::ESE_Ghost, 700, 30, 0.75f, 40, 1000, Enemy::TSTE_Closest, Enemy::ATE_Ranged, 300.0f, Enemy::EPE_Dark), Delay)); //Power 800 Ranged Attack
	}
	else if (enemy == EE_Cow) {
		EnemySpawnQueue->push(SpawnQueue(Enemy::BaseStatStruct(Enemy::ESE_Cow, 2500, 80, 0.3f, 10, 5000, Enemy::TSTE_Closest, Enemy::ATE_Melee), Delay)); //Power 1000 Fast Tank
	}
	else if (enemy == EE_Succ)
	{
		EnemySpawnQueue->push(SpawnQueue(Enemy::BaseStatStruct(Enemy::ESE_Succ, 1500, 60, 0.9f, 30, 500, Enemy::TSTE_Random, Enemy::ATE_Ranged, 100.0f, Enemy::EPE_Pink), Delay)); //Power 1000 Ranged Balanced
	}
}

MapClass::MapClass(Game * game, Scene * sg, MapEnum map, std::queue<SpawnQueue>* esq) : map(map)
{
	EnemySpawnQueue = esq;
	//===================Map1=====================
	if (map == Map1|| map == Map2) {
		//Object
		MapObject.push(new Spawner(game, sg, 500.0f, 490.0f));
		MapObject.push(new Spawner(game, sg, -490.0f, 490.0f));
		MapObject.push(new Spawner(game, sg, 500.0f, -490.0f));
		MapObject.push(new Spawner(game, sg, -490.0f, -490.0f));
		if (map == Map2) {
			MapObject.push(((GameObject*)(new Effect(game, -490.0f, -490.0f, Effect::Map2Spawnpoint, -1))));
			MapObject.push(((GameObject*)(new Effect(game, 500.0f, -490.0f, Effect::Map2Spawnpoint, -1))));
			MapObject.push(((GameObject*)(new Effect(game, -490.0f, 490.0f, Effect::Map2Spawnpoint, -1))));
			MapObject.push(((GameObject*)(new Effect(game, 500.0f, 490.0f, Effect::Map2Spawnpoint, -1))));
		}
	}
}
