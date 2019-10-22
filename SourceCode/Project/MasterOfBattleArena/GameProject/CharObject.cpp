#include "CharObject.h"


void CharObject::CharStatusStruct::Update(const float deltatime)
{
	Status.walking = false;
	if (movex != 0 || movey != 0) {
		Status.walking = true;
	}
	else
	{
		Status.walking = false;
	}
	if(deltatime<1.0f/30.0f)
		speed -= speed*deltatime*30.0f;
	else
	{
		speed -= speed*0.95f;
	}

	if (speed.x > 5 && speed.x < -5)
		speed.x = 0;

	if (speed.y > 5 && speed.y < -5)
		speed.y = 0;

	if (HP > MaxHP) {
		HP = MaxHP;
	}
}

void CharObject::CharStatusStruct::DoHP(const float dmg)
{
	if (HP != 0)
		HP += dmg;
	if (HP < 0)
		HP = 0;
}
