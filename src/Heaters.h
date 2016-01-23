#ifndef Heaters_h
#define Heaters_h

#include "Relay.h"
#include "TempSensors.h"

#define HEAT_OFF -127
#define MAX_COUNT_HEATERS 12
#define HYSTERESIS 1


class Heater: public iNamable
{
	Relay *relay;
	TempSensor *tempsensor;
	
	int Tset;
	int Tsup;
	boolean heat_state;
	boolean heat_current;
	
	void set_state(boolean state);
public:
	Heater(String name, Relay *relay, TempSensor *tempsensor, int Tset = 23, int Tsup = HEAT_OFF, boolean state = false);
	
	int Serialize(int addr);

	void SetTemp(int temp){Tset = temp;};
	void SetTempSupport(int temp){Tsup = temp;};
	
	void On(){heat_state = true;}
	void Off(){heat_state = false;}
	
	void OffForce(){Tset = HEAT_OFF; };
	void OffSupportTemp(){Tsup = HEAT_OFF; };
	
	int GetTemp(){return tempsensor->GetTemp();};
	int GetSetTemp(){return Tset;};
	int GetTempSupport(){return Tsup;};
	boolean GetStatus(){return heat_state;};
	boolean GetCurrentStatus(){return heat_current;};
	
	void check();
};

class Heaters: public iSerializable, public List<Heater*>
{
public:
	static Heaters *instance;
	Heaters();
	virtual void Serialize();
	virtual void Deserialize();
	String command(Command * command);
	void Add(Heater *heater){push(heater);};
	uint8_t GetNum(Heater *);
	Heater *GetByName(String name);
	String help();
	void _check();
	static void check(); //Sheduler every 1000ms (get temp from sensors)
};

#endif
