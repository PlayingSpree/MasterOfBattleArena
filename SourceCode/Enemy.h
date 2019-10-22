#pragma once
#include "CharObject.h"
class Enemy : public GameObject, public CharObject
{
public:
	//Texture
	enum EnemyTextureEnum {
		ETE_Evil,
		ETE_Monster
	};
	enum EnemySpriteEnum {
		ESE_Fighter,
		ESE_DarkAssassin,
		ESE_OldMan,
		ESE_EvilKnight,
		ESE_Mage,
		ESE_Dragon,
		ESE_Assassin,
		ESE_Fat,
		ESE_Bat,
		ESE_ManBat,
		ESE_Frog,
		ESE_Skeleton,
		ESE_Orc,
		ESE_Ghost,
		ESE_Cow,
		ESE_Succ
	};
	enum EnemyProjectileEnum {
		EPE_FireBallBlue,
		EPE_FireBallRed,
		EPE_Dark,
		EPE_Pink
	};
	static std::map<EnemyTextureEnum, sf::Texture*> EnemyTextureMap;

	sf::Texture* texture;
	int SpriteOffset;
	int Team;
	Game* game;

	bool Update(const float deltatime);
	void Draw(sf::RenderWindow* w,const bool UI);
	void SetPosition(sf::Vector2f);

	sf::Vector2f GetPosition();
	enum TargetSelectTypeEnum
	{
		TSTE_Random,
		TSTE_Closest
	};
	enum AttackTypeEnum
	{
		ATE_Melee,
		ATE_Ranged,
		ATE_Explode
	};
	struct BaseStatStruct
	{
		EnemySpriteEnum TextureEnum;
		float MaxHP;
		float MoveSpeed;
		float AttackSpeed;
		float AttackDamage;
		float KnockBack;
		float Range;
		TargetSelectTypeEnum TargetSelectType;
		AttackTypeEnum AttackType;
		EnemyProjectileEnum EnemyProjectileTexture;
		BaseStatStruct(EnemySpriteEnum TextureEnum,const float MaxHP, const float MoveSpeed,const float AttackSpeed, const float AttackDamage , const float KnockBack,TargetSelectTypeEnum tste, AttackTypeEnum ate, const float Range = 0, EnemyProjectileEnum EnemyProjectileTexture = EPE_FireBallBlue) :TextureEnum(TextureEnum), MaxHP(MaxHP), MoveSpeed(MoveSpeed), AttackSpeed(AttackSpeed), AttackDamage(AttackDamage), KnockBack(KnockBack), TargetSelectType(tste), AttackType(ate), Range(Range) , EnemyProjectileTexture(EnemyProjectileTexture){}
	}BaseStat;
	struct EnemyStatusStruct : CharStatusStruct
	{
		void Update(const float);
		float AttackSpeed;
		float AtackDelay;
		GameObject* Target;
	}EStatus;

	Enemy(Game* game, const int team, const BaseStatStruct basestat);
	~Enemy();
};

