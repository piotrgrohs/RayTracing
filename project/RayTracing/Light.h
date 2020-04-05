#ifndef __LIGHT_H
#define __LIGHT_H

#include "vector.h"
#include "Color.h"

// stala PI

#ifndef __PI
#define __PI

const double PI = 3.141592654;

#endif

/*
Klasa Light jest klasa abstrakcyjna. Okresla ono swiatlo i przechowuje
dane o jego pozycji oraz kolorze. Z klasy tej sa dziedziczone
sa konkretne rodzaje swiatel - punktowe i stozkowe 
*/

class Light
{
protected:
	Color col; // kolor swiatla
	Vector pos; // pozycja swiatla
public:

	Light (const Color &c, const Vector &p) : col(c), pos(p) {};

	Color ReturnColor() {return col;} // zwraca kolor
	Vector ReturnPos() {return pos;} // zwraca pozycje

	virtual Vector Direction(const Vector &a) = 0; // zwraca wektor od swiatla do wskazanego punktu, lub zero
	
};


/* 
Swiatlo punktowe. Promieniuje jednakowo we wszystkich kierunkach. 
*/

class PointLight : public Light
{
public:
	PointLight(const Color &c, const Vector &p) : Light(c, p) {}

	virtual Vector Direction(const Vector &a) {return Normalize(pos - a);}
};

/*
Swiatlo stozkowe. Promieniuje tylko w okreslony stozek, poza ktorym 
zrodlo jest niewidoczne (symulacja np. typowej lampy biurkowej)
*/

class SpotLight : public Light
{
private:
	Vector dir; // kierunek stozka oswietlenia
	double alpha; // kat stozka

public:
	SpotLight(const Color &c, const Vector &p, const Vector &d, double a) : Light(c, p)
	{
		dir = Normalize(d);
		alpha = cos(a * PI / 180);
	}

	virtual Vector Direction(const Vector &a)
	{
		Vector temp = Normalize(pos - a);

		if (-Dot(temp, dir) >= alpha)
			return temp; // jestesmy wewnatrz stozka oswietlenia
		else
			return Vector(0,0,0); // poza stozkiem oswietlenia
	}
};


#endif
