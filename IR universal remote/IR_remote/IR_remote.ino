
// void setup() {
//   Serial.begin(115200);
// }

// void loop() {

//   Serial.println("yes");
//   delay(2000);
// }



// Import required libraries

#include <Arduino.h>
#include <IRremote.h>  // >v3.0.0

#define  PIN_SEND D5  // 2 // 23  // 4
// #define PIN_SEND 2 // 23  // 4
const int repeats = 10;
const int response = 2; 

void sendIRFan(int code) {
  IrSender.sendPulseDistanceWidth(38, 1300, 350, 1300, 350, 500, 1200, code, 11, PROTOCOL_IS_LSB_FIRST, response, repeats);
}

void sendIRAC(int code) {
  IrSender.sendSamsung48(0xB24D, 0x29F8, 2);
  // IrSender.sendPulseDistanceWidth(38, 1300, 350, 1300, 350, 500, 1200, code, 11, PROTOCOL_IS_LSB_FIRST, response, repeats);
}

void setup() {
  // Serial port for debugging purposes
  // Serial.begin(9600);
  Serial.begin(115200);
  // setup IR
  IrSender.begin(PIN_SEND);  // Initializes IR sender
  IrSender.enableIROut(38);  // 
}

void loop() {
  sendIRFan(0x10D);
  // sendIRFan(0x20D);
  // // sendIRFan(0x40D);
  delay(1000);
  Serial.println("okay");
  // // sendIRFan(0x8D);
  // // IrSender.sendSamsung48(0xB24D, 0x21F8, 2);
  IrSender.sendSamsung48(0xB24D, 0x7DE, 2);
  delay(1000);
}
