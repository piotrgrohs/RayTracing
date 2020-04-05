#include <math.h>
#include "objects.h"
#include "ray.h"

/*
Metoda znajdujaca przeciecie promienia swiatla (raywp) z kula.
Zwraca zero dla braku przeciecia lub 1 gdy istnieje przeciecie
Wyliczona wartosc "t" zwracana jest przez argument!
*/

int Sphere::FindInter(const Ray &ray1, double &t)
{
	/* 

	UWAGA!!!
	Celem metody jest 
	a) obliczenie pierwiastkow (rownania przeciecia kuli z promieniem)
	b) sprawdzenie warunkow dla obliczonych pierwiastkow
	c) przekazanie informacji o znalezionym pierwiastku (o ile spelnia on warunki). 

	ad. a) Wszystkie obliczenia przeprowadzamy na zmiennych typu 'double'. Pierwiastek równania
	oznaczaj¹cy przeciêcie przyjmujemy za ujemny gdy jest mniejszy niz 0.001 (ze wzgl. na niedokladnosc
	obliczen).
	ad. b) Przekazana do funkcji wartosc parametru t opisuje najbli¿sze znalezione do tej pory 
	przeciecie. _Dodatkowym_ warunkiem ktory musi byc spelniony przez pierwiastek jest aby jego 
	wartosc byla mniejsza niz przekazana wartosc t.
	ad. c) Jezeli funkcja nie znajdzie przeciecia spelniajacego warunki, zwraca 0 i nie zmienia 
	wartosci 't', jezeli natomiast znajdzie przeciecie, zwraca 1 i modyfikuje parametr 't'.
	
	Informacje pomocnicze:
	
	Punkt startowy (= po³o¿enie kamery) oraz wektor kierunkowy 
	zwracaj¹ odpowiednie metody wywo³ane na rzecz obiektu promienia (Ray)

	Vector - klasa reprezentujaca wektor; dostêp do sk³adowych przez operator
			nawiasów kwadratowych: w[0], w[1], w[2]
	Vector Normalize(const Vector &w) - funkcja zwracajaca jako rezultat wektor znormalizowany

	*/


	return 0;
}


/*
Metoda znajduje wektor normalny do kuli w danym jej punkcie
*/ 

Vector Sphere::FindNormal(const Vector &v)
{
	return Normalize(v - pos);
}


/*
Metoda znajdujaca przeciecie dla walca
*/

int Cylinder::FindInter(const Ray &l, double &t)
{
	double	a, b, c; // wsp. rownania
	double	delta;
	double	t1, t2; // pierwiastki
	Vector	temp = l.Point() - point; // wektor pomocniczy
	Vector	dir = l.Direction(); // wektor kierunkowy

	a = Dot(dir, dir * axis);
	b = 2 * Dot(temp, dir * axis);
	c = Dot(temp, temp * axis) - 1; // parametry rownania kwadratowego

	delta = b * b - 4.0 * a * c; // rozwiazujemy rownanie kwadratowe by znalezc przeciecie

	if (delta < 0)
		return 0; // brak przeciec
	else
	{
		delta = sqrt(delta);
		
		t1 = (-b - delta) / (2.0 * a); // obliczenie pierwiastkow...
		if (fabs(t1) < 0.01) t1 = -1.0; //... z zabezpieczeniem na zbyt male wartosci

		t2 = (-b + delta) / (2.0 * a);
		if (fabs(t2) < 0.01) t2 = -1.0;

		if (t1 < 0)
			if (t2 < t)
				if (t2 > 0)
				{
					t = t2;
					return 1; // znaleziono przeciecie
				}
				else
					return 0; // brak przeciec
			else
				return 0; // brak...
		else
			if (t2 < 0)
				if (t1 < t)
					if (t1 > 0)
					{
						t = t1;
						return 1; // przeciecie
					}
					else return 0; // brak
				else
					return 0; // brak
			else
			{
				t1 = t1 < t2 ? t1 : t2; // znajdz mniejszy
				if (t1 < t)
				{
					t = t1;
					return 1; // okay
				}
				else
					return 0; // brak
			}
	}
}

/*
Metoda znajdujaca normalna do walca
*/

Vector Cylinder::FindNormal(const Vector &a)
{
	return Normalize((a - point) * axis);
}

/*
Metoda znajdujaca przeciecie dla stozka
*/

int Cone::FindInter(const Ray &l, double &t)
{
	double	a, b, c; // wspolczynniki rownania
	double	delta; // delta rownania
	double	t1, t2; // pierwiastki
	Vector	temp = l.Point() - point; // wektor pomocniczy
	Vector	dir = l.Direction(); // wektor kierunku

	a = Dot(dir, dir * axis);
	b = 2 * Dot(temp, dir * axis);
	c = Dot(temp, temp * axis);

	delta = b * b - 4.0 * a * c; // rozwiazanie rownania kawadratowego...

	if (delta < 0)
		return 0; // brak przeciec
	else
	{
		delta = sqrt(delta);

		t1 = (-b - delta) / (2.0 * a); // obliczenie pierwiastkow 
		if (fabs(t1) < 0.01) t1 = -1.0; // z uwzglednieniem pewnej dokladnosci

		t2 = (-b + delta) / (2.0 * a);
		if (fabs(t2) < 0.01) t2 = -1.0;

		if (t1 < 0)
			if (t2 < t)
				if (t2 > 0)
				{
					t = t2;
					return 1; // jest przeciecie
				}
				else
					return 0;
			else
				return 0; // brak
		else
			if (t2 < 0)
				if (t1 < t)
					if (t1 > 0)
					{
						t = t1;
						return 1; // przeciecie
					}
					else
						return 0; // brak
				else
					return 0; // brak
			else
			{
				t1 = t1 < t2 ? t1 : t2; // znajdujemy mniejszy
				if (t1 < t)
				{
					t = t1;
					return 1; // przeciecie
				}
				else
					return 0; // brak przeciecia
			}
	}
}


/*
Metoda znajdujaca normalna do stozka
*/

Vector Cone::FindNormal(const Vector &a)
{
	return Normalize((a - point) * axis);
}


/*
Metoda znajdujaca przeciecie z plaszczyzna
- o wiele prostsza niz dla kuli, walca czy stozka
*/

int Plane::FindInter(const Ray &ray_cp, double &t)
{
	// Uwaga: parametry A, B, C plaszczyzny to po prostu skladowe jej wektora normalnego

	return 0;
}

/*
Metoda znajdujaca normalna do plaszczyzny
*/

Vector Plane::FindNormal(const Vector &a)
{

	return Normalize(normal);
}

