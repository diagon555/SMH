#include <Schedule.h>



Scheduler::Scheduler(void (*_func)(), int _delay, int first_delay)
{
	delay = _delay;
	func = _func;
	timer = millis() + first_delay;
}

void Scheduler::run(boolean millis_over)
{
	if(millis_over) timer = 0;
	
	if(millis() > timer + delay)
	{
		(*func)();
		timer = millis();
	}
}

Schedulers::Schedulers()
{
	millis_over = false;
	pre_millis = millis();	
}

void Schedulers::add(void (*func)(), int delay, int first_delay)
{
	this->push(new Scheduler(func, delay, first_delay));
}

void Schedulers::check()
{
	if(millis() < pre_millis) millis_over = true;
	for (link t = head; t != NULL; t = t->next) {
		t->item->run(millis_over);
	}
	millis_over = false;
	pre_millis = millis();
}



