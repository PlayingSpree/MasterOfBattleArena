#pragma once
#include "Game.h"
#include "Player.h"
#include "Bullet.h"
#include "Spawner.h"
#include "Enemy.h"
#include "Map.h"

class SceneGame : public Scene
{
private:
	void AddNewGameObject(GameObject * gop);
public:
	static unsigned int UID;
	const float ViewRad = 200.0f;

	MapClass::MapEnum Map;
	MapClass map;
	Game* game;

	std::vector<GameObject*> objectvector;
	std::map<GameObject::ObjectTypeEnum,std::vector<GameObject*>> ObjectByTypeMapVector;
	std::list<GameObject*> newobjectlist;

	float EnemySpawnQueueTime = 10.0f;
	std::queue<MapClass::SpawnQueue> EnemySpawnQueue;

	enum TextureEnum
	{
		TE_Map1Background, TE_Map2Background,TE_Map2StaticBackground
	};

	std::map<TextureEnum, sf::Texture*> TextureMap;
	std::list<sf::Sprite*> backgroundSpriteList;
	sf::Sprite StaticBackground;

	class FloatingText
	{
	public:
		enum TextType
		{
			DamageRed,
			DamageWhite,
			FloatUp,
			Zoom
		}tt;
		FloatingText(const std::string s, const sf::Vector2f pos, const TextType tt, GameResources * gr, float arg = 0);
		bool Update(float deltatime);
		sf::Text t;
		float time;
		float scale;
	};
	std::list<FloatingText> FloatingTextList;
	std::list<FloatingText> UIFloatingTextList;

	struct PlayerUIstruct
	{
		bool Enabled;
		sf::Text AmmoWeapon[2];
	}PlayerUI[4];

	int wave = 0;
	bool wavecompleted = true;

	struct UI
	{
		unsigned int score;
	}UIdata;

	SceneGame(Game*);
	
	std::vector<GameObject*>::iterator RemoveGameObject(std::vector<GameObject*>::iterator dit);
	~SceneGame();

	void NextWave();

	void loadTexture();
	void AddBackground();

	void loop();
	void UpdateView();
};