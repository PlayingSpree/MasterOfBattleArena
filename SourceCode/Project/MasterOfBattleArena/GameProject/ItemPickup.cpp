#include "ItemPickup.h"
#include "SceneGame.h"
#include "Player.h"
#include "Enemy.h"
#include "Effect.h"

ItemPickup::ItemPickup(Game * g, float x, float y, PickUpEnum pue) : AS(pue < CrystalPos ? g->GameResource->ItemTextture : g->GameResource->CrystalTextture, 48, pue < CrystalPos ? 48 : 96, 3, 0, pue < CrystalPos ? pue : (((pue - CrystalPos) * 2) + 1), 0.2f, true)
{
	ObjectType = GameObject::PickupObj;
	game = g;
	Transfrom.left = x;
	Transfrom.top = y;
	Transfrom.width = 48;
	Transfrom.height = 48;
	AS.Sprite.setOrigin(24, 48);
	Item = pue;
	Time = 30.0f;
	if (pue >= CrystalPos) {
		if (pue == CrystalPos + 5)
			Item = IPE_CrystalRed;
		else if (pue == CrystalPos + 4)
			Item = IPE_CrystalPurple;
		else if (pue == CrystalPos + 6)
			Item = IPE_CrystalYellow;
		else if (pue == CrystalPos + 0)
			Item = IPE_CrystalWhite;
		else if (pue == CrystalPos + 1)
			Item = IPE_CrystalGreen;
		else if (pue == CrystalPos + 2)
			Item = IPE_CrystalCyan;
		else if (pue == CrystalPos + 3)
			Item = IPE_CrystalBlue;
		else if (pue == CrystalPos + 7)
			Item = IPE_CrystalRainbow;
		Time = 10.0f;
		AS.Sprite.setScale(0.5f, 0.5f);
		AS.Sprite.setPosition(Transfrom.left + (Transfrom.width / 2), Transfrom.top + (Transfrom.height / 2.0f));
		SpriteOffset = 1;
	}
	else
	{
		AS.Sprite.setPosition(Transfrom.left + (Transfrom.width / 2), Transfrom.top + Transfrom.height);
	}
}

sf::Vector2f ItemPickup::GetPosition()
{
	return sf::Vector2f(Transfrom.left, Transfrom.top + Transfrom.height);
}

