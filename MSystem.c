//gcc grovepi_digital_read.c grovepi.c -Wall

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "ultrasonic_intruder_detection.h"
#include "pulse.h"
#include "reset.h"

#define MODE1 1
#define MODE2 2
#define MODE3 3
#define MODE4 4


char *argv[] = {"python", "facedetect.py", NULL, "0", NULL};
char *argv2[]={"python", "facedetect2.py", NULL, "0", NULL};
char *argv3[]={"python", "facedetectvideo.py", NULL, "0", NULL};

char num[2];

int main(void) {
	bob: ;
	unsigned int MODE = 1;
	int status;
	int ultrasonicDetectIter = 0;
	ultrasonicCalibrate();
	while(1) {
		printf("In mode: %d\n", MODE);
		switch (MODE) {
			case MODE1: {
				int pid = fork();
				if(pid == 0) {//fork a child and run ultrasonic mode 1 code
					int ret = ultrasonicDetect();
					exit(ret);
				}
				if(waitpid(pid, &status, 0) == pid) {
					status = status >> 8; //8
					if((8 & status) == 8) {
						printf("ultrasonic 1 detected change\n");
					}
					if((4 & status) == 4) {
						printf("ultrasonic 2 detected change\n");
					}
					if((2 & status) == 2) {
						printf("ultrasonic 3 detected change\n");
					}
					if((1 & status) == 1) {
						printf("ultrasonic 4 detected change\n");
					}		
					MODE = MODE2;
					if(status == 0) {
						printf("no change detected\n");
						MODE = MODE1;
					}
				}
				break;
			}
			case MODE2: {
				ultrasonicDetectIter = 0;
				int i;
				for(i = 0; i < 5; i++) {
					printf("Num of ultrasonic detections: %d\n", ultrasonicDetectIter);
					int pid = fork();
					if(pid == 0) {//fork a child and run ultrasonic mode 1 code
						int ret = ultrasonicDetect();
						exit(ret);
					}
					if(waitpid(pid, &status, 0) == pid) {
						status = status >> 8;//8
						if((8 & status) == 8) {
							printf("ultrasonic 1 detected change\n");
							ultrasonicDetectIter++;
						}
						if((4 & status) == 4) {
							printf("ultrasonic 2 detected change\n");
							ultrasonicDetectIter++;
						}
						if((2 & status) == 2) {
							printf("ultrasonic 3 detected change\n");
							ultrasonicDetectIter++;
						}
						if((1 & status) == 1) {
							printf("ultrasonic 4 detected change\n");
							ultrasonicDetectIter++;
						}		
						if(status == 0) {
							printf("no change detected\n");
						}
						else {
							int cam = status >> 4;
							char strCam[1];
							sprintf(strCam, "%d", cam);
							argv[2] = strCam;
							argv[3] = "0";
							pid = fork();
                             
							if(pid == 0) {
								execvp("python", argv);
							}
							if(waitpid(pid, &status, 0) == pid) {
								status = status >> 8;
								if(status == 1) {
									printf("camera detected face\n");
									MODE = MODE3;
									break;
								}
							}
						} 
					}
				}
			
				printf("total ultrasonic detections: %d\n", ultrasonicDetectIter);
				if(ultrasonicDetectIter >= 3 || MODE == MODE3) {
					MODE = MODE3;
				}
				else {
					MODE = MODE1;
				}
				break;
			}
			case MODE3: {
				ultrasonicDetectIter = 0;
				int faceIter = 0;
				int i;
				for(i = 0; i < 8; i++) {
					printf("Num of ultrasonic detections: %d\n", ultrasonicDetectIter);
					printf("Num of face detections: %d\n", faceIter);
					int pid = fork();
					if(pid == 0) {//fork a child and run ultrasonic mode 1 code
						int ret = ultrasonicDetect();
						exit(ret);
					}
					if(waitpid(pid, &status, 0) == pid) {
						status = status >> 8;
						if((8 & status) == 8) {
							printf("ultrasonic 1 detected change\n");
							ultrasonicDetectIter++;
						}
						if((4 & status) == 4) {
							printf("ultrasonic 2 detected change\n");
							ultrasonicDetectIter++;
						}
						if((2 & status) == 2) {
							printf("ultrasonic 3 detected change\n");
							ultrasonicDetectIter++;
						}
						if((1 & status) == 1) {
							printf("ultrasonic 4 detected change\n");
							ultrasonicDetectIter++;
						}		
						if(status == 0) {
							printf("no change detected\n");
						}
						else {
							int cam = status >> 4;
							char strCam[1];
							sprintf(strCam, "%d", cam);
							printf("%s\n", strCam);
							argv2[2] = strCam;
							
							sprintf(num, "%d", faceIter);
							argv2[3] = num;
							pid = fork();
							if(pid == 0) {
								execvp("python", argv2);
							}
							if(waitpid(pid, &status, 0) == pid) {
								status = status >> 8;
								if(status == 1) {
									printf("camera detected face");
									faceIter++;	
								}
								if(faceIter > 1) {
									MODE = MODE4;
									break;
								}
							}
						} 
					}
				}
			
				printf("total ultrasonic detections: %d\n", ultrasonicDetectIter);
				if(ultrasonicDetectIter >= 3 || faceIter > 1) {
					MODE = MODE4;
				}
				else {
					MODE = MODE1;
				}
				break;
			}
			case MODE4: {
				int pid = fork();
				if(pid == 0) {
					argv3[2] =  num;
					execvp("python",argv3);
					
					exit(0);
				}
				blink(BUZZER_PORT, 10);				
				reset();
				kill(pid, SIGKILL);
				turnoff(BUZZER_PORT);
				
				goto bob;
				
				break;
			}
			default:
				break;
			
		}
	}
	
	return 0;
}
