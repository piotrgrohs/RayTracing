#ifndef __TEXTURE_H
#define __TEXTURE_H

#include "vector.h"
#include "color.h"
#include "mapping.h"
#include "picture.h"
#include <string.h>

// bajt i slowo (slowo = 2 * bajt)

typedef unsigned char byte;
typedef	unsigned word;


/*
Klasa tekstury, abstrakcyjna. Tekstura okresla barwe obiektu.
*/

class Texture
{
public:
	Texture() {};

	virtual ~Texture() {};
	virtual Color GetColor(const ShInfo& s) = 0; // pobranie koloru powierzchni
};


/* 
Tekstura jednobarwna
*/

class OneColor : public Texture
{
private:
	Color col; // kolor tekstury

public:

	OneColor(const Color& c): col(c) {}
	// dla tekstury "szarej":
	OneColor(double a = 0) : col(NormalizeColor(Color(a, a, a))) {}
	
	virtual Color GetColor(const ShInfo& s) {return col;}
};


/*
Tekstura okreslajaca kolor na podstawie obrazka, ktory jest wczytywany
z pliku i przechowywany w pamieci
*/

class Image : public Texture
{
private:
	int			loaded; // czy udalo sie wczytac obrazek?
	PicBuffer	buffer; // obrazek
	Mapping		*m; // rodzaj mapowania - ma tutaj zasadnicze znaczenie

	// metoda zwraca kolor pixla o zadanych wspolrzednych;
	// zapisana inline, gdyz wywolywana jest dosc czesto:
	Color GetPixel(unsigned x, unsigned y)
	{
		// sprawdzenie czy zakres nie zostal przekroczony:
		if (x >= (uint32_t)(buffer.x))
			x = x % buffer.x;
		if (y >= (uint32_t)(buffer.y))
			y = y % buffer.y;

		if (loaded) // czy obrazek wczytany?
		{
			long offset = (x + y * buffer.x); 
			return Color((double) buffer.r[offset] / 255,(double) buffer.g[offset] / 255,(double) buffer.b[offset] / 255);
		}
		else
			return Color(1,1,1); // jesli nie ma tekstury, zwroc kolor bialy
	}

public:

	Image(char *fname, Mapping *map = NULL); // konstruktor wczytuje teksture o zadanej nazwie

	// metoda GetColor wywolywana jest czesto i dlatego zdefiniowana zostala
	// jako inline

	virtual Color GetColor(const ShInfo &a)
	{

		Vector UV = m->GetUV(a); // znalezienie wspolrzednych 2d 
		// szereg zmiennych pomocniczych, by uniknac powtarzania tych samych obliczen:
		double xt, yt, xtf, ytf; // wspolrzedne punktu i ich czesci ulamkowe
		Color t1, t2, t3, t4; // kolory w rogach interpolowanego kwadratu

		xt = UV[0] * buffer.x;
		yt = UV[1] * buffer.y; 
		t4 = GetPixel((uint32_t)floor(xt), (uint32_t)floor(yt));
		t3 = GetPixel((uint32_t)floor(xt+1), (uint32_t)floor(yt));
		t2 = GetPixel((uint32_t)floor(xt), (uint32_t)floor(yt+1));
		t1 = GetPixel((uint32_t)floor(xt+1), (uint32_t)floor(yt+1));

		xtf = xt - floor(xt);
		ytf = yt - floor(yt);

		// ponizszy kod odpowiedzialny jest za rozmycie tekstury:
		return ((t1 * xtf + t2 * (1 - xtf)) * ytf + (t3 * xtf + t4 * (1 - xtf)) * (1 - ytf));
		
	}

	void SetMappingType(Mapping *map) {m = map;} // ustawienie rodzaju mapowania
	int IsLoaded() {return loaded;} // czy plik zostal zaladowany?
};


#endif