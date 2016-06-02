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
float thetaZ = 0;
int sensorPin = A4;
int SensorPinB = A5;
int writeXPin = 3;
int writeYPin = 5;
int timeSet = 0;
int writeZPin = 6;
int sensorMin = 286;        // minimum sensor value
int sensorMax = -286;           // maximum sensor value
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
    sensorValue = (int)gyro.data.z;
    // record the maximum sensor value
    if (sensorValue > sensorMax) {sensorMax = sensorValue;}
    // record the minimum sensor value
    if (sensorValue < sensorMin) {sensorMin = sensorValue;}
    delay(1);
  }
  Serial.println("calibration over");
  Serial.print(sensorMax);Serial.print(" max and min for noise ");Serial.println(sensorMin);
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
  Serial.print("Z: "); Serial.println((int)gyro.data.z); Serial.print(" ");
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
  // apply the calibration to the sensor reading
  sensorValue = map(sensorValue, 0, sensorMax, 0, 255);
  // in case the sensor value is outside the range seen during calibration
  sensorValue = constrain(sensorValue, 0, 255);
  // fade the LED using the calibrated value:
  //analogWrite(writeYPin, sensorValue);

  thetaZ = thetaZ + gyro.data.z*(millis()-timeSet);
  sensorValue = (int)gyro.data.z;
  timeSet = millis(); 
  sensorValue = abs(sensorValue);
  // apply the calibration to the sensor reading
  sensorValue = map(sensorValue, 0, sensorMax, 0, 255);
  // in case the sensor value is outside the range seen during calibration
  sensorValue = constrain(sensorValue, 0, 255);
  // fade the LED using the calibrated value:
  analogWrite(writeZPin, sensorValue);
  if(millis()%20 == 0){
    Serial.print("Theta value, time stamp: ");Serial.print(gyro.data.z);Serial.print(", ");Serial.println(millis());
  }
}
