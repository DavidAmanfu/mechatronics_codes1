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
int ireadings[numReadings];     //readings from ir sensor
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int totalIR =0;
int average = 0;                  // the average
int averageIR = 0;


// from calibration These constants won't change:
const int sensorPin = A0;    // pin that the sensor is attached to
const int irSensorPin = A3;  //pin for ir sensor
const int ledPin = 13;        // pin that the LED is attached to

// variables:
int sensorValue = 0;         // the sensor value
int sensorMin = 1023;        // minimum sensor value
int sensorMax = 0;           // maximum sensor value
int irsensorMin = 1023;        // minimum sensor value
int irsensorMax = 0;           // maximum sensor value
int dibsOnFirst = 0;        //hit value

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
    ireadings[thisReading] = 0;
  }
  while (millis() < 7500) {
    //from calibration
    sensorValue = analogRead(sensorPin);
    // record the maximum sensor value
    if (sensorValue > sensorMax) {sensorMax = sensorValue;}
    // record the minimum sensor value
    if (sensorValue < sensorMin) {sensorMin = sensorValue;}
   
    //ir calibration
    sensorValue = analogRead(irSensorPin);
    // record the maximum sensor value
    if (sensorValue > irsensorMax) {irsensorMax = sensorValue;}
    // record the minimum sensor value
    if (sensorValue < irsensorMin) {irsensorMin = sensorValue;}
  
   
   //from smoothing
   // subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = analogRead(sensorPin);
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:

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
  average = total / numReadings;
  averageIR = totalIR / numReadings;
  // send it to the computer as ASCII digits
  //Serial.println(average);
  Serial.println(averageIR);
  delay(1);        // delay in between reads for stability
  }
  Serial.println("calibration over");
  Serial.print("this is your average");Serial.println(average);
  Serial.print("this is your IR average");Serial.println(averageIR);
  Serial.print(sensorMax);Serial.print("max and min for noise");Serial.println(sensorMin);
  Serial.print(irsensorMax);Serial.print("max and min for IR noise");Serial.println(irsensorMin);
  // signal the end of the calibration period
  digitalWrite(13, LOW);
  Serial.print(millis());Serial.println("milliseconds, program start");
}

// the loop routine runs over and over again forever:
void loop() {
  int sensorZValue = analogRead(A0); //read the input on analog pin 0:
  //int zShift = sensorZValue - average; //shift it down.
  int irValue = analogRead(A3);
  int irShift = irValue - averageIR;
  //if sensorZvalue is between max and min, print zero, else print zshift
  if(irValue<=irsensorMax && irValue>=irsensorMin){
    Serial.println(0);int dibsOnFirst=0;
  }
  else{
    //should be an if statement on the print
    Serial.print("first hit ");Serial.println(millis());
    int dibsOnFirst = dibsOnFirst + 1;
    int dibsOnSecond=1;
  }
  
  if(irValue<=irsensorMax && irValue>=irsensorMin){
    Serial.println(0); 
    if(dibsOnSecond==1){
      Serial.print("line1:");Serial.println(millis());
      }
    int dibsOnFirst=0;
    }
  else{
    if(dibsOnFirst==0 &&dibsOnSecond == 1){
      Serial.print("second hit");Serial.println(millis());
      int dibsOnFirst++;dibsOnSecond=2;
    }
  }
    
  if(sensorZValue<=sensorMax && sensorZValue>=sensorMin){
     if(irValue<=irsensorMax && irValue>=irsensorMin){
        if(dibsOnSecond==2){
          Serial.print("line2:");Serial.println(millis());
        }
    int dibsOnFirst=0;
    }
    //Serial.println(0);}
  else{ // print out the value you read:
    if(dibsOnFirst==0 && dibsOnSecond=2){Serial.print("table hit");Serial.println(millis());}
    //Serial.println(zShift);
    int dibsOnFirst=3;
    }
  }
  //delay(10) delay in between reads for stability moved to the end of the loop
  int sensorYValue = analogRead(A1);
  //Serial.print("\n Y-direction Sensor Reading: ");
  //Serial.println(sensorYValue);
  //delay(100);
  int sensorXValue = analogRead(A2);
  //Serial.print("\n X-direction Sensor Reading: ");
  //Serial.println(sensorXValue);
  //delay(100);
  counter++;
   
    //from calibration:
    // read the sensor:
  sensorValue = analogRead(irsensorPin);
  // apply the calibration to the sensor reading
  sensorValue = map(sensorValue, sensorMin, sensorMax, 0, 255);
  // in case the sensor value is outside the range seen during calibration
  sensorValue = constrain(sensorValue, 0, 255);
  // fade the LED using the calibrated value:
  analogWrite(ledPin, sensorValue);
  delay(10);        // delay in between reads for stability
}

