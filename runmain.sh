#!/bin/bash

gcc -std=gnu99 MSystem.c reset.c ultrasonic_intruder_detection.c pulse.c grovepi.c -Wall -o mainsystem
./mainsystem
