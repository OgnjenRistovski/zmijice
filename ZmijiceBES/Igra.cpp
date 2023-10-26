#pragma comment(lib, "winmm.lib")
#include <iostream>
#include <fstream>
#include <conio.h>  
#include <windows.h> 
#include <chrono>    
#include <thread>  
#include <ctime>
#include <fstream>
#include <string>
#include <mmsystem.h>
#include "Igra.h"
#include "Vocka.h"
#include "Zmija.h"
using namespace std;

template <typename T>
T maxScore(T x, T y) 
{
	return (x > y) ? x : y;
}
void Igra::setZmija(Zmija x)
{
	this->zmijica = x;
}
void Igra::setVocka(Vocka x)
{
	this->vockica = x;
}
void Igra::setLubenica(Lubenica x)
{
	this->lubenicica = x;
}
void Igra::setPoen(Poen p)
{
	this->poencic = p;
}
Poen Igra::getPoen()
{
	return poencic;
}
Zmija Igra::getZmija()
{
	return zmijica;
}
Vocka Igra::getVocka()
{
	return vockica;
}
Lubenica Igra::getLubenica()
{
	return lubenicica;
}
int Igra::getDimenzije()
{
	return dimenzije;
}
bool* Igra::getPojedene()
{
	return this->pojedene;
}
bool Igra::getIteracijaIgre()
{
	return iteracijaIgre;
}


