#include "Vector.h"


/*
Metoda zmienia wspolrzedne wektora
*/

void Vector::Change(double a, double b, double c)
{
	x = a;
	y = b;
	z = c;
}


/*
Jednoargumentowy +
*/

Vector &Vector::operator+()
{
	return *this;
}

/*
Jednoargumentowy -
*/

Vector &Vector::operator-()
{
	x = -x;
	y = -y;
	z = -z;
	return *this;
}

/*
Operator +=
*/

Vector &Vector::operator+=(const Vector &other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

/*
Operator -=
*/

Vector &Vector::operator-=(const Vector &other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}

/*
Operator *=
*/

Vector &Vector::operator*=(double k)
{
	x *= k;
	y *= k;
	z *= k;
	return *this;
}

/*
Operator /=
*/

Vector &Vector::operator/=(double k)
{
	x /= k;
	y /= k;
	z /= k;
	return *this;
}

/*
Operator przypisania
*/

Vector &Vector::operator=(const Vector &other)
{
	if (this == &other) return *this;
	x = other.x;
	y = other.y;
	z = other.z;
	return *this;
}

/*
Operator wyciagania wspolrzednych wektora
*/

double &Vector::operator[] (int i)
{
	if (i == 0)
		return x;
	else if (i == 1)
		return y;
	else 
		return z;
}

/*
Jak wyzej tylko ze const
*/

const double &Vector::operator[] (int i) const
{
	if (i == 0)
		return x;
	else if (i == 1)
		return y;
	else 
		return z;
}

// Metody zaprzyjaznione:

/*
Dwuargumentowy +
*/

Vector operator+(const Vector &a, const Vector &b)
{
	return Vector(a.x + b.x, a.y + b.y, a.z + b.z);
}

/*
Dwuargumentowy -
*/

Vector operator-(const Vector &a, const Vector &b)
{
	return Vector(a.x - b.x, a.y - b.y, a.z - b.z);
}

/*
Mnozenie wektora przez stala
*/

Vector operator*(const Vector &a, double k)
{
	return Vector(a.x * k, a.y * k, a.z * k);
}

/*
Mnozenie stalej przez wektor
*/

Vector operator*(double k, const Vector &a)
{
	return Vector(a.x * k, a.y * k, a.z * k);
}

/*
Dwuargumentowy * (mnozenie wektorow przez siebie)
*/

Vector operator*(const Vector &a, const Vector &b)
{
	return Vector(a.x * b.x, a.y * b.y, a.z * b.z);
}

/*
Dwuargumentowy /
*/

Vector operator/(const Vector &a, double k)
{
	return Vector(a.x / k, a.y / k, a.z / k);
}

/*
Iloczyn skalarny - w wyniku otrzymujemy liczbe
*/

double Dot(const Vector &a, const Vector &b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

/*
Iloczyn wektorowy - w wyniku otrzymujemy wektor
*/

Vector Cross(const Vector &a, const Vector &b)
{
	return Vector(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

/*
Normalizowanie wektora (tzn. sprowadzenie go do dlugosci
rownej 1 przy zachowaniu jego kierunku)
*/

Vector Normalize(const Vector &w)
{
	if (Length(w) != 0)
		return (w / Length(w));
	else
		return Vector();
}

/*
Dlugosc wektora
*/

double Length(const Vector &w)
{
	return (double) sqrt(w.x * w.x + w.y * w.y + w.z * w.z);
}





