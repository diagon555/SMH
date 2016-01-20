#ifndef TempSensors_h
#define TempSensors_h

#include <Arduino.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#include "interfaces.h"
#include "Commands.h"
#include "List.h"

#define TEMPERATURE_PRECISION 9
#define MAX_ERROR_REQUEST 5
#define MAX_COUNT_SENSORS 20

class TempSensor: public iNamable
{
	uint8_t deviceAddress[8];
	float temp;
	int aval;
public:
	TempSensor(const uint8_t*, String _name);
	void _set_temp(float temp);
	const uint8_t* GetAddress();
	boolean Availiable();
	float GetTemp();
};

class TempSensors: public List<TempSensor*>, public iSerializable
{
	// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
	OneWire *oneWire;

	// Pass our oneWire reference to Dallas Temperature. 
	DallasTemperature *sensors;

public:
	static TempSensors* instance;
	TempSensors(int pin);
	virtual void Serialize();
	virtual void Deserialize();
	boolean Add(const uint8_t* d_address, String name);
	uint8_t GetNum(TempSensor *);
	TempSensor *GetByName(String name);
	TempSensor *GetByAddress(const uint8_t* d_address);
	String command(Command * command);
	String list();
	void _check();
	static void check(); //Sheduler every 1000ms (get temp from sensors)
};


#endif