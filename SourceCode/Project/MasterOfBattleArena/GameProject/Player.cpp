#include "Player.h"
#include "SceneGame.h"

Player::Player(Game* g, int PlayerModel, int id, int controlnum, int team, const BaseStatStruct BaseStat,const sf::Vector2f pos) : BaseStat(BaseStat)
{
	ObjectType = PlayerObj;
	ControlNum = controlnum;
	ID = SceneGame::UID++;
	PlayerID = id;
	game = g;
	Team = team;
	CharStatus = &PStatus;

	//Status
	PStatus.weapon[0] = game->GameResource->WeaponPreset[game->playdata.playerinfo[id].weapon];
	PStatus.MaxHP = BaseStat.MaxHP;
	PStatus.HP = BaseStat.MaxHP;
	PStatus.movespeed = BaseStat.MoveSpeed;

	if (ID == 0)
		PlayerColor = sf::Color::Red;
	else if (ID == 1)
		PlayerColor = sf::Color::Blue;
	else if (ID == 2)
		PlayerColor = sf::Color::Green;
	else if (ID == 3)
		PlayerColor = sf::Color::Yellow;
	//LoadResource=============================
	playertexture.loadFromFile("Resources\\Texture\\Actor1.png");
	this->PlayerModel = PlayerModel * 4;
	for (int i = 0; i < WeaponCount; i++)
	{
		if (PStatus.weapon[i].Icon == 0) {
			//weapontexture[i][0].loadFromFile("Resources\\Texture\\WeaponIcon0.png");
		}
		weapontexture[i][1] = *game->GameResource->weapontexture[PStatus.weapon[i].Model];
		if (PStatus.weapon[i].ProjectileModel == 0) {
			weapontexture[i][2].loadFromFile("Resources\\Texture\\Projectile0.png");
		}
	}
	//=========================================
	Transfrom.width = 48;
	Transfrom.height = 48;

	AS = AnimatedSprite(&playertexture, 48, 48, 3, 1, 2, 0.0f, true);
	AS.Sprite.setOrigin(24, 48);
	WeaponSprite.setTexture(weapontexture[0][1], true);
	WeaponSprite.setOrigin(WeaponSprite.getTextureRect().width/2.0f, WeaponSprite.getTextureRect().height / 2.0f);
	PStatus.ResetWeapon();
	SetPosition(pos);
}


Player::~Player()
{
}

