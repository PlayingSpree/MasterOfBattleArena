#pragma once
#include "Game.h"
#include "AnimatedSprite.h"
class UIobject
{
public:
	int tag;
	enum MenuTypeEnum
	{
		MT_Sprite,
		MT_Button,
		MT_Text,
		MT_Selection
	}UIType;
	AnimatedSprite AS;
	sf::Sprite sprite;
	sf::RectangleShape shape;
	sf::Text text;
	sf::Vector2f pos;
	int setPos;
	bool havesprite;
	bool haveshape;
	bool havetext;
	bool IsMouseOver;
	bool IsSelected;
	const Game* game;

	void Update();
	void Draw(sf::RenderWindow*);
	void CreateButton(const char * c, const unsigned int size , sf::Text::Style);
	void CreateText(const char*,const unsigned int size, sf::Text::Style s);
	void UpdateButton();

	void ChangeText(const std::string s);

	void ChangeText(const std::wstring*);

	void SetTextAlign();

	void ChangeSprite(sf::Texture * text);

	UIobject(int tag,const Game * game, const MenuTypeEnum mte, const float x, const float y, const int setPos, const char* c = "", const unsigned int size = 50U, sf::Text::Style s = sf::Text::Bold);
	~UIobject();
};