void Igra::pokreni()
{
	while (1)
	{
		uvod();
		podesavanja();
		fajl(poencic.getOpcija());
		PlaySound(TEXT("gamestart.wav"), NULL, SND_FILENAME | SND_ASYNC);
		while (iteracijaIgre && exitToken)
		{
			prozor();
			exitToken = zmijica.unos(iteracijaIgre);
			iteracijaIgre = core();
		}
		iteracijaIgre = false;
		PlaySound(TEXT("smrt.wav"), NULL, SND_FILENAME | SND_ASYNC);
		prozor();
		fajl(poencic.getOpcija());
		system("PAUSE");
		system("cls");
	}
		
}
void Igra::uvod()
{
	PlaySound(TEXT("mainmenu.wav"), NULL, SND_FILENAME | SND_ASYNC);
	fstream f;
	int n = 0, pom, op = 2;
	bool flag = true;
	bool cifra;
	string rec;
	while(flag)
	{
		do
		{
			cifra = true;
			cout << "Dobrodosli u zmijicu! Izaberite neku od sledecih opcija:\n1.Pocetak igre\n2.Uputstva\n3.Izlaz" << endl;
			cin >> rec;
			for (int i = 0; i < rec.length(); i++)
			{
				if (!isdigit(rec[i])) cifra = false;
			}
			if (cifra == false) cout << "Molimo vas unesite samo cifre!" << endl;
			else op = stoi(rec);

			if (op < 1 || op > 3)
			{
				cout << "Niste uneli odgovarajucu dimenziju!" << endl;
				system("PAUSE");
			}
			cin.ignore();
			system("cls");
		}while(op < 1 || op > 3 || cifra == false);
		
		switch (op)
		{
		case 1:
			system("cls");
			do
			{
				system("cls");
				cifra = true;
				cout << "Unesite dimenzije prozora N (15-40, prozor ce biti u NxN obliku):" << endl;
				cin >> rec;
				cin.ignore();
				for (int i = 0; i < rec.length(); i++)
				{
					if (!isdigit(rec[i])) cifra = false;
				}
				if (cifra == false)
				{
					cout << "Molimo vas unesite samo cifre!" << endl;
				}
				else
				{
					n = stoi(rec);
				}
				if (n < 15 || n > 40)
				{
					cout << "Niste uneli odgovarajucu dimenziju!" << endl;
					system("PAUSE");
				}
			} while ((n < 15 || n > 40) || (cifra == false));
			system("cls");
			dimenzije = n;
			int pom;
			do
			{
				system("cls");
				cifra = true;
				cout << "Unesite nacin bodovanja:" << endl << "1. Celobrojno" << endl << "2. Simbolicki" << endl;
				cin >> rec;
				cin.ignore();
				for (int i = 0; i < rec.length(); i++)
				{
					if (!isdigit(rec[i])) cifra = false;
				}
				if (cifra == false)
				{
					cout << "Molimo vas unesite samo cifre!" << endl;
				}
				else
				{
					pom = stoi(rec);
				}
				if (pom < 1 || pom > 2)
				{
					cout << "Niste uneli odgovarajucu opciju!" << endl;
					system("PAUSE");
				}
			} while ((pom < 1 || pom > 2) || (cifra == false));
			poencic.setOpcija(pom);
			flag = false;
			system("cls");
			break;
		case 2:
			system("cls");
			do
			{
				system("cls");
				cifra = true;
				cout << "Vi ste zmijica!\nCilj igre je da pojedete sto vise vockica, ali pritom da se ne sudarite sa zidovima ili sopstvenim repom!\nNa svakih 5 poena imate sansu da pojede lubenicu koja vam daje 2 boda i permanentno usporava zmijicu!\nZa kontrolisanje zmijice koristiti strelice na tastaturi!\nZa izlazak tokom igre pritisnuti taster x!" << endl;
				cout << "-----------------------------------" << endl;
				cout << "1. Povratak na meni\n2. Rekalibracija highscore-a\n";
				cin >> rec;
				cin.ignore();
				for (int i = 0; i < rec.length(); i++)
				{
					if (!isdigit(rec[i])) cifra = false;
				}
				if (cifra == false) cout << "Molimo vas unesite samo cifre!" << endl;
				else
				{
					n = stoi(rec);
				}
				if (n < 1 || n > 2)
				{
					cout << "Niste uneli odgovarajucu dimenziju!" << endl;
					system("PAUSE");
				}
			} while ((n < 1|| n > 2) || (cifra == false));

			if (n == 2)
			{
				try
				{
					f.open("Highscore.txt", ios::out | ios::trunc);

					f << 0 << endl << 0.0;
					f.close();
					cout << "USPESNO REKALIBRIRANJE!!!" << endl;
				}
				catch (...)
				{
					cout << "Greska u radu sa fajlovima! (POGLEDATI UPUTSTVA)" << endl;
					system("PAUSE");
					exit(1);
				}
			}
				system("PAUSE");
				system("cls");
				break;
		case 3:
			system("cls");
			cout << "Pozdrav! \n";
			exit(1);
			break;
		//default:
			//system("cls");
			//cout << "Niste uneli odgovarajucu opciju!\n" << endl;
			//system("cls");
		}
	}
}
void Igra::podesavanja()
{
	iteracijaIgre = true;
	exitToken = true;
	Zmija zm(dimenzije);
	setZmija(zm);
	Vocka vc;
	setVocka(vc);
	Lubenica lb;
	setLubenica(lb);

	duzinaPojedenih = 0;
	pojedene = new bool[dimenzije*dimenzije];
	vockica.generisi(dimenzije);
	lubenicica.generisi(dimenzije);
	zmijica.setSmer(STOP);

	while (!proveri())
	{
		zmijica.generisi(dimenzije);
		lubenicica.generisi(dimenzije);
		vockica.generisi(dimenzije);
	}
	poencic.setIntPoeni(0);
	poencic.setFloatPoeni(0);
}
bool Igra::proveri()
{
	if ((zmijica.getGlavaX() == vockica.getVockaX() && zmijica.getGlavaY() == vockica.getVockaY()) || (zmijica.getGlavaX() == lubenicica.getVockaX() && zmijica.getGlavaY() == lubenicica.getVockaY()) || (zmijica.getGlavaX() <= 0 || zmijica.getGlavaX() >= dimenzije + 1 || zmijica.getGlavaY() < 0 || zmijica.getGlavaY() > dimenzije - 1) || (vockica.getVockaX() <= 0 || vockica.getVockaX() >= dimenzije + 1 || vockica.getVockaY() < 0 || vockica.getVockaY() > dimenzije - 1) || (lubenicica.getVockaX() <= 0 || lubenicica.getVockaX() >= dimenzije + 1 || lubenicica.getVockaY() < 0 || lubenicica.getVockaY() > dimenzije - 1))
		return false;
	else return true;
}
void Igra::fajl(int opc)
{
	int iPom;
	float fPom;
	string pom1, pom2;
	fstream f;
	
	switch (opc)
	{
	case 1:
		try {
			f.open("Highscore.txt", ios::in | ios::app);

			std::getline(f, pom1);
			poencic.setIntHighscore(stoi(pom1));
			std::getline(f, pom2);
			f.close();

			if (!iteracijaIgre)
			{
				iPom = maxScore<int>(poencic.getIntPoeni(), poencic.getIntHighscore());
				f.open("Highscore.txt", ios::out);
				f << iPom << endl << pom2;
				f.close();
			}
		}
		catch (...) {
			cout << "Greska u radu sa fajlovima! (POGLEDATI UPUTSTVA)" << endl;
			system("PAUSE");
			exit(1);
		}
	case 2:
		try {
			f.open("Highscore.txt", ios::in | ios::app);

			std::getline(f, pom1);
			std::getline(f, pom2);
			poencic.setFloatHighscore(stof(pom2));
			f.close();

			if (!iteracijaIgre)
			{
				fPom = maxScore<float>(poencic.getFloatPoeni(), poencic.getFloatHighscore());
				f.open("Highscore.txt", ios::out);
				f << pom1 << endl << fPom;
				f.close();
			}
		}
		catch (...) {
			cout << "Greska u radu sa fajlovima! (POGLEDATI UPUTSTVA)" << endl;
			system("PAUSE");
			exit(1);
		}
	}
}
void Igra::prozor()
{
	int pom = 0;
	//system("cls");
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0,0 });

	for (int i = 0; i < dimenzije + 2; i++)
		cout << "#";
	cout << endl;

	for (int i = 0; i < dimenzije; i++)
	{
		for (int j = 0; j <= dimenzije + 1; j++)
		{
			if (j == 0 || j == dimenzije + 1) cout << "#";
			else if (j == zmijica.getGlavaX() && i == zmijica.getGlavaY())
			{
				if (iteracijaIgre)
					cout << "\033[92mO\033[0m";
					//cout << "O";
				else
					cout << "\033[91mX\033[0m";
					//cout << "X";
			}
			else if (j == vockica.getVockaX() && i == vockica.getVockaY())
				cout << "\033[95mV\033[0m";
				//cout << "V";
			else if (poencic.getIntPoeni() % 5 == 0 && j == lubenicica.getVockaX() && i == lubenicica.getVockaY())
				//cout << "\033[0;96;100mL\033[0m";
				cout << "\033[96mL\033[0m";
				//cout << "L";
			else
			{
				bool rep = false;
				for (int k = 0; k < zmijica.getRepDuzina(); k++)
				{
					if (j == zmijica.getRepX()[k] && i == zmijica.getRepY()[k])
					{
						{
							if (iteracijaIgre) 
								cout << "\033[32mo\033[0m";
								//cout << "o";
							else 
								cout << "\033[31mx\033[0m";
								//cout << "x";
						}
						rep = true;
					}
				}

				if (!rep)
				{
					cout << " ";
				}
			}

		}
		cout << endl;
	}

	for (int i = 0; i < dimenzije + 2; i++)
		cout << "#";
	cout << endl;

	switch (poencic.getOpcija())
	{
	case 1:
		cout << "Poeni: " << poencic.getIntPoeni() << endl;
		cout << "Highscore: " << poencic.getIntHighscore() << endl;
		break;
	case 2:
		cout << "Poeni:     ";
		
		for (int i = 0; i < duzinaPojedenih; i++)
		{
			if (pojedene[i] == true) cout << "\033[95m|\033[0m";
			else cout << "\033[96m|\033[0m" << "\033[96m|\033[0m";
			//cout << "|";
		}
		

		/*
		for (float i = 0; i < poencic.getFloatPoeni(); i += 0.1)
		{
			if (pojedene[pom++] == true) cout << "\033[95mV\033[0m";
			else cout << "\033[36mL\033[0m";
		}
		*/

		cout << endl << "Highscore: ";
		for (float i = 0; i < poencic.getFloatHighscore(); i += 0.1)
			cout << "|";
		cout << endl;
		break;
	}
	cout << "Pritisnite X za kraj igre!" << endl;
	if (iteracijaIgre == false) cout << "Kraj igre! " << endl;
}
bool Igra::core()
{
	bool stanje;
	stanje = poencic.proveraUkupno(dimenzije);
	stanje = zmijica.pokret(dimenzije);

	int pom = zmijica.getRepDuzina();
	int pom1 = poencic.getIntPoeni();
	float pom2 = poencic.getFloatPoeni();
	static int ok = 0;
	if (zmijica.getGlavaX() == vockica.getVockaX() && zmijica.getGlavaY() == vockica.getVockaY())
	{

		switch (poencic.getOpcija())
		{
		case 1:
			poencic.setIntPoeni(pom1+=1);
			break;
		case 2:
			poencic.setIntPoeni(pom1+=1);
			poencic.setFloatPoeni(pom2+=0.1);
			break;
		}
		zmijica.setRepDuzina(pom+=1);
		pojedene[duzinaPojedenih++] = true;
		PlaySound(TEXT("vocka.wav"), NULL, SND_FILENAME | SND_ASYNC);
		while ((zmijica.getGlavaX() == vockica.getVockaX() && zmijica.getGlavaY() == vockica.getVockaY() || (vockica.getVockaX() <= 0 || vockica.getVockaX() >= dimenzije + 1 || vockica.getVockaY() <0 || vockica.getVockaY() > dimenzije - 1)))
		{
			vockica.generisi(dimenzije);
		}
	}
	else if (zmijica.getGlavaX() == lubenicica.getVockaX() && zmijica.getGlavaY() == lubenicica.getVockaY())
	{
		if (poencic.getIntPoeni() % 5 == 0)
		{
			ok += lubenicica.getDelay();
			zmijica.setVremeX(ok);
			zmijica.setVremeY(ok);
			zmijica.setRepDuzina(pom += 2);
			pojedene[duzinaPojedenih++] = false;
			PlaySound(TEXT("lubenica.wav"), NULL, SND_FILENAME | SND_ASYNC);
		}
		else zmijica.setRepDuzina(pom);
		switch (poencic.getOpcija())
		{
		case 1:
			if (poencic.getIntPoeni() % 5 == 0) poencic.setIntPoeni(pom1 += 2);
			else poencic.setIntPoeni(pom1);
			break;
		case 2:
			if (poencic.getIntPoeni() % 5 == 0)
			{
				poencic.setIntPoeni(pom1 += 2);
				poencic.setFloatPoeni(pom2 += 0.2);
			}
			else
			{
				poencic.setIntPoeni(pom1);
				poencic.setFloatPoeni(pom2);
			}
			break;
		}
		if (poencic.getIntPoeni() % 5 == 0)
		{
			while ((zmijica.getGlavaX() == lubenicica.getVockaX() && zmijica.getGlavaY() == lubenicica.getVockaY() || (lubenicica.getVockaX() <= 0 || lubenicica.getVockaX() >= dimenzije + 1 || lubenicica.getVockaY() < 0 || lubenicica.getVockaY() > dimenzije - 1)))
			{
				lubenicica.generisi(dimenzije);
			}
		}
	}

	for (int i = 0; i < zmijica.getRepDuzina(); i++)
	{
		while ((vockica.getVockaX() == zmijica.getRepX()[i] && vockica.getVockaY() == zmijica.getRepY()[i]) || (vockica.getVockaX() == zmijica.getRepX()[i] && vockica.getVockaY() == zmijica.getRepY()[i]))
		{
			vockica.generisi(dimenzije);
			if (poencic.getIntPoeni() % 5 == 0)
				while((lubenicica.getVockaX() == zmijica.getRepX()[i] && lubenicica.getVockaY() == zmijica.getRepY()[i]) || (lubenicica.getVockaX() == zmijica.getRepX()[i] && lubenicica.getVockaY() == zmijica.getRepY()[i]))
					lubenicica.generisi(dimenzije);	
		}
	}
	return stanje;
}




	


