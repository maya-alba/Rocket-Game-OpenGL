#include "position.h"

Position::Position()
{
	this->x = 0.;
	this->y = 0.;
	this->z = 0.;
}

Position::Position(double x, double y, double z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}
Position::~Position()
{

}