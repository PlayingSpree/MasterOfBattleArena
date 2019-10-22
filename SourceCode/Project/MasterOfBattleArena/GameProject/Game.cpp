#include "Game.h"
#include "SceneGame.h"
#include "SceneMainMenu.h"
#include "MenuButton.h"

Game::Game(sf::Clock *c, sf::RenderWindow* w, GameResources* g)
{
	time = 0;
	clock = c;
	GameResource = g;
	window = w;

	sound.UpdateSetting(GameResource->SettingMap[GameResources::SE_Music], GameResource->SettingMap[GameResources::SE_Sound], GameResource->SettingMap[GameResources::SE_MusicPack]);
	scene = new SceneMainMenu(this);
}

Game::~Game()
{
}

void Game::loop()
{
	windowratio = (float)window->getSize().x / (float)window->getSize().y;
	TimeUpdate();
	scene->loop();
	window->display();
	//Pause
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) || sf::Joystick::isButtonPressed(0, 9)) {
		if (playdata.Playing == true) {

			std::vector<sf::Text> UIObjectList;
			UIObjectList.push_back(sf::Text("Paused", GameResource->font, 100U));
			UIObjectList.push_back(sf::Text("<Enter/Joystick1 Select> To Continue ", GameResource->font, 50U));
			UIObjectList.push_back(sf::Text("<Esc/Joystick1 Start> To Back To Main Menu ", GameResource->font, 50U));
			UIObjectList.push_back(sf::Text("Warning! You will lose all progress!", GameResource->font, 30U));

			//UIObjectList.push_back(new UIobject(0, this, UIobject::MT_Text, 0, 440, 0, "Paused", 100U));
			//UIObjectList.push_back(new UIobject(1, this, UIobject::MT_Button, 0, 540, 0, "Continue (Enter/Joystick1 Select)", 30U));
			//UIObjectList.push_back(new UIobject(2, this, UIobject::MT_Button, 0, 580, 0, "Back To Main Menu (Esc/Joystick1 Start)", 30U));
			//UIObjectList.push_back(new UIobject(3, this, UIobject::MT_Text, 0, 620, 0, "Warning! You will lose all progress!", 20U));

			//SetView
			sf::View view;

			unsigned int height = 1080;
			unsigned int width = windowratio*height;

			view.setCenter(width / 2.0f, height / 2.0f);

			view.setSize(width, height);
			window->setView(view);

			sf::RectangleShape rs(sf::Vector2f(width, height));
			rs.setFillColor(sf::Color(0, 0, 0, 128));
			window->draw(rs);

			UIObjectList[0].setPosition(width / 2.0f, 400);
			UIObjectList[1].setPosition(width / 2.0f, 520);
			UIObjectList[2].setPosition(width / 2.0f, 580);
			UIObjectList[3].setPosition(width / 2.0f, 630);

			//DrawMenuObject
			for (size_t i = 0; i < UIObjectList.size(); i++)
			{
				UIObjectList[i].setOrigin(UIObjectList[i].getGlobalBounds().width / 2.0f, UIObjectList[i].getGlobalBounds().height / 2.0f);
				UIObjectList[i].setFillColor(sf::Color::White);
				UIObjectList[i].setOutlineColor(sf::Color(0, 0, 0, 128));
				UIObjectList[i].setOutlineThickness(0.5f);
				if (i == 0)
					UIObjectList[i].setOutlineThickness(1.0f);
				window->draw(UIObjectList[i]);
			}

			window->display();

			float delay = 0.5f;

			while (window->isOpen())
			{
				TimeUpdate();
				delay -= deltatime;
				sf::Event event;
				while (window->pollEvent(event))
				{
					if (event.type == sf::Event::Closed) {
						window->close();
						break;
					}
					else if (event.type == sf::Event::Resized) {
						windowratio = (float)event.size.width / (float)event.size.height;
					}
				}
				if (delay <= 0) {
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) || (sf::Joystick::isButtonPressed(0, 9))) {
						playdata.Playing = false;
						Game::ChangeScene(SMainMenu);
						break;
					}
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) || (sf::Joystick::isButtonPressed(0, 8))) {
						break;
					}
				}
			}
		}
	}
}

void Game::ChangeScene(SecneEnum NextScene) {
	delete scene;
	//Change To Scene
	if (NextScene == SMainMenu)
		scene = new SceneMainMenu(this);
	else if (NextScene == SGame)
		scene = new SceneGame(this);
}
void Game::TimeUpdate() {
	float t = clock->getElapsedTime().asSeconds();
	deltatime = t - time;
	time = t;
}

PlayData::PlayData() {
	playerinfo[1].controlnum = 0;
	playerinfo[1].PlayerModel = 0;
	playerinfo[1].controlnum = 1;
	playerinfo[1].PlayerModel = 1;
	playerinfo[2].controlnum = 2;
	playerinfo[2].PlayerModel = 2;
	playerinfo[3].controlnum = 3;
	playerinfo[3].PlayerModel = 3;
}

void PlayData::initPlayData(const int i) {
	if (i != init) {
		if (i == 0) {
			playerinfo[0].team = 1;
			playerinfo[1].team = 1;
			playerinfo[2].team = 1;
			playerinfo[3].team = 1;
		}
	}
}

void Game::CharEnter(const sf::Uint32 i) {
	Char = i;
}
