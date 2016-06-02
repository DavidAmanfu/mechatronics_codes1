#include <Servo.h>
Servo servoLeft;
Servo servoRight;

const int numReadings = 10;
int readings[numReadings];      // the readings from the analog input
int ireadings[numReadings];     //readings from ir sensor
int readIndex = 0;              // the index of the current reading
int totalRight = 0;                  // the running total
int totalLeft =0;
int rightAverage = 0;                  // the average
int leftAverage = 0;


// from calibration These constants won't change:
const int angleSensor = A2; 
const int rightEye = A0;    // pin that the sensor is attached to
const int leftEye = A1;  //pin for ir sensor
const int still = 1500; //coding in the still position to vary things after that.
// variables:
int sensorRValue = 0;         // the sensor value
int sensorLValue = 0;
int rightMin = 1023;        // minimum sensor value
int rightMax = 0;           // maximum sensor value
int leftMin = 1023;        // minimum sensor value
int leftMax = 0;           // maximum sensor value

void setup() {
  // put your setup code here, to run once:
    // initialize serial communication at 9600 bits per second:
  Serial.begin(19200);
  Serial.println("start");
  // turn on LED to signal the start of the calibration period:
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  servoLeft.attach(13);                      // Attach left signal to P13 
  servoRight.attach(12);                     // Attach right signal to P12 
  digitalWrite(13, LOW); // signal the end of the calibration period
 
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
  
  Serial.print(millis());Serial.println("milliseconds, program start");
}

void loop() {
  // put your main code here, to run repeatedly:
  int angle = analogRead(angleSensor);
  Serial.println(angle);
}