bool ItemPickup::Update(const float deltatime)
{
	Time -= deltatime;
	if (Time <= 0) {
		return true;
	}
	AS.Update(deltatime);
	std::vector<GameObject*>* playerVector = &((SceneGame*)game->scene)->ObjectByTypeMapVector[GameObject::PlayerObj];
	for (int i = 0; i < playerVector->size(); i++)
	{
		if (!((Player*)(*playerVector)[i])->Die) {
			if ((*playerVector)[i]->Transfrom.intersects(Transfrom)) {
				if (Item == IPE_Health) {
					if (((Player*)(*playerVector)[i])->PStatus.HP >= ((Player*)(*playerVector)[i])->PStatus.MaxHP) {
						unsigned int bonus = 3000U;
						game->playdata.Score += bonus;
						(((SceneGame*)(game->scene))->FloatingTextList.push_back(SceneGame::FloatingText(std::string("Health Full +") + std::to_string(bonus), AS.Sprite.getPosition() - sf::Vector2f(0, AS.Sprite.getGlobalBounds().height), SceneGame::FloatingText::FloatUp, game->GameResource, 30)));
					}
					else
					{
					((Player*)(*playerVector)[i])->PStatus.HP = ((Player*)(*playerVector)[i])->PStatus.MaxHP;
					(((SceneGame*)(game->scene))->FloatingTextList.push_back(SceneGame::FloatingText("Health Restored", AS.Sprite.getPosition() - sf::Vector2f(0, AS.Sprite.getGlobalBounds().height), SceneGame::FloatingText::FloatUp, game->GameResource, 30)));
					}
					game->sound.PlaySound3D(GetPosition().x, GetPosition().y, SoundEngine::SE_Item_Heal, 100.0f);
					((SceneGame*)(game->scene))->newobjectlist.push_back(((GameObject*)(new Effect(game, ((*playerVector)[i])->GetPosition().x, ((*playerVector)[i])->GetPosition().y, Effect::ItemHeal, -1))));
				}
				else if (Item == IPE_Bomb)
				{
					std::vector<GameObject*>* EnemyVector = &((SceneGame*)game->scene)->ObjectByTypeMapVector[GameObject::EnemyObj];
					for (int i = 0; i < EnemyVector->size(); i++) {
						if (GGEZ::LengthVector2f(GetPosition() - (*EnemyVector)[i]->GetPosition()) < 500.0f) {
							Enemy* c = (Enemy*)(*EnemyVector)[i];
							float dmgd = c->BaseStat.MaxHP*0.25f + c->EStatus.HP*0.25f + 100.0f;
							c->CharStatus->speed += GGEZ::NormalizeVector2f((*EnemyVector)[i]->GetPosition() - GetPosition())*500.0f;
							//Do dmg
							c->CharStatus->DoHP(-dmgd);
							//Show Dmg
							char s[6];
							_itoa_s(ceilf(dmgd), s, 10);
							(((SceneGame*)(game->scene))->FloatingTextList.push_back(SceneGame::FloatingText(s, c->AS.Sprite.getPosition() - sf::Vector2f(0, c->AS.Sprite.getGlobalBounds().height), SceneGame::FloatingText::DamageWhite, game->GameResource, dmgd)));
						}
					}
					game->sound.PlaySound3D(GetPosition().x, GetPosition().y, SoundEngine::SE_Item_Explode, 100.0f);
					((SceneGame*)(game->scene))->newobjectlist.push_back(((GameObject*)(new Effect(game, GetPosition().x, GetPosition().y, Effect::ItemExplosion, -1))));
					(((SceneGame*)(game->scene))->FloatingTextList.push_back(SceneGame::FloatingText("Explosion", AS.Sprite.getPosition() - sf::Vector2f(0, AS.Sprite.getGlobalBounds().height), SceneGame::FloatingText::FloatUp, game->GameResource, 30)));
				}
				else if (Item >= CrystalPos)
				{
					unsigned int bonus = 25U;
					if (Item == IPE_CrystalRed)
						bonus = 2400U;
					else if (Item == IPE_CrystalPurple)
						bonus = 1600U;
					else if (Item == IPE_CrystalYellow)
						bonus = 3200U;
					else if (Item == IPE_CrystalWhite)
						bonus = 100U;
					else if (Item == IPE_CrystalGreen)
						bonus = 200U;
					else if (Item == IPE_CrystalCyan)
						bonus = 400U;
					else if (Item == IPE_CrystalBlue)
						bonus = 800U;
					else if (Item == IPE_CrystalRainbow)
						bonus = 5000U;
					game->playdata.Score += bonus;
					(((SceneGame*)(game->scene))->FloatingTextList.push_back(SceneGame::FloatingText(std::string("+") + std::to_string(bonus), AS.Sprite.getPosition() - sf::Vector2f(0, AS.Sprite.getGlobalBounds().height), SceneGame::FloatingText::FloatUp, game->GameResource, 30)));
					game->sound.PlaySound3D(GetPosition().x, GetPosition().y, SoundEngine::SE_Item_Crystal, 50.0f);
				}
				return true;
			}
		}
	}
	return false;
}

void ItemPickup::Draw(sf::RenderWindow * w, const bool UI)
{
	if (!UI) {
		if (Item >= CrystalPos) {
			if (Time < 1) {
				SpriteOffset = ((int)((Time) * 16) % 2);
			}
			else if (Time < 3) {
				SpriteOffset = ((int)((Time) * 8) % 2);
			}
			else if (Time < 5) {
				SpriteOffset = ((int)((Time) * 4) % 2);
			}
			AS.SetSprite(SpriteOffset + ((Item - CrystalPos) * 2));
			w->draw(AS.Sprite);
		}
		else
		{
			if (Time < 1) {
				if ((int)((Time) * 16) % 2 == 0)
					w->draw(AS.Sprite);
			}
			else if (Time < 3) {
				if ((int)((Time) * 8) % 2 == 0)
					w->draw(AS.Sprite);
			}
			else if (Time < 5) {
				if ((int)((Time) * 4) % 2 == 0)
					w->draw(AS.Sprite);
			}
			else
			{
				w->draw(AS.Sprite);
			}
		}
	}
}

ItemPickup::~ItemPickup()
{

}