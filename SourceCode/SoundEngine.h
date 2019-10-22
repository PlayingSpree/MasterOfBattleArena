#pragma once
#include <SFML/Audio.hpp>
class SoundEngine
{
public:
	bool music;
	bool sound;
	enum SoundEnum
	{
		SE_Weapon_ak47,
		SE_Weapon_awp,
		SE_Weapon_g3sg1,
		SE_Weapon_m3,
		SE_Weapon_m4a1,
		SE_Weapon_m249,
		SE_Weapon_p90,
		SE_Weapon_scout,
		SE_Weapon_sg552,
		SE_Weapon_ump45,
		SE_Weapon_xm1014,
		SE_Game_End,
		SE_Menu_Click,
		SE_Game_Hit,
		SE_Enemy_Explode,
		SE_Item_Crystal,
		SE_Item_Heal,
		SE_Item_Explode
	};
	sf::SoundBuffer* Sbuff[SE_Item_Explode +1];
	const size_t SoundChannelMax = 32;
	std::vector<sf::Sound*> SoundVector;
	void PlaySound(const SoundEnum);
	void PlaySound3D(float x, float y, const SoundEnum se, const float vol = 100.f);
	int pack = 1;
	enum MusicEnum
	{
		ME_Menu,
		ME_Game_Intro,
		ME_Game_wave1,
		ME_Game_wave5,
		ME_Game_wave10,
		ME_Game_wave1Intro,
		ME_Game_wave5Intro,
		ME_Game_wave10Intro,
		ME_GameOver
	}Mplaying;
	sf::Music Music;
	SoundEngine();
	void PlayMusic(const MusicEnum);
	void UpdateSetting(bool music, bool sound, int pack);
	void Update(const float deltatime);
	~SoundEngine();
};

