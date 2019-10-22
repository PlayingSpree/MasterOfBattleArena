#include "SceneMainMenu.h"



SceneMainMenu::SceneMainMenu(Game* g) : ui(g)
{
	game = g;
	if (game->playdata.Playing) {
		ChangeScreen(MSE_COOPend);
		game->playdata.Playing = false;
		g->sound.PlayMusic(SoundEngine::ME_GameOver);
		ui.BG.setTexture(*game->GameResource->MenuTextture[GameResources::MTE_EndBG], true);
	}
	else
	{
		ChangeScreen(MSE_Main);
		g->sound.PlayMusic(SoundEngine::ME_Menu);
		ui.BG.setTexture(*game->GameResource->MenuTextture[GameResources::MTE_TitleBG], true);
	}
}


SceneMainMenu::~SceneMainMenu()
{
	clear();
}

void SceneMainMenu::clear() {
	ui.clear();
}

void SceneMainMenu::loop()
{
	if (game->sound.Music.getStatus() == sf::Music::Stopped) {
		game->sound.PlayMusic(SoundEngine::ME_Menu);
	}
	ClickDelay -= game->deltatime;
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		ClickDelay = 0;
	}
	game->window->clear(sf::Color::Black);
	ui.Draw();
	if (screen == MSE_Main) {
		if (ui.IsButtonPressed(MBE_Title_PlayCOOP)) {
			game->playdata.PlayMode = PlayData::COOP;
			game->playdata.initPlayData(0);
			ClickDelay = 0.2f;
			ChangeScreen(MSE_COOPLobby);
			return;
		}
		if (ui.IsButtonPressed(MBE_Title_PlayPVP)) {
			ChangeScreen(MSE_HowTo);
			return;
		}
		if (ui.IsButtonPressed(MBE_Title_Credit)) {
			ChangeScreen(MSE_Credit);
			return;
		}
		if (ui.IsButtonPressed(MBE_Title_Setting)) {
			ChangeScreen(MSE_Setting);
			NewWindow = false;
			ClickDelay = 0.2f;
			return;
		}
		if (ui.IsButtonPressed(MBE_Title_HighScore)) {
			game->highscore.LoadHighscore();
			ChangeScreen(MSE_Highscore);
			ui.SelectionUpdate(COOPPlayerSelect, 4, 0);
			return;
		}
		if (ui.IsButtonPressed(MBE_Title_Exit)) {
			game->window->close();
			return;
		}

	}
	if (screen == MSE_Setting) {
		if (ClickDelay <= 0) {
			if (ui.IsButtonPressed(MBE_Setting_WindowMode)) {
				int* i = &game->GameResource->SettingMap[GameResources::SE_WindowMode];
				(*i)++;
				if ((*i) > 2) {
					(*i) = 0;
				}
				ui.FindUIobject(MBE_Setting_WindowMode)->ChangeText(SettingWindowModeText[(*i)]);
				ClickDelay = 0.15f;
				NewWindow = true;
			}
			if (ui.IsButtonPressed(MBE_Setting_FrameLock)) {
				int* i = &game->GameResource->SettingMap[GameResources::SE_FrameLimit];
				(*i)++;
				if ((*i) > 7) {
					(*i) = 0;
				}
				ui.FindUIobject(MBE_Setting_FrameLock)->ChangeText(SettingFrameText[(*i)]);
				ClickDelay = 0.15f;
				NewWindow = true;
			}
			if (ui.IsButtonPressed(MBE_Setting_Sound)) {
				int* i = &game->GameResource->SettingMap[GameResources::SE_Sound];
				(*i)++;
				if ((*i) > 1) {
					(*i) = 0;
				}
				ui.FindUIobject(MBE_Setting_Sound)->ChangeText(SettingSoundText[(*i)]);
				ClickDelay = 0.15f;
				NewWindow = true;
			}
			if (ui.IsButtonPressed(MBE_Setting_Music)) {
				int* i = &game->GameResource->SettingMap[GameResources::SE_Music];
				(*i)++;
				if ((*i) > 1) {
					(*i) = 0;
				}
				ui.FindUIobject(MBE_Setting_Music)->ChangeText(SettingMusicText[(*i)]);
				ClickDelay = 0.15f;
				NewWindow = true;
			}
			if (ui.IsButtonPressed(MBE_Setting_Pack)) {
				int* i = &game->GameResource->SettingMap[GameResources::SE_MusicPack];
				(*i)++;
				if ((*i) > 1) {
					(*i) = 0;
				}
				ui.FindUIobject(MBE_Setting_Pack)->ChangeText(SettingPackText[(*i)]);
				ClickDelay = 0.15f;
				NewWindow = true;
			}
			if (ui.IsButtonPressed(MBE_Setting_Apply)) {
				if (NewWindow) {
					game->GameResource->CreateWindow(game->window);
				}
				game->GameResource->SetFrameLimit(game->window);
				game->sound.UpdateSetting(game->GameResource->SettingMap[GameResources::SE_Music], game->GameResource->SettingMap[GameResources::SE_Sound], game->GameResource->SettingMap[GameResources::SE_MusicPack]);
				ClickDelay = 0.15f;
			}
			if (ui.IsButtonPressed(MBE_Setting_Save)) {
				if (NewWindow) {
					game->GameResource->CreateWindow(game->window);
				}
				game->GameResource->SetFrameLimit(game->window);
				game->sound.UpdateSetting(game->GameResource->SettingMap[GameResources::SE_Music], game->GameResource->SettingMap[GameResources::SE_Sound], game->GameResource->SettingMap[GameResources::SE_MusicPack]);
				game->GameResource->SaveSetting(false);
				ChangeScreen(MSE_Main);
				return;
			}
			if (ui.IsButtonPressed(MBE_Setting_Back)) {
				game->GameResource->LoadSetting();
				if (NewWindow) {
					game->GameResource->CreateWindow(game->window);
				}
				game->GameResource->SetFrameLimit(game->window);
				game->sound.UpdateSetting(game->GameResource->SettingMap[GameResources::SE_Music], game->GameResource->SettingMap[GameResources::SE_Sound], game->GameResource->SettingMap[GameResources::SE_MusicPack]);
				ChangeScreen(MSE_Main);
				return;
			}
		}
	}
	if (screen == MSE_COOPLobby) {
		int joyCount = 0;
		for (int i = 0; i < 4;i++) {
			joyCount += sf::Joystick::isConnected(i)?1:0;
		}
		ui.FindUIobject(MBE_COOPLobby_JoyCon)->ChangeText(std::string("Joystick Connected : ")+std::to_string(joyCount));
		if (ui.IsButtonPressed(MBE_COOPLobby_Play)) {
			game->playdata.Playing = true;
			game->ChangeScene(Game::SGame);
			return;
		}
		if (ui.IsButtonPressed(MBE_COOPLobby_PlayerSelect1)) {
			game->playdata.playerCount = 1;
			ui.SelectionUpdate(COOPPlayerSelect, 4, 0);
		}
		if (ui.IsButtonPressed(MBE_COOPLobby_PlayerSelect2)) {
			game->playdata.playerCount = 2;
			ui.SelectionUpdate(COOPPlayerSelect, 4, 1);
		}
		if (ui.IsButtonPressed(MBE_COOPLobby_PlayerSelect3)) {
			game->playdata.playerCount = 3;
			ui.SelectionUpdate(COOPPlayerSelect, 4, 2);
		}
		if (ui.IsButtonPressed(MBE_COOPLobby_PlayerSelect4)) {
			game->playdata.playerCount = 4;
			ui.SelectionUpdate(COOPPlayerSelect, 4, 3);
		}
		
		if (ClickDelay <= 0) {
			if (ui.IsButtonPressed(MBE_COOPLobby_MapSelect)) {
				game->playdata.map++;
				if (game->playdata.map > 2) {
					game->playdata.map = 1;
				}
				ui.FindUIobject(MBE_COOPLobby_MapSelect)->ChangeText(MapText[game->playdata.map - 1]);
				ClickDelay = 0.15f;
			}
			for (int i = 0; i < 4; i++)
			{
				if (ui.IsButtonPressed(MBE_COOPLobby_PlayerControl + i)) {
					game->playdata.playerinfo[i].controlnum++;
					if (game->playdata.playerinfo[i].controlnum > 7) {
						game->playdata.playerinfo[i].controlnum = 0;
					}
					bool isDope = false;
					do {
						isDope = false;
						for (int i2 = 0; i2 < 4; i2++) {
							if (i2 == i) {
								continue;
							}
							else
							{
								if (game->playdata.playerinfo[i].controlnum == game->playdata.playerinfo[i2].controlnum) {
									isDope = true;
									game->playdata.playerinfo[i].controlnum++;
									if (game->playdata.playerinfo[i].controlnum > 7) {
										game->playdata.playerinfo[i].controlnum = 0;
									}
								}
							}
						}
					} while (isDope);

					ui.FindUIobject(MBE_COOPLobby_PlayerControl + i)->ChangeText(PlayerControlText[game->playdata.playerinfo[i].controlnum]);
					ClickDelay = 0.15f;
				}
				if (ui.IsButtonPressed(MBE_COOPLobby_PlayerCharacterButton + i)) {
					game->playdata.playerinfo[i].PlayerModel++;
					if (game->playdata.playerinfo[i].PlayerModel > 23) {
						game->playdata.playerinfo[i].PlayerModel = 0;
					}

					bool isDope = false;
					do {
						isDope = false;
						for (int i2 = 0; i2 < 4; i2++) {
							if (i2 == i) {
								continue;
							}
							else
							{
								if (game->playdata.playerinfo[i].PlayerModel == game->playdata.playerinfo[i2].PlayerModel) {
									isDope = true;
									game->playdata.playerinfo[i].PlayerModel++;
									if (game->playdata.playerinfo[i].PlayerModel > 23) {
										game->playdata.playerinfo[i].PlayerModel = 0;
									}
								}
							}
						}
					} while (isDope);

					ui.FindUIobject(MBE_COOPLobby_PlayerCharacterButton + i)->ChangeText(std::string("Character : ") + std::to_string(game->playdata.playerinfo[i].PlayerModel + 1) + std::string("/24"));
					int y = game->playdata.playerinfo[i].PlayerModel;
					int x = 0;
					while (y > 3)
					{
						x++;
						y -= 4;
					}
					ui.FindUIobject(MBE_COOPLobby_PlayerCharacter + i)->sprite.setTextureRect(sf::IntRect(144 * x, 144 * y, 144, 144));
					ui.FindUIobject(MBE_COOPLobby_PlayerCharacterReal + i)->sprite.setTextureRect(sf::IntRect(144 * x, 192 * y, 48, 48));
					ClickDelay = 0.15f;
				}
				if (ui.IsButtonPressed(MBE_COOPLobby_PlayerWeaponButton + i)) {
					game->playdata.playerinfo[i].weapon++;
					if (game->playdata.playerinfo[i].weapon > 10) {
						game->playdata.playerinfo[i].weapon = 0;
					}
					ui.FindUIobject(MBE_COOPLobby_PlayerWeaponButton + i)->ChangeText(PlayerWeaponText[game->playdata.playerinfo[i].weapon]);
					ui.FindUIobject(MBE_COOPLobby_PlayerWeapon + i)->ChangeSprite(game->GameResource->weapontexture[game->playdata.playerinfo[i].weapon]);
					ClickDelay = 0.15f;
				}
			}
		}
	}
	if (screen == MSE_COOPend) {
		if (ui.IsButtonPressed(MBE_HighScoreSubmit)) {
			game->highscore.SubmitHighScore(&Name, game->playdata.Score, game->playdata.playerCount - 1);
			game->highscore.LoadHighscore();
			ChangeScreen(MSE_Highscore);
			HighScoreUpdate(game->playdata.playerCount - 1);
			ui.SelectionUpdate(COOPPlayerSelect, 4, game->playdata.playerCount - 1);
			ui.BG.setTexture(*game->GameResource->MenuTextture[GameResources::MTE_TitleBG], true);
		}
		if (ui.IsButtonPressed(MBE_Title_HighScore)) {
			ChangeScreen(MSE_Highscore);
			HighScoreUpdate(game->playdata.playerCount - 1);
			ui.SelectionUpdate(COOPPlayerSelect, 4, game->playdata.playerCount - 1);
			ui.BG.setTexture(*game->GameResource->MenuTextture[GameResources::MTE_TitleBG], true);
		}
		if (game->Char != 0) {
			if (game->Char < 128) {
				if (game->Char == 8) {
					if (Name[0] != 0) {
						Name.erase(Name.size() - 1);
					}
				}
				else if (game->Char != 15 && Name.size() < 20) {
					Name.push_back((wchar_t)game->Char);
				}
			}
			UIobject* u = ui.FindUIobject(MBE_HighScoreEnteredName);

			if (ui.FindUIobject(MBE_HighScoreEnteredName) != 0)
				u->ChangeText(&Name);
			game->Char = 0;
		}
	}
	if (screen == MSE_Highscore) {
		if (ui.IsButtonPressed(MBE_COOPLobby_PlayerSelect1)) {
			HighScoreUpdate(0);
			ui.SelectionUpdate(COOPPlayerSelect, 4, 0);
		}
		if (ui.IsButtonPressed(MBE_COOPLobby_PlayerSelect2)) {
			HighScoreUpdate(1);
			ui.SelectionUpdate(COOPPlayerSelect, 4, 1);
		}
		if (ui.IsButtonPressed(MBE_COOPLobby_PlayerSelect3)) {
			HighScoreUpdate(2);
			ui.SelectionUpdate(COOPPlayerSelect, 4, 2);
		}
		if (ui.IsButtonPressed(MBE_COOPLobby_PlayerSelect4)) {
			HighScoreUpdate(3);
			ui.SelectionUpdate(COOPPlayerSelect, 4, 3);
		}
	}
	if (ui.IsButtonPressed(MBE_BackToMainMenu)) {
		ChangeScreen(MSE_Main);
		ui.BG.setTexture(*game->GameResource->MenuTextture[GameResources::MTE_TitleBG], true);
		return;
	}
}

