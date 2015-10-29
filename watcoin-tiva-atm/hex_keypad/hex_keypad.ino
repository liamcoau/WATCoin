#include <Wire.h>
#include <Keypad.h>

/* ------------------------------------------------------------ */
/*				Keypad Variables		*/
//* ------------------------------------------------------------ */
// Define # of rows and cols on keypad
const byte ROWS = 4;
const byte COLS = 3;

// Map of keypad
char keys[ROWS][COLS] = {
  {
    '1','2','3'        }
  ,
  {
    '4','5','6'        }
  ,
  {
    '7','8','9'        }
  ,
  {
    '*','0','#'        }
};

// Pin layout (cables go from left to right, A0-A6)
byte rowPins[ROWS] = {
  A,A6,A5,A3};
byte colPins[COLS] = {
  A2,A0,A4};

// Initialize keypad object
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup(){
  Wire.begin(2);
}

void loop(){
  Wire.write("hello");
  key = keypad.getKey();

  if (key != NO_KEY){
    Serial.println(key);
  }  
}

