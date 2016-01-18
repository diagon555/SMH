#ifndef COMMANDS_h
#define COMMANDS_h

#include <Arduino.h>

class Command
{
	String command;
	String command_remain;
public:
	Command(String command);
	String Next();
};

class Commands
{
	boolean serial_init;
	String buff_serial;
public:
	Commands();
	void check();
	void check_serial();
};

#endif