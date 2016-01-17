/*
  SMH.cpp
  
  Ivan Korostelev
*/

#include "SMH.h"
//#include "TempSensors.h"




SMHClass::SMHClass()
{
	schedulers = new Schedulers();
	commands = new Commands();
	
	//schedulers.add(&TempSensors::read_temp, 1000, 5000);
	//schedulers.add(&TempSensors::read_temp1, 2000);
	
	//TEMP
	pinMode(13, OUTPUT);
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
