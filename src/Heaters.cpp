#include "Heaters.h"
#include "SMH.h"

Heater::Heater(String name, Relay *relay, TempSensor *tempsensor, int Tset, int Tsup, boolean state)
	:iNamable(name)
{
	this->heat_current = false;
	this->relay = relay;
	this->tempsensor = tempsensor;
	this->Tset = Tset;
	this->Tsup = Tsup;
	this->heat_state = state;
}

int Heater::Serialize(int addr)
{
	EEPROM.update(addr++, SMH.relays->GetNum(relay));//relay
	EEPROM.update(addr++, SMH.tempsensors->GetNum(tempsensor));//tempsensor
	EEPROM.update(addr++, Tset);
	EEPROM.update(addr++, Tsup);
	EEPROM.update(addr++, heat_state);
	String name = GetName();
	for(int i=0; i < 8; i++)
	{
		char ch;
		if(i < name.length())
			ch = name[i];
		else
			ch = 0;
		
		EEPROM.update(addr++, ch);
	}
	
	return addr;
}

void Heater::check()
{
	if(tempsensor->Availiable())
	{
		float temp = tempsensor->GetTemp();
		
		if(Tsup != HEAT_OFF && temp-HYSTERESIS < Tsup)
		{
			set_state(true);
		}
		else if(heat_state && Tset != HEAT_OFF && temp-HYSTERESIS < Tset)
		{
			set_state(true);
		}
		else if(temp > Tset)
		{
			set_state(false);
		}
		else if((!heat_state || Tset == HEAT_OFF) && (Tsup == HEAT_OFF || temp > Tsup))
		{
			set_state(false);
		}
	}
	else
	{
		set_state(false);
	}
}

void Heater::set_state(boolean state)
{
	if(heat_current != state)
	{
		heat_current = state;
		if(state){
			relay->On();
		}
		else{
			relay->Off();
		}
	}
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
		addr = t->item->Serialize(addr);
	}

}

void Heaters::Deserialize()
{
	int addr = start_address;
	uint8_t count = EEPROM.read(addr++);
	if(count > MAX_COUNT_HEATERS) count = 0;
	for (uint8_t idx = 0; idx < count; idx++) {
		Relay *relay = SMH.relays->GetByNum(EEPROM.read(addr++));
		TempSensor *tempsensor = SMH.tempsensors->GetByNum(EEPROM.read(addr++));
		int Tset = EEPROM.read(addr++);
		int Tsup = EEPROM.read(addr++);
		int heat_state = EEPROM.read(addr++);
		
		
		char name[9];
		for(int i=0; i < 8; i++)
		{
			name[i] = EEPROM.read(addr++);
		}
		name[8] = 0;
		
		this->push(new Heater(String(name), relay, tempsensor, Tset, Tsup, heat_state));
	}
}

String Heaters::command(Command * command)
{
	String str;
	
	String cmd = command->Next();
	String cmd_2 = command->Next();
	
	if(cmd == "help") {
		return help();
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
		str += "OK";
		Serialize();
	} else if(cmd == "remove") {
		Heater *heater = GetByName(cmd_2);
		uint8_t idx = 0;
		if(heater)
		{
			idx = GetNum(heater);
		}
		else
		{
			idx = cmd_2.toInt() - 1;
		}
		
		if(idx < count())
		{
			remove(idx);
			str += "ok";
			Serialize();
		}
		else
		{
			str += "Error: invalid index";
		}
	} else if(cmd == "list" || cmd == "status" || cmd == "st") {
		int i = 1;
		for (link t = head; t != NULL; t = t->next) {
			String state;
			if(t->item->GetStatus())
				state = "on";
			else
				state = "off";
			
			String cur_state = "";
			if(t->item->GetCurrentStatus()) cur_state = "!";

			str += String(i) + ": " + t->item->GetName() + " " + cur_state + t->item->GetTemp() + "(" + t->item->GetSetTemp() + ") Tsup:" + t->item->GetTempSupport() + " " + state + "\n\r";
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
			} else if(cmd_2 == "set") {
				int temp;
				String cmd_3 = command->Next();
				if(cmd_3 == "tsup")
				{
					String cmd_4 = command->Next();
					temp = cmd_4.toInt();
					if(temp >= 0 && temp <= 10)
					{
						heater->SetTempSupport(temp);
						str += String("OK SET Tsup ") + temp;
					}
					else
					{
						str += "Error not valid set temp";
						return str;
					}
				}
				else
				{
					temp = cmd_3.toInt();
					if(temp >= 1 && temp <= 30)
					{
						heater->SetTemp(temp);
						str += String("OK SET ") + temp;
					}
					else
					{
						str += "Error not valid set temp";
						return str;
					}
				}
				Serialize();
			} else {
				String state;
				if(heater->GetStatus())
					state = "on";
				else
					state = "off";
				
				String cur_state = "";
				if(heater->GetCurrentStatus()) cur_state = "!";
				
				str += String("Heater ") + heater->GetName() + " " + cur_state + heater->GetTemp() + "(" + heater->GetSetTemp() + ") " + state + "\n\r";
			}
		}
		else
		{
			return help();
		}
	}
	
	return str;	

}

String Heaters::help()
{
	return "heater list | add <name> <relay_name> <tempsensor_name> | remove <name> | <name> (on|off|set <temp>|set tsup <temp>) \n\r";
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
