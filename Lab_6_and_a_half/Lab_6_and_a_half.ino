#include <Servo.h>
#include <Wire.h>
#include <Adafruit_L3GD20.h>
#define USE_I2C // We are using I2C wiring
#ifdef USE_I2C
Adafruit_L3GD20 gyro;
#else // To use SPI, you have to define the pins
#define GYRO_CS 4 // labeled CS
#define GYRO_DO 5 // labeled SA0
#define GYRO_DI 6  // labeled SDA
#define GYRO_CLK 7 // labeled SCL
//Adafruit_L3GD20 gyro(GYRO_CS, GYRO_DO, GYRO_DI, GYRO_CLK);
#endif
Servo servoLeft;                             // Declare left servo signal
Servo servoRight;                            // Declare right servo signal


const int numReadings = 10;
int readings[numReadings];      // the readings from the analog input
int ireadings[numReadings];     //readings from ir sensor
int areadings[numReadings];
int zreadings[numReadings];     // defined for calibration period
int readIndex = 0;              // the index of the current reading
int totalRight = 0;                  // the running total
int totalLeft =0;
int totalAngle = 0;
float totalZ = 0;                  // defined for calibration period
int rightAverage = 0;                  // the average
int leftAverage = 0;
int zeroAngle = 0;
int averageZ = 0;                // defined for calibration period

// from calibration These constants won't change:
const int angleSensor = A2; 
const int rightEye = A0;    // pin that the sensor is attached to
const int leftEye = A1;  //pin for ir sensor
const int still = 1500; //coding in the still position to vary things after that.
const int maxAngle = 982;
const int minAngle = 39;
// variables:
int sensorRValue = 0;         // the sensor value
int sensorLValue = 0;
int ZsensorValue = 0; //defining Z angular velocity,
int sensorAngleValue = 0;
int rightMin = 1023;        // minimum sensor value
int rightMax = 0;           // maximum sensor value
int leftMin = 1023;        // minimum sensor value
int leftMax = 0;           // maximum sensor value
int ZsensorMin = 250;            // minimum sensor value
int ZsensorMax = -250;           // maximum sensor value

float setFreq = 0;
int timeSet = 0;
int angleSet = 0;
int velocitySet = 0;

void setup() {
    // initialize serial communication at 9600 bits per second:
  Serial.begin(38400);
//  if (!gyro.begin(gyro.L3DS20_RANGE_250DPS)) // we want max readable to be 250 degree/sec
//  {
//    Serial.println("Oops ... unable to initialize the L3GD20. Check your wiring!");
//    while (1);
//  }
  Serial.println("start");
  // turn on LED to signal the start of the calibration period:
  pinMode(13, OUTPUT);
  servoLeft.attach(13);                      // Attach left signal to P13 
  servoRight.attach(12);                     // Attach right signal to P12 
  digitalWrite(13, HIGH);
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
    ireadings[thisReading] = 0;
    areadings[thisReading] = 0;
    zreadings[thisReading] = 0;
  }
  while (millis() < 5000) {
    //    gyro.read(); //read data from the gyro

//    sensorRValue = analogRead(rightEye); //from calibration
//    if (sensorRValue > rightMax) {rightMax = sensorRValue;}// record the maximum sensor value
//    if (sensorRValue < rightMin) {rightMin = sensorRValue;}  // record the minimum sensor value
   
   
//    sensorLValue = analogRead(leftEye);
//    if (sensorLValue > leftMax) {leftMax = sensorLValue;}
//    if (sensorLValue < leftMin) {leftMin = sensorLValue;}
  
//    ZsensorValue = (int)gyro.data.z;
//    if (ZsensorValue > ZsensorMax) {ZsensorMax = ZsensorValue;} 
//    if (ZsensorValue < ZsensorMin) {ZsensorMin = ZsensorValue;}
    
   //from smoothing
//  totalRight= totalRight- readings[readIndex];// subtract the last reading:
//  readings[readIndex] = analogRead(rightEye); // read from the sensor:
//  totalRight= totalRight+ readings[readIndex]; // add the reading to the total:
 
//  totalLeft = totalLeft - ireadings[readIndex];
//  ireadings[readIndex] = analogRead(leftEye);
//  totalLeft = totalLeft + ireadings[readIndex];  

  totalAngle = totalAngle - areadings[readIndex];
  areadings[readIndex] = analogRead(angleSensor);
  totalAngle = totalAngle + areadings[readIndex];  

//  totalZ = totalZ - gyro.data.z;
//  zreadings[readIndex] = gyro.data.z;
//  totalZ = totalZ + zreadings[readIndex];
  
  readIndex = readIndex + 1; // advance to the next position in the array:
  if (readIndex >= numReadings) {readIndex = 0;} // if we're at the end of the array...wrap around to the beginning
  // calculate the averages:
//  rightAverage = totalRight/ numReadings;
//  leftAverage = totalLeft / numReadings;
  zeroAngle = totalAngle/numReadings;
//  averageZ = totalZ / numReadings;
  delay(1);        // delay in between reads for stability
  }

  Serial.println("calibration over");
  Serial.print("this is your z offset"); Serial.println(averageZ);
  Serial.println(ZsensorMax); Serial.print(" max and min for noise "); Serial.println(ZsensorMin);
  Serial.print("Right average");Serial.println(rightAverage);
  Serial.print("Left average");Serial.println(leftAverage);
  Serial.print(rightMax);Serial.print("max and min for right IR noise");Serial.println(rightMin);
  Serial.print(leftMax);Serial.print("max and min for left IR noise");Serial.println(leftMin);
  Serial.print("Zero Angle Analog reading: "); Serial.println(zeroAngle);
  digitalWrite(13, LOW); // signal the end of the calibration period
