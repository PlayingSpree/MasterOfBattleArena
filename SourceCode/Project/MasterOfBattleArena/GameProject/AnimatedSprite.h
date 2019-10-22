#pragma once
#include <SFML\Graphics.hpp>
class AnimatedSprite
{
private:
	int ImageWidth;
	int ImageHeight;
	int CurrentFrame;
	int CurrentSprite;
	int FloorPerSprite;
public:
	sf::Texture* Texture;

	int ImageCountPerSprite;

	float Time;
	void setTextureRect();
	bool Bounce;
	bool Reverse;

	sf::Sprite Sprite;
	AnimatedSprite();
	AnimatedSprite(sf::Texture*,int ImageWidth, int ImageHeight, int ImageCountPerSprite, int CurrentFrame, int CurrentSprite, float FramePerSecound,bool bounce, int FloorPerSprite = 1);
	void Update(float deltatime);
	float FramePerSecound;
	void SetSprite(int Sprite, int Frame);
	void SetSprite(int Sprite);
};

