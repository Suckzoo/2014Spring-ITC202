#include <SoftwareSerial.h>
#include <Brain.h>

SoftwareSerial softSerial(10, 11);
Brain brain(softSerial);
int emergency;
char buf[10];
void setup() {
    // Start the software serial.
    emergency=0;
    softSerial.begin(9600);
    // Start the hardware serial. 
    Serial.begin(9600);
    pinMode(A0,OUTPUT);
}
int isEmergencyCall(char buf[]) {
  int i;
  char ans[]="EMERGENCY";
  for(i=0;i<9;i++) {
    if(ans[i]!=buf[i]) return 0;
  }
  return 1;
}
void loop() {
  if(emergency==1) {
    analogWrite(A0,512);
  } else {
    if(Serial.available()>0) {
      int i;
      for(i=0;i<10;i++) {
        buf[i]=0;
      }
      int inputSize=0;
      while(Serial.available()>0) {
        char IncomingByte;
        IncomingByte = Serial.read();
        buf[inputSize]=IncomingByte;
        inputSize++;
      }
      Serial.print("buf : ");
      Serial.println(buf);
      if(isEmergencyCall(buf)) {
        Serial.print("call!");
        emergency=1;
      }
    }
    int xValue,yValue,zValue;
    xValue=analogRead(5);
    yValue=analogRead(4);
    zValue=analogRead(3);
    Serial.println("AB");
    Serial.print(xValue);
    Serial.print(",");
    Serial.print(yValue);
    Serial.print(",");
    Serial.println(zValue);
    Serial.println("AE");
    while(1) {
      if(brain.update()) {
        Serial.println("BEB");
        Serial.println(brain.readErrors());
        Serial.println("BEE");
        Serial.println("BVB");
        Serial.println(brain.readCSV());
        Serial.println("BVE");
        break;
      }
    }
  }
}
