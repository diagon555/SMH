#ifndef INTERFACES_h
#define INTERFACES_h

#include <Arduino.h>
#include <EEPROM.h>

class iSerializable
{
protected:
	int rom_size;
	int start_address;
public:
	iSerializable(int rom_size, int start_address);
	virtual void Serialize() = 0;
	virtual void Deserialize() = 0;
};

class iNamable
{
	String name;
public:
	iNamable(String name);
	String GetName();
};


#endif