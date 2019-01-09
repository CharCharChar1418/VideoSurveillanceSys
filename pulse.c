#include "pulse.h"

void pulse_ms(int port, int delay) {
	if(init()==-1) {
		exit(1);
	}

	//Set pin mode to output
	pinMode(port,1);

	digitalWrite(port,1);
	pi_sleep(delay);
	digitalWrite(port,0);
}

void blink(int port, int sec) {
	int i=sec*(1000/PERIOD);
	for (;i>0;i--) {
		pulse_ms(port,PERIOD);
		pi_sleep(PERIOD);
	}
}

void turnoff (int port) {
	pinMode(port,1);
	digitalWrite(port,0);
}
