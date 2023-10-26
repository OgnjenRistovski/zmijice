#include <iostream>
#include "Poen.h"
using namespace std;

Poen::Poen()
{
	floatPoeni = 0;
	floatHighscore = 0;
	intPoeni = 0;
	intHighscore = 0;
	opcija = 1;
}

int Poen::getIntPoeni()
{
	return intPoeni;
}

int Poen::getIntHighscore()
{
	return intHighscore;
}

int Poen::getOpcija()
{
	return opcija;
}

float Poen::getFloatPoeni()
{
	return floatPoeni;
}

float Poen::getFloatHighscore()
{
	return floatHighscore;
}

void Poen::setIntPoeni(int p)
{
	intPoeni = p;
}

void Poen::setIntHighscore(int p)
{
	intHighscore = p;
}

void Poen::setOpcija(int p)
{
	opcija = p;
}

void Poen::setFloatPoeni(float p)
{
	floatPoeni = p;
}

void Poen::setFloatHighscore(float p)
{
	floatHighscore = p;
}

bool Poen::proveraUkupno(int dimenzije)
{
	switch (opcija)
	{
	case 1:
		if (intPoeni == (dimenzije*dimenzije))
		{
			cout << "Cestitke! Uspesno ste presli igru!";
			return false;
		}
		break;
	case 2:
		if (floatPoeni == ((dimenzije*dimenzije)/10.0))
		{
			cout << "Cestitke! Uspesno ste presli igru!";
			return false;
		}
		break;
	}
	
}
