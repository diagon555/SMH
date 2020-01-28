#ifndef COMMANDS_h
#define COMMANDS_h

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
    bool serial_init;
	String buff_serial;
public:
	Commands();
	void check();
	void check_serial();
};

#endif