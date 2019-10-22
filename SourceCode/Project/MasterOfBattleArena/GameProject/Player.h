#pragma once
#include "CharObject.h"
class Player : public GameObject, public CharObject
{
private:
	void GetControl();
public:
	//Setting
	static const int WeaponCount = 1;

	enum ActionsEnum
	{
		Up, Left, Down, Right, S1, S2, Ult, Swap, Act, Reload, Fire
	};
	bool Action[11];
	int ControlNum;
	sf::Texture playertexture;
	sf::Texture weapontexture[WeaponCount][3];
	sf::Sprite WeaponSprite;
	sf::Color PlayerColor;
	int PlayerID;
	int PlayerModel;
	bool Die;

	Game* game;
	

	bool Update(const float deltatime);

	void Draw(sf::RenderWindow* w, const bool UI);
	void SetPosition(sf::Vector2f Pos);
	sf::Vector2f GetPosition();
	struct BaseStatStruct
	{
		float MaxHP;
		float MaxHPRegenTime;
		float MoveSpeed;
		BaseStatStruct(const float MaxHP, const float MaxHPRegenTime, const float MoveSpeed) : MaxHP(MaxHP), MaxHPRegenTime(MaxHPRegenTime), MoveSpeed(MoveSpeed) {}
	}BaseStat;
	struct PlayerStatusStruct : CharStatusStruct {
		int currentWeapon = 0;
		float WeaponCD = 0;
		float HPRegenCD = 0;
		int WeaponAmmo[WeaponCount];
		int WeaponReserveAmmo[WeaponCount];
		WeaponStruct weapon[WeaponCount];
		void Update(const float);
		void ResetWeapon(const int weapon = -1);
	}PStatus;

	Player(Game * g, int PlayerModel, int id, int controlnum, int team, const BaseStatStruct BaseStat, const sf::Vector2f pos);
	~Player();
};

