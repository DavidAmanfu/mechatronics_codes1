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
Adafruit_L3GD20 gyro(GYRO_CS, GYRO_DO, GYRO_DI, GYRO_CLK);
#endif

int ZsensorValue = 0; //defining Z angular velocity,
int IRsensorValue = 0; //defining IR sensor
float setFreq = 0;
float thetaZ = 30; //to be set every time a new initial angle is used.
int sensorPin = A4;
int SensorPinB = A5;
int timeSet = 0;
int writeZPin = 6; //green

const int IRsensorPin = A3;  //pin for ir sensor 
const int ZsensorPin = A0;

const int ZnumReadings = 10;     // defined for calibration period
int Zreadings[ZnumReadings];     // defined for calibration period
int ZreadIndex = 0;              // defined for calibration period
float totalZ = 0;                  // defined for calibration period
int averageZ = 0;                // defined for calibration period
int ZsensorMin = 250;            // minimum sensor value
int ZsensorMax = -250;           // maximum sensor value

const int IRnumReadings = 10;     // defined for calibration period
int IRreadings[IRnumReadings];     //readings from ir sensor
int IRreadIndex = 0;              // the index of the current reading
int totalIR = 0;                  // defined for calibration period
int averageIR = 0;                // defined for calibration period
int IRsensorMin = 1023;        // minimum sensor value
int IRsensorMax = 0;           // maximum sensor value

void setup()
{
  Serial.begin(74880); // Try to initialise and warn if we couldn't detect the chip
  if (!gyro.begin(gyro.L3DS20_RANGE_250DPS)) // we want max readable to be 250 degree/sec
  {
    Serial.println("Oops ... unable to initialize the L3GD20. Check your wiring!");
    while (1);
  }
  Serial.println("start");
  pinMode(13, OUTPUT); // turn on LED to signal the start of the calibration period
  digitalWrite(13, HIGH);
  while (millis() < 5000) { //calibration period is 5 seconds
    gyro.read(); //read data from the gyro

    ZsensorValue = (int)gyro.data.z;
    if (ZsensorValue > ZsensorMax) {
      ZsensorMax = ZsensorValue; // record the maximum sensor value
    }
    if (ZsensorValue < ZsensorMin) {
      ZsensorMin = ZsensorValue; // record the minimum sensor value
    }
    delay(1);
    //Z smoothing
    totalZ = totalZ - gyro.data.z;
    Zreadings[ZreadIndex] = gyro.data.z;
    totalZ = totalZ + Zreadings[ZreadIndex];

    ZreadIndex = ZreadIndex + 1;
    if (ZreadIndex >= ZnumReadings) {ZreadIndex = 0;}// if we're at the end of the array, wrap around to the beginning
    averageZ = totalZ / ZnumReadings;  // calculate the averages:
    //Serial.println(averageZ);
  

  IRsensorValue = analogRead(IRsensorPin);
  if (IRsensorValue > IRsensorMax) {IRsensorMax = IRsensorValue;} //IR max min values
  if (IRsensorValue < IRsensorMin) {IRsensorMin = IRsensorValue;}
  
  //IR smoothing
  totalIR = totalIR - IRreadings[IRreadIndex];
  IRreadings[IRreadIndex] = analogRead(IRsensorPin);
  totalIR = totalIR + IRreadings[IRreadIndex];

  IRreadIndex = IRreadIndex + 1;
  if (IRreadIndex >= IRnumReadings) {IRreadIndex = 0;}
  averageIR = totalIR / IRnumReadings;   
  //Serial.println(averageIR);
}

Serial.println("calibration over");
Serial.print("this is your z offset"); Serial.println(averageZ);
Serial.println(ZsensorMax); Serial.print(" max and min for noise "); Serial.println(ZsensorMin);
Serial.print("this is your IR average"); Serial.println(averageIR);
Serial.println(IRsensorMax); Serial.print("max and min for IR noise"); Serial.println(IRsensorMin);
digitalWrite(13, LOW);   // signal the end of the calibration period
delay(5000); //enough time to set theta0 to the correct angle with some degree of accuracy
Serial.println(millis()); Serial.println(" milliseconds, program start");
timeSet = millis();
}

void loop()
{
  gyro.read();
  //Serial.print("Z: "); Serial.println((int)gyro.data.z); Serial.print(" ");
  delayMicroseconds(100);
  int  Zvalue = (int)gyro.data.z;
  int IRvalue = analogRead(A3);      //Using this to find time stamp of theta = 0
  if (IRvalue > IRsensorMax + 10) {
    Serial.print("theta = 0 at time t = "); Serial.println(millis()); //this is in milliseconds
    Serial.print("angular velocity is "); Serial.print(Zvalue); Serial.println(" dps"); 
  }
  if (IRvalue < IRsensorMin - 10) { 
    Serial.print("theta = 0 at time t = "); Serial.println(millis()); //this is in milliseconds
    Serial.print("angular velocity is "); Serial.print(Zvalue); Serial.println(" dps"); 
  }
  //
  thetaZ += gyro.data.z*0.001 *(millis() - timeSet); //riemann sum. We use this to find theta
  timeSet = millis();
  Serial.println(Zvalue);
  //the LED is disconnected, the following lines are unneccessary
  //Zvalue = abs(Zvalue);
  //Zvalue = map(Zvalue, 0, ZsensorMax, 0, 255);
  //Zvalue = constrain(Zvalue, 0, 255);
  //analogWrite(writeZPin, Zvalue);
  if (millis() % 200 < 5) { //to track whether the theta value is moderately accurate.
    Serial.print("Theta value, time stamp: "); Serial.print(thetaZ); Serial.print(", "); Serial.println(millis()); //do we wnat gyro data z or do we want thetaZ
  }
  if (thetaZ <0.5 && thetaZ > -0.5) {
    Serial.print("Theta value, Omega value, time stamp: ");Serial.print(thetaZ);Serial.print(", ");Serial.print((int)gyro.data.z);Serial.print(", ");Serial.println(millis());
  }
}
