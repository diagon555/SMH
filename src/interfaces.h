#ifndef INTERFACES_h
#define INTERFACES_h

#include <Arduino.h>
#include <EEPROM.h>

#include "List.h"

class iSerializable
{
protected:
	int rom_size;
	int start_address;
public:
	iSerializable(int rom_size);
	virtual void Serialize()=0;
	virtual void Deserialize()=0;
	void SetSerializableStartAddress(int addr){start_address = addr;};
	int GetSerializableSize(){return rom_size;};
};

class iNamable
{
	String name;
public:
	iNamable(String name);
	String GetName();
};

template<class T>
class iListSearch: public List<T>
{
public:
    typedef typename List<T>::node * link; // synonym for pointer to a node.

	uint8_t GetNum(T);
	T GetByNum(uint8_t num);
	T GetByName(String name);
};

template<class T>
uint8_t iListSearch<T>::GetNum(T item)
{
	uint8_t i = 0;
	for (link t = List<T>::head; t != NULL; t = t->next) {
		if(t->item == item) return i;
		i++;
	}
	
	return 255;
}

template<class T>
T iListSearch<T>::GetByNum(uint8_t num)
{
	if(num >= List<T>::count()) return NULL;
	
	
	for (link t = List<T>::head; t != NULL; t = t->next) {
		if(!num)
		{
			return t->item;
		}
		num--;
	}
	
	return 0;
}

template<class T>
T iListSearch<T>::GetByName(String name)
{
	for (link t = List<T>::head; t != NULL; t = t->next) {
		if(t->item->GetName() == name)
		{
			return t->item;
		}
	}
	
	return 0;
}

#endif