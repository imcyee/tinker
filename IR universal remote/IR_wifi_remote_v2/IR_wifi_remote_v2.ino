#define PIN_SEND D5  // 4

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <IRremote.h>  // >v3.0.0


/*
0xA5AF led off/on

0xD85 30min off
0xDC5 1h off
0xDA5 1.5h off
0xDE5 2.0h off
0xD95 2.5h off
0xDED 3.0h off


0xD95 on hour

mode
0x1DB8 auto
0x2DB8 dry
0xDBD cool

*/

/* Put your SSID & Password */
const char* ssid = "HOMEAWAYUSJ@unifi";       // Enter SSID here
const char* password = "follow!house!rules";  //Enter Password here

ESP8266WebServer server(80);

uint8_t LED1pin = D7;
bool LED1status = LOW;

uint8_t LED2pin = D6;
bool LED2status = LOW;

void setup() {
  Serial.begin(115200);
  delay(100);


  // setup IR
  IrSender.begin(PIN_SEND);  // Initializes IR sender
  IrSender.enableIROut(38);  //

  // pinMode(LED1pin, OUTPUT);
  // pinMode(LED2pin, OUTPUT);

  Serial.println("Connecting to ");
  Serial.println(ssid);

  //connect to your local wi-fi network
  WiFi.begin(ssid, password);

  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handle_OnConnect);
  server.on("/fanlow", handle_fanlow);
  server.on("/fanmid", handle_fanmid);
  server.on("/fanhigh", handle_fanhigh);
  server.on("/fanoff", handle_fanoff);
  server.on("/acon", handle_acon);
  server.on("/acoff", handle_acoff);
  server.on("/ac30", handle_actemp30);
  server.on("/ac29", handle_actemp29);
  server.on("/ac28", handle_actemp28);

  server.on("/acled", handle_acled);
  server.on("/acoff60", handle_acoff60);
  server.on("/acoff90", handle_acoff90);
  server.on("/acauto", handle_acauto);
  server.on("/accool", handle_accool);
  server.on("/acdry", handle_acdry);



  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
}


const int repeats = 3;
const int response = 2;
const int delayI = 2000;
void sendIRFan(int code) {
  IrSender.sendPulseDistanceWidth(38, 1300, 350, 1300, 350, 500, 1200, code, 11, PROTOCOL_IS_LSB_FIRST, response, repeats);
}

void sendIRAC(int code) {
  IrSender.sendSamsung48(0xB24D, 0x29F8, 2);
  // IrSender.sendPulseDistanceWidth(38, 1300, 350, 1300, 350, 500, 1200, code, 11, PROTOCOL_IS_LSB_FIRST, response, repeats);
}


void loop() {
  server.handleClient();
  // if (LED1status) {
  //   digitalWrite(LED1pin, HIGH);
  // } else {
  //   digitalWrite(LED1pin, LOW);
  // }

  // if (LED2status) {
  //   digitalWrite(LED2pin, HIGH);
  // } else {
  //   digitalWrite(LED2pin, LOW);
  // }
}

