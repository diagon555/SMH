#include "interfaces.h"


iSerializable::iSerializable(int rom_size, int start_address)
{
	this->rom_size = rom_size;
	this->start_address = start_address;
}


iNamable::iNamable(String name)
{
	if(name.length() > 8)
		this->name = name.substring(0, 8);
	else
		this->name = name;
}

String iNamable::GetName()
{
	return String(name);
}