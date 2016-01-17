#include "Commands.h"

Commands::Commands()
{
	serial_init = false;
	buff_serial.reserve(255);
}

void Commands::check()
{
	check_serial();
}

void Commands::check_serial()
{
	if(Serial)
	{
		if(!serial_init)
		{
			Serial.println("Welcome to SMH");
			Serial.println();
			Serial.print("#");
			serial_init = true;
		}
		
		if(Serial.available())
		{
			char ch = Serial.read();
			if(ch == 13) {
				buff_serial.trim();
				buff_serial.toLowerCase();
				
				Serial.println();
				Serial.print("do: ");
				Serial.println(buff_serial);
				
				String bm = buff_serial;
				
				while(bm.length())
				{
					bm.trim();
					int idx = bm.indexOf(" ");
					String cmd;
					if(idx != -1) {
						cmd = bm.substring(0, idx);
						bm = bm.substring(idx+1);
					}
					else {
						cmd = bm;
						bm = "";
					}
					
					//Serial.print(String("<")+cmd+ ">|");
				}
				//Serial.println();
				
				if(buff_serial == "help"){
					Serial.println("led on, led off");
				} else if(buff_serial == "led state") {
					Serial.println(String("last: "));
				} else if(buff_serial == "led on") {
					digitalWrite(13, HIGH);
				} else if(buff_serial == "led off") {
					digitalWrite(13, LOW);
				} else if(buff_serial == "ms") {
					Serial.println(millis());
				} else {
					Serial.println("error command");
				}
				
				buff_serial = "";
				Serial.print("#");
			}
			else if(ch == 127) { //backspace
				if(buff_serial.length() > 0)
				{
					buff_serial.remove(buff_serial.length()-1);
					Serial.print(ch);
				}
			}
			else if(ch > 255) {
				;//nothing
			}
			else {
				buff_serial += ch;
				Serial.print(ch);
			}
		}
	}

}