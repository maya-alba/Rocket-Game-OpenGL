#include <GL\glew.h>
#include <GL\freeglut.h>
#include <ctime>
#include <iostream>
#include <string>
#include "asteroid.h"
#include "position.h"

Asteroid::Asteroid(){
    
	this->size = 3.0;
	this->pos = new Position();    
}

Asteroid::~Asteroid(){

}
void Asteroid::setDisplacementVelocity(float dX, float dY, float dZ){
    this->dispX=dX;
    this->dispY=dY;
    this->dispZ=dZ;
}

void Asteroid::setRandomInitPosition(float posX, float posY, float posZ){
    this->pos->x = posX;
    this->pos->y = posY;
    this->pos->z = posZ;
}

void Asteroid::moveOnX(){
    this->pos->x+=this->dispX;
}
void Asteroid::moveOnY(){
    this->pos->y+=this->dispY;
}
void Asteroid::moveOnZ(){
    this->pos->z+=this->dispZ;
}

void Asteroid::drawAsteroid(GLUquadric *quad){
	glPushMatrix();
	glTranslatef(this->pos->x, this->pos->y, this->pos->z);
    gluSphere(quad, this->size, 10, 10);
    //glutWireSphere(this->size,10,10);
	glPopMatrix();
}
