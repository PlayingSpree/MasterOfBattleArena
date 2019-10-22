#include "MenuButton.h"


UIobject::UIobject(int tag, const Game * game, const MenuTypeEnum mte, const float x, const float y, const int setPos, const char* c, const unsigned int size, sf::Text::Style s) : game(game), pos(x, y), setPos(setPos), tag(tag)
{
	UIType = mte;
	switch (UIType)
	{
	case MT_Sprite:
		if (c == "MenuTitle") {
			sprite.setTexture(*game->GameResource->MenuTextture[GameResources::MTE_Title], true);
			sprite.setOrigin(sprite.getTexture()->getSize().x / 2, 0);
		}
		else if(c == "Credit")
		{
			sprite.setTexture(*game->GameResource->MenuTextture[GameResources::MTE_Credit], true);
			sprite.setOrigin(sprite.getTexture()->getSize().x / 2, 0);
		}
		else if (c == "HowTo")
		{
			sprite.setTexture(*game->GameResource->MenuTextture[GameResources::MTE_HowTo], true);
			sprite.setOrigin(sprite.getTexture()->getSize().x / 2, 0);
		}
		havesprite = true;
		break;
	case MT_Button:
	case MT_Selection:
		CreateButton(c, size, s);
		break;
	case MT_Text:
		CreateText(c, size, s);
		break;
	default:
		printf("Noob Dev"); break;
	}
}

void UIobject::Update()
{
	switch (UIType)
	{
	case MT_Sprite:
		if (setPos == 1) {
			sprite.setPosition(game->window->getView().getSize().x - pos.x, pos.y);
		}
		else if (setPos == 0) {
			sprite.setPosition(game->window->getView().getSize().x / 2 + pos.x, pos.y);
		}
		else
		{
			sprite.setPosition(pos.x, pos.y);
		}
		break;
	case MT_Button:
	case MT_Selection:
		IsMouseOver = shape.getGlobalBounds().contains(game->window->mapPixelToCoords(sf::Mouse::getPosition(*game->window)));
	case MT_Text:
		if (setPos == 1) {
			text.setPosition(game->window->getView().getSize().x - pos.x, pos.y);
		}
		else if (setPos == 0) {
			text.setPosition(game->window->getView().getSize().x / 2 + pos.x, pos.y);
		}
		else
		{
			text.setPosition(pos.x, pos.y);
		}
		if (UIType == MT_Button || UIType == MT_Selection)
			UpdateButton();
		break;
	}
}

void UIobject::Draw(sf::RenderWindow *rw)
{
	if (haveshape) {
		rw->draw(shape);
	}
	if (havesprite) {
		rw->draw(sprite);
	}
	if (havetext) {
		rw->draw(text);
	}
}

void UIobject::CreateButton(const char * c, const unsigned int size, sf::Text::Style s)
{
	CreateText(c, size, s);
	shape.setFillColor(sf::Color(255, 255, 255, 0));
	shape.setSize(sf::Vector2f(text.getLocalBounds().width + 30.0f, text.getLocalBounds().height + 30.0f));
	shape.setOrigin(shape.getLocalBounds().width / 2.0f, shape.getLocalBounds().height / 2.0f - 14.0f);
	haveshape = true;
}

void UIobject::CreateText(const char * c, const unsigned int size, sf::Text::Style s)
{
	if (c == "noto") {
		text.setFont(game->GameResource->fontscore);
	}
	else
	{
		text.setFont(game->GameResource->font);
		text.setString(c);
	}
	text.setCharacterSize(size);
	text.setFillColor(sf::Color::Black);
	text.setStyle(s);
	text.setOrigin(text.getLocalBounds().width / 2.0f, text.getLocalBounds().height / 2.0f);
	havetext = true;
}

void UIobject::UpdateButton()
{
	shape.setPosition(text.getPosition());
	sf::Color c(shape.getFillColor());
	float scale;
	if (IsMouseOver) {
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			scale = GGEZ::Smooth(shape.getScale().x, 1.0f, game->deltatime*5.0f);
			c.a = 255;
		}
		else
		{

			scale = GGEZ::Smooth(shape.getScale().x, 1.2f, game->deltatime*5.0f);
			c.a = 128;
			if (UIType == MT_Selection) {
				if (IsSelected) {
					c.a = 192;
				}
			}
		}
	}
	else
	{

		scale = GGEZ::Smooth(shape.getScale().x, 1.0f, game->deltatime);
		c.a = 0;
		if (UIType == MT_Selection) {
			if (IsSelected) {
				c.a = 128;
			}
		}
	}
	shape.setFillColor(c);
	shape.setScale(scale, scale);
	text.setScale(scale, scale);
}

void UIobject::ChangeText(const std::string s) {
	text.setString(s);
	SetTextAlign();
	if (haveshape) {
		shape.setSize(sf::Vector2f(text.getLocalBounds().width + 30.0f, text.getLocalBounds().height + 30.0f));
		shape.setOrigin(shape.getLocalBounds().width / 2.0f, shape.getLocalBounds().height / 2.0f - 14.0f);
	}
}

void UIobject::ChangeText(const std::wstring* s) {
	text.setString(*s);
	SetTextAlign();
}

void  UIobject::SetTextAlign() {
	if (setPos == 1) {
		text.setOrigin(text.getLocalBounds().width, text.getLocalBounds().height / 2.0f);
	}
	else if (setPos == 0) {
		text.setOrigin(text.getLocalBounds().width / 2.0f, text.getLocalBounds().height / 2.0f);
	}
	else
	{
		text.setOrigin(0, text.getLocalBounds().height / 2.0f);
	}
}

void UIobject::ChangeSprite(sf::Texture* text) {
	sprite.setTexture(*text, true);
	sprite.setOrigin(sprite.getTexture()->getSize().x / 2, 0);
}

UIobject::~UIobject()
{
}
