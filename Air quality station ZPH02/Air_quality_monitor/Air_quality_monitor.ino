#include <SoftwareSerial.h>

float duration;
int pulsePin = 4;  // set the pulsePin ( Digital Pin will do )

float duty_c = 0;
float pm;

#define rxPin D7
#define txPin 3

// byte response[9] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
SoftwareSerial mySerial(rxPin, txPin);  // RX, TX

void setup() {
  pinMode(rxPin, INPUT);

  // put your setup code here, to run once:
  Serial.begin(9600);  // Serial Monitor for personal referrence of sensor data during debugging
  mySerial.begin(9600);
}

void loop() {
  // Serial.println("looping");
  // viaPWM();
  // viaSerial();

  testSerial();
}

void testSerial() {
  // Serial.println(mySerial.available());
  // // if (mySerial.available() > 0) {
  // //   int a = mySerial.read();
  // //   Serial.println(a);
  // // }


  while (mySerial.available() && mySerial.peek() != 0xFF) {
    mySerial.read();  // discard
  }

  if (mySerial.available() > 8) {
    // mySerial.read();
    byte measure[9];
    mySerial.readBytes(measure, 9);
    Serial.println(measure[3]);
    Serial.println(measure[4]);
  }

  if (mySerial.overflow()) {
    while (mySerial.available()) {
      Serial.println("cleaning overflow");
    }
  }
}

void viaPWM() {
  // PWM portion
  // put your main code here, to run repeatedly:
  duration = pulseIn(pulsePin, LOW);
  Serial.println("duration");
  Serial.println(duration);
  duty_c = (duration / (1000000)) * 100;
  // pm = map(duty_c, 0, 100, 0, 1000);  // To be configured ^^^ ^^^ ^^^ ^^^
  Serial.println("duty_c");
  Serial.println(duty_c);
  delay(1000);
}


// UART
// data byte 8 byte
// stop byte 1 byte
// we have to read 9 byte
// start byte is 0xFF

// read from serial
// while byte = 0xFF
// start reading
// Common 8-n-1 protocol
// until 9 bytes
// parse byte 3 and 4
// combine and print
void viaSerial() {
  float ppm;
  byte measure[8];
  if (mySerial.available() > 0) {

    mySerial.readBytes(measure, 9);
    // incomingByte = _s.read();
    // float ppm = measure[2] * 256 + measure[3];
    // Serial.print(ppm);
  }

  Serial.println("---");
  for (int i = 0; i < 9; i++) {
    Serial.println(measure[i]);
  }
  Serial.println("---");

  if (measure[0] == 0xff) {
    ppm = measure[2] * 256 + measure[3];  // this formula depends of the sensor is in the dataSheet
    // if (_type == 2) {
    //   ppm = ppm * 0.1;
    // }
    Serial.println(ppm);
    Serial.println(measure[3]);
    Serial.println(measure[4]);
  }



  // mySerial.flush();
  clearSerial();
  delay(500);
}

void clearSerial() {
  while (mySerial.available()) {
    mySerial.read();
  }
}