void handle_OnConnect() {
  LED1status = LOW;
  LED2status = LOW;
  Serial.println("GPIO7 Status: OFF | GPIO6 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status, LED2status));
}

void handle_fanlow() {
  // LED1status = HIGH;
  sendIRFan(0x10D);
  // Serial.println("GPIO7 Status: ON");
  server.send(200, "text/html", SendHTML(true, LED2status));
}

void handle_fanmid() {
  // LED1status = LOW;
  sendIRFan(0x20D);
  // Serial.println("GPIO7 Status: OFF");
  server.send(200, "text/html", SendHTML(false, LED2status));
}


void handle_fanhigh() {
  sendIRFan(0x40D);
  // LED2status = HIGH;
  // Serial.println("GPIO6 Status: ON");
  server.send(200, "text/html", SendHTML(LED1status, true));
}

void handle_fanoff() {
  sendIRFan(0x8D);
  // IrSender.sendSamsung48(0xB24D, 0x7DE, 2);
  // IrSender.sendSamsung48(0xB24D, 0xB27BE0, 2);
  // LED2status = LOW;
  // Serial.println("GPIO6 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status, false));
}



void handle_acon() {
  IrSender.sendSamsung48(0xB24D, 0x21F8, repeats);
  // LED2status = HIGH;
  // Serial.println("GPIO6 Status: ON");
  server.send(200, "text/html", SendHTML(LED1status, true));
}

void handle_acoff() {
  // IrSender.sendSamsung48(0xB24D, 0x7DE, 2);
  // IrSender.sendSamsung48(0xB24D, 0xB27BE0, 2);
  IrSender.sendSamsung48(0xB24D, 0x7DE, repeats);
  // LED2status = LOW;
  // Serial.println("GPIO6 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status, false));
}

//   0x2FD
// 0x3FD
// 0xBFD
// 0x9FD
// 0x1FD 28
// 0x5FD 29
// 0xDFD 30
void handle_actemp30() {
  IrSender.sendSamsung48(0xB24D, 0xDFD, repeats);
  server.send(200, "text/html", SendHTML(LED1status, false));
}


void handle_actemp29() {
  IrSender.sendSamsung48(0xB24D, 0x5FD, repeats);
  server.send(200, "text/html", SendHTML(LED1status, false));
}


void handle_actemp28() {
  IrSender.sendSamsung48(0xB24D, 0x1FD, repeats);
  server.send(200, "text/html", SendHTML(LED1status, false));
}

void handle_acled() {
  IrSender.sendSamsung48(0x52AD, 0xA5AF, repeats);
  // IrSender.sendSamsung48(0xB24D, 0xA5AF, repeats);
  server.send(200, "text/html", SendHTML(LED1status, false));
}

void handle_acoff60() {
  IrSender.sendSamsung48(0xB24D, 0xDC5, repeats);
  server.send(200, "text/html", SendHTML(LED1status, false));
}


void handle_acoff90() {
  IrSender.sendSamsung48(0xB24D, 0xDA5, repeats);
  server.send(200, "text/html", SendHTML(LED1status, false));
}



void handle_acauto() {
  IrSender.sendSamsung48(0xB24D, 0x1DF8, repeats);
  // IrSender.sendSamsung48(0xB24D, 0x1DB8, repeats);
  server.send(200, "text/html", SendHTML(LED1status, false));
}


void handle_accool() {
  IrSender.sendSamsung48(0xB24D, 0xDBD, repeats);
  server.send(200, "text/html", SendHTML(LED1status, false));
}


void handle_acdry() {
  // IrSender.sendSamsung48(0xB24D, 0x2DB8, repeats);
  IrSender.sendSamsung48(0xB24D, 0x2DF8, repeats);
  server.send(200, "text/html", SendHTML(LED1status, false));
}



// 0xA5AF led off/on

// 0xD85 30min off
// 0xDC5 1h off
// 0xDA5 1.5h off
// 0xDE5 2.0h off
// 0xD95 2.5h off
// 0xDED 3.0h off


// 0xD95 on hour

// mode
// 0x1DB8 auto
// 0x2DB8 dry
// 0xDBD cool

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t led1stat, uint8_t led2stat) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>LED Control</title>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr += ".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr += ".button-on {background-color: #1abc9c;}\n";
  ptr += ".button-on:active {background-color: #16a085;}\n";
  ptr += ".button-off {background-color: #34495e;}\n";
  ptr += ".button-off:active {background-color: #2c3e50;}\n";
  ptr += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1>ESP8266 Web Server</h1>\n";
  ptr += "<h3>Using Station(STA) Mode</h3>\n";

  // if (led1stat) {
  //   ptr += "<p>LED1 Status: ON</p><a class=\"button button-off\" href=\"/led1off\">OFF</a>\n";
  // } else {
  //   ptr += "<p>LED1 Status: OFF</p><a class=\"button button-on\" href=\"/led1on\">ON</a>\n";
  // }

  ptr += "<p>LED1 Status: ON</p><a class=\"button button-off\" href=\"/fanlow\">LOW</a>\n";
  ptr += "<p>LED1 Status: OFF</p><a class=\"button button-on\" href=\"/fanmid\">MID</a>\n";
  ptr += "<p>LED1 Status: ON</p><a class=\"button button-off\" href=\"/fanhigh\">HIGH</a>\n";
  ptr += "<p>LED1 Status: OFF</p><a class=\"button button-on\" href=\"/fanoff\">OFF</a>\n";
  ptr += "<p>LED2 Status: ON</p><a class=\"button button-off\" href=\"/acon\">ON</a>\n";
  ptr += "<p>LED2 Status: OFF</p><a class=\"button button-on\" href=\"/acoff\">OFF</a>\n";
  ptr += "<p>LED2 Status: ON</p><a class=\"button button-off\" href=\"/ac30\">AC30</a>\n";
  ptr += "<p>LED2 Status: ON</p><a class=\"button button-off\" href=\"/ac29\">AC29</a>\n";
  ptr += "<p>LED2 Status: OFF</p><a class=\"button button-on\" href=\"/ac28\">AC28</a>\n";


  ptr += "<p>LED2 Status: ON</p><a class=\"button button-off\" href=\"/acled\">Led</a>\n";
  ptr += "<p>LED2 Status: OFF</p><a class=\"button button-on\" href=\"/acoff60\">Off 60min</a>\n";
  ptr += "<p>LED2 Status: ON</p><a class=\"button button-off\" href=\"/acoff90\">Off 90min</a>\n";
  ptr += "<p>LED2 Status: ON</p><a class=\"button button-off\" href=\"/acauto\">Auto</a>\n";
  ptr += "<p>LED2 Status: OFF</p><a class=\"button button-on\" href=\"/accool\">Cool</a>\n";
  ptr += "<p>LED2 Status: OFF</p><a class=\"button button-on\" href=\"/acdry\">Dry</a>\n";



  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}
