//from smoothing the setup routine runs once when you press reset:
int counter =0;
const int numReadings = 10;

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                  // the average

// from calibration These constants won't change:
const int sensorPin = A3;    // pin that the IR sensor is attached to
const int ledPin = 13;        // pin that the LED is attached to

// variables:
int sensorValue = 0;         // the sensor value
int sensorMin = 1023;        // minimum sensor value
int sensorMax = 0;           // maximum sensor value

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
   // while (!Serial) {
   // ; // wait for serial port to connect. Needed for native USB port only
  //}
  Serial.println("start");
  // turn on LED to signal the start of the calibration period:
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
  while (millis() < 3500) {
    //from calibration
    sensorValue = analogRead(sensorPin);
    // record the maximum sensor value
    if (sensorValue > sensorMax) {sensorMax = sensorValue;}
    // record the minimum sensor value
    if (sensorValue < sensorMin) {sensorMin = sensorValue;}
   
   //from smoothing
   // subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = analogRead(sensorPin);
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;
  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }
  // calculate the averages:
  average = total / numReadings;
  // send it to the computer as ASCII digits
  //Serial.println(average);
  delay(1);        // delay in between reads for stability
  }
  Serial.println("calibration over");
  Serial.print("this is your average");Serial.println(average);
  Serial.print(sensorMax);Serial.print("max and min for noise");Serial.println(sensorMin);
  // signal the end of the calibration period, turn off light
  digitalWrite(13, LOW);
  Serial.print(millis());Serial.println("milliseconds, program start");
  int sensorMax = 1.1*sensorMax; //up the max and min, as a factor of safety
  int sensorMin = 1.1*sensorMin;
}


void loop() {
  int sensorZValue = analogRead(A3); //read the input on analog pin 0:
  int zShift = sensorZValue - average; //shift it down
   if(sensorZValue<=sensorMax && sensorZValue>=sensorMin){
     delay(1);//if its noise just wait a second
   }
   else{
    Serial.println(zShift);//if its not noise, print the value read
   }
     //delay(1);        // delay in between reads for stability

  //from calibration, allosw the led to light up with the input values
    sensorValue = analogRead(sensorPin);
    // apply the calibration to the sensor reading
    sensorValue = map(sensorValue, sensorMin, sensorMax, 0, 255);
    // in case the sensor value is outside the range seen during calibration
    sensorValue = constrain(sensorValue, 0, 255);
    // fade the LED using the calibrated value:
    analogWrite(ledPin, sensorValue);
}

