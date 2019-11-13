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
int ledChannel1 = 0;                             // First pump mapped to channel 0
int ledChannel2 = 1;                             // Second pump mapped to channel 1
int resolution_bits = 8;
int resolution = ((1<<resolution_bits)-1);


void setup() {
  ledcSetup(ledChannel1, freq, resolution_bits);
  ledcSetup(ledChannel2, freq, resolution_bits);
  ledcAttachPin(32, ledChannel1);                 // First pump mapped to pin 32
  ledcAttachPin(22, ledChannel2);                 // Second pump mapped to pin 22

  // Set Pin Modes
  // Motor 1 pins
  pinMode(32, OUTPUT);   // PWM Signal pump 1
  pinMode(33, OUTPUT);   // direction 
  pinMode(25, OUTPUT);   // direction - opposite
  // Motor 2 pins 
  pinMode(22, OUTPUT);   // PWM Signal pump 2
  pinMode(21, OUTPUT);   // direction
  pinMode(4, OUTPUT);    // direction - opposite

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

// Function to simplify sending analog signals to pumps
void ledcAnalogWrite(uint8_t channel, uint32_t value, uint32_t valueMax = 255) {
  uint32_t duty = resolution * min(value, valueMax) / valueMax;
  ledcWrite(channel, duty);
}


void loop() {

  // Receive data continuously from remote -- take in as 4 byte string
  int indicator = 1;
  while (indicator != 0) {
    indicator = receive_byte();
  }

  // Remove 10 added before sending
  byte x_val = receiveBuffer[0] - 10;
  byte y_val = receiveBuffer[1] - 10;

  // Output values to motor
  ledcAnalogWrite(ledChannel1, x_val);
  ledcAnalogWrite(ledChannel2, y_val);
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