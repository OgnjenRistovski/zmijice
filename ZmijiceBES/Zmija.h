#pragma once
#include "Vocka.h"
#include "Lubenica.h"
#define TASTER_GORE 72
#define TASTER_DOLE 80
#define TASTER_LEVO 75
#define TASTER_DESNO 77
enum kretanje { STOP, GORE, DOLE, LEVO, DESNO };

class Zmija
{
private:
	int glavaX, glavaY, korak;
	int *repX, *repY, repDuzina;
	int vremeX, vremeY;
	kretanje smer;
public:
	Zmija();
	Zmija(int);
	int getGlavaX();
	int getGlavaY();
	int getRepDuzina();
	int* getRepX();
	int* getRepY();
	kretanje getSmer();
	void setVremeX(int);
	void setVremeY(int);
	void setSmer(kretanje);
	void setRepDuzina(int);
	void generisi(int);
	bool unos(bool);
	bool pokret(int);
};