void SceneMainMenu::ChangeScreen(const MenuScreenEnum mse)
{
	screen = mse;
	clear();
	if (mse == MSE_Main) {
		ui.UIObjectList.push_back(new UIobject(-1, game, UIobject::MT_Sprite, 0, 50, 0, "MenuTitle"));
		ui.UIObjectList.push_back(new UIobject(MBE_Title_PlayCOOP, game, UIobject::MT_Button, 0, 375, 0, "Play"));
		ui.UIObjectList.push_back(new UIobject(MBE_Title_PlayPVP, game, UIobject::MT_Button, 0, 475, 0, "How To Play"));
		ui.UIObjectList.push_back(new UIobject(MBE_Title_HighScore, game, UIobject::MT_Button, 0, 575, 0, "HighScore"));
		ui.UIObjectList.push_back(new UIobject(MBE_Title_Setting, game, UIobject::MT_Button, 0, 675, 0, "Setting"));
		ui.UIObjectList.push_back(new UIobject(MBE_Title_Credit, game, UIobject::MT_Button, 0, 775, 0, "Credit"));
		ui.UIObjectList.push_back(new UIobject(MBE_Title_Exit, game, UIobject::MT_Button, 0, 875, 0, "Exit"));
		ui.UIObjectList.push_back(new UIobject(-1, game, UIobject::MT_Text, 0, 1054, 0, "60010285 Natchapon Santiphiboon / Project for Programming Fundamental subject at KMITL", 24U, sf::Text::Style::Regular));
	}
	else if (mse == MSE_Credit) {
		ui.UIObjectList.push_back(new UIobject(-1, game, UIobject::MT_Sprite, 0, 0, 0, "Credit"));
		ui.UIObjectList.push_back(new UIobject(MBE_BackToMainMenu, game, UIobject::MT_Button, 0, 1000, 0, "Back To Main Menu"));
	}
	else if (mse == MSE_HowTo) {
		ui.UIObjectList.push_back(new UIobject(-1, game, UIobject::MT_Sprite, 0, 0, 0, "HowTo"));
		ui.UIObjectList.push_back(new UIobject(MBE_BackToMainMenu, game, UIobject::MT_Button, 0, 1000, 0, "Back To Main Menu"));
	}
	else if (mse == MSE_Setting) {
		ui.UIObjectList.push_back(new UIobject(-1, game, UIobject::MT_Text, 0, 50, 0, "Game Setting", 100U, sf::Text::Style::Bold));

		ui.UIObjectList.push_back(new UIobject(-1, game, UIobject::MT_Text, 0, 200, 0, "Screen", 50U, sf::Text::Style::Bold));
		ui.UIObjectList.push_back(new UIobject(MBE_Setting_WindowMode, game, UIobject::MT_Button, 0, 260, 0, SettingWindowModeText[game->GameResource->SettingMap[GameResources::SE_WindowMode]].c_str(), 35U));
		ui.UIObjectList.push_back(new UIobject(MBE_Setting_FrameLock, game, UIobject::MT_Button, 0, 320, 0, SettingFrameText[game->GameResource->SettingMap[GameResources::SE_FrameLimit]].c_str(), 35U));

		ui.UIObjectList.push_back(new UIobject(-1, game, UIobject::MT_Text, 0, 390, 0, "Sound&Music", 50U, sf::Text::Style::Bold));
		ui.UIObjectList.push_back(new UIobject(MBE_Setting_Sound, game, UIobject::MT_Button, 0, 450, 0, SettingSoundText[game->GameResource->SettingMap[GameResources::SE_Sound]].c_str(), 35U));
		ui.UIObjectList.push_back(new UIobject(MBE_Setting_Music, game, UIobject::MT_Button, 0, 510, 0, SettingMusicText[game->GameResource->SettingMap[GameResources::SE_Music]].c_str(), 35U));
		ui.UIObjectList.push_back(new UIobject(MBE_Setting_Pack, game, UIobject::MT_Button, 0, 570, 0, SettingPackText[game->GameResource->SettingMap[GameResources::SE_MusicPack]].c_str(), 35U));

		ui.UIObjectList.push_back(new UIobject(MBE_Setting_Back, game, UIobject::MT_Button, -200, 1000, 0, "Cancel"));
		ui.UIObjectList.push_back(new UIobject(MBE_Setting_Save, game, UIobject::MT_Button, 0, 1000, 0, "Save"));
		ui.UIObjectList.push_back(new UIobject(MBE_Setting_Apply, game, UIobject::MT_Button, 200, 1000, 0, "Apply"));
	}
	else if (mse == MSE_COOPend) {
		ui.UIObjectList.push_back(new UIobject(-1, game, UIobject::MT_Text, 0, 75, 0, "Game Over", 200U, sf::Text::Style::Bold));
		ui.UIObjectList.push_back(new UIobject(-1, game, UIobject::MT_Text, 0, 300, 0, "Final Score", 100U, sf::Text::Style::Bold));
		ui.UIObjectList.push_back(new UIobject(-1, game, UIobject::MT_Text, 0, 410, 0, std::to_string(game->playdata.Score).c_str(), 60U, sf::Text::Style::Bold));
		ui.UIObjectList.push_back(new UIobject(MBE_BackToMainMenu, game, UIobject::MT_Button, 0, 1000, 0, "Back To Main Menu"));
		int h = game->highscore.IsHighScore(game->playdata.Score, game->playdata.playerCount - 1);
		if (h != -1) {
			ui.UIObjectList.push_back(new UIobject(MBE_HighScoreSubmit, game, UIobject::MT_Button, 0, 900, 0, "Submit"));
			if (h == 0)
				ui.UIObjectList.push_back(new UIobject(-1, game, UIobject::MT_Text, 0, 550, 0, "New High Score!", 100U, sf::Text::Style::Bold));
			else
				ui.UIObjectList.push_back(new UIobject(-1, game, UIobject::MT_Text, 0, 550, 0, "Top 5 Score!", 80U, sf::Text::Style::Bold));
			ui.UIObjectList.push_back(new UIobject(-1, game, UIobject::MT_Text, 0, 650, 0, "Enter Your Name", 60U, sf::Text::Style::Bold));
			ui.UIObjectList.push_back(new UIobject(MBE_HighScoreEnteredName, game, UIobject::MT_Text, 0, 750, 0, "noto", 50U, sf::Text::Style::Regular));
			game->Char = 0;
		}
		else
		{
			ui.UIObjectList.push_back(new UIobject(MBE_Title_HighScore, game, UIobject::MT_Button, 0, 900, 0, "High Score"));
		}
	}
	else if (mse == MSE_COOPLobby) {
		ui.UIObjectList.push_back(new UIobject(-1, game, UIobject::MT_Text, 0, 50, 0, "Player Setting", 100U, sf::Text::Style::Bold));
		ui.UIObjectList.push_back(new UIobject(MBE_COOPLobby_Play, game, UIobject::MT_Button, 85, 1000, 1, "Start"));
		ui.UIObjectList.push_back(new UIobject(MBE_BackToMainMenu, game, UIobject::MT_Button, 75, 1000, -1, "Back"));
		ui.UIObjectList.push_back(new UIobject(-1, game, UIobject::MT_Text, -200, 170, 0, "Player Count : "));
		ui.UIObjectList.push_back(new UIobject(MBE_COOPLobby_PlayerSelect1, game, UIobject::MT_Selection, 0, 170, 0, "1P"));
		ui.UIObjectList.push_back(new UIobject(MBE_COOPLobby_PlayerSelect2, game, UIobject::MT_Selection, 100, 170, 0, "2P"));
		ui.UIObjectList.push_back(new UIobject(MBE_COOPLobby_PlayerSelect3, game, UIobject::MT_Selection, 200, 170, 0, "3P"));
		ui.UIObjectList.push_back(new UIobject(MBE_COOPLobby_PlayerSelect4, game, UIobject::MT_Selection, 300, 170, 0, "4P"));
		ui.SelectionUpdate(COOPPlayerSelect, 4, game->playdata.playerCount - 1);
		ui.UIObjectList.push_back(new UIobject(-1, game, UIobject::MT_Text, -450, 250, 0, "P1 Setting", 35U));
		ui.UIObjectList.push_back(new UIobject(-1, game, UIobject::MT_Text, -150, 250, 0, "P2 Setting", 35U));
		ui.UIObjectList.push_back(new UIobject(-1, game, UIobject::MT_Text, 150, 250, 0, "P3 Setting", 35U));
		ui.UIObjectList.push_back(new UIobject(-1, game, UIobject::MT_Text, 450, 250, 0, "P4 Setting", 35U));
		ui.UIObjectList.push_back(new UIobject(MBE_COOPLobby_MapSelect, game, UIobject::MT_Button, 0, 820, 0, MapText[game->playdata.map-1].c_str()));
		ui.UIObjectList.push_back(new UIobject(MBE_COOPLobby_JoyCon, game, UIobject::MT_Text, 0, 1000, 0, "Joystick Connected : 0",30U));
		float x = -450;
		for (int i = 0; i < 4; i++)
		{
			ui.UIObjectList.push_back(new UIobject(MBE_COOPLobby_PlayerControl + i, game, UIobject::MT_Button, x, 325, 0, PlayerControlText[game->playdata.playerinfo[i].controlnum].c_str(), 25U));

			UIobject* u = new UIobject(MBE_COOPLobby_PlayerCharacterButton + i, game, UIobject::MT_Button, x, 380, 0, "Character : 1/24", 25U);
			ui.UIObjectList.push_back(u);
			u->ChangeText(std::string("Character : ") + std::to_string(game->playdata.playerinfo[i].PlayerModel + 1) + std::string("/24"));

			int ty = game->playdata.playerinfo[i].PlayerModel;
			int tx = 0;
			while (ty > 3)
			{
				tx++;
				ty -= 4;
			}

			u = new UIobject(MBE_COOPLobby_PlayerCharacter + i, game, UIobject::MT_Sprite, x, 500, 0, "Char1");
			ui.UIObjectList.push_back(u);
			u->ChangeSprite(game->GameResource->facetexture);
			u->sprite.setTextureRect(sf::IntRect(144 * tx, 144 * ty, 144, 144));
			u->sprite.setOrigin(72, 72);

			u = new UIobject(MBE_COOPLobby_PlayerCharacterReal + i, game, UIobject::MT_Sprite, x, 625, 0, "Char1S");
			ui.UIObjectList.push_back(u);
			u->ChangeSprite(game->GameResource->playertexture);
			u->sprite.setTextureRect(sf::IntRect(144 * tx, 192 * ty, 48, 48));
			u->sprite.setOrigin(24,24);

			ui.UIObjectList.push_back(new UIobject(MBE_COOPLobby_PlayerWeaponButton + i, game, UIobject::MT_Button, x, 680, 0, PlayerWeaponText[game->playdata.playerinfo[i].weapon].c_str(), 25U));

			u = new UIobject(MBE_COOPLobby_PlayerWeapon + i, game, UIobject::MT_Sprite, x, 710, 0, "Weapon1");
			ui.UIObjectList.push_back(u);
			u->sprite.setScale(3.0f, 3.0f);
			u->ChangeSprite(game->GameResource->weapontexture[game->playdata.playerinfo[i].weapon]);
			x += 300;
		}
	}
	else if (mse == MSE_Highscore) {
		ui.UIObjectList.push_back(new UIobject(-1, game, UIobject::MT_Text, 0, 50, 0, "High Score", 100U, sf::Text::Style::Bold));
		ui.UIObjectList.push_back(new UIobject(MBE_BackToMainMenu, game, UIobject::MT_Button, 0, 1000, 0, "Back To Main Menu"));
		ui.UIObjectList.push_back(new UIobject(-1, game, UIobject::MT_Text, -200, 170, 0, "Player Count : "));
		ui.UIObjectList.push_back(new UIobject(MBE_COOPLobby_PlayerSelect1, game, UIobject::MT_Selection, 0, 170, 0, "1P"));
		ui.UIObjectList.push_back(new UIobject(MBE_COOPLobby_PlayerSelect2, game, UIobject::MT_Selection, 100, 170, 0, "2P"));
		ui.UIObjectList.push_back(new UIobject(MBE_COOPLobby_PlayerSelect3, game, UIobject::MT_Selection, 200, 170, 0, "3P"));
		ui.UIObjectList.push_back(new UIobject(MBE_COOPLobby_PlayerSelect4, game, UIobject::MT_Selection, 300, 170, 0, "4P"));

		ui.UIObjectList.push_back(new UIobject(MBE_HighScoreName + 0, game, UIobject::MT_Text, 200, 350, -1, "noto"));
		ui.UIObjectList.push_back(new UIobject(MBE_HighScoreName + 1, game, UIobject::MT_Text, 200, 450, -1, "noto"));
		ui.UIObjectList.push_back(new UIobject(MBE_HighScoreName + 2, game, UIobject::MT_Text, 200, 550, -1, "noto"));
		ui.UIObjectList.push_back(new UIobject(MBE_HighScoreName + 3, game, UIobject::MT_Text, 200, 650, -1, "noto"));
		ui.UIObjectList.push_back(new UIobject(MBE_HighScoreName + 4, game, UIobject::MT_Text, 200, 750, -1, "noto"));
		ui.UIObjectList.push_back(new UIobject(MBE_HighScore + 0, game, UIobject::MT_Text, 200, 350, 1, ""));
		ui.UIObjectList.push_back(new UIobject(MBE_HighScore + 1, game, UIobject::MT_Text, 200, 450, 1, ""));
		ui.UIObjectList.push_back(new UIobject(MBE_HighScore + 2, game, UIobject::MT_Text, 200, 550, 1, ""));
		ui.UIObjectList.push_back(new UIobject(MBE_HighScore + 3, game, UIobject::MT_Text, 200, 650, 1, ""));
		ui.UIObjectList.push_back(new UIobject(MBE_HighScore + 4, game, UIobject::MT_Text, 200, 750, 1, ""));
		HighScoreUpdate(0);
	}
}

void SceneMainMenu::HighScoreUpdate(const int player)
{
	for (size_t i = 0; i < 5; i++)
	{
		std::wstring ws = std::to_wstring(i + 1) + std::wstring(L". ") + game->highscore.score[player][i].name;
		ui.FindUIobject(MBE_HighScoreName + i)->ChangeText(&ws);
		ui.FindUIobject(MBE_HighScore + i)->ChangeText(&std::to_wstring(game->highscore.score[player][i].Score));
	}
}
