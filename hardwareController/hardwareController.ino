#include <SoftwareSerial.h>
#include <Brain.h>

SoftwareSerial softSerial(10, 11);
Brain brain(softSerial);
int emergencyFlag, errorGeneratorFlag, steadyState;
int x0,y0,z0;
int samples=0;
void accelCalibration();
void measureAcceleration();
void readEEG();
float variance[8], vThreshold[8], eegQueue[8][20];
int queuePointer;
float data[8],squaredMean[8],mean[8];
int eegErrorScore,accelScore;
float normScore;
void setup() {
  // Start the software serial.
  softSerial.begin(9600);
  // Start the hardware serial. 
  Serial.begin(9600);
  pinMode(A0,OUTPUT);
  emergencyFlag=0;
  errorGeneratorFlag=0;
  steadyState=0;
  samples=0;
  queuePointer=0;
  //={,,,,60820,60820,85190,85190}
  vThreshold[0]=(float)168636;
  vThreshold[1]=(float)1278959;
  vThreshold[2]=(float)17879965;
  vThreshold[3]=(float)17879965;
  vThreshold[4]=(float)60820;
  vThreshold[5]=(float)60820;
  vThreshold[6]=(float)85190;
  vThreshold[7]=(float)85190;
  delay(500);
  while(!Serial.available()) {
    1+1;
    //Gwiyomi;
  }
  if(Serial.available()) {
    Serial.read();
    analogWrite(A0,512);
      delay(200);
      analogWrite(A0,0);
    Serial.write('C');
      accelCalibration();
      analogWrite(A0,512);
      delay(400);
      analogWrite(A0,0);
   }
}
void loop() {
  if(emergencyFlag==1) {
    analogWrite(A0,512);
  } else {
    while(Serial.available() && errorGeneratorFlag==0) {
      if(Serial.read()=='G') {
        errorGeneratorFlag = 1;
        Serial.write("G");
        analogWrite(A0,512);
        delay(300);
        analogWrite(A0,0);
        analogWrite(A0,512);
        delay(300);
        analogWrite(A0,0);
        analogWrite(A0,512);
        delay(300); 
        analogWrite(A0,0);
      }
    }
    samples++;
    readEEG();
    measureAcceleration();
    if((errorGeneratorFlag==1/*|| eegErrorScore>=6*/) && accelScore>=15) {
      emergencyFlag=1;
      Serial.print('E');
    } /*else if(eegErrorScore>=4 && accelScore>=6) {
      analogWrite(A0,512);
      delay(50);
      analogWrite(A0,0);
      analogWrite(A0,512);
      delay(50);
      analogWrite(A0,0);
      analogWrite(A0,512);
      delay(50);
      analogWrite(A0,0);
      analogWrite(A0,512);
      delay(50);
      analogWrite(A0,0);
      Serial.print('W');
    }*/
  }
}
void accelCalibration() {
  int i;
  for(i=0;i<50;i++) {
    x0+=analogRead(5);
    y0+=analogRead(4);
    z0+=analogRead(3);
    delay(100);
  }
  x0=x0/50;
  y0=y0/50;
  z0=z0/50;
}
void readEEG() {
  eegErrorScore=0;
  while(1) {
    if(brain.update()) {
      data[0]=(float)brain.readDelta();
      data[1]=(float)brain.readTheta();
      data[2]=(float)brain.readLowAlpha();
      data[3]=(float)brain.readHighAlpha();
      data[4]=(float)brain.readLowBeta();
      data[5]=(float)brain.readHighBeta();
      data[6]=(float)brain.readLowGamma();
      data[7]=(float)brain.readMidGamma();
      break;
    }
  }
  int i;
  for(i=0;i<8;i++) {
    if(samples==20) {
      mean[i]=(mean[i]*samples-eegQueue[i][queuePointer]+data[i])/samples;
      squaredMean[i]=(squaredMean[i]*samples-eegQueue[i][queuePointer]*eegQueue[i][queuePointer]+data[i]*data[i])/samples;
      variance[i]=squaredMean[i]-mean[i]*mean[i];
      eegQueue[i][queuePointer]=data[i];
    } else {
      mean[i]=(mean[i]*(samples-1)+data[i])/samples;
      squaredMean[i]=(squaredMean[i]*(samples-1)+data[i]*data[i])/samples;
      variance[i]=squaredMean[i]-mean[i]*mean[i];
    }
  }
  if(samples==20) {
    for(i=0;i<8;i++) {
      if(variance[i]>=vThreshold[i]) eegErrorScore++;
    }
    samples--;
    queuePointer++;
    if(queuePointer==20) queuePointer=0;
  }
}

void measureAcceleration() {
  int X[5],Y[5],Z[5];
  int xValue,yValue,zValue;
  int i,j;
  for(i=0;i<5;i++) {
    X[i]=analogRead(5);
    Y[i]=analogRead(4);
    Z[i]=analogRead(3);
    delay(20);
  }
  for(i=0;i<4;i++) {
    for(j=0;j<4-i;j++) {
      if(X[j]>X[j+1]) {int temp=X[j]; X[j]=X[j+1]; X[j+1]=temp;}
      if(Y[j]>Y[j+1]) {int temp=Y[j]; Y[j]=Y[j+1]; Y[j+1]=temp;}
      if(Z[j]>Z[j+1]) {int temp=Z[j]; Z[j]=Z[j+1]; Z[j+1]=temp;}
    }
  }
  xValue=X[2];
  yValue=Y[2];
  zValue=Z[2];
  xValue=xValue-x0;
  yValue=yValue-y0;
  zValue=zValue-z0;
  if(yValue>15 && (xValue*xValue+yValue*yValue+zValue*zValue)>=943) {
    steadyState = 1;
  }
  if(xValue*xValue+yValue*yValue+zValue*zValue>=1200 && steadyState == 1) {
    steadyState = 2;
    accelScore++;
  } else accelScore=0;
}
