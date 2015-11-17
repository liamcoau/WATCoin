#include <Wire.h>
#include <TwoWire.h>
TwoWire bus;
void setupI2C(){
  bus = TwoWire(1);
  bus.begin();
}

char getKeypadChar(){
	bus.beginTransmission(9);
	bus.write(0x01); //We want a keypad byte.
	bus.endTransmission();

	bus.requestFrom(9,1);
	while(bus.available()<1);
	return (char)bus.read(); 
}

char watcard[5];
void getWatcard(){
	bus.beginTransmission(9);
	bus.write(0x02); //We want a keypad byte.
	bus.endTransmission();
	watcard[4]='\0';
	bus.requestFrom(9,4);
	while(bus.available()<4);
	watcard[0]=bus.read();
	watcard[1]=bus.read();
	watcard[2]=bus.read();
	watcard[3]=bus.read();
}