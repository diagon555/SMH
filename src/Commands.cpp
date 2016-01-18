#include "SMH.h"
#include "Commands.h"

Command::Command(String cmd)
{
	command = cmd;
	command_remain = cmd;
	command_remain.trim();
}

String Command::Next()
{
	int idx = command_remain.indexOf(" ");
	String cmd;
	if(idx != -1) {
		cmd = command_remain.substring(0, idx);
		command_remain = command_remain.substring(idx+1);
	}
	else {
		cmd = command_remain;
		command_remain = "";
	}	
	
	command_remain.trim();
	
	return cmd;
}

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
				//Serial.print("do: ");
				//Serial.println(buff_serial);
				
				Command command(buff_serial);
				
				String cmd = command.Next();
				
				if(cmd == "help"){
					Serial.println("SMH 1.0.0");
					Serial.println("Command list:");
					Serial.println("tempsensors led ms help");
					Serial.println("for detiles type <comandname> help");
				} else if(cmd == "led") {
					String cmd_2 = command.Next();
					if(cmd_2 == "on") {
						digitalWrite(13, HIGH);
						Serial.println("ok");
					}
					if(cmd_2 == "off") {
						digitalWrite(13, LOW);
						Serial.println("ok");
					}
					if(cmd_2 == "help") {
						Serial.println("led on | off | help");
					}
				} else if(cmd == "tempsensors" or cmd == "tsens") {
					Serial.println(SMH.tempsensors->command(&command));
				} else if(cmd == "ms") {
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