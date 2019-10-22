#include "SoundEngine.h"


SoundEngine::SoundEngine()
{
	//SetUpSound
	for (size_t i = 0; i < SoundChannelMax; i++)
	{
		SoundVector.push_back(new sf::Sound());
	}
	//LoadSound
	Sbuff[SE_Weapon_ak47] = new sf::SoundBuffer();
	Sbuff[SE_Weapon_ak47]->loadFromFile("Resources/Sound/Game/ak47.wav");
	Sbuff[SE_Weapon_awp] = new sf::SoundBuffer();
	Sbuff[SE_Weapon_awp]->loadFromFile("Resources/Sound/Game/awp.wav");
	Sbuff[SE_Weapon_g3sg1] = new sf::SoundBuffer();
	Sbuff[SE_Weapon_g3sg1]->loadFromFile("Resources/Sound/Game/g3sg1.wav");
	Sbuff[SE_Weapon_m3] = new sf::SoundBuffer();
	Sbuff[SE_Weapon_m3]->loadFromFile("Resources/Sound/Game/m3.wav");
	Sbuff[SE_Weapon_m4a1] = new sf::SoundBuffer();
	Sbuff[SE_Weapon_m4a1]->loadFromFile("Resources/Sound/Game/m4a1.wav");
	Sbuff[SE_Weapon_m249] = new sf::SoundBuffer();
	Sbuff[SE_Weapon_m249]->loadFromFile("Resources/Sound/Game/m249.wav");
	Sbuff[SE_Weapon_p90] = new sf::SoundBuffer();
	Sbuff[SE_Weapon_p90]->loadFromFile("Resources/Sound/Game/p90.wav");
	Sbuff[SE_Weapon_scout] = new sf::SoundBuffer();
	Sbuff[SE_Weapon_scout]->loadFromFile("Resources/Sound/Game/scout.wav");
	Sbuff[SE_Weapon_sg552] = new sf::SoundBuffer();
	Sbuff[SE_Weapon_sg552]->loadFromFile("Resources/Sound/Game/sg552.wav");
	Sbuff[SE_Weapon_ump45] = new sf::SoundBuffer();
	Sbuff[SE_Weapon_ump45]->loadFromFile("Resources/Sound/Game/ump45.wav");
	Sbuff[SE_Weapon_xm1014] = new sf::SoundBuffer();
	Sbuff[SE_Weapon_xm1014]->loadFromFile("Resources/Sound/Game/xm1014.wav");
	Sbuff[SE_Game_End] = new sf::SoundBuffer();
	Sbuff[SE_Game_End]->loadFromFile("Resources/Sound/OWEliminationEnd.ogg");
	Sbuff[SE_Menu_Click] = new sf::SoundBuffer();
	Sbuff[SE_Menu_Click]->loadFromFile("Resources/Sound/wpn_moveselect.wav");
	Sbuff[SE_Game_Hit] = new sf::SoundBuffer();
	Sbuff[SE_Game_Hit]->loadFromFile("Resources/Sound/Game/knife_hit.wav");
	Sbuff[SE_Enemy_Explode] = new sf::SoundBuffer();
	Sbuff[SE_Enemy_Explode]->loadFromFile("Resources/Sound/Game/explode4.wav");
	Sbuff[SE_Item_Crystal] = new sf::SoundBuffer();
	Sbuff[SE_Item_Crystal]->loadFromFile("Resources/Sound/Game/Coin.ogg");
	Sbuff[SE_Item_Heal] = new sf::SoundBuffer();
	Sbuff[SE_Item_Heal]->loadFromFile("Resources/Sound/Game/Recovery.ogg");
	Sbuff[SE_Item_Explode] = new sf::SoundBuffer();
	Sbuff[SE_Item_Explode]->loadFromFile("Resources/Sound/Game/c4_explode.wav");

	//3D Setup
	sf::Listener::setDirection(0.f, 0.f, -1.f);
}

void SoundEngine::PlaySound(const SoundEnum se)
{
	if (!sound) {
		return;
	}
	for (size_t i = 0; i < SoundChannelMax; i++)
	{
		if (SoundVector[i]->getStatus() == sf::Sound::Status::Stopped) {
			SoundVector[i]->setRelativeToListener(true);
			SoundVector[i]->setPosition(0.0f, 0.0f, 0.0f);
			SoundVector[i]->setBuffer(*Sbuff[se]);
			SoundVector[i]->setVolume(100.0f);
			SoundVector[i]->play();
			break;
		}
	}
}

