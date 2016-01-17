#ifndef INTERFACES_h
#define INTERFACES_h

class iSerializable
{
public:
	virtual int Serialize(int addr);
	virtual int Deserialize(int addr);
};

class iNamable
{
	char name[9];
public:
	
}


#endif