//
//  Serial.println("Servo Test");
//  servoLeft.writeMicroseconds(1300);         // Pin 13 clockwise
//  servoRight.writeMicroseconds(1700);        // Pin 12 clockwise
//  delay(3000);                               // ..for 3 seconds 
//  servoLeft.writeMicroseconds(1500);         // Pin 13 counterclockwise
//  servoRight.writeMicroseconds(1500);        // Pin 12 counterclockwise
//  delay(3000);                               // ..for 3 seconds 
//  servoLeft.writeMicroseconds(1700);         // Pin 13 stay still
//  servoRight.writeMicroseconds(1300);        // Pin 12 stay still
//  delay(3000);
//  servoLeft.writeMicroseconds(1500);         // Pin 13 counterclockwise
//  servoRight.writeMicroseconds(1500);        // Pin 12 counterclockwise
//  delay(3000);               
  //servoRight.write();
  
  digitalWrite(13, HIGH); 
  delay(1000);                               // ..for 3 seconds 
  digitalWrite(13, LOW); 
  delay(1500);                               // ..for 3 seconds 
  digitalWrite(13, HIGH);
  delay(2000);                               // ..for 3 seconds
  digitalWrite(13, LOW); 
  delay(1500);                               // ..for 3 seconds
  digitalWrite(13, HIGH);
  delay(3000);                               // ..for 3 seconds  
  digitalWrite(13, LOW); 
  
  Serial.print(millis());Serial.println(" milliseconds, program start");
  timeSet = millis();
}

void loop() {
  //gyro.read();
  delayMicroseconds(100);  
  //int  Zvalue = (int)gyro.data.z;
  //int irRValue = analogRead(rightEye);
  //int irLValue = analogRead(leftEye); 
  int angle = analogRead(angleSensor);
  Serial.println(angle);
  int velocity = (angle-angleSet)/(millis()-timeSet);
  int acceleration = (velocity-velocitySet)/(millis()-timeSet);
  timeSet = millis();
  angleSet = angle;
  velocitySet = velocity;

  //movement
  //servoLeft.writeMicroseconds(write according to the linear region/1300);
  //servoRight.writeMicroseconds(write according to the linear region/1300);
//982 drive backwards; 39 drive forwards.
}


  //ANALOG TO ANGLE MAPPING
//  trueAngle = analogRead(angleSensor);
//  trueAngle = map(trueAngle, 39, 982, -90, 90);
//  trueAngle = constrain(trueAngle, -90, 90);


  
 
//  if(irRValue>=2*rightAverage){servoLeft.writeMicroseconds(1500);}//what values are you reading, what are you doing here
//  else{servoLeft.writeMicroseconds(1300);}
//  if(irLValue>=2*leftAverage){servoRight.writeMicroseconds(1500);}
//  else{servoRight.writeMicroseconds(1700);}
//int irRShift = irRValue - rightAverage;
//int irLShift = irLValue - rightAverage;

  //Right LED mapping
  //sensorRValue = analogRead(rightEye);
  // apply the calibration to the sensor reading
  //sensorRValue = map(sensorRValue, sensorMin, sensorMax, 0, 200);
  // in case the sensor value is outside the range seen during calibration
  //sensorRValue = constrain(sensorRValue, 0, 200);
  // fade the LED using the calibrated value:
  //analogWrite(rightLED, sensorRValue);

  //left LED mapping
  //sensorLValue = analogRead(lefttEye);
  //sensorValue = map(sensorLValue, sensorMin, sensorMax, 0, 200);
  //sensorValue = constrain(sensorLValue, 0, 200);
  //analogWrite(leftLED, sensorLValue);}
  //codewith IR1 is the other sketch you'll need eventually.

