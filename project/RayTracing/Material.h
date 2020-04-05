#ifndef __MATERIAL_H
#define __MATERIAL_H

#include "vector.h"

class Ray;
class Texture;
class Object;

/*
Material to agregat z informacjami o rodzaju powierzchni.
Okresla wspolczynniki odbic i zalaman swiatla oraz kolor
*/

class Material
{
public:
	Texture*	texture; // tekstura powierzchni okreslajaca jej barwe
	// odbicie zwierciadlane:
	double		specular; 
	double		specfactor; 
	double		reflection; 
	// zalamanie:
	double		refraction; 
	double		reffactor; 
};

/*
Pomocniczy agragat do przechowywania tymczasowej informacji o aktualnie
badanym punkcie powierzchni
*/

class ShInfo
{
public:
	Vector	normal;
	Vector	reflect;
	Vector	point;
	Object *object;
	Ray *ray;
};
#endif
