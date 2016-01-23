#ifndef Relay_h
#define Relay_h

#include <Arduino.h>

#include "interfaces.h"
#include "List.h"
#include "Commands.h"

#define MAX_COUNT_RELAY 16


class Relay: public iNamable
{
	uint8_t pin;
	int shift_register;
	
	uint8_t on_status;
	
	uint8_t status;
protected:
	void set_status(uint8_t status);
public:
	Relay(String name, uint8_t pin, uint8_t shift_register=NULL, uint8_t on_status=1, uint8_t status=0);
	
	uint8_t GetPin(){return pin;};
	int GetShiftReg(){return shift_register;};
	uint8_t GetOnStatus(){return on_status;};
	uint8_t GetStatus(){return status;};
	
	void On();
	void Off();
};

class Relays: public List<Relay*>, public iSerializable
{
public:
	Relays();
	virtual void Serialize();
	virtual void Deserialize();
	String command(Command * command);
	void Add(Relay *relay){push(relay);};
	uint8_t GetNum(Relay *);
	Relay *GetByNum(uint8_t num);
	Relay *GetByName(String name);
	String help();
};

#endif
