#include <Keypad.h>
#include <Wire.h>
#include <TwoWire.h>

/* ------------------------------------------------------------ */
/*				Keypad Variables		*/
//* ------------------------------------------------------------ */
// Define # of rows and cols on keypad
const byte ROWS = 4;
const byte COLS = 3;

// Map of keypad
char keys[ROWS][COLS] = {
  {
    '1','2','3'            }
  ,
  {
    '4','5','6'            }
  ,
  {
    '7','8','9'            }
  ,
  {
    '*','0','#'            }
};

// Pin layout (cables go from left to right, A0-A6)
byte rowPins[ROWS] = {
  35,34,33,32};
byte colPins[COLS] = {
  15,13,12};

// Initialize keypad object
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

TwoWire bus;


int type=0;

void setupI2C(){
  bus = TwoWire(1);
  bus.begin(9);
  bus.onReceive(receiveEvent);
  bus.onRequest(requestEvent);
}

void requestEvent(){
	if(type==0x01){
		//Keypad read
		char key;
  		while (key = keypad.getKey() && key == NO_KEY);
		bus.write(key);
	}else if(type==0x02){
		while(Serial.available()<4);
		bus.write(Serial.read());
		bus.write(Serial.read());
		bus.write(Serial.read());
		bus.write(Serial.read());
	}
}

void receiveEvent(int number){
	while(bus.available()<1);
	int type=bus.read();

}