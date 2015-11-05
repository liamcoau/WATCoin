#include <Wire.h>
#include <TwoWire.h>

TwoWire w;
void setup()
{
  w = TwoWire(1);
  w.begin();
  Serial.begin(9600);
}

void loop()
{
  w.requestFrom(1, 6);
  while(w.available())
  {
    char c = w.read();
    Serial.print(c);
  }
  delay(500);
}
