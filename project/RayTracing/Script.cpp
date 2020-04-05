#include "script.h"

#include <string.h>
#include <iostream>
#include <fstream>
#include <strstream>
#include <iomanip>
using namespace std;

/*
Metoda dodajaca element do listy - zwraca 0 lub -1 (-> blad)
W przypadku gdy juz istnieje element o podanej nazwie, nie jest
on dodawany powtornie.
*/

template <class T> int List<T>::Add(T *memb, const char *name)
{
	Element *temp;

	if (!head)
	{
		head = new Element;
		
		if (!head)
			return -1; // blad alokacji

		head->member = memb;
		head->next = 0;
		strcpy_s(head->name, strlen(name)+1, name);
	}
	else
	{
		//przewijamy liste
		
		for (temp = head; temp->next; temp = temp->next);

		temp->next = new Element;

		if (!temp->next)
			return -1; // blad alokacji
		temp->next->next = 0;
		temp->next->member = memb;
		strcpy_s(temp->next->name, strlen(name)+1, name);
	}
	return 0;
}

/*
Metoda zwracajaca element z listy, wyszukiwany na podstawie
nazwy. W razie braku zwraca 0
*/

template <class T> T *List<T>::GiveMemb(const char *name)
{
	Element *temp = head;

	while (temp)
	{
		if (strcmp(name, temp->name) == 0)
			return temp->member;
		temp = temp->next;
	}

	return 0;
}

/*
Metoda wyszukujaca na podstawie kolejnego numeru
*/

template <class T> T *List<T>::GiveMemb(int n)
{
	Element *temp = head;

	for (int i = 0; i < n; i++)
	{
		temp = temp->next;
		if (!temp) return 0;
	}

	if (!head)
		return 0;

	return temp->member;
}

/*
Destruktor listy usuwa nie tylko sama liste, ale takze
wszystkie znajdujace sie na niej elementy. Ulatwia to bardzo
obsluge skryptu (po rezerwacji pamieci na element i wrzuceniu
go na liste nie trzeba pozniej pamietac o jego zwolnieniu,
gdyz zrobi to za nas lista), ale nalezy byc ostroznym!
*/

template <class T> List<T>::~List()
{
	Element *temp = head;

	while(temp)
	{
		head = temp->next;
		delete temp->member; // uwaga! usuwamy element listy!!!!
		delete temp;
		temp = head;
	}
}


//////////////////////////////////////////////////////////////////////////		

/*
Konstruktor skryptu probuje otworzyc plik o podanej nazwie.
Ustawia flage isopened na odpowiednia wartosc
*/

Script::Script(const char *name)
{


	file.open(name, ios::in); // do odczytu

	if(!file)
		isopened = 0;
	else
		isopened = 1; // ok, udalo sie otworzyc
}

/*
Destruktor w razie potrzeby zamyka plik
*/

Script::~Script()
{
	if (isopened)
		file.close();

}

/*
Metoda analizujaca komende TRACE
- czyli wykonanie renderingu sceny.
*/
 

int Script::Trace()
{


	int i = 0; // licznik
	
	while ((tempo = objects.GiveMemb(i++)) != 0)
	{
		if (thescene.AddObj(tempo) == -1) // wrzucamy obiekty na scene
		{
			cout << "Memory allocation failed" << ends;
			return -3;
		}
	}
	
	cout << "Number of objects in scene = " << i-1 << endl;

	if (i == 1)
	{
		cout << "No objects - raytracing not done" << ends;
		return -2;
	}
	i = 0;
	

	while ((templ = lights.GiveMemb(i++)) != 0)
	{
		if (thescene.AddLight(templ) == -1) // wrzucamy swiatla
		{
			cout << "Memory allocation failed" << ends;
			return -3;
		}
	}
	
	cout << "Number of lights in scene = " << i-1 << endl;
	if (i == 1)
	{
		cout << "No lights - raytracing not done" << ends;
		return -2;
	}
	
	if (thescene.Trace() == 0) // i renderujemy!
		return 0;
	else
	{
		cout << "Error during raytracing (probably file error)" << ends;
		return -1;
	}
}


