#pragma once
class GameObject
{
public:
	enum ObjectTypeEnum
	{
		PlayerObj,
		BulletObj,
		EnemyObj,
		SpawnerObj,
		PickupObj,
		EffectObj
	}ObjectType;
	unsigned int ID;
	sf::FloatRect Transfrom;
	virtual void Draw(sf::RenderWindow* w, const bool UI) = 0;
	virtual sf::Vector2f GetPosition() = 0;
	virtual bool Update(const float deltatime) = 0;
};