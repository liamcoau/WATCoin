#define LED RED_LED
#define GLED GREEN_LED
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  pinMode(GLED, OUTPUT);
}

char name[256]={0};
uint16_t coins=0;
union uint16_to_bytes {
    uint16_t asUint;
    uint8_t asBytes[2];
};

//If it's a new user, will return 1. If it's an old user, will return 0;
uint8_t getName(char* cid){
  Serial.write(0x01); //Info request.
  Serial.write((uint8_t*)cid,4);
  
  while(Serial.available()<1);
  int x=Serial.read();
  Serial.write(x);
  if(x==-1){
    digitalWrite(LED,HIGH);
  }
  char resp=(char)x;
  if(resp=='X') digitalWrite(GLED,HIGH);
  
  if(resp=='X'){
    //Got the user. Read the data:
    while(Serial.available()<2);
    uint8_t upper=Serial.read();
    uint8_t lower=Serial.read();
    union uint16_to_bytes x;
    x.asBytes[1]=lower;
    x.asBytes[0]=upper;
    coins=x.asUint;
    return 0;
  }else if(resp=='Y'){
    return 1;
  }
}

void sendName(char*cid, char*questid){
  Serial.write(0x02);
  Serial.write((uint8_t*)cid,4);
  Serial.write((uint8_t)strlen(questid));
  Serial.write(questid);
}

void withdraw(char*cid, uint16_t amount){
  Serial.write(0x03);
  Serial.write((uint8_t*)cid,4);
  union uint16_to_bytes x;
  x.asUint=amount;
  Serial.write(x.asBytes[1]);
  Serial.write(x.asBytes[0]);
}

void loop()
{
  //This just works as a test of the communcations.
  
  delay(100);
  if(getName("abcd")){
    //new user
    sendName("abcd","aj3roth");
  }else{
    withdraw("abcd",5000);
  }

  delay(1000);
  if(getName("abcd")){
    //new user
    sendName("abcd","aj3roth");
  }else{
    withdraw("abcd",5000);
  }
  
//  withdraw("abcd",5000);

  while(1);
}
