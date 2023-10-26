#pragma once
#include "Zmija.h"
#include "Vocka.h"
#include "Lubenica.h"
#include "Poen.h"

class Igra
{
private:
	int dimenzije;
	bool* pojedene;
	int duzinaPojedenih;
	bool iteracijaIgre;
	bool exitToken;
	Zmija zmijica;
	Vocka vockica;
	Lubenica lubenicica;
	Poen poencic;
public:
	void setZmija(Zmija);
	void setVocka(Vocka);
	void setLubenica(Lubenica);
	void setPoen(Poen);
	Poen getPoen();
	Zmija getZmija();
	Vocka getVocka();
	Lubenica getLubenica();
	int getDimenzije();
	bool* getPojedene();
	bool getIteracijaIgre();
	void pokreni();
	void uvod();
	void podesavanja();
	bool proveri();
	void fajl(int);
	void prozor();
	bool core();

};

