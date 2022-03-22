#include <asf.h>
#include "samd21.h"
#include "samd21e18a.h"

//Bit manipulation functions for shift register
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))

//QTPY pin definitions
#define serialData PORT_PA10;  //Shift register DS pin, QTPY MO pin
#define shiftClk PORT_PA11;    //Shift register SH_CP pin, QTPY SCK pin
#define latchClk PORT_PA03;    //Shift register ST_CP pin, QTPY A1 pin
unsigned char shiftData;       //Data to be loaded into shift register

void ledControl(int soc, int sys, int acc, int gyro, int mag);

int main (void)
{
	system_init();
	
	REG_PORT_DIRSET0 = 0x00000C08; //Sets MO, SCK, A1 as outputs

	ledControl(100,3,3,3,3); 
	
	while(1);
}


//Function takes SoC and sensor calibration status to control indicator lights on device
void ledControl(int soc, int sys, int acc, int gyro, int mag){
	REG_PORT_OUTCLR0 = latchClk; //Take latch pin low to prevent LEDs turning on

	//4 LSBs of shiftData are set with sensor calibration status - a status value of 3 means successful calibration
	if(acc==3) {bitSet(shiftData,0);} else {bitClear(shiftData,0);}
	if(gyro==3) {bitSet(shiftData,1);} else {bitClear(shiftData,1);}
	if(mag==3) {bitSet(shiftData,2);} else {bitClear(shiftData,2);}
	if((acc==3)&&(gyro==3)&&(mag==3)&&(sys==3)) {bitSet(shiftData,3);} else {bitClear(shiftData,3);}

	//4 MSBS are set with state of charge of battery
	if(soc >= 75){bitSet(shiftData, 4); bitSet(shiftData, 5); bitSet(shiftData, 6); bitSet(shiftData, 7);}
  	else if((soc < 75) && (soc >= 50)){bitClear(shiftData, 4); bitSet(shiftData, 5); bitSet(shiftData, 6); bitSet(shiftData, 7);}
  	else if((soc < 50) && (soc >= 25)){bitClear(shiftData, 4); bitClear(shiftData, 5); bitSet(shiftData, 6); bitSet(shiftData, 7);}
  	else if(soc < 25){bitClear(shiftData, 4); bitClear(shiftData, 5); bitClear(shiftData, 6); bitSet(shiftData, 7);}

	//Load shiftData value into shift register 1 bit at a time MSB first
	uint8_t i;
	for(i=0;i<8;i++){
		if((shiftData & 128) != 0){
			REG_PORT_OUTSET0 = serialData;
		}
		else{
			REG_PORT_OUTCLR0 = serialData;
		}
		shiftData <<= 1;

		//Pulse clock pin
		REG_PORT_OUTSET0 = shiftClk;
		REG_PORT_OUTCLR0 = shiftClk;
	}

	//Set latch pin to light LEDs
	REG_PORT_OUTSET0 = latchClk;
}
