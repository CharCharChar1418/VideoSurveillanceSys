#ifndef ULTRASONIC_INTRUDER_DETECTION_F_H
#define ULTRASONIC_INTRUDER_DETECTION_F_H

#include "grovepi.h"

#define SAMPLES 10				// the number of samples used in getting the mean value in ultrasonicDetect()
#define ULTRASONIC_CHANNEL 4	// the number of ultrasonic channel used
#define US_DELAY 50				// the amount of ultrasonic delay for every measurement
#define NOISE_THRESHOLD 100		// define the noise threshold in cm value. if there's only one data which is more than threshold, it's highly probable that it's noise
#define DETECTION_THRESHOLD 50	// define the changing value that will be treated as detection
#define CALIBRATION_SAMPLE 10	// define the amount of calibration iteration

#define SENSOR_CHANNEL1		4
#define SENSOR_CHANNEL2		3
#define SENSOR_CHANNEL3		7
#define SENSOR_CHANNEL4		8

int ultrasonicDetect (void);
void ultrasonicCalibrate (void);

#endif
