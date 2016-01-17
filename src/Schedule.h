#ifndef Schedule_h
#define Schedule_h

#include <Arduino.h>

#include <List.h>

class Scheduler
{
	unsigned long timer;
	void (*func)();
public:
	int delay;
	Scheduler(void (*func)(), int _delay, int first_delay = 1000);
	void run(boolean millis_over);
};

class Schedulers: public List<Scheduler*>
{
	int test;
	boolean millis_over;
	unsigned long pre_millis;
public:
	Schedulers();
	void add(void (*func)(), int delay, int first_delay = 1000);
	void check();
};


#endif