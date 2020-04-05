#include "Color.h"

/*
Metoda normalizujaca wartosc koloru
*/

Color NormalizeColor(const Color &col)
{
	double max = 1;

	if (col[0] > max) max = col[0];
	if (col[1] > max) max = col[1];
	if (col[2] > max) max = col[2];

	return col / max;
}