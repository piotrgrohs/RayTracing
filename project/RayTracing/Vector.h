#ifndef __VECTOR_H
#define __VECTOR_H

#include <math.h>

/* 
Klasa Vector opisuje wektor w matematycznym sensie, definiujac
dla niego podstawowe dzialania.

Klasa ta opisuje zarowno obiekty bedace wektorami, jak i punkty
(punkt mozemy traktowac jako wektor zaczepiony w (0,0,0)
*/
class Vector
{
private:
	double x, y, z; // wspolrzedne wektora

public:
	Vector(double a = 0, double b = 0, double c = 0) : x(a), y(b), z(c) {} // konstruktor (takze domyslny)
	Vector(const Vector &other) // konstruktor kopiujacy
	{
		x = other.x;
		y = other.y;
		z = other.z;
	}
	
	void Change(double a, double b, double c); // zmienianie wspolrzednych

	// operatory arytmetyczne

	Vector& operator+(); 
	Vector& operator-();
	Vector& operator=(const Vector &other);

	Vector& operator+=(const Vector &other);
	Vector& operator-=(const Vector &other);
	Vector& operator*=(double k);
	Vector& operator/=(double k);

	// operatory indeksacji

	double& operator[] (int i); 
	const double& operator[] (int i) const;

	// pozostale operatory zostaly zaprzyjaznione:

	friend Vector operator+(const Vector &a, const Vector& b);
	friend Vector operator-(const Vector &a, const Vector& b);
	friend Vector operator*(const Vector &a, double k); // dwie podobne metody:
	friend Vector operator*(double k, const Vector &a); // przemiennosc mnozenia...
	friend Vector operator*(const Vector &a, const Vector &b);
	friend Vector operator/(const Vector &a, double k);	


	friend double  Dot(const Vector &a, const Vector &b); // iloczyn skalarny
	friend Vector Cross(const Vector &a, const Vector &b); // iloczyn wektorowy
	friend double  Length(const Vector &a); // dlugosc wektora
	friend Vector Normalize(const Vector &a); // normalizowanie wektora
};


#endif