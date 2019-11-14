#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <Servo.h>

// Servo and pump pin setup
Servo servo1;
Servo servo2; 
int servo1Pin = 5;
int servo2Pin = 6;
// int servo3Pin = 9;
// int servo4Pin = 10;
// int pumpPin = 13;

// WiFi UDP connection setup
const char* ssid = "iPhone";
const char* password = "mechatronics";

WiFiUDP udp;
unsigned int receivePort = 2808;                 // Arbitrary port for receiving
const unsigned int UDP_PACKET_SIZE = 1;          // Size of packets
byte receiveBuffer[UDP_PACKET_SIZE + 1];         // Packet of joystick data 

IPAddress myIP(192, 168, 1, 156);                // glove IP


void setup() {
  // Set Pin Mode
  pinMode(servo1Pin, OUTPUT);    // PWM Signal servo 1
  pinMode(servo2Pin, OUTPUT);    // PWM Signal servo 2
  // pinMode(servo3Pin, OUTPUT);    // PWM Signal serv0 3
  // pinMode(servo4Pin, OUTPUT);    // PWM Signal servo 4
  // pinMode(pumpPin, OUTPUT);      // Digital out pump

  // digitalWrite(pumpPin, HIGH);
  servo1.attach(servo1Pin);
  servo1.write(0);
  servo2.attach(servo2Pin);
  servo2.write(0);

  //Set up serial connection
  Serial.begin(115200);
  Serial.print("Connecting to "); Serial.println(ssid);

  // Configure WiFi
  WiFi.config(myIP, IPAddress(192, 168, 1, 1), IPAddress(255, 255, 255, 0));
  WiFi.begin(ssid, password);
  udp.begin(receivePort);

  // Wait for WiFi connection to be established
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected as");
  Serial.print(WiFi.localIP());
  
  // null terminate buffer
  receiveBuffer[UDP_PACKET_SIZE] = 0;
}

int i = 0;
void loop() {

  // Receive data continuously from VR Interface -- take in as 1 byte string
  int indicator = 1;
  while (indicator != 0) {
    indicator = receive_byte();
  }

  // Remove 10 added before sending
  byte received = receiveBuffer[0] - 10;

  byte out1_byte = received && 0b00000001;
  byte out2_byte = received && 0b00000010;
  byte out3_byte = received && 0b00000100;
  byte out4_byte = received && 0b00001000;

  if (out1_byte != 0) {
      out1 = 180;
  } else {
      out1 = 0;
  }

  if (out2_byte != 0) {
      out2 = 180;
  } else {
      out2 = 0;
  }

  if (out3_byte != 0) [
      out3 = 180;
  ] else {
      out3 = 0;
  }

  if (out4_byte != 0) {
      out4 = 180;
  } else {
      out4 = 0;
  }

  // Output values to motor
  servo1.write(out1);
  servo2.write(out2);
  // servo3.write(out3);
  // servo4.write(out4);
}


// Subroutine for receiving bytes
// return 0 if successful, 1 if packet empty
int receive_byte() {
  // Parse packet, read if packet not empty
  int cb = udp.parsePacket();
  if (cb) {
    udp.read(receiveBuffer, UDP_PACKET_SIZE);
    return 0;
  } else {
    return 1;
  }
}
