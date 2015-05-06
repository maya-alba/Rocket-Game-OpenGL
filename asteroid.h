#ifndef _ASTEROID_H_
#define _ASTEROID_H_

#include "position.h"

class Asteroid
{
public:
	double size;
	Position *pos;
    float dispX;
    float dispY;
    float dispZ;

public:
	Asteroid();
	~Asteroid();
    void setDisplacementVelocity(float dX, float dY, float dZ);
	void setRandomInitPosition(float posX, float posY, float posZ);
    void moveOnX();
    void moveOnY();
    void moveOnZ();
	void drawAsteroid(GLUquadric *quad);
};

#endif