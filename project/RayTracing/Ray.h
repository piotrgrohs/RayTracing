#ifndef __RAY_H
#define __RAY_H

class Vector;
class Scene;
class Objects;

/*
Promien swiatla - jest on puszczany przez kazdy piksel ekranu, by wyznaczyc
jego przeciecie z obiektami i znalezc kolor danego punktu ekranu
*/

class Ray
{
	friend class Scene;

private:
	Vector dir; // wektor kierunkowy
	Vector p; // polozenie kamery (punkt startowy)

public:
	Ray() {}
	Ray(const Vector& a, const Vector& b) : p(a), dir(Normalize(b)) {}
	Ray(const Ray& l) : dir(l.dir), p(l.p) {}

	Vector GetPoint(double t) {return p + t * dir;} // wylicza wspolrzedne punktu na podstawie parametru t
	Vector Reflect(const Vector &w) {return Normalize(dir - 2 * w * Dot(w, dir));} // zwraca wektor odbity
	Vector Direction() const {return dir;} // zwraca kierunek w postaci znormalizowanej
	Vector Point() const {return p;} // zwraca polozenie kamery (punkt startowy)

};

#endif