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

// Local variables
TwoWire bus;

void setup(){
  Serial.begin(9600);

  // Initialise to bus 1  
  bus = TwoWire(1);
  bus.begin();
}

void loop(){

  
  char key = keypad.getKey();

  if (key != NO_KEY){
    bus.beginTransmission(2); // Begin transmission to address #1 (master-atm)
    bus.write(key);
    bus.endTransmission();
    Serial.println("sent");
  }  
  delay(1);
}





