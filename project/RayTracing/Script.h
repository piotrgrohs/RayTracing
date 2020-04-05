#ifndef __SCRIPT_H
#define __SCRIPT_H

#include <fstream>
using namespace std;
#include "scene.h"

/*
Klasa listy jednokierunkowej oparta na szablonie. Lista przechowuje
obiekty, ktore przy usuwaniu listy sa USUWANE z pamieci. 
Lista moze wiec przechowywac tylko elementy dla ktorych 
uzytkownik przydzielil sobie pamiec i po usunieciu listy pamiec 
ta zostaje zwolniona (nie nalezy juz recznie usuwac tych obiektow).

Do listy mozna tylko dodawac obiekty, gdyz w obecnym jej zastosowaniu
nie zachodzi potrzeba usuwania elementow z listy
*/

template <class T> class List
{
private:
	struct Element
	{
		char	name[16]; // nazwa elementu
		T		 *member; // skladnik listy
		Element	 *next; // wskaznik na nast. element
	};

	Element *head; // glowa listy

public:
	List() : head(0) {}
	~List();

	int Add(T *memb, const char *name); // dodanie elementu o zadanej nazwie
	T *GiveMemb(const char *name); // znalezienie elementu na podstawie nazwy (w razie braku zwraca null)
	T *GiveMemb(int n); // znalezienie n-tego elementu (w razie braku zwraca null)
};

/*
Klasa skryptu sluzy do otwarcia pliku skryptu i jego analizy oraz wykonania.
Analizator skryptu odpowiedzialny jest za sprawdzanie poprawnosci
parametrow obiektow, swiatel, materialow itd.
*/

class Script
{
private:
	fstream file; // pilk skryptu

	char errorm[255]; // komunikat bledu
	int isopened; // czy plik otwarty?

	Scene thescene; // scena
	Light *templ; // tymczasowe swiatlo
	Object *tempo; // tymczasowy wskaznik na obiekty

	char temp[255]; // bufor tymczasowy
	char name[16]; // bufor na wczytywane stringi
	char name2[16]; // bufor na wczytywane stringi

	// szereg list niezbednych do przechowywania wczytanych informacji
	List<Object> objects; // obiekty
	List<Light> lights; // swiatla
	List<Mapping> maps; // mapowania
	List<Material> mats; // materialy
	List<Texture> txts; // tekstury

	// ponizej szereg metod odpowiedzialnych za analize poszczegolnych
	// fragmentow skryptu. Maja one ujednolicony system bledow:
	// - gdy wszystko OK, zwracane jest 0
	// - gdy blad pliku -1
	// - gdy blad skryptu -2
	// - gdy blad pamieci -3

	int Trace();
	int Name();
	int Map();
	int Text();
	int Mat();
	int Light();
	int Obj();
	int Sce();

public:
	Script (const char *name);
	~Script();

	int DoScript(); // wykonaj skrypt; w razie bledu zwracana wartosc < 0
	
	const char *Error() {return errorm;} // zwraca komunikat bledu
};



#endif