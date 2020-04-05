#ifndef __PICTURE_H
#define __PICTURE_H

#include <fstream>
using namespace std;

/* 
Agregat opisujacy pixel 24-bitowy w formacie BMP
*/

class Pixel24 
{ 
public:
	unsigned char b;
	unsigned char g;
	unsigned char r;
};
	
/*
Klasa bufora obrazka. Zawiera trzy tablice na wartosci skladowych
koloru oraz rozmiary obrazka
*/

class PicBuffer 
{
public:
	// skladowe sa upublicznione by umozliwic w razie potrzeby 
	// bardzo szybki dostep bez odwolywania sie do metod klasy

	unsigned char * r; // poszczegolne skladowe
	unsigned char * g;
	unsigned char * b;
	long x; // rozmiary
	long y;

	PicBuffer() : r(0), g(0), b(0), x(0), y(0) {}
	~PicBuffer();

	void SetXY(long x1, long y1) {x = x1; y = y1;} // ustalenie rozmiarow
	int Init(); // rezerwuje pamiec obrazka o zadanych rozmiarach, zwracajac -1 w razie niepowodzenia
	void Set(long offset, const Pixel24 &p) // metoda wpisuje kolor pod zadany adres
	{
		if (r && g && b) // czy bufor istnieje??
		{
			r[offset] = p.r;
			g[offset] = p.g;
			b[offset] = p.b;
		}
	}
};

/*
Klasa Picture jest klasa abstrakcyjna, opisujaca obiekty bedace plikami
zawierajacymi obrazki. Konstruktor otwiera plik, a destruktor go zamyka,
dzieki temu w razie naglego wyjscia z programu nie trzeba zaprzatac sobie
glowy zamykaniem otwartych plikow.
*/
class Picture
{
protected:
	int isopened; // czy plik otwarty?
	int mode; // 0 - odczyt, 1 - zapis
	int error; // flaga bledu
	fstream file; // plik obrazka
	
public:
	Picture(const char *name, int mode = 0);
	~Picture();

	int IsError() {return error;} // zwraca informacje o bledzie

	// ponizsze metody zwracaja -1 w razie niepowodzenia:
	virtual int PixelWt(const Pixel24 &p) = 0; // zapisuje piksel do pliku
	virtual int PixelRd(Pixel24 &pxl) = 0; // odczytuje piksel z pliku
};


/*
Klasa BMPfile umozliwia tworzenie obiektow odwolujacych sie
do plikow BMP. Zaimplementowane zostaly tylko pliki BMP 
w formacie 24-bitowym
*/

class BMPfile : public Picture
{

protected:

	typedef unsigned char dword[4]; // struktura 4-bajtowa
	typedef unsigned char word[2]; // struktura 2-bajtowa
	class BMPheader
	{ 
    public:
	  // naglowek pliku BMP;
	  // zachowano oryginalne angielskie komentarze zmiennych
		char id[2]; // ID = 'BM'
		dword size; // filesize
		dword rvd; // reserved (zawsze 0)
		dword bof; // bitmap offset
		dword hdrsize; // bitmapheader size (powinno byc 40)
		dword width; // bitmap width
		dword height; // bitmap height
		word planes; // number of planes (zazwyczaj 1)
		word bpp;	// bytes per pixel
		dword comp; // compression type (ja rozpatruje tylko przypadek 0)
		dword psize; // picture size (bytes) (tu jest 0)
		dword hres; // horizontal resolution (tu i ponizej to juz roznie bywa)
		dword vres; // vertical resolution
		dword cols; // no. of used colors
		dword icols; // no. of important colors
	};


/* Ponizsze funkcje odpowiedzialne sa za konwersje long<>word i
long<>dword. Zastosowalem je ze wzgledu na rozne rozmiary (w bajtach)
typu LONG dla roznych kompilatorow i systemow (program dzieki temu latwiej
mozna przeniesc na inne platformy) */

	long Dwordtol(dword a) // dword -> long
	{
		long A = (long) a[0];
		long B = (long) a[1];
		long C = (long) a[2];
		long D = (long) a[3];

		B <<= 8;
		C <<= 16;
		D <<= 24L;

		return (A+B+C+D);
	}

	void Ltodword(long a, dword number) // long -> dword
	{
		number[0] = (char) (a & 0xFF);
		number[1] = (char) ((a & 0xFF00) >> 8);
		number[2] = (char) ((a & 0xFF0000) >> 16);
		number[3] = (char) ((a & 0xFF000000) >> 24);
	}


	unsigned long Wordtol(word a) // word -> long
	{
		long A = (long) a[0];
		long B = (long) a[1];

		B *= 256L;

		return(A+B);

	}

	void Ltoword(long a, word number) // long -> word
	{
		number[0] = (char) (a & 0xFF);
		number[1] = (char) ((a & 0xFF00) >> 8);
	}

public:
	BMPfile(const char *name, int mode = 0) : Picture(name, mode) {}

	// ponizsze metody zwracaja -1 w razie bledu:
	int ReadHeader(PicBuffer &buffer); // odczyt naglowka i wypelnienie bufora
	int WriteHeader(int width, int height); // zapis naglowka
	virtual int PixelWt(const Pixel24 &p); // zapis pixela
	virtual int PixelRd(Pixel24 &p); // odczyt pixela
};


#endif