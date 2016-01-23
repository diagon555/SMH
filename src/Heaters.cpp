#include "Heaters.h"
#include "SMH.h"

Heater::Heater(String name, Relay *relay, TempSensor *tempsensor, int Tset, int Tsup, boolean state)
	:iNamable(name)
{
	this->relay = relay;
	this->tempsensor = tempsensor;
	this->Tset = Tset;
	this->Tsup = Tsup;
	this->heat_state = state;
}

void Heater::check()
{
	//
}

//Heaters
Heaters* Heaters::instance;

Heaters::Heaters()
	:iSerializable(1+((1+1+1+1+1+8)*MAX_COUNT_HEATERS))
{
	Heaters::instance = this;
}

void Heaters::Serialize()
{
	int addr = start_address;
	EEPROM.update(addr++, count());
	for (link t = head; t != NULL; t = t->next) {
		addr += 5;
		String name = t->item->GetName();
		for(int i=0; i < 8; i++)
		{
			char ch;
			if(i < name.length())
				ch = name[i];
			else
				ch = 0;
			
			EEPROM.update(addr++, ch);
		}
	}

}

void Heaters::Deserialize()
{
	int addr = start_address;
	uint8_t count = EEPROM.read(addr++);
	if(count > MAX_COUNT_HEATERS) count = 0;
	for (uint8_t idx = 0; idx < count; idx++) {
		addr += 5;
		
		char name[9];
		for(int i=0; i < 8; i++)
		{
			name[i] = EEPROM.read(addr++);
		}
		name[8] = 0;
		
		//this->push(new Heater(String(name)));
	}	
}

String Heaters::command(Command * command)
{
	String str;
	
	String cmd = command->Next();
	String cmd_2 = command->Next();
	
	if(cmd == "help") {
		str += "heater list | add <name> <relay_name> <tempsensor_name> | <name> (on|off|set <temp>|set tsup <temp>) \n\r";
	} else if(cmd == "add") {
		String str_relay = command->Next();
		Relay *relay = SMH.relays->GetByName(str_relay);
		if(!relay)
		{
			str += "not found relay";
			return str;
		}
		
		String str_tsens = command->Next();
		TempSensor *tempsensor = SMH.tempsensors->GetByName(str_tsens);
		if(!tempsensor)
		{
			str += "not found tempsensor";
			return str;
		}
		
		Add(new Heater(cmd_2, relay, tempsensor));
		//Serialize();
	} else if(cmd == "list" || cmd == "status") {
		int i = 1;
		for (link t = head; t != NULL; t = t->next) {
			String state;
			if(t->item->GetStatus())
				state = "on";
			else
				state = "off";
			
			str += String(i) + ": " + t->item->GetName() + " " + t->item->GetTemp() + "(" + t->item->GetSetTemp() + ") " + state + "\n\r";
			i++;
		}
	} else {
		Heater *heater = GetByName(cmd);
		if(heater)
		{
			if(cmd_2 == "on"){
				heater->On();
				str += "OK ON";
			} else if(cmd_2 == "off") {
				heater->Off();
				str += "OK OFF";
			} else {
				String state;
				if(heater->GetStatus())
					state = "on";
				else
					state = "off";
				str += String("Heater ") + heater->GetName() + " " + heater->GetTemp() + "(" + heater->GetSetTemp() + ") " + state + "\n\r";
			}
		}
		else
		{
			str += "unknown command";
		}
	}
	
	return str;	

}

uint8_t Heaters::GetNum(Heater *heater)
{
	uint8_t i = 255;
	for (link t = head; t != NULL; t = t->next) {
		if(t->item == heater) return i;
		i++;
	}
	
	return i;
}

Heater *Heaters::GetByName(String name)
{
	for (link t = head; t != NULL; t = t->next) {
		if(t->item->GetName() == name)
		{
			return t->item;
		}
	}	
}

void Heaters::_check()
{
	for (link t = head; t != NULL; t = t->next) {
		t->item->check();
	}

}

void Heaters::check() //Sheduler every 1000ms
{
	Heaters::instance->_check();
}
