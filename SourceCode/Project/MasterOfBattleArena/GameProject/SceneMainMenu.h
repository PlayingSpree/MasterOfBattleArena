#pragma once
#include "Game.h"
#include "MenuButton.h"
#include "UIsystem.h"
class SceneMainMenu : public Scene
{
public:
	Game* game;
	UIsystem ui;

	SceneMainMenu(Game*);
	~SceneMainMenu();
	void clear();
	void loop();
	enum MenuScreenEnum
	{
		MSE_Main,
		MSE_COOPend,
		MSE_COOPLobby,
		MSE_Highscore,
		MSE_Setting,
		MSE_PVPLobby,
		MSE_Credit,
		MSE_HowTo
	}screen;
	enum MenuButtonEnum
	{
		MBE_Title_PlayCOOP,
		MBE_Title_PlayPVP,
		MBE_Title_HighScore,
		MBE_Title_Setting,
		MBE_Title_Credit,
		MBE_Title_Exit,
		MBE_BackToMainMenu,
		MBE_COOPLobby_Play,
		MBE_COOPLobby_PlayerSelect1,
		MBE_COOPLobby_PlayerSelect2,
		MBE_COOPLobby_PlayerSelect3,
		MBE_COOPLobby_PlayerSelect4,
		MBE_HighScore,
		MBE_HighScoreName = 17,
		MBE_HighScoreSubmit = 22,
		MBE_HighScoreEnteredName,
		MBE_COOPLobby_PlayerControl,
		MBE_COOPLobby_PlayerCharacterButton = 28,
		MBE_COOPLobby_PlayerWeaponButton = 32,
		MBE_COOPLobby_PlayerCharacter = 36,
		MBE_COOPLobby_PlayerWeapon = 40,
		MBE_COOPLobby_PlayerCharacterReal = 44,
		MBE_COOPLobby_MapSelect = 48,
		MBE_COOPLobby_JoyCon,
		MBE_Setting_WindowMode,
		MBE_Setting_FrameLock,
		MBE_Setting_Music,
		MBE_Setting_Sound,
		MBE_Setting_Pack,
		MBE_Setting_Save,
		MBE_Setting_Apply,
		MBE_Setting_Back
	};
	bool NewWindow = false;
	std::string SettingWindowModeText[3] = { "Window Mode(1/3) : Window","Window Mode(2/3) : Windowed Fullscreen" ,"Window Mode(3/3) : Fullscreen"};
	std::string SettingFrameText[8] = { "Framerate Limit (1/8) : OFF","Framerate Limit (2/8) : V-Sync","Framerate Limit (3/8) : 30 FPS", "Framerate Limit (4/8) : 60 FPS", "Framerate Limit (5/8) : 72 FPS", "Framerate Limit (6/8) : 120 FPS", "Framerate Limit (7/8) : 144 FPS", "Framerate Limit (8/8) : 300 FPS" };
	std::string SettingMusicText[2] = { "Music : OFF","Music : ON" };
	std::string SettingSoundText[2] = { "Sound : OFF","Sound : ON" };
	std::string SettingPackText[2] = { "Music Pack (1/2) : Heroes","Music Pack (2/2) : Magic" };
	int COOPPlayerSelect[4] = { MBE_COOPLobby_PlayerSelect1,MBE_COOPLobby_PlayerSelect2,MBE_COOPLobby_PlayerSelect3,MBE_COOPLobby_PlayerSelect4 };
	std::string PlayerControlText[8] = { "Control : Keyboard 1","Control : Keyboard 2" ,"Control : Keyboard 3","Control : Keyboard 4","Control : Joystick 1","Control : Joystick 2","Control : Joystick 3","Control : Joystick 4"};
	std::string PlayerWeaponText[11] = { "Weapon 1/11 : AK47","Weapon 2/11 : AWP" ,"Weapon 3/11 : G3SG1","Weapon 4/11 : M3","Weapon 5/11 : M4A1","Weapon 6/11 : M249","Weapon 7/11 : P90","Weapon 8/11 : Scout" ,"Weapon 9/11 : SG552" ,"Weapon 10/11 : UMP45" ,"Weapon 11/11 : XM1014" };
	std::string MapText[2] = { "Arena(2/2) : Underground","Arena(1/2) : Mountain"};
	void ChangeScreen(const MenuScreenEnum);
	void HighScoreUpdate(const int player);
	std::wstring Name;
	float ClickDelay;
};

