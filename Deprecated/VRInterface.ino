// #include <Arduino.h>
// #include <WiFi.h>
// #include <WiFiUdp.h>
// #include <SoftwareSerial.h>
// #include <SerialCommand.h>

// SerialCommand sCmd;

// // Set up WiFi UDP connection
// const char* ssid = "iPhone";
// const char* password = "mechatronics";

// WiFiUDP UDPTestServer;
// unsigned int sendPort = 2808;                // send port - this is the other controller's receive port
// IPAddress myIPaddress(192, 168, 1, 157);
// IPAddress targetIPaddress(192, 168, 1, 156);

// // Setting up the packets for sending and receiving
// const int UDP_PACKET_SIZE = 1;
// char sendBuffer[UDP_PACKET_SIZE];

// // Servo and pump pin setup
// Servo servo1;
// Servo servo2; 
// int servo1Pin = 5;
// int servo2Pin = 6;
// // int servo3Pin = 9;
// // int servo4Pin = 10;
// // int pumpPin = 13;

// void setup() {

//   // Set up serial connection
//   Serial.begin(9600);
//   while (!Serial);

//   sCmd.addCommand("PING", pingHandler);

//   Serial.print("Connecting to "); Serial.println(ssid);

//   // Configure WiFi
//   // WiFi.config(myIPaddress, IPAddress(192, 168, 1, 1), 
//   // IPAddress(255, 255, 255, 0));
//   // WiFi.begin(ssid, password);
//   // UDPTestServer.begin(sendPort);

//   // // Wait until the wifi is connected
//   // while(WiFi.status() != WL_CONNECTED) {
//   //     delay(500);
//   //     Serial.print(".");
//   // }
//   Serial.println("WiFi connected as");
//   Serial.print(WiFi.localIP());
  
// }


// void pingHandler () {
//   char *arg;
//   arg = sCmd.next();
//   if (arg != NULL) {
//     send_package(arg);
//     Serial.print("sent"); Serial.println(arg);
//   } else {
//     Serial.println("PONG");
//   }
// }

// void loop() {
//   if (Serial.available() > 0) {
//     Serial.println("yuppp");
//     sCmd.readSerial();
//   }
//   Serial.println("waiting...");
//   delay(1000);
// }


// // Helper function for sending
// void send_package(byte received_byte) {
//   // Store byte values in buffer and null terminate
//   sendBuffer[0] = received_byte;
//   sendBuffer[1] = 0;
  
//   // Send packet to IP target
//   UDPTestServer.beginPacket(targetIPaddress, sendPort);
//   UDPTestServer.print(sendBuffer);
//   UDPTestServer.endPacket();
// }
