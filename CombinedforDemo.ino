#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <SoftwareSerial.h>
#include <SerialCommand.h>
#include <Servo.h>

SerialCommand sCmd;

// Servo and pump pin setup
Servo servo1;
Servo servo2; 
int servo1Pin = 5;
int servo2Pin = 3;
int out1 = 0;
int out2 = 0;

// Setting up the packets for sending and receiving
// const int UDP_PACKET_SIZE = 1;
// char sendBuffer[UDP_PACKET_SIZE];

void setup() {

  // Set up serial connection
  Serial.begin(9600);
  while (!Serial);

  pinMode(servo1Pin, OUTPUT);    // PWM Signal servo 1
  pinMode(servo2Pin, OUTPUT);    // PWM Signal servo 2

  // digitalWrite(pumpPin, HIGH);
  servo1.attach(servo1Pin);
  servo1.write(0);
  servo2.attach(servo2Pin);
  servo2.write(0);

  sCmd.addCommand("PING1", ping1Handler);
  sCmd.addCommand("PING2", ping2Handler);
  
}

// More robust function. Can take in values after just Ping and handle them
// void pingHandler () {
//   char *arg;
//   arg = sCmd.next();
//   if (arg != NULL) {
//     handle_package(arg);
//     Serial.print("sent"); Serial.println(arg);
//   } else {
//     Serial.println("PONG");
//   }
// }

void ping1Handler() {
  out1 = 170;
  Serial.println("PONG1");
}

void ping2Handler() {
  out2 = 180;
  Serial.println("PONG2");
}


void loop() {
  if (Serial.available() > 0) {
    Serial.println("collision detected");
    sCmd.readSerial();
  }
  servo1.write(out1);
  servo2.write(out2);
  out1 = 0;
  out2 = 0;
  delay(1000);
  // Serial.println("waiting...");
}


// Helper function for sending
// void handle_package(char *string) {
//   if (strcmp(*string,"1") == 0) {
//       out1 = 180;
//   } else if (!strcmp(*string,"2") == 0) {
//       out2 = 180;
//   }
//   // Output values to motor
//   servo1.write(out1);
//   servo2.write(out2);
//   out1 = 0;
//   out2 = 0;
// } 