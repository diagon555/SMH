/*
  SMH.cpp
  
  Ivan Korostelev
*/

#include "SMH.h"
#include "TempSensors.h"

#include <String.h>



SMHClass::SMHClass()
{
	schedulers.add(&TempSensors::read_temp, 1000, 5000);
	schedulers.add(&TempSensors::read_temp1, 2000);
			
	buff.reserve(255);
	started = false;
	pinMode(13, OUTPUT);
}

void SMHClass::set_logfile_path(String path)
{
	logfile_path = path;
}


void SMHClass::set_printer(Print & prn)
{
	printer = &prn;
}



void SMHClass::loop(void)
{
	schedulers.check();
	//read command
	//printer->println("Test message");

	if(Serial)
	{
		if(!started)
		{
			printer->println("Welcome to SMH");
			printer->println();
			printer->print("#");
			started = true;
		}
	if(Serial.available())
	{
		char ch = Serial.read();
		if(ch == 13) {
			buff.trim();
			buff.toLowerCase();
			
			printer->println();
			printer->print("do: ");
			printer->println(buff);
			
			String bm = buff;
			
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
				
				//printer->print(String("<")+cmd+ ">|");
			}
			//printer->println();
			
			if(buff == "help"){
				printer->println("led on, led off");
			} else if(buff == "led state") {
				printer->println(String("last: "));
			} else if(buff == "led on") {
				digitalWrite(13, HIGH);
			} else if(buff == "led off") {
				digitalWrite(13, LOW);
			} else if(buff == "ms") {
				printer->println(millis());
			} else {
				printer->println("error command");
			}
			
			buff = "";
			printer->print("#");
		}
		else if(ch == 127) { //backspace
			if(buff.length() > 0)
			{
				buff.remove(buff.length()-1);
				printer->print(ch);
			}
		}
		else if(ch > 255) {
			;//nothing
		}
		else {
			buff += ch;
			printer->print(ch);
		}
	}
	}
}

SMHClass SMH;