/*
Metoda analizujaca komende NAME
- ustawia nazwe pliku do renderowania
*/

int Script::Name()
{
	file >> setw(sizeof(temp)) >> temp;
	
	if (!file.good())
	{
		cout << "Scene name error" << ends;
		return -2;
	}
	
	thescene.SetFile(temp);

	return 0;
}

/*
Metoda analizujaca komende MAP
- ustawienie nowego rodzaju mapowania
*/

int Script::Map()
{
	file >> setw(sizeof(name)) >> name; // rodzaj
	file >> setw(sizeof(name2)) >> name2; // nazwa
	
	if (!file.good())
	{
		cout << "Mapping parameters error" << ends;
		return -2;
	}
	
	if (strcmp(name, "SPH") == 0) // sferyczne
	{
		Spherical *sph = new Spherical;
		if (!sph || maps.Add(sph, name2) == -1)
		{
			cout << "Memory allocation failed" << ends;
			return -3;
		}
		
	}
	else if (strcmp(name, "CYL") == 0) // cylindryczne
	{
		double h; // zmienna tymczasowa
		
		file >> h;
		
		if (!file.good() || h <= 0)
		{
			cout << "Illegal cylindrical map (" << name2 << ") height" << ends;
			return -2;
		}
		
		Cylindrical *cyl = new Cylindrical(h);
		if (!cyl || maps.Add(cyl, name2) == -1)
		{
			cout << "Memory allocation failed" << ends;
			return -3;
		}
		
		
	}
	else if (strcmp(name, "PLA") == 0) // plaszczyznowe
	{
		double x, y; // tymczasowe zmienne
		
		file >> x >> y;
		
		if (!file.good() || x <= 0 || y <= 0)
		{
			cout << "Illegal planar mapping (" << name2 << ") size" << ends;
			return -2;
		}
		
		Planar *pla = new Planar(x, y);
		if (!pla || maps.Add(pla, name2) == -1)
		{
			cout << "Memory allocation failed" << ends;
			return -3;
		}
		
		
	}
	else
	{
		cout << "Unknown mapping type: " << name << ends;
		return -2;
	}

	return 0;
}

/*
Metoda analizujaca komende TEXT
- wprowadza nowa teksture
*/

int Script::Text()
{
	file >> setw(sizeof(name)) >> name; // rodzaj
	file >> setw(sizeof(name2)) >> name2; // nazwa
	
	if (!file.good())
	{
		cout << "Texture parameters error" << ends;
		return -2;
	}
	
	if (strcmp(name, "ONEC") == 0) // jednokolorowa
	{
		double r, g, b; // parametry tymczasowe
		
		file >> r >> g >> b;
		
		if (!file.good() || r < 0 || r > 1 || g < 0 || g > 1 || b < 0 || b > 1)
		{
			cout << "Illegal color (" << name2 << ") parameters" << ends;
			return -2;
		}
		OneColor *onec = new OneColor(Color(r, g, b));
		if (!onec || txts.Add(onec, name2) == -1)
		{
			cout << "Memory allocation failed" << ends;
			return -3;
		}
		
	}
	else if (strcmp(name, "IMG") == 0) // obrazek
	{
		file >> setw(sizeof(temp)) >> temp; // nazwa pliku z tekstura
		file >> setw(sizeof(name)) >> name; // nazwa mappowania
		
		if (!file.good())
		{
			cout << "Image texture parameters error" << ends;
			return -2;
		}
		
		Mapping *map = maps.GiveMemb(name); // czy istnieje dane mapowanie?
		if (map == 0)
		{
			cout << "Mapping " << name << " not defined." << ends;
			return -2;
		}
		
		Image *img = new Image(temp, map);
		if (!img)
		{
			cout << "Memory allocation failed" << ends;
			return -3;
		}
		
		if (img->IsLoaded() == 0)
		{
			cout << "Texture (" << temp << ") loading cout" << ends;
			return -2;
		}
		
		if (txts.Add(img, name2) == -1)
		{
			cout << "Memory allocation failed" << ends;
			return -3;
		}
		
	}
	else
	{
		cout << "Unknown texture type: " << name << ends;
		return -2;
	}

	return 0;
}

