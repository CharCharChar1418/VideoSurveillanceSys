#include "ultrasonic_intruder_detection.h"

// ************************ NON-FULL-AUTOMATIC ASSIGNMENT, DEPENDS ON .H FILE *********************************************
int ultrasonicPort[ULTRASONIC_CHANNEL] = {SENSOR_CHANNEL1,SENSOR_CHANNEL2,SENSOR_CHANNEL3,SENSOR_CHANNEL4};				// define the address of the port
// ************************ NON-FULL-AUTOMATIC ASSIGNMENT, DEPENDS ON .H FILE *********************************************

int val[ULTRASONIC_CHANNEL][SAMPLES];							// container to store samples for every channel
int default_val[ULTRASONIC_CHANNEL] = {0,0,0,0};		// define the number for every channel which corresponds to no detection
int mean_detection_val[ULTRASONIC_CHANNEL] = {0,0,0,0};			// define the mean detection value of every channel for samples


void insertionSort(int * data, int length){
    int temp,i,j;
    for (i = 1; i < length; i++) {
        for(j = i ; j > 0 ; j--){
        if(data[j] < data[j-1]){
            temp = data[j];
            data[j] = data[j-1];
            data[j-1] = temp;
            }
        }
    }
}

int ultrasonicDetect (void) {
	//Exit on failure to start communications with the GrovePi
	if(init()==-1){
		//exit(1);
		printf("sensor failure\n");
		return 0;
	}
	
	//Set pin mode to input
	pinMode(4,0);
	pinMode(3,0);
	pinMode(7,0);
	pinMode(8,0);
	
	// poll every ultrasonic channel and save the value to array "val"
	for (int i=0; i<SAMPLES; i++) {
		for (int j=0; j<ULTRASONIC_CHANNEL; j++) {
			val[j][i] = ultrasonicRead(ultrasonicPort[j]);
			pi_sleep(US_DELAY);
		}
	}
	
	// init the mean detection val
	for (int i = 0; i<ULTRASONIC_CHANNEL; i++) {
		mean_detection_val[i] = 0;
	}
	
	// compute the ultrasonic mean value
	for (int i=0; i<ULTRASONIC_CHANNEL; i++) {
		for (int j=0; j<SAMPLES; j++) {
			
			int diff1, diff2;
			// check if the current val is changing by more than NOISE_THRESHOLD at the next val
			if (val[i][j]-val[i][j+1] >= 0) {
					diff1 = val[i][j]-val[i][j+1];
				}
			else {
				diff1 = val[i][j+1]-val[i][j];
			}
			
			if (j<SAMPLES-2) {
				if (diff1 >= NOISE_THRESHOLD) { // at this point, a big change is detected. If the change remains in the next cycle, then it's a signal. If the change doesn't remain, then it's a noise.
					if (val[i][j] - val[i][j+2] >= 0) {
						diff2 = val[i][j] - val[i][j+2];
					}
					else {
						diff2 = val[i][j+2] - val[i][j];
					}
					
					if (diff2 <= NOISE_THRESHOLD) { // if diff2 <= NOISE_THRESHOLD, then the big change doesn't remain, so it was noise,,, in this case, remove the noise
						val[i][j+1] = val[i][j];
					}
				}
			}
			if (j == SAMPLES-2) {
				val[i][j+1] = val[i][j];
			}

			mean_detection_val[i] += val[i][j];		// compute total value of val
		}
	}


	printf("threshold\tdata\n");
	int output_val = 0;
	int largest_val = 0;
	int largest_addr = 0;
	for (int i = 0; i<ULTRASONIC_CHANNEL; i++) {
		mean_detection_val[i] /= SAMPLES;
		
		// print ultrasonic which detects intruder in 4 bits LSB, every bit for every ultrasonic
		printf("%i\t\t%i\t",default_val[i],mean_detection_val[i]);
		if (DETECTION_THRESHOLD <= (default_val[i] - mean_detection_val[i]) ) {

			// search for the largest change address
			if (largest_val < (default_val[i]-mean_detection_val[i]) ) {
				largest_val = default_val[i]-mean_detection_val[i];
				largest_addr=i;
				printf("largest addr : %d",largest_addr);
			}

			printf("INTRUDER DETECTED!!!");
			output_val += (1<<i);
		}
		printf("\n");
	}

	// use bit 5-6 as status bit for ultrasonic which got the highest change
	output_val += (largest_addr<<4);
	printf("out val : %d\n",output_val);
	printf("\n");
	return output_val;
}


void ultrasonicCalibrate (void) {
	int all_calibration[ULTRASONIC_CHANNEL][CALIBRATION_SAMPLE];			// define the detection value of every channel for samples
	
	// do ultrasonicDetect() for CALIBRATION_SAMPLE times, and then put the value into all_callibration array
	for (int i = 0; i<CALIBRATION_SAMPLE; i++) {
		ultrasonicDetect();
		for (int j = 0; j<ULTRASONIC_CHANNEL; j++) {
			all_calibration[j][i]=mean_detection_val[j];
		}
	}
	
	// sort the data in all_calibration based on its value
	for (int i = 0; i<ULTRASONIC_CHANNEL; i++) {
		insertionSort(all_calibration[i],CALIBRATION_SAMPLE);
	}
	
	// take the median data and put it into default_val
	for (int i = 0; i<ULTRASONIC_CHANNEL; i++) {
		default_val[i] = all_calibration[i][CALIBRATION_SAMPLE/2];
		printf("calibration val %i: %i\t",i,default_val[i]);
		printf("\n");
	}
	
}
