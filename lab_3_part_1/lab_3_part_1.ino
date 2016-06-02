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
const int rightLED = 12;        // pin that the LED is attached to
const int leftLED = 11;
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
  pinMode(leftLED, OUTPUT); //turn the left and right LEDs to output mode
  pinMode(rightLED, OUTPUT);
  digitalWrite(13, HIGH);
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
    ireadings[thisReading] = 0;
  }
  while (millis() < 5000) {
    //from calibration
    sensorRValue = analogRead(rightEye);
    // record the maximum sensor value
    if (sensorRValue > rightMax) {rightMax = sensorRValue;}
    // record the minimum sensor value
    if (sensorRValue < rightMin) {rightMin = sensorRValue;}
   
   
    sensorLValue = analogRead(leftEye);
    if (sensorLValue > leftMax) {leftMax = sensorLValue;}
    if (sensorLValue < leftMin) {leftMin = sensorLValue;}
  
   
   //from smoothing
   // subtract the last reading:
  totalRight= totalRight- readings[readIndex];
  // read from the sensor:
  readings[readIndex] = analogRead(rightEye);
  // add the reading to the total:
  totalRight= totalRight+ readings[readIndex];
  // advance to the next position in the array:

  //IR smoothing
  totalLeft = totalLeft - ireadings[readIndex];
  ireadings[readIndex] = analogRead(leftEye);
  totalLeft = totalLeft + ireadings[readIndex];  
  
  readIndex = readIndex + 1;
  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }
  // calculate the averages:
  rightAverage = totalRight/ numReadings;
  leftAverage = totalLeft / numReadings;
  // send it to the computer as ASCII digits
  //Serial.println(rightAverage);
  //Serial.println(leftAverage);
  delay(1);        // delay in between reads for stability
  }
  Serial.println("calibration over");
  Serial.print("Right average");Serial.println(rightAverage);
  Serial.print("Left average");Serial.println(leftAverage);
  Serial.print(rightMax);Serial.print("max and min for right IR noise");Serial.println(rightMin);
  Serial.print(leftMax);Serial.print("max and min for left IR noise");Serial.println(leftMin);
  // signal the end of the calibration period
  digitalWrite(13, LOW);
  Serial.print(millis());Serial.println("milliseconds, program start");
}

void loop() {
  int irRValue = analogRead(rightEye);
  int irLValue = analogRead(leftEye); 
  
  if(irRValue>=2*rightMax){digitalWrite(leftLED, HIGH);}//what values are you reading, what are you doing here
  else{digitalWrite(leftLED,LOW);}
  if(irLValue>=2*leftMax){digitalWrite(rightLED, HIGH);}
  else{digitalWrite(rightLED,LOW);}
}

