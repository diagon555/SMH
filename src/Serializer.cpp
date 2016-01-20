#include "Serializer.h"

Serializer::Serializer()
{
	cur_address = EEPROM_START_ADDRESS;
}

void Serializer::Add(iSerializable *obj)
{
	int size = obj->GetSerializableSize();
	obj->SetSerializableStartAddress(cur_address);
	cur_address += size;
	push(obj);
}

void Serializer::LoadFromEEPROM()
{
	for (link t = head; t != NULL; t = t->next) {
		t->item->Deserialize();
	}
}