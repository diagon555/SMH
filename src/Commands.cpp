#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include "SMH.h"
#include "Commands.h"
#include "USART.h"

extern USART Serial;

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

void* PosixThreadCheckCommand(void* data)
{
    // Do some work here.
    while (1)
    {
        ((Commands *)data)->check_serial();
        usleep(10);
    }

    return NULL;
}

//Commands
Commands::Commands()
{
	serial_init = false;
	buff_serial.reserve(255);
// Create the thread using POSIX routines.
    pthread_attr_t  attr;
    pthread_t       posixThreadID;
    int             returnVal;

    returnVal = pthread_attr_init(&attr);
    assert(!returnVal);
    returnVal = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    assert(!returnVal);

    int     threadError = pthread_create(&posixThreadID, &attr, &PosixThreadCheckCommand, this);

    returnVal = pthread_attr_destroy(&attr);
    assert(!returnVal);
    if (threadError != 0)
    {
        Serial.println("Cannot start commands thread");
    }

}

void Commands::check()
{
	check_serial();
}

void Commands::check_serial()
{
	static bool pre_n_or_r = false;

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
        if(ch != 10 && ch != 13) pre_n_or_r = false;

        if(ch == 10 || ch == 13) { //New line
            if(pre_n_or_r) return;
            pre_n_or_r = true;

            buff_serial.trim();
            buff_serial.toLowerCase();

            Serial.println();
            //Serial.print("do: ");
            //Serial.println(buff_serial);

            Command command(buff_serial);

            String cmd = command.Next();

//            if(cmd == "relay") {
//                Serial.println(SMH.relays->command(&command));
//            } else if(cmd == "tempsensors" or cmd == "tsens") {
//                Serial.println(SMH.tempsensors->command(&command));
//            } else if(cmd == "heat" or cmd == "heater" or cmd == "heaters") {
//                Serial.println(SMH.heaters->command(&command));
//            } else if(cmd == "ms") {
//                Serial.println(millis());
//            } else {
            if(1)
            {
                Serial.println("SMH 1.0.0");
                Serial.println("Command list:");
                Serial.println("relay tempsensors heater ms help");
                Serial.println("for detiles type <comandname> help");
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
        else {
            buff_serial += ch;
            Serial.print(ch);
        }
    }
}