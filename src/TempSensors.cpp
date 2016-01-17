#include <TempSensors.h>

TempSensors::TempSensors()
{
	oneWire = new OneWire(ONE_WIRE_BUS);
	sensors = new DallasTemperature(oneWire);
	sensors->begin();
}

void TempSensors::read_temp()
{
	Serial.println("test");
}

String TempSensors::list()
{
	DeviceAddress tempDeviceAddress;
	
	sensors->begin(); // search again
	int numberOfDevices = sensors->getDeviceCount();
	sensors->requestTemperatures();

	String str = "found: ";
	str += numberOfDevices;
	str += " ParasitePower: ";
	if (sensors->isParasitePowerMode()) str += "ON";
	else str += "OFF";
	str += "\n";
	
	for(int i=0;i<numberOfDevices; i++)
	{
		str += i+1;
		str += " ";
		
		str += "<unassigned>";
		
		str += " ";		
		
		// Search the wire for address
		if(sensors->getAddress(tempDeviceAddress, i))
		{
			for (uint8_t i = 0; i < 8; i++)
			{
				if (tempDeviceAddress[i] < 16) str += "0";
				str += String(tempDeviceAddress[i], HEX);
			}
			
			str += " ";
		}
		
		float temp = sensors->getTempC(tempDeviceAddress);
		str += temp;
		str += "\n";
	}
	
	
	
	return str;
}

