#include "Bullet.h"
#include "SceneGame.h"

Bullet::Bullet(Game* g, const GameObject* owner, const int team, const sf::Vector2f pos, const sf::Vector2f dir, const WeaponStruct * ws, const sf::Texture * tx, const float offset)
{
	Offset = offset;
	game = g;
	ID = SceneGame::UID++;
	ObjectType = GameObject::BulletObj;
	sprite.setTexture(*tx, true);
	sprite.setPosition(pos);
	sprite.setRotation(atan2(dir.y, dir.x) * 57.29578f);
	sprite.setOrigin(0, sprite.getTextureRect().height / 2.0f);
	Distance = 0;
	Time = 0;
	Damage = ws->Damage;
	Falloff = ws->ProjectileFalloff;
	Dir = dir*ws->ProjectileSpeed;
	Range = ws->Range;
	KnockBack = ws->KnockBack;
	rect = sprite.getGlobalBounds();
	pen = 0;
	OwnerType = owner->ObjectType;
	Owner = owner;
	Team = team;
	
}

Bullet::Bullet(Game* g, const GameObject* owner, const int team, const sf::Vector2f pos, const sf::Vector2f dir, const Enemy::BaseStatStruct* ebs, const sf::Texture * tx)
{
	game = g;
	ID = SceneGame::UID++;
	ObjectType = GameObject::BulletObj;
	sprite.setTexture(*tx, true);
	sprite.setPosition(pos);
	sprite.setRotation(atan2(dir.y, dir.x) * 57.29578f);
	sprite.setOrigin(0, sprite.getTextureRect().height / 2.0f);
	Distance = 0;
	Time = 0;
	Damage = ebs->AttackDamage;
	Falloff = 1.0f;
	Dir = dir*100.0f;
	Range = ebs->Range;
	KnockBack = ebs->KnockBack;
	rect = sprite.getGlobalBounds();
	pen = 0;
	OwnerType = owner->ObjectType;
	Owner = owner;
	Team = team;
}

Bullet::~Bullet()
{
}

bool Bullet::checkcoll(const std::vector<GameObject*>* objectvector)
{
	for (int i = 0; i < objectvector->size(); i++)
	{
		GameObject* g = (*objectvector)[i];
		if (g == Owner)
			continue;

		if (g->Transfrom.intersects(rect)) {
			if ((g->ObjectType == GameObject::PlayerObj) || (g->ObjectType == GameObject::EnemyObj)) {
				CharObject* c;
				if (g->ObjectType == GameObject::PlayerObj) {
					if (((Player*)(*objectvector)[i])->Die) {
						continue;
					}
					((Player*)(*objectvector)[i])->PStatus.HPRegenCD = ((Player*)(*objectvector)[i])->BaseStat.MaxHPRegenTime;
					c = (CharObject*)(Player*)g;
				}
				else
					c = (CharObject*)(Enemy*)g;
				if (c->Team != Team) {
					//Check If Already Hit
					bool b = true;
					for (int i = 0; i < ObjectHit.size(); i++)
					{
						if (ObjectHit[i] == (*objectvector)[i]->ID) {
							b = false;
							break;
						}
					}
					if (b) {
						float dmgd = Damage*fminf(fmax(((Range - Distance) / Range)*(1.0f / (1.0f - Falloff)), 0.0f), 1.0f);
						c->CharStatus->speed += GGEZ::NormalizeVector2f(Dir)*KnockBack;
						//Do dmg
						c->CharStatus->DoHP(-dmgd);
						//Show Dmg
						//printf("Hit For %6.2f Damage (Distance %5.2f%% Falloff Modifier %.2f%%).\n", dmgd, (Distance / Range)*100.0f, ((Range - Distance) / Range)*(1.0f / (1.0f - Falloff)*100.0f));
						char s[6];
						_itoa_s(ceilf(dmgd), s, 10);
						(((SceneGame*)(game->scene))->FloatingTextList.push_back(SceneGame::FloatingText(s, c->AS.Sprite.getPosition() - sf::Vector2f(0, c->AS.Sprite.getGlobalBounds().height*Offset), g->ObjectType == GameObject::PlayerObj ? SceneGame::FloatingText::DamageRed : SceneGame::FloatingText::DamageWhite, game->GameResource, dmgd)));
						if (--pen < 0)
							return true;
						ObjectHit.push_back((*objectvector)[i]->ID);
					}
				}
			}
		}
	}
	return false;
}


bool Bullet::Update(const float deltatime)
{
	sprite.move(Dir*deltatime);
	rect = sprite.getGlobalBounds();
	Distance += sqrtf((Dir.x*Dir.x) + (Dir.y*Dir.y))*deltatime;
	sprite.setColor(sf::Color(255, 255, 255, int(fminf(255.0f*(Range - Distance) / Range, 255.0f))));
	if (Distance > Range) {
		return true;
	}
	return Bullet::checkcoll(&(((SceneGame*)(game->scene))->objectvector));
}

void Bullet::Draw(sf::RenderWindow* w, const bool UI)
{
	if (!UI) {
		w->draw(sprite);
	}
}

sf::Vector2f Bullet::GetPosition()
{
	return sprite.getPosition();
}
