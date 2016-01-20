#ifndef INTERFACES_h
#define INTERFACES_h

#include <Arduino.h>

class iSerializable
{
public:
	virtual int Serialize(int addr);
	virtual int Deserialize(int addr);
};

class iNamable
{
	String name;
public:
	iNamable(String name);
	String GetName();
};


#endif