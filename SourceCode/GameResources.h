#pragma once
#include <vector>
#include <queue>
#include <map>
#include <string>
#include <thread>
#include <list>
#include <math.h>
#include "AnimatedSprite.h"

struct WeaponStruct
{
	int WeaponID;
	enum WeapontypeEnum
	{
		AutoGun
	}WeaponType;
	int Icon;
	int Model;
	int ProjectileModel;
	float ProjectileSpeed;
	float ProjectileFalloff;
	float Damage;
	float Firerate;
	float Range;
	int Ammo;
	int ReserveAmmo;
	float Spread;
	float KnockBack;
	int shot = 1;
};

class GameResources
{
public:
	enum WeaponPresetEnum
	{
		ak47,
		awp,
		g3sg1,
		m3,
		m4a1,
		m249,
		p90,
		scout,
		sg552,
		ump45,
		xm1014
	};
	WeaponStruct WeaponPreset[11];
	struct SettingChoice
	{
		enum WindowSize
		{
			WS4d3r640x480,
			WS4d3r800x600,
			WS4d3r960x720,
			WS4d3r1024x768,
			WS4d3r1280x960,
			WS4d3r1440x1080,
			WS4d3r1600x1200,
			WS4d3r1920x1440,
			WS16d10r1280x800,
			WS16d10r1440x900,
			WS16d10r1920x1200,
			WS16d10r2560x1600,
			WS16d9r1280x720,
			WS16d9r1366x768,
			WS16d9r1600x900,
			WS16d9r1920x1080,
			WS16d9r2560x1440,
			WS16d9r3840x2160,
			WS21d9r2560x1080
		};
		enum SE_WindowMode
		{
			WMwindow, WMborderless, WMfullscreen
		};
		enum SE_FrameLimit
		{
			FLOFF,FLVSync,FL30, FL60, FL72, FL120, FL144, FL300
		};
		enum SE_Music
		{
			MusicOff, MusicOn
		};
		enum SE_Sound
		{
			SoundOff, SoundOn
		};
		enum SE_MusicPack
		{
			Heroes, Magic
		};
	};
	enum SettingEnum
	{
		SE_WindowSize,
		SE_WindowRatio,
		SE_WindowMode,
		SE_FrameLimit,
		SE_Music,
		SE_Sound,
		SE_MusicPack,
		SE_KeybindKB1,
		SE_KeybindKB2,
		SE_KeybindKB3,
		SE_KeybindKB4,
		SE_KeybindJS1,
		SE_KeybindJS2,
		SE_KeybindJS3,
		SE_KeybindJS4
	};
	std::map<SettingEnum, int> SettingMap;
	std::vector<int> Keybind[8];
	void SaveSetting(bool);
	void LoadSetting();

	sf::Font font;
	sf::Font fontscore;
	//Preload Texture
	enum MenuTexttureEnum
	{
		MTE_Title,
		MTE_TitleBG,
		MTE_Credit,
		MTE_HowTo,
		MTE_EndBG,
		MTE_LastTexture
	};
	sf::Texture* MenuTextture[MTE_LastTexture];
	sf::Texture* ItemTextture;
	sf::Texture* CrystalTextture;
	sf::Texture* weapontexture[11];
	sf::Texture* facetexture;
	sf::Texture* playertexture;
	sf::Texture* Effecttexture[4];
	void CreateWindow(sf::RenderWindow* window);
	void SetFrameLimit(sf::RenderWindow * window);
	GameResources();
	~GameResources();
};