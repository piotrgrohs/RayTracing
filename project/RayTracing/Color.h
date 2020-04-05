#ifndef __COLOR_H
#define __COLOR_H

#include "Vector.h"

/* 
Klasa Color opisuje obiekty bedace kolorami. Kolor ma trzy skladowe,
ktore sa tutaj interpretowane jako RGB.

Klasa Color nie rozni sie prawie niczym od Vector. Inny jest jednak
sposob normalizacji wektora i koloru, stad wprowadzenie nowej klasy
*/

class Color : public Vector
{
public :
	Color () {};
	Color (double r, double g, double b) : Vector(r, g, b) {};
	Color (const Vector &w) : Vector(w) {};

	friend Color NormalizeColor(const Color &col);
};

#endif