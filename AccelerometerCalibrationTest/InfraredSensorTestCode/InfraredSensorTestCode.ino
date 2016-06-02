/*AnalogReadSerial: Reads analog input (pin 0), prints result to the serial monitor.
  Graphical representation is available using serial plotter (Tools > Serial Plotter menu)
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.
*/
//heres something else: Analog INput, calibration and smoothing
//from smoothing the setup routine runs once when you press reset:
int counter =0;
const int numReadings = 10;

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

int inputPin = A3;
// from calibration These constants won't change:
const int sensorPin = A3;    // pin that the sensor is attached to
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
  while (millis() < 7500) {
    //from calibration
    sensorValue = analogRead(inputPin);
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
  // calculate the average:
  average = total / numReadings;
  // send it to the computer as ASCII digits
  Serial.println(average);
  delay(1);        // delay in between reads for stability
  }
  Serial.println("calibration over");
  Serial.println("this is your average");
  Serial.println(average);
  Serial.println("max and min for noise");
  Serial.println(sensorMax);
  Serial.println(sensorMin);
  // signal the end of the calibration period
  digitalWrite(13, LOW);
}

// the loop routine runs over and over again forever:
void loop() {
  int irSensor = analogRead(A3);
  Serial.println(irSensor);
  sensorValue = analogRead(sensorPin);
  // apply the calibration to the sensor reading
  sensorValue = map(sensorValue, sensorMin, sensorMax, 0, 255);
  // in case the sensor value is outside the range seen during calibration
  sensorValue = constrain(sensorValue, 0, 255);
  // fade the LED using the calibrated value:
  analogWrite(ledPin, sensorValue);
}

