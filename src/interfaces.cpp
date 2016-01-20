#include "interfaces.h"

int iSerializable::Serialize(int addr)
{
	;
}

int iSerializable::Deserialize(int addr)
{
	;
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