void SoundEngine::PlaySound3D(float x, float y, const SoundEnum se, const float vol)
{
	if (!sound) {
		return;
	}
	for (size_t i = 0; i < SoundChannelMax; i++)
	{
		if (SoundVector[i]->getStatus() == sf::Sound::Status::Stopped) {
			SoundVector[i]->setRelativeToListener(false);
			SoundVector[i]->setAttenuation(0.5f);
			SoundVector[i]->setMinDistance(120.0f);
			SoundVector[i]->setVolume(vol);
			SoundVector[i]->setPosition(x, y, 0.0f);
			SoundVector[i]->setBuffer(*Sbuff[se]);
			SoundVector[i]->play();
			break;
		}
	}
}

void SoundEngine::PlayMusic(const MusicEnum me) {
	Mplaying = me;
	if (!music) {
		return;
	}
	Music.setLoop(false);
	if (me == ME_Menu) {
		if (pack == 0) {
			int i = rand() % 3;
			if (i == 0)
				Music.openFromFile("Resources/Music/02_battlerite_soundtrack_mountain_spirit_MASTER.ogg");
			else if (i == 1)
			{
				Music.openFromFile("Resources/Music/LoL Season 5 Login.ogg");
			}
			else if (i == 2)
			{
				Music.openFromFile("Resources/Music/Paladin Elim.ogg");
			}
		}
		else if (pack == 1)
		{
			Music.openFromFile("Resources/Music/Invasion - Matchmaking.ogg");
		}
	}
	else if (me == ME_Game_Intro)
	{
		if (pack == 0) {
			Music.openFromFile("Resources/Music/Game/1/Music from the Heroes of the Storm - Hanamura Intro.ogg");
		}
		else if (pack == 1)
		{
			Music.openFromFile("Resources/Music/Game/2/rest.ogg");
		}
	}
	else if (me == ME_Game_wave1Intro)
	{
		if (pack == 0) {
			return;
		}
		else if (pack == 1)
		{
			Music.openFromFile("Resources/Music/Game/2/play1start.ogg");
		}
	}
	else if (me == ME_Game_wave5Intro)
	{
		if (pack == 0) {
			return;
		}
		else if (pack == 1)
		{
			Music.openFromFile("Resources/Music/Game/2/play2start.ogg");
		}
	}
	else if (me == ME_Game_wave10Intro)
	{
		if (pack == 0) {
			return;
		}
		else if (pack == 1)
		{
			Music.openFromFile("Resources/Music/Game/2/play3start.ogg");
		}
	}
	else if (me == ME_Game_wave1)
	{
		if (pack == 0) {
			Music.openFromFile("Resources/Music/Game/1/Music from the Heroes of the Storm - Hanamura LVL 1A.ogg");
		}
		else if (pack == 1)
		{
			Music.openFromFile("Resources/Music/Game/2/play1.ogg");
		}
	}
	else if (me == ME_Game_wave5)
	{
		if (pack == 0) {
			Music.openFromFile("Resources/Music/Game/1/Music from the Heroes of the Storm - Hanamura LVL 10A.ogg");
		}
		else if (pack == 1)
		{
			Music.openFromFile("Resources/Music/Game/2/play2.ogg");
		}
	}
	else if (me == ME_Game_wave10)
	{
		if (pack == 0) {
			Music.openFromFile("Resources/Music/Game/1/Music from the Heroes of the Storm - Hanamura LVL 20A.ogg");
		}
		else if (pack == 1)
		{
			Music.openFromFile("Resources/Music/Game/2/play3.ogg");
		}
	}
	else if (me == ME_GameOver)
	{
		if (pack == 0) {
			Music.openFromFile("Resources/Sound/OWEliminationEnd.ogg");
		}
		else if (pack == 1)
		{
			Music.openFromFile("Resources/Music/Valoran City Park - We'll Get Them Next Time.ogg");
		}
	}
	else
	{
		return;
	}
	Music.play();
}

void SoundEngine::UpdateSetting(bool bMusic,bool Sound,int Pack) {
	if (bMusic == false || pack != Pack) {
		Music.stop();
	}
	music = bMusic;
	sound = Sound;
	pack = Pack;
}

void SoundEngine::Update(const float deltatime)
{

}

SoundEngine::~SoundEngine()
{
}
