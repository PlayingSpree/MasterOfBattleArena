#include "GameResources.h"
#include <iostream>
#include <fstream>

void GameResources::SaveSetting(bool ResetToDefault)
{
	std::ofstream file;
	file.open("setting.ggez");
	//Default Setting
	if (ResetToDefault) {
		SettingMap[SE_WindowMode] = SettingChoice::WMfullscreen;
		SettingMap[SE_FrameLimit] = SettingChoice::FLVSync;
		SettingMap[SE_Music] = SettingChoice::MusicOn;
		SettingMap[SE_Sound] = SettingChoice::SoundOn;
		SettingMap[SE_MusicPack] = SettingChoice::Heroes;

		//Keybind
		const int KBPlayer1[] = {
		sf::Keyboard::W,
		sf::Keyboard::A,
		sf::Keyboard::S,
		sf::Keyboard::D,
		sf::Keyboard::LControl,
		sf::Keyboard::LAlt,
		sf::Keyboard::F,
		sf::Keyboard::Q,
		sf::Keyboard::E,
		sf::Keyboard::R,
		sf::Keyboard::LShift
		};
		Keybind[0] = std::vector<int>(KBPlayer1, KBPlayer1 + sizeof(KBPlayer1) / sizeof(KBPlayer1[0]));
		const int KBPlayer2[] = {
		sf::Keyboard::I,
		sf::Keyboard::J,
		sf::Keyboard::K,
		sf::Keyboard::L,
		sf::Keyboard::H,
		sf::Keyboard::Y,
		sf::Keyboard::SemiColon,
		sf::Keyboard::U,
		sf::Keyboard::O,
		sf::Keyboard::P,
		sf::Keyboard::RAlt
		};
		Keybind[1] = std::vector<int>(KBPlayer2, KBPlayer2 + sizeof(KBPlayer2) / sizeof(KBPlayer2[0]));
		const int KBPlayer3[] = {
		sf::Keyboard::Up,
		sf::Keyboard::Left,
		sf::Keyboard::Down,
		sf::Keyboard::Right,
		sf::Keyboard::Delete,
		sf::Keyboard::End,
		sf::Keyboard::PageDown,
		sf::Keyboard::Insert,
		sf::Keyboard::Home,
		sf::Keyboard::PageUp,
		sf::Keyboard::RControl
		};
		Keybind[2] = std::vector<int>(KBPlayer3, KBPlayer3 + sizeof(KBPlayer3) / sizeof(KBPlayer3[0]));
		const int KBPlayer4[] = {
		sf::Keyboard::Numpad8,
		sf::Keyboard::Numpad4,
		sf::Keyboard::Numpad5,
		sf::Keyboard::Numpad6,
		sf::Keyboard::Numpad1,
		sf::Keyboard::Numpad2,
		sf::Keyboard::Numpad3,
		sf::Keyboard::Numpad7,
		sf::Keyboard::Numpad9,
		sf::Keyboard::Add,
		sf::Keyboard::Numpad0
		};
		Keybind[3] = std::vector<int>(KBPlayer4, KBPlayer4 + sizeof(KBPlayer4) / sizeof(KBPlayer4[0]));
		const int JSPlayer[] = { 12,14,13,15,6,7,2,3,1,0,5 };
		for (int i = 4; i < 8; i++) {
			Keybind[i] = std::vector<int>(JSPlayer, JSPlayer + sizeof(JSPlayer) / sizeof(JSPlayer[0]));
		}
	}
	for (int i = 0; i <= (int)SE_KeybindJS4; i++)
	{
		if (i >= SettingEnum::SE_KeybindKB1&&i <= SettingEnum::SE_KeybindJS4) {
			for (int j = 0; j <= 10; j++) {
				if (j == 10 && i == SettingEnum::SE_KeybindJS4) {
					file << Keybind[i - SettingEnum::SE_KeybindKB1][j];
				}
				else
				{
					file << Keybind[i - SettingEnum::SE_KeybindKB1][j] << std::endl;
				}
			}
		}
		else
			file << SettingMap[(SettingEnum)i] << std::endl;
	}
	file.close();
}

