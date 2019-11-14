#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>

// Set up WiFi UDP connection
const char* ssid = "iPhone";
const char* password = "mechatronics";

WiFiUDP udp;
unsigned int receivePort = 2808;                 // Arbitrary port for receiving
const unsigned int UDP_PACKET_SIZE = 4;          // Size of packets
byte receiveBuffer[UDP_PACKET_SIZE + 1];         // Packet of joystick data 

// Set IP address
IPAddress myIP(192, 168, 1, 156);                // glove IP

// Constants for ledc analog writing
int freq = 150;
int pump1Pin = 0;
int pump2Pin = 1;    
int pump3Pin = 2;
int pump4Pin = 3;
int resolution_bits = 8;
int resolution = ((1<<resolution_bits)-1);


void setup() {

  // Set Pin Modes
  pinMode(32, OUTPUT);   // PWM Signal pump 1
  pinMode(22, OUTPUT);   // PWM Signal pump 2

  // Set up serial connection
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


void loop() {

  // Receive data continuously from VR Interface -- take in as 4 byte string
  int indicator = 1;
  while (indicator != 0) {
    indicator = receive_byte();
  }

  // Remove 10 added before sending
  byte x_val = receiveBuffer[0] - 10;
  byte y_val = receiveBuffer[1] - 10;

  // Output values to motor
  analogWrite(pump1Pin, x_val);
  analogWrite(pump2Pin, y_val);
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