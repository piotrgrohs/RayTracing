#include "texture.h"

/*
Konstruktor dla tekstury obrazkowej byl dosc rozlegly,
dlatego umieszczony zostal tutaj, a nie w ciele klasy.
*/

Image::Image(char *fname, Mapping *map) : m(map)
{
	BMPfile	file(fname, 0); // otwarcie pliku, jesli byl problem,
	// to zaraz zostanie wykryty przy probie odczytu naglowka
	
	loaded = 0;
	
	if (file.ReadHeader(buffer) == 0) // wczytaj naglowek
	{
		if (buffer.Init() == 0) // czy udalo sie z pamiecia?
		{
			Pixel24 p; // pixel tymczasowy...

			for (long y = buffer.y - 1; y >= 0; y--)
			{
				long position = y * (buffer.x);
				for (long x = 0; x < buffer.x; x++)
				{
					file.PixelRd(p); // wczytaj pixel
					buffer.Set(position++, p); // i wpisz do bufora

				}
			}
			
			if (!file.IsError())
				loaded = 1; // udalo sie
		}
	}
}