#include "UIsystem.h"


UIsystem::UIsystem(Game* game) : game(game)
{
}

UIsystem::~UIsystem()
{
}

void UIsystem::SelectionUpdate(const int* taglist, const size_t size, const int selection)
{
	for (size_t i = 0; i < UIObjectList.size(); i++)
	{
		for (size_t it = 0; it < size; it++)
		{
			if (UIObjectList[i]->tag == taglist[it]) {
				UIObjectList[i]->IsSelected = selection==it?true:false;
				UIObjectList[i]->text.setStyle(selection == it ? sf::Text::Bold:sf::Text::Regular);
				UIObjectList[i]->shape.setFillColor(selection == it ? sf::Color::Yellow : sf::Color::White);
			}
		}
	}
}

UIobject* UIsystem::FindUIobject(const int tag)
{
	for (size_t i = 0; i < UIObjectList.size(); i++)
	{
		if (UIObjectList[i]->tag == tag) {
			return UIObjectList[i];
		}
	}
	return 0;
}

bool UIsystem::IsButtonPressed(const int tag)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			for (size_t i = 0; i < UIObjectList.size(); i++)
			{
				if (UIObjectList[i]->tag == tag) {
					return UIObjectList[i]->IsMouseOver;
				}
			}
	}
	return false;
}

void UIsystem::RemoveUIobject(const int tag)
{
	for (size_t i = 0; i < UIObjectList.size(); i++)
	{
		if (UIObjectList[i]->tag == tag) {
			delete UIObjectList[i];
			UIObjectList.erase(UIObjectList.begin() + i--);
		}
	}
}

void UIsystem::Draw() {
	//SetView
	sf::View view;

	unsigned int height = 1080;
	unsigned int width = game->windowratio*height;

	view.setCenter(width / 2.0f, height / 2.0f);

	view.setSize(width, height);
	game->window->setView(view);
	//Draw BG
	float factor;

	unsigned int twidth = BG.getTexture()->getSize().x;
	unsigned int theight = BG.getTexture()->getSize().y;

	float tfactor = (float)twidth / (float)theight;
	if (game->windowratio > tfactor)
		factor = (float)width / (float)twidth;
	else
		factor = (float)height / (float)theight;
	BG.setScale(factor, factor);
	game->window->draw(BG);
	//DrawMenuObject
	for (size_t i = 0; i < UIObjectList.size(); i++)
	{
		UIObjectList[i]->Update();
		UIObjectList[i]->Draw(game->window);
	}
}

void UIsystem::clear() {
	for (size_t i = 0; i < UIObjectList.size(); i++)
	{
		delete UIObjectList[i];
	}
	UIObjectList.clear();
}
