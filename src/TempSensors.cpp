#include "TempSensors.h"

TempSensor::TempSensor(const uint8_t* d_address, String _name)
	:iNamable(_name)
{
	for(int i = 0; i < 8; i++)
		deviceAddress[i] = d_address[i];
}

void TempSensor::_request()
{
	temp = 25;
}

const uint8_t* TempSensor::GetAddress()
{
	return deviceAddress;
}

boolean TempSensor::Availiable()
{
	return true;
}

float TempSensor::GetTemp()
{
	return temp;
}

//TempSensors

TempSensors* TempSensors::instance;

TempSensors::TempSensors(int pin)
{
	TempSensors::instance = this;
	
	oneWire = new OneWire(pin);
	sensors = new DallasTemperature(oneWire);
	sensors->begin();
}

boolean TempSensors::Add(const uint8_t* d_address, String name)
{
	this->push(new TempSensor(d_address, name));
}

uint8_t TempSensors::GetNum(TempSensor * sensor)
{
	uint8_t i = 0;
	for (link t = head; t != NULL; t = t->next) {
		if(t->item == sensor) return i;
		i++;
	}
	
	return i;
}

TempSensor *TempSensors::GetByName(String name)
{
	for (link t = head; t != NULL; t = t->next) {
		if(t->item->GetName() == name)
		{
			return t->item;
		}
	}
	
	return 0;
}

TempSensor *TempSensors::GetByAddress(const uint8_t* d_address)
{
	boolean match;
	for (link t = head; t != NULL; t = t->next) {
		const uint8_t *addr = t->item->GetAddress();
		
		match = true;
		for(int i=0; i<8; i++)
		{
			if(d_address[i] != addr[i])
			{
				match = false;
				break;
			}
		}
		
		if(match)
			return t->item;
	}
	
	return 0;
}

String TempSensors::command(Command * command)
{
	String str;
	
	String cmd = command->Next();
	String cmd_2 = command->Next();
	String cmd_3 = command->Next();
	
	if(cmd == "help") {
		str += "tempsensors list | assign <num> <name> | remove <name> or <num>\n\r";
	} else if(cmd == "list") {
		str += list();
	} else if(cmd == "assign") {
		if(cmd_3.length() == 0)
		{
			str += "Error: name not exists";
			return str;
		}
		
		unsigned int num = cmd_2.toInt() - 1;
		DeviceAddress tempDeviceAddress;
		if(sensors->getAddress(tempDeviceAddress, num))
		{
			if(GetByAddress(tempDeviceAddress))
			{
				str += "Error: this sensor alredy assigned, use remove first";
				return str;
			}
			
			if(GetByName(cmd_3))
			{
				str += "Error: this name exists";
				return str;
			}
			
			if(Add(tempDeviceAddress, cmd_3))
			{
				str += "ok ";
				for (uint8_t i = 0; i < 8; i++)
				{
					if (tempDeviceAddress[i] < 16) str += "0";
					str += String(tempDeviceAddress[i], HEX);
				}
				str += " assigned to ";
				str += cmd_3;
			}
			else
			{
				str += "Error";
			}
		}
		else
		{
			str += "Error: invalid sensor number";
		}
	} else if(cmd == "remove") {
		TempSensor *sensor = GetByName(cmd_2);
		uint8_t idx = 0;
		if(sensor)
		{
			idx = GetNum(sensor);
		}
		else
		{
			idx = cmd_2.toInt() - 1;
		}
		
		if(idx < count())
		{
			remove(idx);
			str += "ok";
		}
		else
		{
			str += "Error: invalid index";
		}
	} else {
		str += "unknown command";
	}
	
	return str;
}

String TempSensors::list()
{
	sensors->begin(); // search again
	int numberOfDevices = sensors->getDeviceCount();
	sensors->requestTemperatures();

	String str = "found: ";
	str += numberOfDevices;
	str += " ParasitePower: ";
	if (sensors->isParasitePowerMode()) str += "ON";
	else str += "OFF";
	str += "\n\r";
	
	str += "Storage:\n\r";
	
	int i = 1;
	for (link t = head; t != NULL; t = t->next) {
		str += String(i++) + ": " + t->item->GetName() + " ";
		
		const uint8_t * sensorAddress = t->item->GetAddress();
		for (uint8_t i = 0; i < 8; i++)
		{
			if (sensorAddress[i] < 16) str += "0";
			str += String(sensorAddress[i], HEX);
		}
		
		if(t->item->Availiable())
		{
			str += String(" ") + t->item->GetTemp();
		}
		else
		{
			str += " unaval";
		}
		
		str += "\n\r";
	}
	
	DeviceAddress tempDeviceAddress;
	str += "Wire scan:\n\r";
	for(int i=0;i<numberOfDevices; i++)
	{
		str += i+1;
		str += " ";
		
		
		
		// Search the wire for address
		if(sensors->getAddress(tempDeviceAddress, i))
		{
			TempSensor *sensor = GetByAddress(tempDeviceAddress);
			if(sensor)
			{
				str += sensor->GetName() + " ";
			}
			else
			{
				str += "<unassigned> ";		
			}
			
			for (uint8_t i = 0; i < 8; i++)
			{
				if (tempDeviceAddress[i] < 16) str += "0";
				str += String(tempDeviceAddress[i], HEX);
			}
			
			str += " ";
		}
		else
		{
			str += "sensor error";
		}
		
		float temp = sensors->getTempC(tempDeviceAddress);
		str += temp;
		str += "\n\r";
	}
	
	
	
	return str;
}

void TempSensors::_check()
{
	for (link t = head; t != NULL; t = t->next) {
		t->item->_request();
	}
}

void TempSensors::check()
{
	TempSensors::instance->_check();
}
