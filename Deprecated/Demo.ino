#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <Servo.h>

// Servo pin setup
Servo servo1;
Servo servo2; 
int servo1Pin = 5;
int servo2Pin = 6;


void setup() {
  // Set Pin Modes
  pinMode(servo1Pin, OUTPUT);    // PWM Signal servo 1
  pinMode(servo2Pin, OUTPUT);    // PWM Signal servo 2
  servo1.attach(servo1Pin);
  servo1.write(0);
  servo2.attach(servo2Pin);
  servo2.write(0);
}


void loop() {
  servo1.write(0);
  servo2.write(0);
  delay(2000);
  servo1.write(180);
  servo2.write(180);
  delay(2000);
}