/*
Metoda analizujaca komende MAT
- wprowadzenie nowego rodzaju materialu
*/

int Script::Mat()
{
	double spc, spf, rfl, rfr, rff; // zmienne tymczasowe
	
	file >> setw(sizeof(name)) >> name; // nazwa materialu
	file >> setw(sizeof(name2)) >> name2; // nazwa tekstury
	
	if (!file.good())
	{
		cout << "Material parameters error" << ends;
		return -2;
	}
	
	Texture *txt = txts.GiveMemb(name2);
	if (txt == 0)
	{
		cout << "Texture " << name2 << " not defined." << ends;
		return -2;
	}
	
	file >> spc >> spf >> rfl >> rfr >> rff;
	
	if (!file.good() || spc < 0 || spf < 0 || rfl < 0 || rfr < 0 || rff < 0 || rfl > 1 || rfr > 1)
	{
		cout << "Illegal material (" << name << ") parameters" << ends;
		return -2;
	}
	
	Material *mat = new Material;
	if (!mat)
	{
		cout << "Memory allocation failed" << ends;
		return -3;
	}
	
	mat->texture = txt;
	mat->reffactor = rff;
	mat->reflection = rfl;
	mat->refraction = rfr;
	mat->specfactor = spf;
	mat->specular = spc;
	
	if (mats.Add(mat, name) == -1)
	{
		cout << "Memory allocation failed" << ends;
		return -3;
	}
	
	return 0;
}

/*
Metoda analizujaca komende LIGHT
- dodaje nowe swiatlo
*/

int Script::Light()
{
	file >> setw(sizeof(name)) >> name; // rodzaj
	file >> setw(sizeof(name2)) >> name2; // nazwa
	
	if (!file.good())
	{
		cout << "Light parameters error" << ends;
		return -2;
	}
	
	if (strcmp(name, "PT") == 0) // punktowe
	{
		double r, g, b, x, y, z; // tymczasowe zmienne
		
		file >> r >> g >> b >> x >> y >> z;
		
		if (!file.good() || r<0 || r>1 || g<0 || g>1 || b<0 || b>1)
		{
			cout << "Illegal light (" << name2 << ") parameters" << ends;
			return -2;
		}
		
		PointLight *pt = new PointLight(Color(r, g, b), Vector(x, y, z));
		if (!pt || lights.Add(pt, name2) == -1)
		{
			cout << "Memory allocation failed" << ends;
			return -3;
		}
		
		
	}
	else if (strcmp(name, "SPOT") == 0) // stozkowe
	{
		double r, g, b, x1, y1, z1, x2, y2, z2, a; // tymczasowe zmienne
		
		file >> r >> g >> b >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> a;
		
		if (!file.good() || r<0 || r>1 || g<0 || g>1 || b<0 || b>1 || a==0 || (x1==x2 && y1==y2 && z1==z2))
		{
			cout << "Illegal light (" << name2 << ") parameters" << ends;
			return -2;
		}
		
		SpotLight *spot = new SpotLight(Color(r, g, b), Vector(x1, y1, z1), Vector (x2, y2, z2), a);
		if (!spot || lights.Add(spot, name2) == -1)
		{
			cout << "Memory allocation failed" << ends;
			return -3;
		}
		
	}
	else
	{
		cout << "Unknown light type: " << name << ends;
		return -2;
	}
	
	return 0;	
}

/*
Metoda analizujaca komende OBJ
- dodaje nowy obiekt
*/

