#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>

// Set up WiFi UDP connection
const char* ssid = "iPhone";
const char* password = "mechatronics";

WiFiUDP UDPTestServer;
unsigned int sendPort = 2808;                // send port - this is the other controller's receive port
IPAddress myIPaddress(192, 168, 1, 157);
IPAddress targetIPaddress(192, 168, 1, 156);

// Setting up the packets for sending and receiving
const int UDP_PACKET_SIZE = 2;
char sendBuffer[UDP_PACKET_SIZE];

void setup() {
  pinMode(36, INPUT);                        // x input pin
  pinMode(39, INPUT);                        // y input pin

  // Set up serial connection
  Serial.begin(115200);
  Serial.print("Connecting to "); Serial.println(ssid);

  // Configure WiFi
  WiFi.config(myIPaddress, IPAddress(192, 168, 1, 1), 
  IPAddress(255, 255, 255, 0));
  WiFi.begin(ssid, password);
  UDPTestServer.begin(sendPort);

  // Wait until the wifi is connected
  while(WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println("WiFi connected as");
  Serial.print(WiFi.localIP());
  
}

void loop() {
  int x = analogRead(36) + 10;  // x input - adding 10 so that we never send a 0 and mess up the receive function with a null termination
  float x_value = (float) x;    // convert value to float
  byte x_byte = (byte) x_value; // cast it as a byte
  
  int y = analogRead(39) + 10;  // y input - adding 10 so that we never send a 0 and mess up the receive function with a null termination
  float y_value = (float) y;    // convert value to float
  byte y_byte = (byte) y_value; // cast it as a byte

  send_package(x_byte, y_byte);
}


// Helper function for sending
void send_package(byte x_value, byte y_value) {
  // Store byte values in buffer and null terminate
  sendBuffer[0] = x_value;
  sendBuffer[1] = y_value;
  sendBuffer[2] = 0;
  
  // Send packet to IP target
  UDPTestServer.beginPacket(targetIPaddress, sendPort);
  UDPTestServer.printf("%s", sendBuffer);
  UDPTestServer.endPacket();

  // // Print contents of buffer to serial monitor for debugging
  // Serial.println("The x value is: ");
  // Serial.println(x_value);
  // Serial.println();
  // Serial.println("The y value is: ");
  // Serial.println(y_value);
}

