#include "Enemy.h"
#include "SceneGame.h"
#include "ItemPickup.h"
#include "Effect.h"
#include <vector>

std::map<Enemy::EnemyTextureEnum, sf::Texture* > Enemy::EnemyTextureMap;

Enemy::Enemy(Game * g, const int team, const BaseStatStruct basestat) : BaseStat(basestat)
{
	game = g;
	ID = SceneGame::UID++;
	ObjectType = GameObject::EnemyObj;
	CharStatus = &EStatus;

	EStatus.MaxHP = basestat.MaxHP;
	EStatus.HP = basestat.MaxHP;
	EStatus.movespeed = basestat.MoveSpeed;
	EStatus.AttackSpeed = basestat.AttackSpeed;

	if (BaseStat.AttackType == ATE_Ranged) {
		BaseStat.AttackType = ATE_Melee;
	}

	if (basestat.TargetSelectType == TSTE_Closest) {
		std::vector<GameObject*>* v = &((SceneGame*)game->scene)->ObjectByTypeMapVector[GameObject::PlayerObj];
		int c = 0;
		float l = GGEZ::LengthVector2f(GetPosition() - (*v)[0]->GetPosition());
		for (int i = 1; i < v->size(); i++)
		{
			float nl = GGEZ::LengthVector2f(GetPosition() - (*v)[0]->GetPosition());
			if (nl < l) {
				l = nl;
				c = i;
			}
		}
		EStatus.Target = (*v)[c];
	}
	else
	{
		int s = ((SceneGame*)game->scene)->ObjectByTypeMapVector[GameObject::PlayerObj].size();
		int i = 0;
		if (s > 1) {
			i = rand() % s;
		}
		EStatus.Target = ((SceneGame*)game->scene)->ObjectByTypeMapVector[GameObject::PlayerObj][i];
	}
	EnemyTextureEnum ete = basestat.TextureEnum < 8 ? ETE_Evil : ETE_Monster;
	std::map<EnemyTextureEnum, sf::Texture*>::iterator it = EnemyTextureMap.find(ete);
	if (it == EnemyTextureMap.end()) {
		sf::Texture* tx = new sf::Texture();
		if (ete == ETE_Evil)
			tx->loadFromFile("Resources\\Texture\\Evil.png");
		else
		{
			tx->loadFromFile("Resources\\Texture\\Monster.png");
		}
		EnemyTextureMap[ete] = tx;
		texture = tx;
	}
	else
	{
		texture = EnemyTextureMap[ete];
	}
	//Set AS
	Transfrom.width = 48;
	Transfrom.height = 48;

	SpriteOffset = (basestat.TextureEnum % 8) * 4;

	AS = AnimatedSprite(texture, 48, 48, 3, 1, 4 + SpriteOffset, 0.0f, true);
	AS.Sprite.setOrigin(24, 48);
}

Enemy::~Enemy()
{
}