int Script::Obj()
{
	char namet[16]; // dodatkowy bufor
	
	file >> setw(sizeof(name)) >> name; // rodzaj
	file >> setw(sizeof(name2)) >> name2; // nazwa
	
	if (!file.good())
	{
		cout << "Object parameters error" << ends;
		return -2;
	}
	
	if (strcmp(name, "SPH") == 0) // kula
	{
		double x, y, z, r; // tymczasowe parametry
		
		file >> x >> y >> z >> r;
		file >> setw(sizeof(namet)) >> namet; // nazwa materialu
		
		if (!file.good() || r < 0)
		{
			cout << "Illegal sphere (" << name2 << ") parameters" << ends;
			return -2;
		}
		
		Material *mat = mats.GiveMemb(namet); // czy istnieje taki material?
		if (mat == 0)
		{
			cout << "Material " << namet << " not defined." << ends;
			return -2;
		}
		
		Sphere *sph = new Sphere(x, y, z, r, mat);
		if (!sph || objects.Add(sph, name2) == -1)
		{
			cout << "Memory allocation failed" << ends;
			return -3;
		}
	}

	else if (strcmp(name, "CYL") == 0) // walec
	{
		double x, z, a, c; // tymczasowe parametry
		
		file >> x >> z >> a >> c;
		file >> setw(sizeof(namet)) >> namet; // nazwa materialu
		
		if (!file.good())
		{
			cout << "Illegal cylinder (" << name2 << ") parameters" << ends;
			return -2;
		}
		
		Material *mat = mats.GiveMemb(namet); // czy material istnieje?
		if (mat == 0)
		{
			cout << "Material " << namet << " not defined." << ends;
			return -2;
		}
		
		Cylinder *cyl = new Cylinder(x, z, a, c, mat);
		if (!cyl || objects.Add(cyl, name2) == -1)
		{
			cout << "Memory allocation failed" << ends;
			return -3;
		}
	}

	else if (strcmp(name, "CON") == 0) // stozek
	{
		double x, y, z, a, c; // tymczasowe parametry
		
		file >> x >> y >> z >> a >> c;
		file >> setw(sizeof(namet)) >> namet; // nazwa materialu
		
		if (!file.good())
		{
			cout << "Illegal cone (" << name2 << ") parameters" << ends;
			return -2;
		}
		
		Material *mat = mats.GiveMemb(namet); // czy material istnieje?
		if (mat == 0)
		{
			cout << "Material " << namet << " not defined." << ends;
			return -2;
		}
		
		Cone *con = new Cone(x, y, z, a, c, mat);
		if (!con || objects.Add(con, name2) == -1)
		{
			cout << "Memory allocation failed" << ends;
			return -3;
		}
	}

	else if (strcmp(name, "PLA") == 0) // plaszczyzna
	{
		double x, y, z, d; // tymczasowe parametry
		
		file >> x >> y >> z >> d;
		file >> setw(sizeof(namet)) >> namet; // nazwa materialu
		
		if (!file.good())
		{
			cout << "Illegal plane (" << name2 << ") parameters" << ends;
			return -2;
		}
		
		Material *mat = mats.GiveMemb(namet); // czy material istnieje?
		if (mat == 0)
		{
			cout << "Material " << namet << " not defined." << ends;
			return -2;
		}
		
		Plane *pla = new Plane(x, y, z, d, mat);
		if (!pla || objects.Add(pla, name2) == -1)
		{
			cout << "Memory allocation failed" << ends;
			return -3;
		}
		
	}
	else
	{
		cout << "Unknown object type: " << name << ends;
		return -2;
	}

	return 0;
}

/*
Metoda analizujaca komende SCE
- ustawia parametry sceny
*/

