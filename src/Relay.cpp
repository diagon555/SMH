#include "Relay.h"

Relay::Relay(String name, uint8_t pin, uint8_t shift_register, uint8_t on_status, uint8_t status)
	:iNamable(name)
{
	this->shift_register = 0;
	this->on_status = on_status;
	if(!this->shift_register) // check if not shift register
	{
		pinMode(pin, OUTPUT);
		digitalWrite(pin, !on_status);
		this->status = 0;
		this->pin = pin;
	}
}

void Relay::set_status(uint8_t status)
{
	if(!shift_register)
	{
		if(this->status != status)
		{
			if(status) digitalWrite(pin, on_status);
			if(!status) digitalWrite(pin, !on_status);
			
			this->status = status;
		}
	}
}

void Relay::On()
{
	set_status(1);
}

void Relay::Off()
{
	set_status(0);
}



//Relays

Relays::Relays()
	:iSerializable(1+((1+1+1+1+8)*MAX_COUNT_RELAY))
{
	;
}

void Relays::Serialize()
{
	int addr = start_address;
	EEPROM.update(addr++, count());
	for (link t = head; t != NULL; t = t->next) {
		EEPROM.update(addr++, t->item->GetPin());
		EEPROM.update(addr++, t->item->GetShiftReg());
		EEPROM.update(addr++, t->item->GetOnStatus());
		EEPROM.update(addr++, t->item->GetStatus());
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

void Relays::Deserialize()
{
	int addr = start_address;
	uint8_t count = EEPROM.read(addr++);
	if(count > MAX_COUNT_RELAY) count = 0;
	for (uint8_t idx = 0; idx < count; idx++) {
		
		uint8_t pin = EEPROM.read(addr++);
		uint8_t shift_reg = EEPROM.read(addr++);
		uint8_t on_state = EEPROM.read(addr++);
		int status = EEPROM.read(addr++);
	
		char name[9];
		for(int i=0; i < 8; i++)
		{
			name[i] = EEPROM.read(addr++);
		}
		name[8] = 0;
		
		this->push(new Relay(String(name), pin, shift_reg, on_state, status));
	}
}

String Relays::command(Command *command)
{
	String str;
	
	String cmd = command->Next();
	String cmd_2 = command->Next();
	String cmd_3 = command->Next();
	
	if(cmd == "help") {
		return help();
	} else if(cmd == "add") {
		if(cmd_2 != "shiftreg")
		{
			Add(new Relay(cmd_2, cmd_3.toInt()));
			str += String("Relay ") + cmd_2 + " was added";
			Serialize();
		}	
	} else if(cmd == "remove") {
		Relay *relay = GetByName(cmd_2);
		uint8_t idx = 0;
		if(relay)
		{
			idx = GetNum(relay);
		}
		else
		{
			idx = cmd_2.toInt() - 1;
		}
		
		if(idx < count())
		{
			remove(idx);
			str += String("OK Relay ") + idx + " Removed";
			Serialize();
		}
		else
		{
			str += "Error: invalid index";
		}
	} else if(cmd == "list" || cmd == "status" || cmd == "st") {
		int i = 1;
		for (link t = head; t != NULL; t = t->next) {
			str += String(i) + ": " + t->item->GetName() + " pin" + t->item->GetPin() + " status:" + t->item->GetStatus() + "\n\r";
			i++;
		}
	} else {
		Relay *relay = GetByName(cmd);
		if(relay)
		{
			if(cmd_2 == "on"){
				relay->On();
				str += String("Relay ") + relay->GetName() + " ON";
			} else if(cmd_2 == "off") {
				relay->Off();
				str += String("Relay ") + relay->GetName() + " OFF";
			} else {
				str += String("Relay ") + relay->GetName() + " pin:" + relay->GetPin() + " status:" + relay->GetStatus() + "\n\r";
			}
		}
		else
		{
			return help();
		}
	}
	
	return str;	
}

uint8_t Relays::GetNum(Relay *relay)
{
	uint8_t i = 0;
	for (link t = head; t != NULL; t = t->next) {
		if(t->item == relay) return i;
		i++;
	}
	
	return 255;
}

Relay *Relays::GetByNum(uint8_t num)
{
	if(num >= count()) return NULL;
	
	
	for (link t = head; t != NULL; t = t->next) {
		if(!num)
		{
			return t->item;
		}
		num--;
	}
	
	return 0;
}

Relay *Relays::GetByName(String name)
{
	for (link t = head; t != NULL; t = t->next) {
		if(t->item->GetName() == name)
		{
			return t->item;
		}
	}
	
	return NULL;
}

String Relays::help()
{
	return "relay list | add <name> <pin> ?<st_on=1> | add shiftreg <name> <pin> ?<st_on=1> | remove <name> or <num> | <name> (?on|off)\n\r";
}