bool Enemy::Update(const float deltatime)
{
	std::vector<GameObject*>* PlayerVector = &((SceneGame*)(game->scene))->ObjectByTypeMapVector[GameObject::PlayerObj];
	//Die
	if (EStatus.HP <= 0) {
		((SceneGame*)(game->scene))->newobjectlist.push_back(((GameObject*)(new Effect(game, GetPosition().x, GetPosition().y, Effect::DeadEnemy, BaseStat.TextureEnum))));
		unsigned int s = (unsigned int)BaseStat.AttackDamage*(1.0f / (BaseStat.AttackSpeed + 1.0f))*((BaseStat.MoveSpeed + 3.0f) / 3.0f)*BaseStat.MaxHP / 20.0f;
		game->playdata.Score += s;

		(((SceneGame*)(game->scene))->FloatingTextList.push_back(SceneGame::FloatingText(std::string("+") + std::to_string(s), AS.Sprite.getPosition() - sf::Vector2f(0, AS.Sprite.getGlobalBounds().height), SceneGame::FloatingText::FloatUp, game->GameResource, 30)));

		if (s > 13333.2f) {
			if (GGEZ::Rand()*20000.0f < 13333.2f) {
				((SceneGame*)(game->scene))->newobjectlist.push_back(((GameObject*)(new ItemPickup(game, Transfrom.left, Transfrom.top, (ItemPickup::PickUpEnum)(rand() % ItemPickup::CrystalPos)))));
				return true;
			}
		}
		else
		{
			if (GGEZ::Rand()*20000.0f < s) {
				((SceneGame*)(game->scene))->newobjectlist.push_back(((GameObject*)(new ItemPickup(game, Transfrom.left, Transfrom.top, (ItemPickup::PickUpEnum)(rand() % ItemPickup::CrystalPos)))));
				return true;
			}
		}
		int clv = 0;
		float chance = 4000.0f;
		for (size_t i = 0; i < 7; i++)
		{
			if (GGEZ::Rand()*chance < s) {
				clv++;
			}
			chance *= 1.4f;
		}
		((SceneGame*)(game->scene))->newobjectlist.push_back(((GameObject*)(new ItemPickup(game, Transfrom.left, Transfrom.top, (ItemPickup::PickUpEnum)(ItemPickup::CrystalPos + clv)))));
		return true;
	}
	//Status=========================
	EStatus.Update(deltatime);
	//Retarget
	if (((Player*)EStatus.Target)->Die)
	{
		int s = ((SceneGame*)game->scene)->ObjectByTypeMapVector[GameObject::PlayerObj].size();
		int i = 0;
		if (s > 1) {
			i = rand() % s;
		}
		EStatus.Target = ((SceneGame*)game->scene)->ObjectByTypeMapVector[GameObject::PlayerObj][i];
	}
	if (BaseStat.TargetSelectType == TSTE_Closest) {
		int c = 0;
		float l = 99999999.99f;
		for (int i = 0; i < PlayerVector->size(); i++)
		{
			if (!((Player*)(*PlayerVector)[i])->Die) {
				float nl = GGEZ::LengthVector2f(GetPosition() - (*PlayerVector)[i]->GetPosition());
				if (nl < l) {
					l = nl;
					c = i;
				}
			}
		}
		EStatus.Target = (*PlayerVector)[c];
	}
	//MoveAI
	sf::Vector2f move = GGEZ::MoveToVector(GetPosition(), EStatus.Target->GetPosition(), EStatus.movespeed*deltatime);
	EStatus.movex = move.x;
	EStatus.movey = move.y;
	EStatus.Status.walking = true;
	//Move
	SetPosition(GetPosition() + sf::Vector2f(EStatus.movex, EStatus.movey));
	SetPosition(GetPosition() + (EStatus.speed * deltatime));
	if (GetPosition().x > 512.0f) {
		SetPosition(sf::Vector2f(512.0f, GetPosition().y));
	}
	if (GetPosition().x < -512.0f) {
		SetPosition(sf::Vector2f(-512.0f, GetPosition().y));
	}
	if (GetPosition().y > 512.0f) {
		SetPosition(sf::Vector2f(GetPosition().x, 512.0f));
	}
	if (GetPosition().y < -512.0f) {
		SetPosition(sf::Vector2f(GetPosition().x, -512.0f));
	}
	//Coll
	std::vector<GameObject*>* ObjectVector = &((SceneGame*)(game->scene))->ObjectByTypeMapVector[GameObject::EnemyObj];
	for (int i = 0; i < ObjectVector->size(); i++)
	{
		if ((*ObjectVector)[i] == this) {
			continue;
		}
		if ((*ObjectVector)[i]->Transfrom.intersects(Transfrom)) {
			Enemy* c = (Enemy*)(*ObjectVector)[i];
			c->CharStatus->speed += GGEZ::NormalizeVector2f(c->GetPosition() - GetPosition())*1000.0f*deltatime;
		}
	}
	//Attack==================================
	EStatus.AtackDelay -= deltatime;

	if (EStatus.AtackDelay <= 0)
		//Melee
		if (BaseStat.AttackType == ATE_Melee) {
			for (int i = 0; i < PlayerVector->size(); i++)
			{
				if (!((Player*)(*PlayerVector)[i])->Die)
					if ((*PlayerVector)[i]->Transfrom.intersects(Transfrom)) {
						Player* c = (Player*)(*PlayerVector)[i];
						c->PStatus.HPRegenCD = c->BaseStat.MaxHPRegenTime;
						float dmgd = BaseStat.AttackDamage;
						c->CharStatus->speed += GGEZ::NormalizeVector2f(move)*BaseStat.KnockBack;
						//Do dmg
						c->CharStatus->DoHP(-dmgd);
						//Show Dmg
						char s[6];
						_itoa_s(ceilf(dmgd), s, 10);
						(((SceneGame*)(game->scene))->FloatingTextList.push_back(SceneGame::FloatingText(s, c->AS.Sprite.getPosition() - sf::Vector2f(0, c->AS.Sprite.getGlobalBounds().height), SceneGame::FloatingText::DamageRed, game->GameResource, dmgd)));
						EStatus.AtackDelay = EStatus.AttackSpeed;
						game->sound.PlaySound3D(GetPosition().x, GetPosition().y, SoundEngine::SE_Game_Hit, 100.0f);
					}
			}
		}
	//Explode
	if (BaseStat.AttackType == ATE_Explode) {
		for (int i = 0; i < PlayerVector->size(); i++)
		{
			if (!((Player*)(*PlayerVector)[i])->Die)
				if ((*PlayerVector)[i]->Transfrom.intersects(Transfrom)) {
					bool Exploded = false;
					for (int i = 0; i < PlayerVector->size(); i++) {
						if (GGEZ::LengthVector2f(GetPosition() - (*PlayerVector)[i]->GetPosition()) < BaseStat.Range) {
							Player* c = (Player*)(*PlayerVector)[i];
							c->PStatus.HPRegenCD = c->BaseStat.MaxHPRegenTime;
							float dmgd = BaseStat.AttackDamage;
							c->CharStatus->speed += GGEZ::NormalizeVector2f((*PlayerVector)[i]->GetPosition() - GetPosition())*BaseStat.KnockBack;
							//Do dmg
							c->CharStatus->DoHP(-dmgd);
							//Show Dmg
							char s[6];
							_itoa_s(ceilf(dmgd), s, 10);
							(((SceneGame*)(game->scene))->FloatingTextList.push_back(SceneGame::FloatingText(s, c->AS.Sprite.getPosition() - sf::Vector2f(0, c->AS.Sprite.getGlobalBounds().height), SceneGame::FloatingText::DamageRed, game->GameResource, dmgd)));
							Exploded = true;
						}
					}
					if (Exploded) {
						game->sound.PlaySound3D(GetPosition().x, GetPosition().y, SoundEngine::SE_Enemy_Explode, 100.0f);
						((SceneGame*)(game->scene))->newobjectlist.push_back(((GameObject*)(new Effect(game, GetPosition().x, GetPosition().y, Effect::FrogExplosion, -1))));
						return true;
					}
				}
		}
	}
	//Draw=====================================
	//Dir
	if (!(move.x == 0 || move.y == 0)) {
		float angle = atan2f(move.y, move.x)*57.2958f;
		if (angle < -135.0f || angle >= 135.0f)
			EStatus.dir = 1;
		else if (angle < -45.0f)
			EStatus.dir = 3;
		else if (angle < 45.0f)
			EStatus.dir = 2;
		else if (angle < 135.0f)
			EStatus.dir = 0;
	}
	//Anim
	if (EStatus.Status.walking) {
		AS.FramePerSecound = 20.0f / EStatus.movespeed;
		AS.SetSprite(SpriteOffset + EStatus.dir);
	}
	else
	{
		AS.Time = 99;
		AS.FramePerSecound = 0.0f;
		AS.SetSprite(SpriteOffset + EStatus.dir, 1);
	}
	AS.Update(deltatime);
	return false;
}