void GameResources::LoadSetting()
{
	//ResetSetting
	SettingMap.clear();
	for (int i = 0; i < 8; i++)
	{
		Keybind[i].clear();
	}
	//Load Setting File
	std::string line;
	std::ifstream file;
	file.open("setting.ggez", std::ios::in);

	if (!file)
	{
		SaveSetting(true);
	}
	else
	{
		for (int i = 0; i <= (int)SE_KeybindJS4; i++)
		{
			std::getline(file, line);
			if (i >= SettingEnum::SE_KeybindKB1&&i <= SettingEnum::SE_KeybindJS4) {
				for (int j = 0; j <= 10; j++) {
					if (j != 0)
						std::getline(file, line);
					Keybind[i - SettingEnum::SE_KeybindKB1].push_back(std::stoi(line));
				}
			}
			else
				SettingMap[(SettingEnum)i] = std::stoi(line);
		}
	}
	file.close();
}

void GameResources::CreateWindow(sf::RenderWindow* window)
{
	//Windows Setting
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	if (SettingMap[SE_WindowMode] == SettingChoice::WMwindow) {
		window->create(sf::VideoMode(800, 600), "MOBA:Master Of Battle Arena", sf::Style::Default, settings);

	}
	else if (SettingMap[SE_WindowMode] == SettingChoice::WMborderless) {
		window->create(desktop, "MOBA:Master Of Battle Arena", sf::Style::None, settings);
	}
	else if (SettingMap[SE_WindowMode] == SettingChoice::WMfullscreen) {
		window->create(desktop, "MOBA:Master Of Battle Arena", sf::Style::Fullscreen, settings);
	}
	SetFrameLimit(window);
	sf::Image i;
	i.loadFromFile("Resources/Icon.png");
	window->setIcon(48,48,i.getPixelsPtr());
}

void GameResources::SetFrameLimit(sf::RenderWindow* window) {
	if (SettingMap[SE_FrameLimit] == SettingChoice::FLVSync) {
		window->setFramerateLimit(0);
		window->setVerticalSyncEnabled(true);
	}
	else
	{
		unsigned int limit = 0;
		if (SettingMap[SE_FrameLimit] == SettingChoice::FLOFF) {
			limit = 0;
		}
		else if (SettingMap[SE_FrameLimit] == SettingChoice::FL30) {
			limit = 30;
		}
		else if (SettingMap[SE_FrameLimit] == SettingChoice::FL60) {
			limit = 60;
		}
		else if (SettingMap[SE_FrameLimit] == SettingChoice::FL72) {
			limit = 72;
		}
		else if (SettingMap[SE_FrameLimit] == SettingChoice::FL120) {
			limit = 120;
		}
		else if (SettingMap[SE_FrameLimit] == SettingChoice::FL144) {
			limit = 144;
		}
		else if (SettingMap[SE_FrameLimit] == SettingChoice::FL300) {
			limit = 300;
		}
		window->setFramerateLimit(limit);
		window->setVerticalSyncEnabled(false);
	}
}