int Script::Sce()
{
	file >> setw(sizeof(name)) >> name; // rodzaj
	
	if (!file.good())
	{
		cout << "Scene parameters cout" << ends;
		return -2;
	}
	
	if (strcmp(name, "FOV") == 0) // pole widzenia
	{
		unsigned f; // tymczasowy parametr
		
		file >> f;
		
		if (!file.good() || f <= 0)
		{
			cout << "Invalid FOV value" << ends;
			return -2;
		}
		
		thescene.SetFOV(f);
	}

	else if (strcmp(name, "CAM") == 0) // kamera
	{
		double x1, y1, z1, x2, y2, z2, roll; // tymczasowe zmienne
		
		file >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> roll;
		
		if (!file.good() || x2 == 0 && y2 == 0 && z2 == 0)
		{
			cout << "Invalid camera direction (0, 0, 0)" << ends;
			return -2;
		}
		
		thescene.SetCam(Vector(x1, y1, z1), Vector(x2, y2, z2));
		thescene.SetRoll(roll);
	}

	else if (strcmp(name, "RES") == 0) // rozdzielczosc i aspect
	{
		int x, y; // tymczasowe zmienne
		double a;
		
		file >> x >> y >> a;
		
		if (!file.good() || x <= 0 || y <= 0 || a <= 0) 
		{
			cout << "Illegal scene resolution" << ends;
			return -2;
		}
		
		thescene.SetH(x);
		thescene.SetV(y);
		thescene.SetAspect(a);
	}

	else if (strcmp(name, "FOG") == 0) // mgla
	{
		double min, max, r, g, b; // tymczasowe zmienne
		
		file >> min >> max >> r >> g >> b;
		
		if (!file.good() || min>max || r<0 || r>1 || g<0 || g>1 || b<0 || b>1)
		{
			cout << "Illegal fog parameters." << ends;
			return -2;
		}
		thescene.SetFog(min, max, Color(r, g, b));
		thescene.FogEn();
	}

	else if (strcmp(name, "AMB") == 0) // kolor swiatla rozproszonego
	{
		double r, g, b;
		
		file >> r >> g >> b;
		
		if (!file.good() || r<0 || r>1 || g<0 || g>1 || b<0 || b>1)
		{
			cout << "Illegal ambient color parameters." << ends;
			return -2;
		}
		
		thescene.SetAmbient(Color(r, g, b));
	}

	else
	{
		cout << "Unknown scene command: " << name << ends;
		return -2;
	}

	return 0;
}

/*
Metoda analizujaca caly skrypt
- odczytuje komende a anstepnie przekazuje sterowanie do odpowiedniej funkcji.
*/

int Script::DoScript()
{
	if (!isopened)
	{
		cout << "Error opening script file!" << ends;
		return -1;
	}

	do
	{
		int rval; // zwrocona przez funkcje wartosc

		file >> setw(sizeof(name)) >> name; // wczytaj komende;
		

		if (strcmp(name,"TRACE") == 0)
		{
			return Trace();
		}
		
		else if (strcmp(name, "NAME") == 0)
		{
			if ((rval = Name()) != 0)
				return rval;
		}
		
		else if(strcmp(name, "MAP") == 0)
		{
			if ((rval = Map()) != 0)
				return rval;
		}
		
		else if (strcmp(name, "TEXT") == 0)
		{
			if ((rval = Text()) != 0)
				return rval;
		}

		else if (strcmp(name, "MAT") == 0)
		{
			if ((rval = Mat()) != 0)
				return rval;
		}

		else if (strcmp(name, "LIGHT") == 0) // swiatlo
		{
			if ((rval = Light()) != 0)
				return rval;
		}

		else if (strcmp(name, "OBJ") == 0) // obiekt
		{
			if ((rval = Obj()) != 0)
				return rval;
		}

		else if (strcmp(name, "SCE") == 0) // parametry sceny
		{
			if ((rval = Sce()) != 0)
				return rval;
		}

		else if (strcmp(name, "END") == 0) // koniec pliku (bez liczenia sceny)
		{
			return 0;
		}

		else if (name[0] == '/' && name[1] == '*') // komentarz
		{
			do
			{
				file >> setw(sizeof(temp)) >> temp;
			}
			while (temp[strlen(temp) - 2] != '*' && temp[strlen(temp) - 1] != '/' && !file.eof());
		}

		else
		{
			cout << "Unknown command: " << name << ends;
			return -2;
		}

	}
	while (!file.eof());

	cout << "Unexpected end of file." << ends;
	return -1;
}

