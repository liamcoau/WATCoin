#include <Wire.h>
#include <TwoWire.h>

TwoWire w;

void setup(){
  w = TwoWire(1);
  w.begin(1);
  w.onRequest(requestEvent);
  Serial.begin(9600);
}

void loop(){
  delay(100);
}

void requestEvent(){
  w.write("hello ");
}
