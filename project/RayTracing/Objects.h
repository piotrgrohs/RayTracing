#ifndef __OBJECTS_H
#define __OBJECTS_H

#include "Vector.h"
class Ray;
class Vector;
class Scene;
class Material;

/*
Klasa abstrakcyjna opisujaca obiekty 3d. Wlasciwoscia
kazdego obiektu jest jego material, okreslajacy takie rzeczy
jak kolor (tekstura), wspolczynniki odbicia i zalamania etc.

Najwazniejsza metoda jest FindInter, ktora znajduje przeciecie
promienia swiatla z danym obiektem.
*/

class Object
{
	friend class Scene;

protected:

	Material *mat;

public:
	Object() : mat(0) {;}
	Object(Material *m) : mat(m) {;}

	void SetMaterial(Material *m) {mat = m;} // ustaw material
	Material *ReturnMaterial() {return mat;} // zwroc rodzaj materialu

	virtual int FindInter(const Ray &r, double &t) = 0; // znajdz przeciecie
	virtual Vector FindNormal(const Vector &v) = 0; // znajdz normalna

};

/*
Kula opisana jest przez wspolrzedne srodka i promien
*/

class Sphere: public Object
{
private:
	Vector	pos; // pozycja
	double	rad; // promien
	double	rr; // kwadrat promienia (dla przyspieszenia obliczen)

public:

	Sphere() : Object(), pos(), rad(0), rr(0) {}
	Sphere(const Vector &p, double r, Material *m) : Object(m), pos(p), rad(r), rr(r * r) {}
	Sphere(double x, double y, double z, double r, Material *m) : Object(m), pos(x, y, z), rad(r), rr(r*r) {}

	virtual int FindInter(const Ray &r, double &t); // znajdz przeciecie
	virtual Vector FindNormal(const Vector &v); // znajdz normalna

};


/*
Walec (nieskonczony) opisany jest przez wspolrzedne srodka i swoja os
*/

class Cylinder: public Object
{
private:

	Vector point; // srodek walca
	Vector axis; // os walca

public:
	Cylinder() : Object(), point(), axis() {}
	Cylinder(const Vector &p, const Vector &a, Material *m) : Object(m), point(p), axis(a) {}
	Cylinder(double x, double z, double a, double c, Material *m) : Object(m), point(x, 0, z), axis(1 / (a * a), 0, 1 / (c * c))  {}

	virtual int FindInter(const Ray &l, double &t);
	virtual Vector FindNormal(const Vector &a);
};


/*
Stozek nieskonczony, podobnie jak i walec, opisany jest przez wspolrzedne srodka
i wektor osi
*/

class Cone : public Object
{
private:

	Vector point; // srodek
	Vector axis; // os

public:

	Cone() : Object(), point(), axis() {}
	Cone(const Vector &p, const Vector &a, Material *m) : Object(m), point(p), axis(a) {}
	Cone(double x, double y, double z, double a, double c, Material *m): Object(m), point(x, y, z), axis(1 / (a * a), -1, 1 / (c * c)) {}

	virtual int FindInter(const Ray &l, double &t);
	virtual Vector FindNormal(const Vector &a);
};


/*
Plaszczyzna opisana jest przez wektor normalny oraz parametr "d",
stanowiacy uzupelnienie rownania plaszczyzny Ax + By + Cz +d = 0
*/

class Plane : public Object
{
private:
	Vector	normal; // normalna do plaszczyzny
	double	d; // parametr D rownania plaszczyzny

public:
	Plane() : Object(), normal(), d(0) {}
	Plane(const Vector &n, double r, Material *m) : Object(m), normal(n), d(r) {}
	Plane(double x, double y, double z, double r, Material *m) : Object(m), normal(x, y, z), d(r) {}

	virtual int FindInter(const Ray &l, double &t);
	virtual Vector FindNormal(const Vector &a);
};


#endif