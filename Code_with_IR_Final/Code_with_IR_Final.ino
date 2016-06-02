
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
float thetaZ = 0; //used for method 2, finding theta 
int sensorPin = A4;
int SensorPinB = A5;
int timeSet = 0;
int writeZPin = 6; //green

const int IRsensorPin = A3;  //pin for ir sensor

const int ZnumReadings = 10;     // defined for calibration period
int Zreadings[ZnumReadings];     //latest 10 readings from Z sensor
int ZreadIndex = 0;              // the index of the initial reading
int totalZ = 0;                  // defined for calibration period
int averageZ = 0;                // defined for calibration period
int ZsensorMin = 250;            // minimum value initially set to maximum readable value
int ZsensorMax = -250;           // maximum value intially set to minimum readable value

const int IRnumReadings = 10;     // defined for calibration period
int IRreadings[IRnumReadings];     //latest 10 readings from ir sensor
int IRreadIndex = 0;              // the index of the initial reading
int totalIR = 0;                  // defined for calibration period
int averageIR = 0;                // defined for calibration period
int IRsensorMin = 1023;        // minimum value initially set to maximum readable value
int IRsensorMax = 0;           // maximum value intially set to minimum readable value

void setup()
{
  Serial.begin(57600); // Try to initialise and warn if we couldn't detect the chip
  if (!gyro.begin(gyro.L3DS20_RANGE_250DPS)) // we want max readable to be 250 degree/sec
  {Serial.println("Oops ... unable to initialize the L3GD20. Check your wiring!");
    while (1);}
  Serial.println("start calibration"); //lets user know that calibration process is starting
  pinMode(13, OUTPUT); // turn on LED to signal the start of the calibration period
  digitalWrite(13, HIGH);
 
  while (millis() < 5000) { //calibration period is 5 seconds
    gyro.read(); //read data from the gyro
    
    ZsensorValue = (int)gyro.data.z; //Zsensor value is the nearest integer angular velocity in Z
    if (ZsensorValue > ZsensorMax) {ZsensorMax = ZsensorValue;} // record the maximum sensor value
    if (ZsensorValue < ZsensorMin) {ZsensorMin = ZsensorValue;} // record the minimum sensor value
    delay(1);

    //keeps a running total of last 10 values
    totalZ = totalZ - (int)gyro.data.z; //subtracts oldest of the 10 values from total
    Zreadings[ZreadIndex] = (int)gyro.data.z; //updates with newest value 
    totalZ = totalZ + Zreadings[ZreadIndex]; // updates the total

    ZreadIndex = ZreadIndex + 1;        //moves to the new oldest of the 10 values
    if (ZreadIndex >= ZnumReadings) {ZreadIndex = 0;} 
    // if we're at the end of the array, wrap around to the beginning
    averageZ = totalZ / ZnumReadings;  // calculates the average value read:

  IRsensorValue = analogRead(IRsensorPin);
  if (IRsensorValue > IRsensorMax) {IRsensorMax = IRsensorValue;} //IR max min values
  if (IRsensorValue < IRsensorMin) {IRsensorMin = IRsensorValue;}
  
  //IR smoothing
  totalIR = totalIR - IRreadings[IRreadIndex];
  IRreadings[IRreadIndex] = analogRead(IRsensorPin);
  totalIR = totalIR + IRreadings[IRreadIndex];

  IRreadIndex = IRreadIndex + 1;
  if (IRreadIndex >= IRnumReadings) { // if we're at the end of the array, wrap around to the beginning
    IRreadIndex = 0;
  }
  averageIR = totalIR / IRnumReadings;   // calculate the averages:
}

Serial.println("calibration over");
Serial.print("this is your z offset"); Serial.println(averageZ);
Serial.print(" min and max for noise "); Serial.println(ZsensorMin); Serial.println(ZsensorMax); 
Serial.print("min and max for IR noise"); Serial.println(IRsensorMin); Serial.println(IRsensorMax);
Serial.println(" milliseconds, program start"); Serial.println(millis());
digitalWrite(13, LOW);   // signal the end of the calibration period
timeSet = millis();
}

void loop()
{
  gyro.read(); //take data from the gyro
  delayMicroseconds(100); // 0.1 ms delay between readings
  int  Zvalue = (int)gyro.data.z; // defined for purpose of Reimann sum, this is the velocity data
  int IRvalue = analogRead(A3);  //Using this to find time stamp of theta = 0

  //method 1: use IR sensor to print time of theta = 0
  //method 2: Use maximum angular velocity to find frequency
 // if (IRvalue > IRsensorMax + 10) { // Only have both because I don't remember which one I actually need
 //   Serial.print("theta = 0 at time t = "); Serial.println(millis()); //this is in milliseconds
 //   Serial.print("angular velocity is "); Serial.print(Zvalue); Serial.println(" dps"); 
 // }
 // if (IRvalue < IRsensorMin - 10) { // Only have both because I don't remember which one I actually need
 //   Serial.print("theta = 0 at time t = "); Serial.println(millis()); //this is in milliseconds
 //   Serial.print("angular velocity is "); Serial.print(Zvalue); Serial.println(" dps"); 
 // }
  //method 3: use Reiman sum to print time of theta = 0
  thetaZ +=  Zvalue * 0.001 * (millis() - timeSet); //riemann sum. We use this to find theta
  if ((thetaZ < 2) && (thetaZ > 2)){
    Serial.print("theta ="); Serial.println(thetaZ); 
    Serial.print("time t = "); Serial.println(millis());
    Serial.print("angular velocity is "); Serial.print(Zvalue); Serial.println(" dps");
  }
timeSet = millis();
}
