#include <Servo.h> // Include servo library
Servo servoRight; // Declare left servo
void setup() // Built in initialization block
{
servoRight.attach(13); // Attach left signal to pin 13
servoRight.writeMicroseconds(1500); // 1.3 ms full speed clockwise
}
void loop() // Main loop auto-repeats
{ // Empty, nothing needs repeating
}

