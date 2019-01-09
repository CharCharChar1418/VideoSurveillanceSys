#include "reset.h"

void reset(void) {
	int dval;
	//Exit on failure to start communications with the GrovePi
	if(init()==-1)
		exit(1);

	//Set pin mode to input
	pinMode(RST_BUTTON,0);
	while (dval != 1){
		dval=digitalRead(RST_BUTTON);
		//printf("dval: %d\n",dval);
	}
}
