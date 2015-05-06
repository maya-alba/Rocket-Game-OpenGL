#include <GL\glew.h>
#include <GL\freeglut.h>
#include "rocket.h"
#include "position.h"

Rocket::Rocket(double size)
{
	this->size = size;
	this->pos = new Position();
}

Rocket::~Rocket()
{

}

void Rocket::setInitPosition()
{
    //120,0,100
    this->pos->x=85.;
    this->pos->y=0.;
    this->pos->z=240.;
}

void Rocket::moveOnX(float displacement){
    this->pos->x+=displacement;
}
void Rocket::moveOnY(float displacement){
    this->pos->y+=displacement;
}
void Rocket::moveOnZ(float displacement){
    this->pos->z+=displacement;
}

void Rocket::drawRocket()
{
	glPushMatrix();
	glTranslatef(this->pos->x, this->pos->y, this->pos->z);
    glutWireSphere(this->size,10,10);
	glPopMatrix();
}
