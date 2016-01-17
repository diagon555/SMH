#ifndef TempSensors_h
#define TempSensors_h

#include <Arduino.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 53
#define TEMPERATURE_PRECISION 9

class TempSensor
{
	DeviceAddress deviceAddress;
private:

};

class TempSensors
{
	// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
	OneWire *oneWire;

	// Pass our oneWire reference to Dallas Temperature. 
	DallasTemperature *sensors;

public:
	TempSensors();
	static void read_temp();
	String list();
};


#endif