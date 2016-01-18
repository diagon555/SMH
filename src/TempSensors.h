#ifndef TempSensors_h
#define TempSensors_h

#include <Arduino.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#include "Commands.h"
#include "List.h"

#define ONE_WIRE_BUS 53
#define TEMPERATURE_PRECISION 9

class TempSensor
{
	uint8_t deviceAddress[8];
	String name;
	float temp;
	
public:
	TempSensor(const uint8_t*, String _name);
	void _request();
	String GetName();
	const uint8_t* GetAddress();
	boolean Availiable();
	float GetTemp();
};

class TempSensors: public List<TempSensor*>
{
	// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
	OneWire *oneWire;

	// Pass our oneWire reference to Dallas Temperature. 
	DallasTemperature *sensors;

public:
	TempSensors();
	String command(Command * command);
	boolean Add(const uint8_t* d_address, String name);
	TempSensor *GetByName(String name);
	TempSensor *GetByAddress(const uint8_t* d_address);
	String list();
};


#endif