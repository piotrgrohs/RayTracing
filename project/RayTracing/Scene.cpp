#include "scene.h"
#include "picture.h"

#include <stdio.h>
/*
Metoda dodajaca element do listy
*/

template <class T> int SList<T>::Add(T *member)
{
	Element	*temp;

	if (!head) // nie ma jeszcze zadnych elementow
	{
		head = new Element;
		
		if (!head)
			return -1; // blad alokacji

		head->member = member;
		head->next = 0;
	}
	else
	{
		// przewijamy liste i nowy element na koncu wstawiamy
		for (temp = head; temp->next; temp = temp->next);

		temp->next = new Element;
		
		if (!temp->next)
			return -1; // blad alokacji

		temp->next->next = 0;
		temp->next->member = member;
	}
	return 0; // ok
}

/*
Metoda zwracajaca element o zadanym numerze
*/

template <class T> T *SList<T>::Return(int number)
{
	Element	*temp = head;

	for (int i = 0; i < number; i++)
	{
		temp = temp->next;
		if (!temp) return 0; // blad wykroczenia poza liste!
	}

	return temp->member; // znaleziono...
}

/*
Destruktor usuwa liste, nie usuwajac jednak elementow znajdujacych
sie na liscie
*/

template <class T> SList<T>::~SList()
{
	Element	*temp = head;

	while(temp)
	{
		head = temp->next;
		delete temp;
		temp = head;
	}
}



/*
Konstruktor sceny ustawia szereg parametrow na watosci domyslne,
by nie trzeba bylo ich wszystkich ustawiac potem recznie
*/

Scene::Scene()
{
	strcpy_s(name, 1, "");
	ambient = Color();
	campos = Vector();
	camdir = Vector(0,0,1);

	xres = yres = 200;
	roll = 0;
	aspect = 1;
	fogen = 0;
	maxdeep = 3;
	SetFOV(90);
	SetFog(0,2000,Color(0.4, 0.4, 0.4));

}


/*
Metoda liczaca natezenie mgly w danej odleglosci od obserwatora
*/

double Scene::CalcFog(double depth)
{
	double a; // tymczasowa zmienna

	a = (depth - dmin) / (dmax - dmin);
	if (a > 1) a = 1;
	if (a < 0) a = 0;
	return a;
}

/*
Metoda ustawiajaca nazwe pliku, do ktorego nastapi zapis
wygenerowanej sceny
*/

void Scene::SetFile(const char *fname)
{
	strncpy_s(name, strlen(fname)+1, fname, sizeof(name));
}

/*
Metoda ustawiajaca pozycje kamery i kierunek patrzenia
*/

void Scene::SetCam(const Vector &pos, const Vector &dir)
{
	campos = pos;
	camdir = dir;
}

/*
Metoda ustawiajaca zasieg mgly i jej kolor
*/

void Scene::SetFog(double min, double max, Color col)
{
	dmin = min;
	dmax = max;
	fog = col;
}

/*
Metoda ustawiajaca pole widzenia
*/

void Scene::SetFOV(unsigned int f)
{
	double a = FOV * PI / 180; // przeliczenie na radiany
	FOV = f;
	focus = yres / (2 * tan(a / 2));
}

/*
Metoda dodajaca obiekt do listy
Zwraca -1 w razie bledu
*/

int Scene::AddObj(Object *obj)
{
	return objects.Add(obj);
}

/*
Metoda dodajaca swiatlo do listy
Zwraca -1 w razie bledu
*/

int Scene::AddLight(Light *lt)
{
	return lights.Add(lt);
}

/*
Metoda znajdujaca przeciecie z najblizszym mozliwym obiektem
*/

Object *Scene::FindNearest(const Ray &l, double &t)
{
	Object *nearest = 0, *temp;
	int i = 0;

	t = maxray; // ograniczamy najwieksza glebokosc...

	while ((temp = objects.Return(i++)) != 0)
	{
		if (temp->FindInter(l, t)) // szukanie przeciecia z kazdym objektem...
			nearest = temp; // FindInter zwroci wartosc niezerowa tylko jesli znajdzie blizsze przeciecie
	}

	return nearest;
}

/*
Metoda liczaca kolor w danym punkcie (pobieranym z "info").
Zmienna deep oznacza na ktorym poziomie rekurencji jestesmy,
gdyz metoda ta wywoluje siebie rekurencyjnie w przypadku odbicia
i zalamania swiatla.
*/

