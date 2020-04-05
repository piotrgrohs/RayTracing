#ifndef __MAPPING_H
#define __MAPPING_H

#include <math.h>

#include "material.h"

// frac bedzie zwracalo czesc ulamkowa liczby:
#define frac(x) ((x)-floor(x))
#define __WP

#ifndef __PI
#define __PI

const double PI = 3.141592654;

#endif

/*
Klasa Mapping jest abstrakcyjna. Okresla ona odwzorowanie punktu
z przestrzeni trojwymiarowej na przestrzen dwuwymiarowa. Na podstawie
wspolrzednych w przestrzeni 3d znajdowany jest punkt 2d, odpowiadajacy
barwie obiektu w danym miejscu.

Funkcje GetUV zostaly zdefiniowane jako INLINE ze wzgledu na wymagana
szybkosc dzialania (sa wywolywane bardzo czesto), ponadto sa to
jedyne metody w tych klasach, dlatego nie wplywa to na przejrzystosc
*/

class Mapping
{
public:
	virtual Vector GetUV(const ShInfo &a) = 0; // pobranie wspolrzednych 2D 
};

/*
Mapowanie sferyczne uzywane jest glownie do kul, ale takze do wszelakich
obiektow zblizonych ksztaltem do kuli.
*/

class Spherical : public Mapping
{
public:
	
	virtual Vector GetUV(const ShInfo &s)
	{
		/*
		  UWAGA: 
		  - Wektor normalny do powierzchni w badanym punkcie przechowuje struktura ShInfo (=> s.normal)
		  - Jest zdefiniowana stala o nazwie PI
		  - Istnieje funkcja 'acos()'
		  - Obliczone u,v nalezy zwrocic jako wektor o wspolrzednych [u, v, 0]
		  - Wystepujace we wzorach na u,v wartosci x, y, z, R sa NIEZNANE, nalezy je stamtad wyeliminowac matematycznie (na kartce - nie moga wystepowac w kodzie!)
		*/
		return Vector(0, 0, 0);

	}
};

/* 
Mapowanie cylindryczne uzywane jest praktycznie tylko do walcow 
oraz stozkow.
*/

class Cylindrical : public Mapping
{
private:
	double height; // wysokosc tekstury

public:
	Cylindrical(double a) : height(a) {}

	virtual Vector GetUV(const ShInfo &s)
	{
		double v = s.point[1] / height; // wspolrzedna y
		double u = acos(s.normal[0]) / PI; // wsp. x

		v = frac(v); // tekstura powtarzalna
		return Vector(u, fabs(v), 0);
	}
};

/*
Mapowanie plaszczyznowe sluzy do oblozenia tekstura obiektow plaskich
(czyli np. plaszczyzn). Tekstura jest jakby rzucana z rzutnika przezroczy
na dany obiekt.
*/

class Planar : public Mapping
{
private:
	double x, y; // rozmiary tekstury

public:
	Planar(double x1, double y1) : x(x1), y(y1) {}

	virtual Vector GetUV(const ShInfo &a)
	{
		double max = fabs(a.normal[0]);
		double u, v;

		if (fabs(a.normal[1]) > max)
		{
			max = fabs(a.normal[1]);
			if (fabs(a.normal[2]) > max)
			{
				u = a.point[0];
				v = a.point[2];
			}
			else
			{
				u = a.point[0];
				v = a.point[2];
			}
		}
		else
		{
			if (fabs(a.normal[2]) > max)
			{
				u = a.point[0];
				v = a.point[1];
			}
			else
			{
				u = a.point[1];
				v = a.point[2];
			}
		}

		u /= x;
		u -= floor(u);
		v /= y;
		v -= floor(v);

		return Vector(fabs(u), fabs(v), 0);
	}

	void SetXY(double x1, double y1) // ustalenie nowych rozmiarow tekstury
	{
		x = x1;
		y = y1;
	}
};






#endif
