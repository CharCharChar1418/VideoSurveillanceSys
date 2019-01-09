//GrovePi Example for using the digital read command
#include "grovepi.h"
//gcc grovepi_digital_read.c grovepi.c -Wall
int main(void)
{		
	int dval;
	
	//Exit on failure to start communications with the GrovePi
	if(init()==-1)
		exit(1);
	
	//Set pin mode to input
	pinMode(3,0);
	while(1)
	{
		dval=ultrasonicRead(3);
		printf("UltraSonic read %d\n", dval);
		//Sleep for 50ms
		pi_sleep(50);
	}
   	return 1;
}
