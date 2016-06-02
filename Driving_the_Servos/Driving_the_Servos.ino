#include <Servo.h>
Servo servoLeft;                             // Declare left servo signal
Servo servoRight;                            // Declare right servo signal


const int numReadings = 10;
int readings[numReadings];      // the readings from the analog input
int ireadings[numReadings];     //readings from ir sensor
int readIndex = 0;              // the index of the current reading
int totalRight = 0;                  // the running total
int totalLeft =0;
int rightAverage = 0;                  // the average
int leftAverage = 0;


// from calibration These constants won't change:
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
    // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial.println("start");
  // turn on LED to signal the start of the calibration period:
  pinMode(13, OUTPUT);
  servoLeft.attach(13);                      // Attach left signal to P13 
  servoRight.attach(12);                     // Attach right signal to P12 
  digitalWrite(13, HIGH);
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
    ireadings[thisReading] = 0;
  }
  while (millis() < 5000) {
    sensorRValue = analogRead(rightEye); //from calibration
    if (sensorRValue > rightMax) {rightMax = sensorRValue;}// record the maximum sensor value
    if (sensorRValue < rightMin) {rightMin = sensorRValue;}  // record the minimum sensor value
   
   
    sensorLValue = analogRead(leftEye);
    if (sensorLValue > leftMax) {leftMax = sensorLValue;}
    if (sensorLValue < leftMin) {leftMin = sensorLValue;}
  
   
   //from smoothing
  totalRight= totalRight- readings[readIndex];// subtract the last reading:
  readings[readIndex] = analogRead(rightEye); // read from the sensor:
  totalRight= totalRight+ readings[readIndex]; // add the reading to the total:
  
  totalLeft = totalLeft - ireadings[readIndex];
  ireadings[readIndex] = analogRead(leftEye);
  totalLeft = totalLeft + ireadings[readIndex];  
  
  readIndex = readIndex + 1; // advance to the next position in the array:
  if (readIndex >= numReadings) { // if we're at the end of the array...
    readIndex = 0; // ...wrap around to the beginning:
  }
  // calculate the averages:
  rightAverage = totalRight/ numReadings;
  leftAverage = totalLeft / numReadings;
  delay(1);        // delay in between reads for stability
  }

  Serial.println("calibration over");
  Serial.print("Right average");Serial.println(rightAverage);
  Serial.print("Left average");Serial.println(leftAverage);
  Serial.print(rightMax);Serial.print("max and min for right IR noise");Serial.println(rightMin);
  Serial.print(leftMax);Serial.print("max and min for left IR noise");Serial.println(leftMin);
  digitalWrite(13, LOW); // signal the end of the calibration period

  Serial.println("Servo Test");
  servoLeft.writeMicroseconds(1300);         // Pin 13 clockwise
  servoRight.writeMicroseconds(1700);        // Pin 12 clockwise
  delay(3000);                               // ..for 3 seconds 
  servoLeft.writeMicroseconds(1500);         // Pin 13 counterclockwise
  servoRight.writeMicroseconds(1500);        // Pin 12 counterclockwise
  delay(3000);                               // ..for 3 seconds 
  servoLeft.writeMicroseconds(1700);         // Pin 13 stay still
  servoRight.writeMicroseconds(1300);        // Pin 12 stay still
  delay(3000);
  servoLeft.writeMicroseconds(1500);         // Pin 13 counterclockwise
  servoRight.writeMicroseconds(1500);        // Pin 12 counterclockwise
  delay(3000);               
  //servoRight.write();
  
  digitalWrite(13, HIGH); 
  delay(2000);                               // ..for 3 seconds 
  digitalWrite(13, LOW); 
  delay(1000);                               // ..for 3 seconds 
  digitalWrite(13, HIGH);
  delay(2000);                               // ..for 3 seconds
  digitalWrite(13, LOW); 
  delay(1000);                               // ..for 3 seconds
  digitalWrite(13, HIGH);
  delay(3000);                               // ..for 3 seconds  
  digitalWrite(13, LOW); 
  
  Serial.print(millis());Serial.println("milliseconds, program start");
}

void loop() {
  int irRValue = analogRead(rightEye);
  int irLValue = analogRead(leftEye); 
  
  if(irRValue>=2*rightAverage){servoLeft.writeMicroseconds(1500);}//what values are you reading, what are you doing here
  else{servoLeft.writeMicroseconds(1300);}
  if(irLValue>=2*leftAverage){servoRight.writeMicroseconds(1500);}
  else{servoRight.writeMicroseconds(1700);}
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

}

