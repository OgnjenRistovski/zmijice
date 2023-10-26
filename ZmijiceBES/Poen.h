#pragma once
class Poen
{
private:
	float floatPoeni, floatHighscore;
	int intPoeni, intHighscore, opcija;
public:
	Poen();
	int getIntPoeni();
	int getIntHighscore();
	int getOpcija();
	float getFloatPoeni();
	float getFloatHighscore();
	void setIntPoeni(int);
	void setIntHighscore(int);
	void setOpcija(int);
	void setFloatPoeni(float);
	void setFloatHighscore(float);
	bool proveraUkupno(int);

};

