//Ognjen Ristovski RT-1/21
#include <iostream>
#include <fstream>
#include <conio.h>  //Ova biblioteka se koristi za korisnicki unos tj. komande za pomeranje same zmije tipa kbhit()
#include <windows.h> //Ova biblioteka se koristi zbog SetConsoleCursorPosition()
#include <chrono>    //Upotreba ove i <thread> datoteke zbog vremenske pauze da igra ne bi bila prebrza
#include <thread>  
#include <ctime>    //Ova biblioteku koristimo zbog srand()
#include "Zmija.h"
using namespace std;

//Definisanje strelica za kretanje
#define TASTER_GORE 72
#define TASTER_DOLE 80
#define TASTER_LEVO 75
#define TASTER_DESNO 77

//Deklarisanje funkcija

//Funkcija koja sluzi kao meni da korisniku da opcije za pocetak, opis kontrola/igre i izlaz
int uvod();

//Funkcija koja podesava samu postavku igre tipa pocetne koordinate zmije, vocke, poeni itd.             
void podesavanja(int dimenzije);

//Funkcija za dokumentovanje highscore-a
void fajl();

//Funkcija koja kreira sam prozor/prostor gde ce se odvijati igra
void prozor(int dimenzije);

//Funkcija koja registruje unos sa tastature
void unos();

//Glavni kod/logika kako sama igra funkcionise
void igra(int dimenzije);

//Deklarisanje globalnih promenljivih

ifstream fin;
ofstream fout;
fstream f;

struct zmija
{
	int glavaX, glavaY, korak = 1;
	int *repX, *repY, repDuzina = 0;
};
zmija zmijica;
struct vocka
{
	int vockaX, vockaY;
};
vocka vockica;
struct poen
{
	int poeni, highscore;
};
poen poencic;

//Proverava da li je igra trenutno aktivna
bool iteracijaIgre;
bool zidSmrt;

int pom;

//Koriscenjem enum kreiramo svoj tip koji moze imati vrednosti STOP, GORE, DOLE, LEVO i DESNO
//Posle ovoga kreiramo promenljivu smer kojoj dodeljujemo nas novokreirani tip kretanje
enum kretanje { STOP = 0, GORE, DOLE, LEVO, DESNO };
kretanje smer;

