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

char key;

void setup(){
  Serial.begin(9600);  
}

void loop(){

  key = keypad.getKey();

  if (key != NO_KEY){
    Serial.println(key);
  }  
}





