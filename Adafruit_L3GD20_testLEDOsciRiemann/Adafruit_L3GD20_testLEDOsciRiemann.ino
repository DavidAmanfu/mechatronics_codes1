/***************************************************
  This is an example for the Adafruit Triple-Axis Gyro sensor

  These sensors use I2C or SPI to communicate, 2 pins (I2C)
  or 4 pins (SPI) are required to interface.

 ****************************************************/

#include <Wire.h>
#include <Adafruit_L3GD20.h>

// Comment this next line to use SPI
#define USE_I2C

#ifdef USE_I2C
// The default constructor uses I2C
Adafruit_L3GD20 gyro;
#else
// To use SPI, you have to define the pins
#define GYRO_CS 4 // labeled CS
#define GYRO_DO 5 // labeled SA0
#define GYRO_DI 6  // labeled SDA
#define GYRO_CLK 7 // labeled SCL
Adafruit_L3GD20 gyro(GYRO_CS, GYRO_DO, GYRO_DI, GYRO_CLK);
#endif
int sensorValue = 0;
float setFreq = 0; 
float thetaZ = 0;
int sensorPin = A4;
int SensorPinB = A5;
int writeXPin = 3; //blue
int writeYPin = 5; //red
int timeSet = 0;
int writeZPin = 6; //green 
int sensorMin = 250;        // minimum sensor value
int sensorMax = -250;           // maximum sensor value
int irsensorPin = A0;

const int numReadings = 10;
int ireadings[numReadings];     //readings from ir sensor
int readIndex = 0;              // the index of the current reading
int totalIR =0;
int averageIR = 0;
const int irSensorPin = A3;  //pin for ir sensor CHANGE THIS AS YOU SEE FIT. IT MIGHT NOT BE A3. DOUBLE CHECK
int irsensorMin = 1023;        // minimum sensor value
int irsensorMax = 0;           // maximum sensor value

void setup()
{
  Serial.begin(9600);

  // Try to initialise and warn if we couldn't detect the chip
  if (!gyro.begin(gyro.L3DS20_RANGE_250DPS))
    //if (!gyro.begin(gyro.L3DS20_RANGE_500DPS))
    //if (!gyro.begin(gyro.L3DS20_RANGE_2000DPS))
  {
    Serial.println("Oops ... unable to initialize the L3GD20. Check your wiring!");
    while (1);
  }
  Serial.println("start");
  // turn on LED to signal the start of the calibration period:
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  while (millis() < 5000) {
    gyro.read();
    //from calibration

//this is necessary for the IR sensor
    //IR max min values
    sensorValue = analogRead(irSensorPin);
    if (sensorValue > irsensorMax) {irsensorMax = sensorValue;}
    if (sensorValue < irsensorMin) {irsensorMin = sensorValue;}
 //IR smoothing
  totalIR = totalIR - ireadings[readIndex];
  ireadings[readIndex] = analogRead(irsensorPin);
  totalIR = totalIR + ireadings[readIndex];  
  
  readIndex = readIndex + 1;
  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }
  // calculate the averages:
  averageIR = totalIR / numReadings;
  Serial.println(averageIR);
  }
  
  Serial.println("calibration over");
  Serial.print("this is your IR average");Serial.println(averageIR);
  Serial.print(sensorMax);Serial.print(" max and min for noise ");Serial.println(sensorMin);
  Serial.print(irsensorMax);Serial.print("max and min for IR noise");Serial.println(irsensorMin);
  // signal the end of the calibration period
  digitalWrite(13, LOW);
  Serial.print(millis()); Serial.println(" milliseconds, program start");
  timeSet = millis();
}

void loop()
{
  gyro.read();
  //Serial.print("X: "); Serial.print((int)gyro.data.x);   Serial.print(" ");
  //Serial.print("Y: "); Serial.print((int)gyro.data.y);   Serial.print(" ");
  //Serial.print("Z: "); Serial.println((int)gyro.data.z); Serial.print(" ");
  delayMicroseconds(100);

  sensorValue = (int)gyro.data.x;
  sensorValue = abs(sensorValue);
  // apply the calibration to the sensor reading
  sensorValue = map(sensorValue, 0, sensorMax, 0, 255);
  // in case the sensor value is outside the range seen during calibration
  sensorValue = constrain(sensorValue, 0, 255);
  // fade the LED using the calibrated value:
  //analogWrite(writeXPin, sensorValue);
  

  sensorValue = (int)gyro.data.y;
  sensorValue = abs(sensorValue);
  sensorValue = map(sensorValue, 0, sensorMax, 0, 255);
  sensorValue = constrain(sensorValue, 0, 255);
  //analogWrite(writeYPin, sensorValue);

  //this small portion is only necessary because we're solving the lab in two ways
  int irValue = analogRead(A3); //DOUBLE CHECK TO MAKE SURE THE SENSOR IS IN PIN A3
  int irShift = irValue - averageIR;
  if(abs(irShift) > 10){
    Serial.print("theta = 0 at time t = ");Serial.println(millis()); //this is in milliseconds
    }
  
  thetaZ += gyro.data.z*0.001*(millis()-timeSet);//riemann. if we know When the angle is  zero then we dont need an IR sensor.
  sensorValue = (int)gyro.data.z;
  timeSet = millis(); 
  setFreq = gyro.data.z/(10*sqrt(100-(thetaZ*thetaZ)));
  Serial.println(sensorValue);
  sensorValue = abs(sensorValue);
  sensorValue = map(sensorValue, 0, sensorMax, 0, 255);
  sensorValue = constrain(sensorValue, 0, 255);
  analogWrite(writeZPin, sensorValue);
  if(millis()%20 == 0){
    Serial.print("Theta value, time stamp: ");Serial.print(gyro.data.z);Serial.print(", ");Serial.println(millis());//do we wnat gyro data z or do we want thetaZ
  }
  if(millis()%1000 > 20){
    Serial.print("set frequency attempt: ");Serial.println(setFreq);
  }
//the IR sensor is coded in, and we must now validate the quality of the response given by our calculation otherwise.
}