int main()
{
	//Glavna funkcija programa, poziva sve ostale prethodno definisane osim uvod() funkcije koju poziva promenljiva pom

	//Kreira se pomocna promenljiva koja poziva funkciju uvod i od nje dobija dimenzije prozora igre
	int pom;
	pom = uvod();

	//Posto duzina niza koja predstavlja rep zmije zavisi od prozora igre, mora se dinamicki dodeliti nakon dobavljanja informacije o velicini prozora
	zmijica.repX = new int[pom*pom];
	zmijica.repY = new int[pom*pom];

	//Provera dinamicke dodele
	if (zmijica.repX == NULL)
	{
		cout << "Greska pri dinamickoj dodeli memorije!";
		exit(1);
	}
	if (zmijica.repY == NULL)
	{
		cout << "Greska pri dinamickoj dodeli memorije!";
		exit(1);
	}

	//Pozivanje funkcija za postavku igre, vocke, zmije, smera i slicnog
	//Takodje pozivanja fajl funkcije da se dobije prethodni highscore ako postoji na racunaru
	podesavanja(pom);
	fajl();
	while (iteracijaIgre)
	{
		prozor(pom);
		unos();
		igra(pom);
		if (smer == LEVO || smer == DESNO)
		{
			//Komanda koja pauzira na 50 milisekundi da bi igrac lakse mogao da prati sta se desava
			//Korisceno je umesto sleep() jer je argument za sleep odredjen u sekundama
			this_thread::sleep_for(40ms);
		}

		//Koristi se da uskladi brzinu jer bez delay-a je prebrza zmija, takodje je vertikalno kretanje brze nego horizontalno
		if (smer == GORE || smer == DOLE)
		{
			this_thread::sleep_for(80ms);
		}
		//this_thread::sleep_for(50ms);   
	}

	//Pozivanje fajla nakon prestanka igre za upis highscore-a ako je probijen rekord
	fajl();

	//Poziva se ponovo ova funkcija jer ovog puta iteracijaIgre je false i zmija se umesto slova Z(z) menja sa X(x) jer je mrtva/kraj igre
	prozor(pom);

	//Oslobadjanje dinamicko dodeljenog niza za rep
	delete zmijica.repX;
	delete zmijica.repY;

	system("pause");
	return 0;
}
int uvod()
{
	//U ovom delu programa koristi se system("cls") za ciscenje ekrana i on takodje moze da se koristi tokom igre za zmiju, ali ekran previse treperi
	int n;
	while (1)
	{
		cout << "Dobrodosli u zmijicu! Izaberite neku od sledecih opcija:\n1.Pocetak igre\n2.Kontrole i Cilj\n3.Izlaz" << endl;
		switch (cin.get())
		{
		case '1':
			system("cls");
			do
			{
				cout << "Unesite dimenzije prozora N (15-30, prozor ce biti u NxN obliku)\nNAPOMENA! Sto je veci prozor, zmija ima sporiju kretnju jer mora preci veci teren!" << endl;
				cin >> n;
				if (n < 15 || n>30) cout << "Niste uneli odgovarajucu dimenziju!" << endl;
			} while (n < 15 || n>30);
			system("cls");
			return n;
		case '2':
			system("cls");
			cout << "Vi ste zmijica!\nCilj igre je da pojedete sto vise vockica, ali pritom da se ne sudarite sa zidovima ili sopstvenim repom!\nZa kontrolisanje zmijice koristiti strelice na tastaturi!\nZa izlazak tokom igre pritisnuti taster x!\nPritisnite ENTER za nastavak!" << endl;
			cin.ignore();
			cin.get();
			system("cls");
			break;
		case '3':
			system("cls");
			cout << "Pozdrav! Hvala sto ste igrali!\n";
			exit(1);
			break;
		default:
			system("cls");
			cout << "Niste uneli odgovarajucu opciju!\n" << endl;
		}
	}

}
void podesavanja(int dimenzije)
{
	//Igra je pokrenuta
	iteracijaIgre = true;

	//Pri pocetku igre zmija se nece kretati        
	smer = STOP;

	//Funkcija koja generise novi seed za rand() na racunaru 
	//Koristi time() funkciju koja koristi unutrasnje vreme racunara za generisanje seed-a, a argument je nullptr jer je potrebna samo vrednost        
	srand(time(nullptr));

	//Funkcija rand() generise broj od 0 do 32767 i onda se uradi moduo tog broja sa sirinom kako bi se dobio broj manji od sirine prozora.
	//Ovo u kombinaciji sa srand sluzi da se dobije proizvoljna lokacija zmije unutar prozora  
	zmijica.glavaX = rand() % dimenzije;
	zmijica.glavaY = rand() % dimenzije;

	//Isto kao i za zmiju, generise se proizvoljno mesto za vocku unutar granica
	vockica.vockaX = rand() % dimenzije;
	vockica.vockaY = rand() % dimenzije;

	//Moze doci do slucaja da se zmija i vocka kreiraju na istom mestu ili da su zmija ili vocka van zidova, pa ce se generisati nove pozicije sve dok uslovi nisu ispunjeni
	while ((zmijica.glavaX == vockica.vockaX && zmijica.glavaY == vockica.vockaY) || (zmijica.glavaX <= 0 || zmijica.glavaX >= dimenzije + 1 || zmijica.glavaY<0 || zmijica.glavaY>dimenzije - 1) || (vockica.vockaX <= 0 || vockica.vockaX >= dimenzije + 1 || vockica.vockaY<0 || vockica.vockaY>dimenzije - 1))
	{
		//Generise se novi seed za rand() jer protekli nije odgovarao za uslove igre
		srand(time(nullptr));
		zmijica.glavaX = rand() % dimenzije;
		zmijica.glavaY = rand() % dimenzije;
		vockica.vockaX = rand() % dimenzije;
		vockica.vockaY = rand() % dimenzije;
	}
	//Poeni na pocetku igre su 0;
	poencic.poeni = 0;
}
void fajl()
{
	//Otvara datoteku za citanje i takodje append jer ako ne postoji datoteka da je kreira, nakon se radi provera otvaranja
	f.open("highscore.txt", ios::in | ios::app);
	if (!f)
	{
		cout << "Neuspesno otvaranje datoteke!";
		exit(1);
	}

	//Uzima se vrednost datoteke ako ona postoji i upisuje se u promenljivu highscore i pritom zatvara fajl
	f >> poencic.highscore;
	f.close();

	if (!iteracijaIgre)
	{
		if (poencic.poeni > poencic.highscore)
		{
			//Otvara ponovo fajl za upis, radi proveru i ako su trenutni poeni veci od highscore-a, upisuje ih u fajl i zatvara ga
			f.open("highscore.txt", ios::out);
			if (!f)
			{
				cout << "Neuspesno otvaranje datoteke!";
				exit(1);
			}
			f << poencic.poeni;
			f.close();
		}
	}

}
void prozor(int dimenzije)
{
	//Funkcija za ciscenje ekrana koja manje trepeni nego system("cls"), jer bi se svaki pokret preklapao jedan preko drugog.
	//Prvi parametar predstavlja bafer ekrana, u ovom slucaju GetStdHandle(STD_OUTPUT_HANDLE) funkcija, STD_OUTPUT_HANDLE je trenutno aktivan bafer konzole, a "Handle" je zapravo pokazivac na neki objekat.
	//Drugi parametar predstavlja koordinate gde ce se cursor prikazati.
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0,0 });

	//Unutar ove petlje se pravi gornji/donji zid koji predstavlja sirinu, ali dodaje se 2 zbog coskova jer prostor UNUTAR zidova treba da bude sirina*visina             
	for (int i = 0; i < dimenzije + 2; i++)
		cout << "#";
	cout << endl;

	//Unutar ove petlje se pravi ostatak levog i desnog zida sve do donjeg
	//for petlja sa i predstavlja visinu, dok j predstavlja sirinu
	//Za lakse razumevanje prozora i zasto se negde nalazi dimenzije+1 i slicno:
	/*
		xxxxxxx
		x----rx
		x-----x
		o-----y
		x-----x
		x----zx
		xxxxxxx

		Ovo je 5x5 prozor, ali sa zidovima je zapravo 7x7 da bi prostor ZA IGRU I KRETANJE ZMIJE bio 5x5
		For petlja sa promenljivom i --> Visina
		For petlja sa promenljivom j --> Sirina
		Gornji i donji zid su zasebne petlje pa se ne odnose na sredisnji deo
		Slovo o predstavlja mesto gde je sirina=0 tj. to je prva pozicija, dok je skroz desno vrednost 6
		Slovo y predstavlja upravo sirinu gde se vidi da je njena vrednost 6 (Zbog ovoga su dimenzije+1 tj. sirina+1 jer ide od vrednosti 0 do 6 --> Ukupno 7)
		Slovo r predstavlja mesto gde je visina=0 tj. ceo prvi red spada u nultu iteraciju petlje (Ceo prvi red je visina 0)
		Slovo z predstavlja mesto gde je visina=4 sto znaci da nije bilo nikakvo dodavanje jer ima tacno 5 polja za visinu (Od vrednosti 0 do 4 --> Ukupno 5)
	*/
	for (int i = 0; i < dimenzije; i++)
	{
		for (int j = 0; j <= dimenzije + 1; j++)
		{
			//Ovde se proverava da li se iteracija nalazi na poziciji levog zida(j==0) ili desnog zida(j==dimenzije+1) i ako jeste crta zid, uostalom prazno mesto
			if (j == 0 || j == dimenzije + 1) cout << "#";
			//Ovde se proverava da li se vrednost zmije nalazi na datoj poziciji i ako jeste crta zmiju    
			else if (j == zmijica.glavaX && i == zmijica.glavaY)
			{
				if (iteracijaIgre == true) cout << "Z";
				else cout << "X";
			}
			//Kao i prethodna linija, samo sto se crta vocka umesto zmije  
			else if (j == vockica.vockaX && i == vockica.vockaY) cout << "V";
			//U narednom bloku ulazi se u for petlju koja prolazi kroz duzinu repa i kad se poklope koordinate za visinu i sirinu sa repom iscrtava se rep
			//Ako dolazi do iscrtavanja repa, bool rep postaje true i onda sva ostala mesta koja nisu zid, glava, vocka ili rep postaju znak za razmak
			else
			{
				bool rep = false;
				for (int k = 0; k < zmijica.repDuzina; k++)
				{
					if (j == zmijica.repX[k] && i == zmijica.repY[k])
					{
						{
							if (iteracijaIgre == true) cout << "z";
							else cout << "x";
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

	//Prikaz poena i highscore-a na ekranu.
	cout << "Poeni: " << poencic.poeni << endl;
	cout << "Highscore: " << poencic.highscore << endl;
	if (iteracijaIgre == false) cout << "Kraj igre!" << endl;

}
void unos()
{
	//Deo programa koji proverava unos, krece se od kbhit() funkcije iz biblioteke conio.h koji sluzi za konzolni ulaz/izlaz
	//kbhit je skraceno od Keyboard Hit i registruje da li je pritisnut neki taster
	//Ulazi se u switch koji ima funkciju getch() kao parametar
	//Koristi se getch() jer za razliku od getchar() ne koristi bafer pa se unet karakter odma prosledjuje bez pritiska tastera ENTER
	//Unutar switch-a pravimo slucajeve za pritiskanje strelica na tastaturi
	//U svakom od mogucih slucajeva za kretanje se nalazi po jedan if koji sprecava da se ide suprotnom putanjom jer bi zmija usla u samu sebe tj. sopstveni rep
	//Ako je pritisnut neki taster koji nije ponudjen, nista se nece dogoditi (slucaj default)
	if (_kbhit())
	{
		switch (_getch())
		{
		case TASTER_GORE:
			if (smer != DOLE) smer = GORE;
			break;
		case TASTER_DOLE:
			if (smer != GORE) smer = DOLE;
			break;
		case TASTER_LEVO:
			if (smer != DESNO) smer = LEVO;
			break;
		case TASTER_DESNO:
			if (smer != LEVO) smer = DESNO;
			break;
		case 'x':
			iteracijaIgre = false;
			cout << "Kraj igre!" << endl;
		default:
			break;
		}
	}
}
void igra(int dimenzije)
{
	//Ovo je glavni deo programa gde se opisuje kako funkcionise sam rep zmije
	//Kada rep ne postoji, ne ulazi se u for petlju i nulti indeks repX i repY se pamti kao sama glava zmije
	//Cim se pojede vocka i repDuzina postane 1, u iteraciji for petlje bi se proslo samo jedanput i imali bi ovakav slucaj
			//repX[1]=repX[0];
			//repY[1]=repY[0];
	//Ovim se pamti gde je trenutno glava i nakon for petlje imamo
			//repX[0]=glavaX;
			//repY[0]=glavaY;
	//Ovim se menjaju same koordinate glave tj. indeksi za glavu zmije dobijaju sledecu iteraciju vrednosti (Da nema ovoga rep i glava bi bili na istom mestu)
	//Nakon ovoga se crta rep, ovo sve je moguce jer redosled funkcija ide:
			//prozor()
			//unos()
			//igra()
	//Znaci prvo se prikaze slika na ekranu, pa se odradi unos tj. dolazi do promene koordinata glaveX i na kraju ide sama funkcija za igru gde se odvija ova petlja za rep
	//Nakon nje dolazi ponovo do prikaza trenutnog ekrana i odma nakon nje do sledeceg unosa, i ako nema nikakvog unosa zmija krece u trenutnom smeru
	//Prelaz izmedju funkcije prozor() i unos() pa do sledeceg prozora() je previse brza za ljudsko oko pa ne dolazi do problema oko repa
	for (int i = zmijica.repDuzina; i > 0; i--)
	{
		zmijica.repX[i] = zmijica.repX[i - 1];
		zmijica.repY[i] = zmijica.repY[i - 1];
	}
	zmijica.repX[0] = zmijica.glavaX;
	zmijica.repY[0] = zmijica.glavaY;


	//Deo programa zaduzen za kretanje, STOP se javlja samo pri pocetku igre 
	switch (smer)
	{
	case GORE:
		zmijica.glavaY -= zmijica.korak;
		break;
	case DOLE:
		zmijica.glavaY += zmijica.korak;
		break;
	case LEVO:
		zmijica.glavaX -= zmijica.korak;
		break;
	case DESNO:
		zmijica.glavaX += zmijica.korak;
		break;
	}

	//U ovoj for petlji proverava se da li je glava u istoj lokaciji kao i rep tj. da li zmija jede svoj rep i ako jeste slucaj dolazi do kraja igre
	for (int i = 0; i < zmijica.repDuzina; i++)
	{
		if (zmijica.glavaX == zmijica.repX[i] && zmijica.glavaY == zmijica.repY[i]) iteracijaIgre = false;
	}

	//Ako igrac uspe da pojede svaku vocku na ekranu koja za prozor npr. 5x5 iznosi 25 polja gde vocka moze da se nadje, igrac pobedjuje
	if (poencic.poeni == (dimenzije*dimenzije))
	{
		iteracijaIgre = false;
		cout << "Cestitke! Uspesno ste presli igru!";
	}

	//Ako se zmija sudari sa zidom dolazi do kraja igre
	//Ovde se za svaki od zidova proverava zasebno da bi doslo do pomeranja repa jer kada glava udari u zid ona se idalje nalazi u njemu
	//Da bi se korektno iscrtali iksevi kad dodje do smrti zmije potrebno je sve vratiti za jedno mesto unazad
	if (zmijica.glavaX <= 0)
	{
		zmijica.glavaX++;
		for (int i = 0; i < zmijica.repDuzina; i++)
		{
			zmijica.repX[i]++;
		}
		iteracijaIgre = false;
	}
	else if (zmijica.glavaX >= dimenzije + 1)
	{
		zmijica.glavaX--;
		for (int i = 0; i < zmijica.repDuzina; i++)
		{
			zmijica.repX[i]--;
		}
		iteracijaIgre = false;
	}
	else if (zmijica.glavaY < 0)
	{
		zmijica.glavaY++;
		for (int i = 0; i < zmijica.repDuzina; i++)
		{
			zmijica.repY[i]++;
		}
		iteracijaIgre = false;
	}
	else if (zmijica.glavaY >= dimenzije)
	{
		zmijica.glavaY--;
		for (int i = 0; i < zmijica.repDuzina; i++)
		{
			zmijica.repY[i]--;
		}
		iteracijaIgre = false;
	}
	//Moze se takodje napraviti i varijanta igre gde zmija prolazi kroz zidove
	//if(zmijica.glavaX<=0) zmijica.glavaX=dimenzije+1; else if(zmijica.glavaX>=dimenzije+1) zmijica.glavaX=0;
	//if(zmijica.glavaY<0) zmijica.glavaY=dimenzije-1; else if(zmijica.glavaY>dimenzije-1) zmijica.glavaY=0;

	//Ako se poklopi pozicija zmije i vocke tj. zmija je pojela vocku onda se dobija poen, rep zmije raste i ponovo se generise vocka
	if (zmijica.glavaX == vockica.vockaX && zmijica.glavaY == vockica.vockaY)
	{
		poencic.poeni++;
		zmijica.repDuzina++;
		while ((zmijica.glavaX == vockica.vockaX && zmijica.glavaY == vockica.vockaY) || (vockica.vockaX <= 0 || vockica.vockaX >= dimenzije + 1 || vockica.vockaY<0 || vockica.vockaY>dimenzije - 1))
		{
			srand(time(nullptr));
			vockica.vockaX = rand() % dimenzije;
			vockica.vockaY = rand() % dimenzije;
		}
	}

	//Ako se poklopi da se vocka stvori unutar repa zmije, generise se nova vocka
	for (int i = 0; i < zmijica.repDuzina; i++)
	{
		while (vockica.vockaX == zmijica.repX[i] && vockica.vockaY == zmijica.repY[i])
		{
			srand(time(nullptr));
			vockica.vockaX = rand() % dimenzije;
			vockica.vockaY = rand() % dimenzije;
		}
	}
}


