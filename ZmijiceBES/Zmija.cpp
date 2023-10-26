#include <iostream>
#include <fstream>
#include <conio.h>  
#include <windows.h> 
#include <chrono>    
#include <thread>  
#include <ctime>
#include <math.h>
#include "Zmija.h"
#include "Lubenica.h"
using namespace std;

Zmija::Zmija()
{
	glavaX = 0;
	glavaY = 0;
	repX = nullptr;
	repY = nullptr;
	smer = STOP;
	korak = 1;
	repDuzina = 0;
	vremeX = 0;
	vremeY = 0;
}

Zmija::Zmija(int dimenzije)
{
	srand(time(nullptr));
	glavaX = rand() % dimenzije;
	glavaY = rand() % dimenzije;
	repX = new int[dimenzije*dimenzije];
	repY = new int[dimenzije*dimenzije];
	smer = STOP;
	korak = 1;
	repDuzina = 0;
	vremeX = 800 / dimenzije;
	vremeY = 1300 / dimenzije;
}

int Zmija::getGlavaX()
{
	return this->glavaX;
}

int Zmija::getGlavaY()
{
	return this->glavaY;
}

int Zmija::getRepDuzina()
{
	return this->repDuzina;
}

int * Zmija::getRepX()
{
	return this->repX;
}

int * Zmija::getRepY()
{
	return this->repY;
}

kretanje Zmija::getSmer()
{
	return this->smer;
}

void Zmija::setVremeX(int x)
{
	vremeX += x;
}

void Zmija::setVremeY(int x)
{
	vremeY += x;
}

void Zmija::setSmer(kretanje x)
{
	smer = x;
}

void Zmija::setRepDuzina(int x)
{
	repDuzina = x;
}

void Zmija::generisi(int dimenzije)
{
	srand(time(nullptr));
	glavaX = rand() % dimenzije;
	glavaY = rand() % dimenzije;
}

bool Zmija::unos(bool iteracija)
{
	/*
	while (1) 
	{
		if (_kbhit())
		{
			switch (_getch())
			{
			case TASTER_GORE:
				if (smer != DOLE) smer = GORE;
				return true;
			case TASTER_DOLE:
				if (smer != GORE) smer = DOLE;
				return true;
			case TASTER_LEVO:
				if (smer != DESNO) smer = LEVO;
				return true;
			case TASTER_DESNO:
				if (smer != LEVO) smer = DESNO;
				return true;
			case 'x':
				cout << "Kraj igre!" << endl;
				return false;
			default:
				return true;
			}
		}
		
	}
	*/

	if (GetAsyncKeyState(VK_LEFT) && smer != DESNO) {
		smer = LEVO;
		return true;
	}
	if (GetAsyncKeyState(VK_RIGHT) && smer != LEVO) {
		smer = DESNO;
		return true;
	}
	if (GetAsyncKeyState(VK_UP) && smer != DOLE) {
		smer = GORE;
		return true;
	}
	if (GetAsyncKeyState(VK_DOWN) && smer != GORE) {
		smer = DOLE;
		return true;
	}
	if (GetAsyncKeyState(VK_RETURN)) {
		smer = STOP;
	}
	if (GetAsyncKeyState('X'))
	{
		return false;
	}


	return true;
	
}

bool Zmija::pokret(int dimenzije)
{

	for (int i = repDuzina; i > 0; i--)
	{
		repX[i] = repX[i - 1];
		repY[i] = repY[i - 1];
	}
	repX[0] = glavaX;
	repY[0] = glavaY;

	switch (smer)
	{
	case GORE:
		glavaY -= korak;
		break;
	case DOLE:
		glavaY += korak;
		break;
	case LEVO:
		glavaX -= korak;
		break;
	case DESNO:
		glavaX += korak;
		break;
	}

	
	if (smer == LEVO || smer == DESNO)
	{
		this_thread::sleep_for(chrono::milliseconds(vremeX));
	}

	if (smer == GORE || smer == DOLE)
	{
		this_thread::sleep_for(chrono::milliseconds(vremeY));
	}

	for (int i = 0; i < repDuzina; i++)
	{
		if (glavaX == repX[i] && glavaY == repY[i]) return false;
	}
	

	if (glavaX <= 0)
	{
		glavaX++;
		for (int i = 0; i < repDuzina; i++)
		{
			repX[i]++;
		}
		return false;
	}
	else if (glavaX >= dimenzije + 1)
	{
		glavaX--;
		for (int i = 0; i < repDuzina; i++)
		{
			repX[i]--;
		}
		return false;
	}
	else if (glavaY < 0)
	{
		glavaY++;
		for (int i = 0; i < repDuzina; i++)
		{
			repY[i]++;
		}
		return false;
	}
	else if (glavaY >= dimenzije)
	{
		glavaY--;
		for (int i = 0; i < repDuzina; i++)
		{
			repY[i]--;
		}
		return false;
	}
}


