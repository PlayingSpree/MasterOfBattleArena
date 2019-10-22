#pragma once
#include "MenuButton.h"

class UIsystem
{
public:
	std::vector<UIobject*> UIObjectList;
	sf::Sprite BG;
	Game* game;

	bool IsButtonPressed(const int tag);
	void RemoveUIobject(const int tag);

	void Draw();
	void clear();
	UIsystem(Game* game);
	~UIsystem();
	void SelectionUpdate(const int * taglist, const size_t size, const int selection);
	UIobject* FindUIobject(const int tag);
};

