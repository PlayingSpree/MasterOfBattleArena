#include "AnimatedSprite.h"

AnimatedSprite::AnimatedSprite()
{
}

AnimatedSprite::AnimatedSprite(sf::Texture* Texture, int ImageWidth, int ImageHeight, int ImageCountPerSprite, int CurrentFrame, int CurrentSprite, float FramePerSecound, bool bounce,int FloorPerSprite)
{
	this->Texture = Texture;
	Sprite = sf::Sprite(*Texture);
	this->ImageWidth = ImageWidth;
	this->ImageHeight = ImageHeight;
	this->ImageCountPerSprite = ImageCountPerSprite;
	this->CurrentFrame = CurrentFrame;
	this->CurrentSprite = CurrentSprite;
	this->FramePerSecound = FramePerSecound;
	this->FloorPerSprite = FloorPerSprite;
	this->Time = 0.0f;
	setTextureRect();
	Bounce = bounce;
	Reverse = false;
}
void AnimatedSprite::Update(float deltatime)
{
	Time += deltatime;
	if (FramePerSecound > 0) {
		if (Time >= FramePerSecound) {
			if (Reverse)
				CurrentFrame--;
			else
				CurrentFrame++;
			if (CurrentFrame >= ImageCountPerSprite*FloorPerSprite) {
				if (Bounce) {
					Reverse = true;
					CurrentFrame -= 2;
				}
				else
				{
					CurrentFrame = 0;
				}
			}
			if (CurrentFrame < 0) {
				if (Bounce) {
					Reverse = false;
					CurrentFrame = 1;
				}
				else
				{
					CurrentFrame = ImageCountPerSprite*FloorPerSprite - 1;
				}
			}
			setTextureRect();
			Time = 0;
		}
	}
}
void AnimatedSprite::SetSprite(int Sprite, int frame)
{
	CurrentSprite = Sprite;
	CurrentFrame = frame;
	setTextureRect();
}
void AnimatedSprite::SetSprite(int Sprite)
{
	SetSprite(Sprite, CurrentFrame);
}
void AnimatedSprite::setTextureRect()
{
	sf::IntRect rect;
	int left = ImageWidth*CurrentFrame;
	int top = ImageHeight*CurrentSprite*FloorPerSprite;
	if (FloorPerSprite > 1) {
		int i = CurrentFrame;
		while (i>=ImageCountPerSprite)
		{
			top += ImageHeight;
			left -= ImageCountPerSprite*ImageWidth;
			i -= ImageCountPerSprite;
		} 
	}
	int y = Texture->getSize().y;
	while (top >= y)
	{
		left += ImageCountPerSprite*ImageWidth;
		top -= y;
	}
	rect.width = ImageWidth;
	rect.height = ImageHeight;
	rect.left = left;
	rect.top = top;

	Sprite.setTextureRect(rect);
}