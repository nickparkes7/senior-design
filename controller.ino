#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>

// Set up WiFi UDP connection
const char* ssid = "iPhone";
const char* password = "mechatronics";

WiFiUDP udp;
unsigned int receivePort = 2808;                 // Arbitrary port for receiving
const unsigned int UDP_PACKET_SIZE = 2;          // Size of packets
byte receiveBuffer[UDP_PACKET_SIZE + 1];         // Packet of joystick data 

// Set IP address
IPAddress myIP(192, 168, 1, 156);                // car IP

// Constants for ledc analog writing
int freq = 150;
int ledChannel1 = 0;                             // First motor mapped to channel 0
int ledChannel2 = 1;                             // Second motor mapped to channel 1
int resolution_bits = 8;
int resolution = ((1<<resolution_bits)-1);


void setup() {
  ledcSetup(ledChannel1, freq, resolution_bits);
  ledcSetup(ledChannel2, freq, resolution_bits);
  ledcAttachPin(32, ledChannel1);                 // First motor mapped to pin 32
  ledcAttachPin(22, ledChannel2);                 // Second motor mapped to pin 22

  // Set Pin Modes
  // Motor 1 pins
  pinMode(32, OUTPUT);   // PWM Signal motor 1
  pinMode(33, OUTPUT);   // direction 
  pinMode(25, OUTPUT);   // direction - opposite
  // Motor 2 pins 
  pinMode(22, OUTPUT);   // PWM Signal motor 2
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

// Function to simplify sending analog signals to motors
void ledcAnalogWrite(uint8_t channel, uint32_t value, uint32_t valueMax = 255) {
  uint32_t duty = resolution * min(value, valueMax) / valueMax;
  ledcWrite(channel, duty);
}


void loop() {

  // Receive data continuously from remote -- take in as 2 byte string
  int indicator = 1;
  while (indicator != 0) {
    indicator = receive_byte();
  }

  // Remove 10 added before sending
  byte x_val = receiveBuffer[0] - 10;
  byte y_val = receiveBuffer[1] - 10;

  // Convert readings from remote into movement of 2 motors

  // Map joystick values to steering values in range [0,1]
  float steering;
  if (x_val < 80) {
    steering = 0.00625 * (float)x_val;
  } else {
    steering = 0.00285714 * (float)x_val + 0.271429;
  }

  // Keep both wheels at full power if going straight
  // Decrease power linearly on one wheel if turning
  float left_steer;
  if (steering < 0.5) {
    left_steer = 2*steering;
  } else {
    left_steer = 1;
  }

  float right_steer;
  if (steering < 0.5) {
    right_steer = 1;
  } else {
    right_steer = -2*steering + 2;
  }

  // Map joystick values to throttle values in range [0,1]
  // and set motors to go forward or backward according to direction of joystick
  float throttle;
  if (y_val < 80) {
    throttle = 1 - (float)y_val/80.0;
    digitalWrite(33, HIGH); // Motor 1
    digitalWrite(25, LOW); // Motor 1
    digitalWrite(21, HIGH); // Motor 2
    digitalWrite(4, LOW); // Motor 2
  } else {
    throttle = (float)y_val/175.0 - 16.0/35.0;
    digitalWrite(33, LOW); // Motor 1
    digitalWrite(25, HIGH); // Motor 1
    digitalWrite(21, LOW); // Motor 2
    digitalWrite(4, HIGH); // Motor 2
  }

  // // Use for debugging
  // Serial.print("x_val: "); Serial.println(x_val);
  // Serial.print("steering: "); Serial.println(steering);
  // Serial.println("");
  // Serial.print("y_val: "); Serial.println(y_val);
  // Serial.print("throttle: "); Serial.println(throttle);
  // Serial.println("");


  // Covert throttle and steering values to motor speeds in range [0,255]
  float left_motor_speed_float = 255 * throttle * left_steer;
  int left_motor_speed = (int) left_motor_speed_float;
  // Serial.print("left: "); Serial.println(left_motor_speed);

  float right_motor_speed_float = 255 * throttle * right_steer;
  int right_motor_speed = (int) right_motor_speed_float;
  // Serial.print("right: "); Serial.println(right_motor_speed); 

  // Serial.println("");
  // Serial.println("");

  // Output values to motor
  ledcAnalogWrite(ledChannel1, left_motor_speed);
  ledcAnalogWrite(ledChannel2, right_motor_speed);
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