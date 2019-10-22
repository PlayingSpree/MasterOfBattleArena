#include "Effect.h"
#include "Enemy.h"

Effect::Effect(Game * g, const float x, const float y, const EffectEnum ee, const int arg) : EE(ee)
{
	ObjectType = GameObject::EffectObj;
	game = g;
	if (ee == DeadEnemy) {
		int SpriteOffset = arg % 8;
		Enemy::EnemyTextureEnum ete = arg < 8 ? Enemy::EnemyTextureEnum::ETE_Evil : Enemy::EnemyTextureEnum::ETE_Monster;
		AS = AnimatedSprite(Enemy::EnemyTextureMap[ete], 48, 48, 3, 1, SpriteOffset * 4, 0.0f, true);
		AS.Sprite.setOrigin(24, 48);
		AS.Sprite.setRotation(90.0f);
		AS.Sprite.setPosition(x, y);
		Time = 1.0f;
		Fading = Time;
		UI = false;
	}
	if (ee == FrogExplosion) {
		AS = AnimatedSprite(g->GameResource->Effecttexture[0], 192, 192, 5, 0, 0, 0.101f, false, 5);
		AS.Sprite.setOrigin(96, 96);
		AS.Sprite.setPosition(x, y);
		AS.Sprite.setScale(1.0f, 1.0f);
		Time = 1.2f;
		UI = true;
	}
	if (ee == ItemExplosion) {
		AS = AnimatedSprite(g->GameResource->Effecttexture[1], 192, 192, 5, 11, 0, 0.101f, false, 6);
		AS.Sprite.setColor(sf::Color(255, 255, 255, 128));
		AS.Sprite.setOrigin(96, 96);
		AS.Sprite.setPosition(x, y);
		AS.Sprite.setScale(5.0f, 5.0f);
		Time = 1.9f;
		UI = true;
	}
	if (ee == ItemHeal) {
		AS = AnimatedSprite(g->GameResource->Effecttexture[3], 192, 192, 5, 9, 0, 0.101f, false, 6);
		AS.Sprite.setColor(sf::Color(255, 255, 255, 128));
		AS.Sprite.setOrigin(96, 96);
		AS.Sprite.setPosition(x, y);
		AS.Sprite.setScale(0.5f, 0.5f);
		Time = 0.9f;
		UI = true;
	}
	if (ee == Map2Spawnpoint) {
		AS = AnimatedSprite(g->GameResource->Effecttexture[2], 48, 48, 0, 0, arg == 1 ? 0:1, arg == 1 ? 5.0f : 2.5f, true);
		AS.Sprite.setColor(sf::Color(255, 255, 255, 128));
		AS.Sprite.setOrigin(0, 0);
		AS.Sprite.setPosition(x-24, y-24);
		AS.Sprite.setScale(1.0f, 1.0f);
		Time = arg == 1 ? 1.0f:0.0f;
		UI = false;
		Fading = arg == 1 ? Time:0;
		PosOffset = -10000.0f;
	}
	if (Time != 0.0f) {
		havelife = true;
	}
}

sf::Vector2f Effect::GetPosition()
{
	return sf::Vector2f(AS.Sprite.getPosition().x, AS.Sprite.getPosition().y + PosOffset);
}

bool Effect::Update(const float deltatime)
{
	if (havelife) {
		Time -= deltatime;
		if (Time <= 0) {
			return true;
		}
	}
	if (Fading>0) {
		AS.Sprite.setColor(sf::Color(255, 255, 255, 255 * Fading * Time));
	}
	AS.Update(deltatime);
	return false;
}

void Effect::Draw(sf::RenderWindow * w, const bool ui)
{
	if (UI == ui) {
		w->draw(AS.Sprite);
	}
}

Effect::~Effect()
{
}
