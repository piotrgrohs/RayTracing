#ifndef __SCENE_H
#define __SCENE_H

#include "vector.h"
#include "color.h"
#include "light.h"
#include "objects.h"
#include "ray.h"
#include "material.h"
#include "texture.h"

#include <math.h>

// stala ograniczajaca promien odgornie
const double maxray = 1000000.0;

/*
Pomocnicza lista dla sceny, jednokierunkowa, uzywana
do przechowywania swiatel i obiektow
*/

template <class T> class SList
{
private:
	struct Element
	{
		T	*member;
		Element	*next;
	};

	Element *head;

public:
	SList() : head(0) {}
	~SList();

	int Add(T *member); // dodanie obiektu na liste; zwraca -1 w razie bledu
	T *Return(int n); // zwrocenie obiektu o zadanym numerze
};

/*
Rdzen calego programu - klasa sceny laczy wszystkie informacje o obiektach,
swiatlach, kamerze itd. oraz za pomoca metody Trace liczy obrazek.

Szereg metod dostepnych z zewnatrz sluzy do ustawienia wszystkich
niezbednych parametrow sceny.
*/



class Scene
{
private:
	SList<Object>	objects; // lista obiektow sceny
	SList<Light>	lights; // lista swiatel sceny

	Color		ambient; // kolor tla

	unsigned	xres, yres; // rozdzielczosc

	double		FOV; // pole widzenia (kat)
	double		focus; // odleglosc kamery od ekranu
	double		aspect; // wsp. proporcjonalnosci

	Vector		campos; // pozycja kamery
	Vector		camdir; // kierunek patrzenia
	double		roll; // obrot kamery wokol osi patrzenia

	Color		fog; // kolor mgly
	double		dmax, dmin; // zasieg mgly
	int			fogen; // czy mgla jest aktywna?

	int			maxdeep; // maksymalna ilosc zaglebien rekurencji

	char		name[255]; // nazwa pliku


	Object *FindNearest(const Ray &l, double &t); // znalezienie najblizszego przeciecia 
	Color CalcColor(const ShInfo* info, int deep); // obliczenie koloru w punkcie z zadana liczba zaglebien
	double CalcFocus(); // obliczenie parametru focus
	double CalcFog(double depth); // obliczenie mgly w zadanej odleglosci od obserwatora

public:

	Scene();


	void SetAmbient(const Color &col) {ambient = col;} // ustawia kolor oswietlenia rozproszonego
	void SetFog(double min, double max, Color col); // ustawia parametry mgly
	void FogEn() {fogen = 1;} // wlacza mgle
	void FogDis() {fogen = 0;} // wylacza mgle
	void SetAspect(double a) {aspect = a;} // ustawia stosunek pikseli (szer./wys.)
	void SetH(unsigned H) {xres = H;} // ustawia szerokosc obrazka
	void SetV(unsigned V) {yres = V;} // ustawia wysokosc obrazka
	void SetFOV(unsigned FOV); // ustawia pole widzenia
	void SetFile(const char *fname); // ustawia nazwe pliku do renderowania
	void SetCam(const Vector &pos, const Vector &dir); // ustawia kamere
	void SetRoll(double r) {roll = r;} // ustawia obrot kamery
	void SetDeep(int max) {maxdeep = max;} // ustawia maksymalna wartosc zaglebienia

	int AddObj(Object *obj); // dodanie obiektu do sceny
	int AddLight(Light *lt); // dodanie swiatla do sceny

	int Trace(); // generuje scene
};

#endif