GameResources::GameResources()
{
	LoadSetting();
	//Weapon===================================
	WeaponPreset[ak47].WeaponID = ak47;
	WeaponPreset[ak47].WeaponType = WeaponStruct::AutoGun;
	WeaponPreset[ak47].Icon = ak47;
	WeaponPreset[ak47].Model = ak47;
	WeaponPreset[ak47].ProjectileModel = 0;
	WeaponPreset[ak47].ProjectileSpeed = 2000.0f;
	WeaponPreset[ak47].ProjectileFalloff = 0.2f;
	WeaponPreset[ak47].Damage = 30.0f;
	WeaponPreset[ak47].Firerate = 7.0f;
	WeaponPreset[ak47].Range = 540.0f;
	WeaponPreset[ak47].Ammo = 30;
	WeaponPreset[ak47].ReserveAmmo = 180;
	WeaponPreset[ak47].Spread = 4.0f;
	WeaponPreset[ak47].KnockBack = 100.0f;
	WeaponPreset[awp].WeaponID = awp;
	WeaponPreset[awp].WeaponType = WeaponStruct::AutoGun;
	WeaponPreset[awp].Icon = awp;
	WeaponPreset[awp].Model = awp;
	WeaponPreset[awp].ProjectileModel = 0;
	WeaponPreset[awp].ProjectileSpeed = 2500.0f;
	WeaponPreset[awp].ProjectileFalloff = 0.2f;
	WeaponPreset[awp].Damage = 400.0f;
	WeaponPreset[awp].Firerate = 0.5f;
	WeaponPreset[awp].Range = 1200.0f;
	WeaponPreset[awp].Ammo = 5;
	WeaponPreset[awp].ReserveAmmo = 15;
	WeaponPreset[awp].Spread = 0.0f;
	WeaponPreset[awp].KnockBack = 500.0f;
	WeaponPreset[g3sg1].WeaponID = g3sg1;
	WeaponPreset[g3sg1].WeaponType = WeaponStruct::AutoGun;
	WeaponPreset[g3sg1].Icon = g3sg1;
	WeaponPreset[g3sg1].Model = g3sg1;
	WeaponPreset[g3sg1].ProjectileModel = 0;
	WeaponPreset[g3sg1].ProjectileSpeed = 2200.0f;
	WeaponPreset[g3sg1].ProjectileFalloff = 0.3f;
	WeaponPreset[g3sg1].Damage = 80.0f;
	WeaponPreset[g3sg1].Firerate = 2.5f;
	WeaponPreset[g3sg1].Range = 820.0f;
	WeaponPreset[g3sg1].Ammo = 10;
	WeaponPreset[g3sg1].ReserveAmmo = 30;
	WeaponPreset[g3sg1].Spread = 0.5f;
	WeaponPreset[g3sg1].KnockBack = 150.0f;
	WeaponPreset[m3].WeaponID = m3;
	WeaponPreset[m3].WeaponType = WeaponStruct::AutoGun;
	WeaponPreset[m3].Icon = m3;
	WeaponPreset[m3].Model = m3;
	WeaponPreset[m3].ProjectileModel = 0;
	WeaponPreset[m3].ProjectileSpeed = 1000.0f;
	WeaponPreset[m3].ProjectileFalloff = 0.1f;
	WeaponPreset[m3].Damage = 25.0f;
	WeaponPreset[m3].Firerate = 1.0f;
	WeaponPreset[m3].Range = 360.0f;
	WeaponPreset[m3].Ammo = 8;
	WeaponPreset[m3].ReserveAmmo = 16;
	WeaponPreset[m3].Spread = 10.0f;
	WeaponPreset[m3].KnockBack = 50.0f;
	WeaponPreset[m3].shot = 10;
	WeaponPreset[m4a1].WeaponID = m4a1;
	WeaponPreset[m4a1].WeaponType = WeaponStruct::AutoGun;
	WeaponPreset[m4a1].Icon = m4a1;
	WeaponPreset[m4a1].Model = m4a1;
	WeaponPreset[m4a1].ProjectileModel = 0;
	WeaponPreset[m4a1].ProjectileSpeed = 2000.0f;
	WeaponPreset[m4a1].ProjectileFalloff = 0.4f;
	WeaponPreset[m4a1].Damage = 25.0f;
	WeaponPreset[m4a1].Firerate = 8.0f;
	WeaponPreset[m4a1].Range = 720.0f;
	WeaponPreset[m4a1].Ammo = 30;
	WeaponPreset[m4a1].ReserveAmmo = 180;
	WeaponPreset[m4a1].Spread = 2.0f;
	WeaponPreset[m4a1].KnockBack = 80.0f;
	WeaponPreset[m249].WeaponID = m249;
	WeaponPreset[m249].WeaponType = WeaponStruct::AutoGun;
	WeaponPreset[m249].Icon = m249;
	WeaponPreset[m249].Model = m249;
	WeaponPreset[m249].ProjectileModel = 0;
	WeaponPreset[m249].ProjectileSpeed = 2000.0f;
	WeaponPreset[m249].ProjectileFalloff = 0.4f;
	WeaponPreset[m249].Damage = 35.0f;
	WeaponPreset[m249].Firerate = 7.0f;
	WeaponPreset[m249].Range = 360.0f;
	WeaponPreset[m249].Ammo = 100;
	WeaponPreset[m249].ReserveAmmo = 200;
	WeaponPreset[m249].Spread = 5.0f;
	WeaponPreset[m249].KnockBack = 120.0f;
	WeaponPreset[p90].WeaponID = p90;
	WeaponPreset[p90].WeaponType = WeaponStruct::AutoGun;
	WeaponPreset[p90].Icon = p90;
	WeaponPreset[p90].Model = p90;
	WeaponPreset[p90].ProjectileModel = 0;
	WeaponPreset[p90].ProjectileSpeed = 2000.0f;
	WeaponPreset[p90].ProjectileFalloff = 0.1f;
	WeaponPreset[p90].Damage = 20.0f;
	WeaponPreset[p90].Firerate = 12.0f;
	WeaponPreset[p90].Range = 360.0f;
	WeaponPreset[p90].Ammo = 50;
	WeaponPreset[p90].ReserveAmmo = 150;
	WeaponPreset[p90].Spread = 4.0f;
	WeaponPreset[p90].KnockBack = 40.0f;
	WeaponPreset[scout].WeaponID = scout;
	WeaponPreset[scout].WeaponType = WeaponStruct::AutoGun;
	WeaponPreset[scout].Icon = scout;
	WeaponPreset[scout].Model = scout;
	WeaponPreset[scout].ProjectileModel = 0;
	WeaponPreset[scout].ProjectileSpeed = 2500.0f;
	WeaponPreset[scout].ProjectileFalloff = 0.2f;
	WeaponPreset[scout].Damage = 200.0f;
	WeaponPreset[scout].Firerate = 1.0f;
	WeaponPreset[scout].Range = 1200.0f;
	WeaponPreset[scout].Ammo = 10;
	WeaponPreset[scout].ReserveAmmo = 30;
	WeaponPreset[scout].Spread = 0.0f;
	WeaponPreset[scout].KnockBack = 100.0f;
	WeaponPreset[sg552].WeaponID = sg552;
	WeaponPreset[sg552].WeaponType = WeaponStruct::AutoGun;
	WeaponPreset[sg552].Icon = sg552;
	WeaponPreset[sg552].Model = sg552;
	WeaponPreset[sg552].ProjectileModel = 0;
	WeaponPreset[sg552].ProjectileSpeed = 2000.0f;
	WeaponPreset[sg552].ProjectileFalloff = 0.3f;
	WeaponPreset[sg552].Damage = 35.0f;
	WeaponPreset[sg552].Firerate = 6.0f;
	WeaponPreset[sg552].Range = 720.0f;
	WeaponPreset[sg552].Ammo = 30;
	WeaponPreset[sg552].ReserveAmmo = 180;
	WeaponPreset[sg552].Spread = 2.0f;
	WeaponPreset[sg552].KnockBack = 80.0f;
	WeaponPreset[ump45].WeaponID = ump45;
	WeaponPreset[ump45].WeaponType = WeaponStruct::AutoGun;
	WeaponPreset[ump45].Icon = ump45;
	WeaponPreset[ump45].Model = ump45;
	WeaponPreset[ump45].ProjectileModel = 0;
	WeaponPreset[ump45].ProjectileSpeed = 2000.0f;
	WeaponPreset[ump45].ProjectileFalloff = 0.3f;
	WeaponPreset[ump45].Damage = 20.0f;
	WeaponPreset[ump45].Firerate = 10.0f;
	WeaponPreset[ump45].Range = 360.0f;
	WeaponPreset[ump45].Ammo = 30;
	WeaponPreset[ump45].ReserveAmmo = 180;
	WeaponPreset[ump45].Spread = 3.0f;
	WeaponPreset[ump45].KnockBack = 50.0f;
	WeaponPreset[xm1014].WeaponID = xm1014;
	WeaponPreset[xm1014].WeaponType = WeaponStruct::AutoGun;
	WeaponPreset[xm1014].Icon = xm1014;
	WeaponPreset[xm1014].Model = xm1014;
	WeaponPreset[xm1014].ProjectileModel = 0;
	WeaponPreset[xm1014].ProjectileSpeed = 2000.0f;
	WeaponPreset[xm1014].ProjectileFalloff = 0.2f;
	WeaponPreset[xm1014].Damage = 12.0f;
	WeaponPreset[xm1014].Firerate = 2.0f;
	WeaponPreset[xm1014].Range = 360.0f;
	WeaponPreset[xm1014].Ammo = 6;
	WeaponPreset[xm1014].ReserveAmmo = 36;
	WeaponPreset[xm1014].Spread = 5.0f;
	WeaponPreset[xm1014].KnockBack = 30.0f;
	WeaponPreset[xm1014].shot = 10;
	//LoadResources=============================
	//WeaponTexture
	weapontexture[0] = new sf::Texture();
	weapontexture[1] = new sf::Texture();
	weapontexture[2] = new sf::Texture();
	weapontexture[3] = new sf::Texture();
	weapontexture[4] = new sf::Texture();
	weapontexture[5] = new sf::Texture();
	weapontexture[6] = new sf::Texture();
	weapontexture[7] = new sf::Texture();
	weapontexture[8] = new sf::Texture();
	weapontexture[9] = new sf::Texture();
	weapontexture[10] = new sf::Texture();
	weapontexture[0]->loadFromFile("Resources\\Texture\\ak47.png");
	weapontexture[1]->loadFromFile("Resources\\Texture\\awp.png");
	weapontexture[2]->loadFromFile("Resources\\Texture\\g3sg1.png");
	weapontexture[3]->loadFromFile("Resources\\Texture\\m3.png");
	weapontexture[4]->loadFromFile("Resources\\Texture\\m4a1.png");
	weapontexture[5]->loadFromFile("Resources\\Texture\\m249.png");
	weapontexture[6]->loadFromFile("Resources\\Texture\\p90.png");
	weapontexture[7]->loadFromFile("Resources\\Texture\\scout.png");
	weapontexture[8]->loadFromFile("Resources\\Texture\\sg552.png");
	weapontexture[9]->loadFromFile("Resources\\Texture\\ump45.png");
	weapontexture[10]->loadFromFile("Resources\\Texture\\xm1014.png");
	//LoadMenuTexture
	MenuTextture[MTE_Title] = new sf::Texture();
	MenuTextture[MTE_Title]->loadFromFile("Resources/MenuTexture/MenuTitle.png");
	MenuTextture[MTE_Title]->setSmooth(true);
	MenuTextture[MTE_TitleBG] = new sf::Texture();
	MenuTextture[MTE_TitleBG]->loadFromFile("Resources/MenuTexture/MenuBackground.jpeg");
	MenuTextture[MTE_TitleBG]->setSmooth(true);
	MenuTextture[MTE_Credit] = new sf::Texture();
	MenuTextture[MTE_Credit]->loadFromFile("Resources/MenuTexture/Credit.png");
	MenuTextture[MTE_Credit]->setSmooth(true);
	MenuTextture[MTE_HowTo] = new sf::Texture();
	MenuTextture[MTE_HowTo]->loadFromFile("Resources/MenuTexture/How to play.png");
	MenuTextture[MTE_HowTo]->setSmooth(true);
	MenuTextture[MTE_EndBG] = new sf::Texture();
	MenuTextture[MTE_EndBG]->loadFromFile("Resources/Texture/Arena2BG.jpg");
	MenuTextture[MTE_EndBG]->setSmooth(true);
	//ItemTexttureEnum
	ItemTextture = new sf::Texture();
	ItemTextture->loadFromFile("Resources/Texture/ItemPickup.png");
	CrystalTextture = new sf::Texture();
	CrystalTextture->loadFromFile("Resources/Texture/Crystal.png");
	//LoadFont
	if (!font.loadFromFile("Resources\\Font\\arial.ttf"))
	{
		printf("Font Load Failed.Check Game Files!");
		abort();
	}
	if (!fontscore.loadFromFile("Resources\\Font\\NotoSansDisplay-Regular.ttf"))
	{
		printf("Font Load Failed.Check Game Files!");
		abort();
	}
	//LoadFace
	facetexture = new sf::Texture();
	facetexture->loadFromFile("Resources/MenuTexture/Actor1.png");
	facetexture->setSmooth(true);
	playertexture = new sf::Texture();
	playertexture->loadFromFile("Resources/Texture/Actor1.png");
	Effecttexture[0] = new sf::Texture();
	Effecttexture[0]->loadFromFile("Resources/Texture/Explosion1.png");
	Effecttexture[1] = new sf::Texture();
	Effecttexture[1]->loadFromFile("Resources/Texture/Explosion2.png");
	Effecttexture[2] = new sf::Texture();
	Effecttexture[2]->loadFromFile("Resources/Texture/SpawnPoint.png");
	Effecttexture[3] = new sf::Texture();
	Effecttexture[3]->loadFromFile("Resources/Texture/Recovery2.png");
}


GameResources::~GameResources()
{
}