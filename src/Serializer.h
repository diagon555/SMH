#ifndef Serializer_h
#define Serializer_h

#define EEPROM_START_ADDRESS 100

#include "List.h"
#include "interfaces.h"

class Serializer: public List<iSerializable*>
{
	int cur_address;
public:
	Serializer();
	void Add(iSerializable *obj);
	void LoadFromEEPROM();
};

#endif