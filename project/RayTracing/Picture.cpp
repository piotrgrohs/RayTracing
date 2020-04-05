#include "picture.h"

#include <string.h>

/*
Destruktor bufora obrazka - usuwa (jesli zostaly zaalokowane) odpowiednie
bufory skladowe
*/

PicBuffer::~PicBuffer()
{
	if (r)
		delete r;
	if (g)
		delete g;
	if (b)
		delete b;
}

/*
PicBuffer rezerwuje pamiec dla bufora na podstawie zawartych w nim
rozmiarow. W razie niepowodzenia zwraca -1
*/

int PicBuffer::Init()
{
	if (r || g || b)
		return -1; // bufor juz zajety!

	if (x <= 0 || y <= 0)
		return -1; // nieprawidlowy rozmiar

	if (!(r = new unsigned char[x * y]))
		return -1;

	if (!(g = new unsigned char[x * y]))
		return -1;

	if (!(b = new unsigned char[x * y]))
		return -1;

	return 0;
}


/*
Konstruktor klasy obrazka otwiera plik do zapisu lub odczytu
i w razie bledu ustawia odpowiednie flagi
*/

Picture::Picture(const char *name, int mode)
{
	error = 0;
	isopened = 0;
	if (mode == 0)
		file.open(name, ios::binary | ios::in); // do odczytu
	else
		file.open(name, ios::binary | ios::out); // do zapisu

	if(!file)
		error = 1; // blad otwarcia
	else
		isopened = 1; // ok, udalo sie otworzyc
}

/*
Destruktor sprzata po obrazku zamykajac otwarty plik
*/

Picture::~Picture()
{
	if (isopened)
		file.close();
}


/*
Metoda odczytujaca naglowek pliku BMP i sprawdzajaca poprawnosc
tego naglowka. W razie problemow zwraca -1.
*/

int BMPfile::ReadHeader(PicBuffer &buffer)
{
	BMPheader header;

	if (!isopened)
	{
		error = 2;
		return -1;
	}

	if(!file.read((char *)&header, sizeof(BMPheader)))
	{
		error = 1; // blad odczytu naglowka!
		return -1;
	}
	else
	{
		if (Dwordtol(header.comp) != 0 || header.id[0] != 'B' || header.id[1] != 'M' || Dwordtol(header.bpp) != 24)
		{
			error = 1; // zly rodzaj pliku!
			return -1;
		}
		else
		{
			buffer.x = Dwordtol(header.width); // ustaw rozmiar
			buffer.y = Dwordtol(header.height);

			error = 0; // i gotowe
			return 0;
		}
	}	
}


/*
Metoda zapisujaca naglowek do pliku BMP.
Na poczatku ustawia pola naglowka na wymagane wartosci,
a nastepnie zapisuje caly naglowek.
W razie bledu zwraca -1.
*/

int BMPfile::WriteHeader(int width, int height)
{
    BMPheader header; // nasz naglowek
	
	if (!isopened)
	{
		error = 1;
		return -1;
	}
		
		// tu wypelniamy naglowek, nie ma sie co rozwodzic, te wartosci po
		// prostu musza tam byc
		
	strcpy_s(header.id, strlen("BM")+1, "BM");
	Ltodword(width * height * 3 + 54,header.size);
	Ltodword(0, header.rvd);
	Ltodword(54, header.bof);
	Ltodword(40, header.hdrsize);
	Ltodword(width, header.width);
	Ltodword(height, header.height);
	Ltoword(1, header.planes);
	Ltoword(24, header.bpp);
	Ltodword(0, header.comp);
	Ltodword(0, header.psize);
	Ltodword(2834, header.hres);
	Ltodword(2834, header.vres);
	Ltodword(0, header.cols);
	Ltodword(0, header.icols);
	
	
	
	if(!file.write((char *)&header, sizeof(BMPheader)))
	{
		error = 1;
		return -1;
	}
	else
	{
		error = 0;
		return 0;
	}
}


/*
Metoda odczytujaca kolejny pixel z pliku BMP.
Nalezy pamietac, ze plik BMP jest zapisany "do gory nogami"
W razie bledu zwraca -1
*/

int BMPfile::PixelRd(Pixel24 &p)
{
	if (!isopened)
	{
		error = 1; // plik nie jest otwarty
		return -1;
	}

	if(!file.read((char *) &p, 3))
	{
		error = 1; // blad odczytu
		return -1;
	}
	else
	{
		error = 0;
		return 0;
	}
}


/*
Metoda zapisujaca pixel do pliku BMP.
Podobnie jak przy odczycie nalezy pamietac o specyficznej
strukturze BMP ("do gory nogami"
*/

int BMPfile::PixelWt(const Pixel24 &p)
{

	if (!isopened)
	{
		error = 1; // plik nie jest otwarty!
		return -1;
	}
	
	if(!file.write((char *) &p, 3))
	{
		error = 1; // blad zapisu
		return -1;
	}
	else
	{
		error = 0; // ok
		return 0;
	}
}
	