Color Scene::CalcColor(const ShInfo *info, int deep)
{
	Color	finalcol; // kolor koncowy
	Color	pointcolor; // kolor tymczasowy
	Light	*lt; // tymczasowe swiatlo
	Object	*obj; // tymczasowy obiekt
	ShInfo	tempinfo;
	ShInfo	*ninfo = &tempinfo; // pomocnicza tymczasowa struktura ShInfo
	int		i = 0; // licznik petli
	double	tmax, t; // parametry promienia
	double	depth = Length(info->point - campos); // odleglosc od kamery
	Ray		newray; // tymczasowy promien swiatla


	pointcolor = info->object->mat->texture->GetColor(*info); // pobranie koloru
	finalcol = ambient * pointcolor; // nalozenie swiatla tla

	while ((lt = lights.Return(i++)) != 0) // dla kazdego swiatla...
	{
		Vector dir = lt->Direction(info->point); // kierunek swiatla
		double  shadow; // cien
		double  spc, temp; // tymczasowe zmienne pomocnicze

		newray = Ray(info->point, dir); // puszczamy promien z powierzchni do swiatla
		tmax = Length(lt->ReturnPos() - info->point); // dla cienia szukamy obiektow miedzy aktualnym punktem a swiatlem

		if (Length(dir) > 0)
		{
			shadow = 1; // ustawiamy cien na maksymalna wartosc
			t = tmax;

			for (int j = 0; (obj = objects.Return(j)) != 0; j++)
				if (obj->FindInter(newray, t))
				{
					shadow *= obj->mat->refraction; // przemnoz cien przez zalamanie
				}

			if (shadow > 0) // jesli jest jakikolwiek cien, to zmien kolor obiektu w tym punkcie
			{
				if ((temp = Dot(info->normal, dir)) >0)
					finalcol += shadow * temp * lt->ReturnColor() * pointcolor;
				if ((spc = info->object->mat->specular) != 0)
					if (( temp = Dot(info->reflect, dir)) > 0)
						finalcol += shadow * pow(temp, info->object->mat->specfactor) * spc * lt->ReturnColor();
			}
		}
	}

	if (deep > maxdeep)
		return finalcol; // nie zglebiamy sie dalej...

	if (info->object->mat->reflection && Length(info->reflect) > 0) // czy jest odbicie?
	{
		newray = Ray(info->point, info->reflect);
		tmax = maxray;

		if ((obj = FindNearest(newray, tmax)) != 0) // liczymy odbity promien...
		{
			ninfo->object = obj;
			ninfo->point = newray.GetPoint(tmax);
			ninfo->normal = obj->FindNormal(ninfo->point);
			ninfo->reflect = newray.Reflect(ninfo->normal);
			ninfo->ray = &newray;

			finalcol += info->object->mat->reflection * CalcColor(ninfo, deep + 1); // rekurencja
		}
	}

	if (info->object->mat->refraction) // zalamanie swiatla
	{
		Vector raydir = info->ray->Direction(); // kierunek promienia
		Vector newdir; // nowy kierunem
		Vector N = info->normal; // normalna

		double dot = -Dot(N,raydir); // kilka zmiennych do wyznaczenia zalamania...
		double gamma = info->object->mat->reffactor;
		double sqrtrm;
		

		if (dot > 0)
			gamma = 1 / gamma;
		else
		{
			dot = -dot;
			N = -N;
		}

		sqrtrm = 1.0 - gamma * gamma * (1 - dot * dot);

		if (sqrtrm > 0) // czy bedzie promien zalamany?
		{
			sqrtrm = gamma * dot - sqrt(sqrtrm);
			newdir = (raydir * gamma) + (N * sqrtrm);
			newray = Ray(info->point, newdir);

			tmax = maxray;
			if ((obj = FindNearest(newray, tmax)) != 0) // szukamy przeciecia promienia zalamanego...
			{
				ninfo->object = obj;
				ninfo->point = newray.GetPoint(tmax);
				ninfo->normal = obj->FindNormal(ninfo->point);
				if (info->object == obj)
					ninfo->reflect = Vector();
				else
					ninfo->reflect = newray.Reflect(ninfo->normal);
				ninfo->ray = &newray;

				finalcol += info->object->mat->refraction * CalcColor(ninfo, deep + 1);
			}
		}
	}

	if (fogen) // dodanie efektu mgly w razie potrzeby...
	{
		double a = CalcFog(depth);
		finalcol = a * fog + (1 - a) * finalcol;
	}

	return finalcol; // zwracamy ostateczny kolor...
}

