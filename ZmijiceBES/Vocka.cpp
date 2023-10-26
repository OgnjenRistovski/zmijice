#include <iostream>
#include <fstream>
#include <conio.h>  
#include <windows.h> 
#include <chrono>    
#include <thread>  
#include <ctime>
#include "Vocka.h"

Vocka::Vocka()
{
	vockaX = 0;
	vockaY = 0;
}

int Vocka::getVockaX()
{
	return this->vockaX;
}

int Vocka::getVockaY()
{
	return this->vockaY;
}

void Vocka::generisi(int dimenzije)
{
	srand(time(nullptr));
	vockaX = rand() % dimenzije;
	vockaY = rand() % dimenzije;
}