void Enemy::Draw(sf::RenderWindow * w, const bool UI)
{
	if (UI) {
		sf::Vector2f p(Transfrom.left, Transfrom.top);

		sf::RectangleShape HealthBar(sf::Vector2f(Transfrom.width - 2.0f, 8.0f));
		HealthBar.setPosition(p - sf::Vector2f(0.0f, 17.0f));
		sf::RectangleShape HealthBarBG(HealthBar);

		sf::RectangleShape HealthBarBoarder(sf::Vector2f(Transfrom.width + 2.0f, 12.0f));

		HealthBar.setFillColor(sf::Color::Red);
		HealthBar.setScale((EStatus.HP / EStatus.MaxHP), 1.0f);

		HealthBarBG.setFillColor(sf::Color(128, 128, 128, 255));

		HealthBarBoarder.setFillColor(sf::Color::Black);
		HealthBarBoarder.setPosition(p - sf::Vector2f(2.0f, 19.0f));

		w->draw(HealthBarBoarder);
		w->draw(HealthBarBG);
		w->draw(HealthBar);
	}
	else
	{
		w->draw(AS.Sprite);
	}
}

void Enemy::SetPosition(sf::Vector2f Pos)
{
	Pos -= sf::Vector2f((Transfrom.width / 2), Transfrom.height);
	Transfrom.left = Pos.x;
	Transfrom.top = Pos.y;
	AS.Sprite.setPosition(Transfrom.left + (Transfrom.width / 2), Transfrom.top + Transfrom.height);
}

sf::Vector2f Enemy::GetPosition()
{
	return  AS.Sprite.getPosition();
}

void Enemy::EnemyStatusStruct::Update(const float deltatime)
{
	CharStatusStruct::Update(deltatime);
}
