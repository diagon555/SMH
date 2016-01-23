/*
  SMH.cpp
  
  Ivan Korostelev
*/

#include "SMH.h"


#define ONEWIREPIN 53


SMHClass::SMHClass()
{
	Serial.begin(9600);
	Serial.println("SMH start");
	
	serializer = new Serializer();
	schedulers = new Schedulers();
	commands = new Commands();

	
	tempsensors = new TempSensors(ONEWIREPIN);
	relays = new Relays();
	heaters = new Heaters();
	
	serializer->Add(tempsensors);
	serializer->Add(relays);
	serializer->Add(heaters);
	
	schedulers->add(TempSensors::check, 1000, 3000);
	schedulers->add(Heaters::check, 1000, 3000);
	
	serializer->LoadFromEEPROM();
}

void SMHClass::set_logfile_path(String path)
{
	logfile_path = path;
}


void SMHClass::set_printer(Print & prn)
{
	printer = &prn;
}



void SMHClass::loop(void)
{
	schedulers->check();
	commands->check();
}

SMHClass SMH;
