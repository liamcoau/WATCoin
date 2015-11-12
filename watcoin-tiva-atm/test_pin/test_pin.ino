
#define MAX_N 65

void setup()
{
  Serial.begin(9600);
    for(int i = 1; i < MAX_N; i++)
    pinMode(i, INPUT);
}

void loop()
{
  for(int i = 1; i < MAX_N; i++)
    if(i != 17 && i != 10 && i != 61 && digitalRead(i))
      Serial.println(i);
   
   delay(5);
}