bool Player::Update(const float deltatime)
{
	//Die
	if (PStatus.HP == 0) {
		Die = true;
	}
	if (!Die) {
		//Control========================
		GetControl();
		//Status=========================
		PStatus.Update(deltatime);
		//Move===========================
		SetPosition(GGEZ::MoveTo(GetPosition(), GetPosition() + sf::Vector2f(PStatus.movex, PStatus.movey), PStatus.movespeed*deltatime));
		SetPosition(GetPosition() + (PStatus.speed * deltatime));
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
		//Actions========================
		if (Action[Fire]) {
			if (PStatus.WeaponCD <= 0) {
				float Angle = 0;
				if (PStatus.dir == 0) {
					Angle = 90;
				}
				if (PStatus.dir == 1) {
					Angle = 180;
				}
				if (PStatus.dir == 2) {
					Angle = 0;
				}
				if (PStatus.dir == 3) {
					Angle = -90;
				}
				
				sf::Vector2f pos;
				pos.x = PStatus.dir == 1 ? Transfrom.left : (PStatus.dir == 2 ? Transfrom.left + Transfrom.width : Transfrom.left + (Transfrom.width / 2));
				pos.y = PStatus.dir == 3 ? Transfrom.top : (PStatus.dir == 0 ? Transfrom.top + Transfrom.height : Transfrom.top + (Transfrom.height / 2));
				for (size_t i = 0; i < PStatus.weapon[PStatus.currentWeapon].shot; i++)
				{
					float NewAngle = Angle;
					NewAngle += -(PStatus.weapon[PStatus.currentWeapon].Spread / 2.0f) + GGEZ::Rand()*PStatus.weapon[PStatus.currentWeapon].Spread;
					NewAngle *= 0.01745329f;
					sf::Vector2f dir(cos(NewAngle),sin(NewAngle));
					((SceneGame*)(game->scene))->newobjectlist.push_back(((GameObject*)(new Bullet(game, this, Team, pos, dir, &PStatus.weapon[PStatus.currentWeapon], &weapontexture[PStatus.currentWeapon][2],(float)(i+1) / (float)PStatus.weapon[PStatus.currentWeapon].shot))));
				}
				PStatus.WeaponCD = 1.0f / PStatus.weapon[PStatus.currentWeapon].Firerate;
				sf::Vector2f soundpos(GetPosition());
				game->sound.PlaySound3D(soundpos.x,soundpos.y,(SoundEngine::SoundEnum)PStatus.weapon->WeaponID,40.0f);
			}
		}
		//Buff===========================
		PStatus.Status.attacking = PStatus.WeaponCD > 0;
		//Draw===========================
		if (PStatus.Status.walking) {
			AS.FramePerSecound = 20.0f / PStatus.movespeed;
			AS.SetSprite(PStatus.dir + PlayerModel);
		}
		else
		{
			AS.Time = 99;
			AS.FramePerSecound = 0.0f;
			AS.SetSprite(PStatus.dir + PlayerModel, 1);
		}
		AS.Update(deltatime);
		if (PStatus.dir == 0) {
			WeaponSprite.setPosition(Transfrom.left + (Transfrom.width / 2), Transfrom.top + (Transfrom.height * 0.9f));
			WeaponSprite.setScale(0.5f, 1);
			WeaponSprite.setRotation(90);
		}
		else if (PStatus.dir == 1) {
			WeaponSprite.setPosition(Transfrom.left + (Transfrom.width / 2) - 10, Transfrom.top + (Transfrom.height * 0.66f));
			WeaponSprite.setScale(-1, 1);
			WeaponSprite.setRotation(0);
		}
		else if (PStatus.dir == 2) {
			WeaponSprite.setPosition(Transfrom.left + (Transfrom.width / 2) + 10, Transfrom.top + (Transfrom.height * 0.66f));
			WeaponSprite.setScale(1, 1);
			WeaponSprite.setRotation(0);
		}
		else if (PStatus.dir == 3) {
			WeaponSprite.setScale(0, 0);
		}
	}
	return false;
}

void Player::Draw(sf::RenderWindow* w, const bool UI) {
	if (!Die) {
		if (UI) {
			sf::Vector2f p(Transfrom.left, Transfrom.top);

			sf::RectangleShape HealthBar(sf::Vector2f(Transfrom.width - 2.0f, 8.0f));
			HealthBar.setPosition(p - sf::Vector2f(0.0f, 17.0f));
			sf::RectangleShape HealthBarBG(HealthBar);

			sf::RectangleShape HealthBarBoarder(sf::Vector2f(Transfrom.width + 2.0f, 12.0f));

			HealthBar.setFillColor(sf::Color::Green);
			HealthBar.setScale((PStatus.HP / PStatus.MaxHP), 1.0f);

			HealthBarBG.setFillColor(sf::Color(128, 128, 128, 255));

			HealthBarBoarder.setFillColor(sf::Color::Black);
			HealthBarBoarder.setPosition(p - sf::Vector2f(2.0f, 19.0f));

			w->draw(HealthBarBoarder);
			w->draw(HealthBarBG);
			w->draw(HealthBar);
		}
		else
		{
			//Draw CircleShape
			sf::CircleShape cs(Transfrom.width / 2);
			cs.setFillColor(PlayerColor - sf::Color(0, 0, 0, 150));
			cs.scale(1, 0.3f);
			cs.setPosition(Transfrom.left, Transfrom.top + (Transfrom.height*(0.85f)));
			w->draw(cs);
			AS.Sprite.setRotation(0);
			if (PStatus.dir == 0) {
				w->draw(AS.Sprite);
				w->draw(WeaponSprite);
			}
			else
			{
				w->draw(WeaponSprite);
				w->draw(AS.Sprite);
			}
		}
	}
	else if (!UI)
	{
		AS.Sprite.setRotation(90.0f);
		sf::CircleShape cs(Transfrom.width / 2);
		cs.setFillColor(PlayerColor - sf::Color(0, 0, 0, 150));
		cs.scale(1, 0.3f);
		cs.setPosition(Transfrom.left, Transfrom.top + (Transfrom.height*(0.85f)));
		w->draw(cs);
		w->draw(AS.Sprite);
	}
}

