#ifndef _ROCKET_H_
#define _ROCKET_H_

#include "position.h"

class Rocket
{
public:
	double size;
	Position *pos;
public:
	Rocket(double size);
	~Rocket();
	void setInitPosition();
    void moveOnX(float displacement);
    void moveOnY(float displacement);
    void moveOnZ(float displacement);
	void drawRocket();
};

#endif