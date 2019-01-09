#ifndef PULSE_H
#define PULSE_H

#include "grovepi.h"

//#define LIGHT_PORT 5
#define BUZZER_PORT 6

#define PERIOD 100

void pulse_ms(int port, int delay);
void blink(int port, int sec);
void turnoff(int port);

#endif
