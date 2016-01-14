#include <SMH.h>


void setup() {
	Serial.begin(9600);
	
	SMH.set_printer(Serial);
	SMH.set_logfile_path("smh.log");
}

void loop() {
  // put your main code here, to run repeatedly:
	SMH.loop();
}