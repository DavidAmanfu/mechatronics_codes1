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
int dibsOnSecond = 0;       //have you read a value (counter)

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
  while (millis() < 3500) {
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
  ireadings[readIndex] = analogRead(irSensorPin);
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


void loop() {
  int sensorZValue = analogRead(A0); //read the input on analog pin 0 and 3:
  int irValue = analogRead(A3);
  int irShift = irValue - averageIR;
  //if IRvalue is under the threshold, its noise
  if(irValue<=520){
    Serial.println(0);//ir reads noise
    if(dibsOnSecond==3){
      Serial.print("line2: ");Serial.println(millis());
      dibsOnSecond=4;
    }
    if(dibsOnSecond==1){ //if you've gone through the code and now are reading noise, and you've already hit the first value
      Serial.println("line1: ");Serial.println(millis()); //print the millis
      dibsOnSecond = 2; //update the counter
    }  
  }
  else{
    //Serial.println(irShift);
    if(dibsOnSecond == 2){ //you're no longer reading noise (First=1) and you've hit the counter twice for time stamps
      Serial.print("second hit ");Serial.println(millis()); //print that out
      dibsOnSecond = 3; //update the counter
    }
    if(dibsOnSecond==0){
      Serial.print("first hit ");Serial.println(millis());
      dibsOnSecond = 1; //you've hit the first line
    }
    else{}
  }  //you're reading a value
  
   if(sensorZValue>300 && dibsOnSecond==4){
     Serial.print("table hit ");Serial.println(millis());
   }
     delay(1);        // delay in between reads for stability
  
    sensorValue = analogRead(irSensorPin);
    // apply the calibration to the sensor reading
    sensorValue = map(sensorValue, sensorMin, sensorMax, 0, 255);
    // in case the sensor value is outside the range seen during calibration
    sensorValue = constrain(sensorValue, 0, 255);
    // fade the LED using the calibrated value:
    analogWrite(ledPin, sensorValue);
}