int Scene::Trace()
{

	// szereg zmiennych uzytych w obliczeniach, nie bede ich opisywal
	// szczegolowo, gdyz mija sie to z celem...
	double t;
	double alpha, beta, gamma;
	double length = sqrt(pow(camdir[0],2) + pow(camdir[2], 2));
	double mx1, mx2, mx3, my1, my2, my3, mz1, mz2, mz3;

	Object	*obj; // tu bedzie trzymany obiekt
	ShInfo	tempinfo;
	ShInfo	*info = &tempinfo; // tu struktura okreslajaca aktualny pixel w 3d
	Ray		ray; // promien ktory puscimy z kamery
	Vector	dir, rotdir; // kierunki, w tym rotacja
	Color	col; // kolor punktu
	int x, y, hx = xres/2, hy = yres/2; // rozmiary obrazka
	
	Pixel24		pxl; // pixel do zapisu do pliku

	BMPfile mypic(name, 1); // tworzymy plik do zapisu...
	mypic.WriteHeader(xres, yres); // zapis naglowka

	if (mypic.IsError())
		return -1; // problem z zapisem do pliku!

	// teraz szereg obliczen dla znalezienia odwzorowania plaszczyzny ekranu 
	// z 3d na 2d na podstawie parametrow ustawienia kamery...

	if (camdir[2]!=0)
		alpha = atan2(camdir[0], camdir[2]);
	else
		alpha = 0;

	beta  = -asin(camdir[1]/length);
	gamma = roll * PI / 180;

	mx1 = sin(alpha) * sin(beta) * sin (gamma) + cos(alpha) * cos(gamma);
	my1 = cos(beta) * sin(gamma);
	mz1 = sin(alpha) * cos(gamma) - cos(alpha) * sin(beta) * sin(gamma);

	mx2 = sin(alpha) * sin(beta) * cos (gamma) - cos(alpha) * sin(gamma);
	my2 = cos(beta) * cos(gamma);
	mz2 = -cos(alpha) * sin(beta) * cos(gamma) - sin(alpha) * sin(gamma);

	mx3 = -sin(alpha) * cos(beta);
	my3 = sin(beta);
	mz3 = cos(alpha) * cos(beta);

	// glowna petla ponizej liczy kazdy pixel ekranu :

	for (y = yres - 1; y >= 0; y--) // z dolu do gory, gdyz taki format ma BMP...
	{
		for (x = 0; x < xres; x++) // i z lewej do prawej...
		{
			// liczymy wspolrzedne promienia ktory zostanie wypuszczony...

			dir = Vector(aspect * ((double)x - hx), (double)hy - y, focus);

			rotdir[0] = mx1 * dir[0] + my1 * dir[1] + mz1 * dir[2];
			rotdir[1] = mx2 * dir[0] + my2 * dir[1] + mz2 * dir[2];
			rotdir[2] = mx3 * dir[0] + my3 * dir[1] + mz3 * dir[2];

			ray = Ray(Vector(0, 0, -focus) + campos, rotdir);

			// szukamy przeciec z obiektami:

			if ((obj = FindNearest(ray, t)) != 0)
			{
				info->object = obj;
				info->point = ray.GetPoint(t);
				info->normal = obj->FindNormal(info->point);
				info->reflect = ray.Reflect(info->normal);
				info->ray = &ray;

				col = NormalizeColor(CalcColor(info,0)); // jesli jest przeciecie, to znajdz kolor
				
			}
			else
				if (fogen)
					col = fog; // jesli nie ma przeciec, ale jest mgla, to daj kolor mgly
				else
					col = Color(); // a w przeciwnym razie czarny (0, 0, 0)

			// przeksztalcenie wektora koloru do pixela 24-bit

			pxl.r = (uint8_t)(255.0 * col[0]);
			pxl.g = (uint8_t)(255.0 * col[1]);
			pxl.b = (uint8_t)(255.0 * col[2]);

			mypic.PixelWt(pxl); // zapis do pliku
			

		}
		printf("%d \r",y); // uzyto printf ze wzgledow czysto kosmetycznych (latwiejsze uzycie "\r")
	}
	
	if (mypic.IsError())
		return -1; // byly problemy z zapisem do pliku...

	return 0;
}