void Player::SetPosition(sf::Vector2f Pos)
{
	Pos -= sf::Vector2f((Transfrom.width / 2), Transfrom.height);
	Transfrom.left = Pos.x;
	Transfrom.top = Pos.y;
	AS.Sprite.setPosition(Transfrom.left + (Transfrom.width / 2), Transfrom.top + Transfrom.height);
}

sf::Vector2f Player::GetPosition()
{
	return AS.Sprite.getPosition();
}

void Player::GetControl()
{
	for (int i = 0; i <= 10; i++)
	{
		Action[i] = false;
		if (ControlNum < 4) {
			if (sf::Keyboard::isKeyPressed((sf::Keyboard::Key)(game->GameResource->Keybind[ControlNum][i]))) {
				Action[i] = true;
			}
		}
		else {
			if (i < 4)
				i = 4;
			if (sf::Joystick::isButtonPressed(ControlNum - 4, (sf::Keyboard::Key)(game->GameResource->Keybind[ControlNum][i])))
			{
				Action[i] = true;
			}
		}
	}
	if (ControlNum < 4) {
		PStatus.movex = (Action[Left] ? -1.0f : 0.0f) + (Action[Right] ? 1.0f : 0.0f);
		PStatus.movey = (Action[Up] ? -1.0f : 0.0f) + (Action[Down] ? 1.0f : 0.0f);
	}
	else
	{
		PStatus.movex = sf::Joystick::getAxisPosition(ControlNum - 4, sf::Joystick::X) / 100.0f;
		if (PStatus.movex < 0.1f&&PStatus.movex > -0.1f) {
			PStatus.movex = 0;
		}
		PStatus.movey = sf::Joystick::getAxisPosition(ControlNum - 4, sf::Joystick::Y) / 100.0f;
		if (PStatus.movey < 0.1f&&PStatus.movey > -0.1f) {
			PStatus.movey = 0;
		}
	}
	if (PStatus.movey != 0)
		PStatus.dir = (PStatus.movey < 0) ? 3 : 0;
	if (PStatus.movex != 0)
		PStatus.dir = (PStatus.movex < 0) ? 1 : 2;
}

void Player::PlayerStatusStruct::Update(const float deltatime)
{
	WeaponCD -= deltatime;
	HPRegenCD -= deltatime;
	if (HPRegenCD <= 0) {
		HP += MaxHP * deltatime * -HPRegenCD / 100.0f;
	}
	CharStatusStruct::Update(deltatime);
}

void Player::PlayerStatusStruct::ResetWeapon(const int weaponid)
{
	if (weaponid < 0) {
		for (int i = 0; i < WeaponCount; i++)
		{
			WeaponAmmo[i] = weapon[i].Ammo;
			WeaponReserveAmmo[i] = weapon[i].ReserveAmmo;
		}
	}
	else
	{
		WeaponAmmo[weaponid] = weapon[weaponid].Ammo;
		WeaponReserveAmmo[weaponid] = weapon[weaponid].ReserveAmmo;
	}
}
