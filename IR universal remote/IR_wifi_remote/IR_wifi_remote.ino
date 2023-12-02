/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-relay-module-ac-web-server/

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

// Import required libraries

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h> 
#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <AsyncElegantOTA.h>
#include <IRremote.h> // >v3.0.0

#define PIN_SEND D5 // 4

// Set to true to define Relay as Normally Open (NO)
#define RELAY_NO true

// Set number of relays
#define NUM_ELEMENTS 7
<Adafruit_SHT31 sht31 = Adafruit_SHT31();

// Assign each GPIO to a relay
// except first one others are just placeholder
int relayGPIOs[NUM_ELEMENTS] = {26, 27, 25, 33, 34, 35, 36};

char arr[7][10] = {"Light", "Low", "Medium", "High", "Off", "AC on", "AC off"};

// Replace with your network credentials
const char *ssid = "HOMEAWAYUSJ@unifi";
const char *password = "follow!house!rules";

const char *PARAM_INPUT_1 = "relay";
const char *PARAM_INPUT_2 = "state";

// Set your Static IP address
IPAddress local_IP(192, 168, 0, 136);
// Set your Gateway IP address
IPAddress gateway(192, 168, 0, 1);

IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8);   // optional
IPAddress secondaryDNS(8, 8, 4, 4); // optional

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 3.0rem;}
    p {font-size: 3.0rem;}
    body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
    .switch {position: relative; display: inline-block; width: 120px; height: 68px} 
    .switch input {display: none}
    .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 34px}
    .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 68px}
    input:checked+.slider {background-color: #2196F3}
    input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
    .btn9 {
      background-color: #405cf5; border-radius: 6px; padding: 16px;
      border-width: 0; color: #fff; cursor: pointer;
      font-size: 20px; background-color: #405cf5; border-radius: 6px;
      padding: 16px; border-width: 0; color: #fff; cursor: pointer; font-size: 20px;}
    .btn9:focus {
      box-shadow: rgba(50, 50, 93, .1) 0 0 0 1px inset, rgba(50, 50, 93, .2) 0 6px 15px 0, rgba(0, 0, 0, .1) 0 2px 2px 0, rgba(50, 151, 211, .3) 0 0 0 4px;
    }
  </style>
</head>
<body>
  <h2>Ooi Cun</h2>
  %BUTTONPLACEHOLDER%
<script>function toggleCheckbox(element) {
  var xhr = new XMLHttpRequest();
  if(element.checked){ xhr.open("GET", "/action?relay="+element.id+"&state=1", true); }
  else { xhr.open("GET", "/action?relay="+element.id+"&state=0", true); }
  xhr.send();
}</script>
</body>
</html>
)rawliteral";

String relayState(int numRelay)
{
  if (RELAY_NO)
  {
    if (digitalRead(relayGPIOs[numRelay - 1]))
    {
      return "";
    }
    else
    {
      return "checked";
    }
  }
  else
  {
    if (digitalRead(relayGPIOs[numRelay - 1]))
    {
      return "checked";
    }
    else
    {
      return "";
    }
  }
  return "";
}

// Replaces placeholder with button section in your web page
String processor(const String &var)
{
  // Serial.println(var);
  if (var == "BUTTONPLACEHOLDER")
  {

    String buttons = "";

    float t = sht31.readTemperature();
    float h = sht31.readHumidity();

    if (!isnan(t))
    { // check if 'is not a number'
      Serial.print("Temp *C = ");
      Serial.print(t);
      Serial.print("\t\t");
      buttons += "<h4>Temperature = " + String(t) + "</h4>";
    }
    else
    {
      Serial.println("Failed to read temperature");
    }

    if (!isnan(h))
    { // check if 'is not a number'
      Serial.print("Hum. % = ");
      Serial.println(h);
      buttons += "<h4>Humidity = " + String(h) + "</h4>";
    }
    else
    {
      Serial.println("Failed to read humidity");
    }

    for (int i = 1; i <= NUM_ELEMENTS; i++)
    {
      String relayStateValue = relayState(i);
      // buttons += "<h4>Relay #" + String(i) + " - GPIO " + relayGPIOs[i - 1] + "</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"" + String(i) + "\" " + relayStateValue + "><span class=\"slider\"></span></label>";

      if (i == 1)
      {
        buttons += "<h4>" + String(arr[i - 1]) + " - (GPIO " + relayGPIOs[i - 1] + ")</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"" + String(i) + "\" " + relayStateValue + "><span class=\"slider\"></span></label>";
      }
      else
      {
        // buttons += "<h4>" + String(arr[i - 1]) + " - (GPIO " + relayGPIOs[i - 1] + ")</h4><label class=\"switch\"><button onclick=\"toggleCheckbox(this)\" id=\"" + String(i) + "\" " + relayStateValue + ">Trigger</button><span class=\"slider\"></span></label>";
        buttons += "<h4>Fan: " + String(arr[i - 1]) + "</h4><button class=\"btn9\" onclick=\"toggleCheckbox(this)\" id=\"" + String(i) + "\" " + relayStateValue + ">Trigger</button>";
      }
    }
    return buttons;
  }
  return String();
}

const int repeats = 10;
const int response = 2;
const int delayI = 2000;
void sendIRFan(int code)
{
  IrSender.sendPulseDistanceWidth(38, 1300, 350, 1300, 350, 500, 1200, code, 11, PROTOCOL_IS_LSB_FIRST, response, repeats);
}

void sendIRAC(int code)
{
  IrSender.sendSamsung48(0xB24D, 0x29F8, 2);
  // IrSender.sendPulseDistanceWidth(38, 1300, 350, 1300, 350, 500, 1200, code, 11, PROTOCOL_IS_LSB_FIRST, response, repeats);
}

void setup()
{
  // Serial port for debugging purposes
  Serial.begin(115200);

  // setup IR
  IrSender.begin(PIN_SEND); // Initializes IR sender
  IrSender.enableIROut(38); //

  // Set all relays to off when the program starts - if set to Normally Open (NO), the relay is off when you set the relay to HIGH
  for (int i = 1; i <= NUM_ELEMENTS; i++)
  {
    pinMode(relayGPIOs[i - 1], OUTPUT);
    if (RELAY_NO)
    {
      digitalWrite(relayGPIOs[i - 1], HIGH);
    }
    else
    {
      digitalWrite(relayGPIOs[i - 1], LOW);
    }
  }

  // Configures static IP address
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS))
  {
    Serial.println("STA Failed to configure");
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println("WIFI connected");

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/html", index_html, processor); });

  // Send a GET request to <ESP_IP>/action?relay=<inputMessage>&state=<inputMessage2>
  server.on("/action", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    String inputMessage;
    String inputParam;
    String inputMessage2;
    String inputParam2;
    // GET input1 value on <ESP_IP>/action?relay=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1) & request->hasParam(PARAM_INPUT_2)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
      inputParam2 = PARAM_INPUT_2;
      if(RELAY_NO){
        Serial.print("NO ");
        if((inputMessage.toInt() )== 2){ //low
          sendIRFan(0x10D); 
        } else if((inputMessage.toInt() )== 3){ // mid
          sendIRFan(0x20D);
        } else if((inputMessage.toInt() )== 4){ // hight
          sendIRFan(0x40D);
        } else if((inputMessage.toInt() )== 5){ //off
          sendIRFan(0x8D);
        } else if((inputMessage.toInt() )== 6){ // ac on
          IrSender.sendSamsung48(0xB24D, 0x21F8, 2);
        } else if((inputMessage.toInt() )== 7){ // ac off
          IrSender.sendSamsung48(0xB24D, 0x7DE, 2);
        }
        
        
         else {
          digitalWrite(relayGPIOs[inputMessage.toInt()-1], !inputMessage2.toInt());
        }

        // if((inputMessage.toInt() )== 2){ //low
        //   IrSender.sendPulseDistanceWidth(38, 1300, 350, 1300, 350, 500, 1200, 0x10D, 11, PROTOCOL_IS_LSB_FIRST, response, repeats); 
        // } else if((inputMessage.toInt() )== 3){ // mid
        //   IrSender.sendPulseDistanceWidth(38, 1250, 400, 1300, 350, 450, 1250, 0x20D, 11, PROTOCOL_IS_LSB_FIRST, response, repeats);
        // } else if((inputMessage.toInt() )== 4){ // hight
        //   IrSender.sendPulseDistanceWidth(38, 1250, 400, 1300, 350, 450, 1250, 0x40D, 11, PROTOCOL_IS_LSB_FIRST, response, repeats);
        // } else if((inputMessage.toInt() )== 5){ //off
        //   IrSender.sendPulseDistanceWidth(38, 1250, 400, 1300, 350, 450, 1250, 0x8D, 11, PROTOCOL_IS_LSB_FIRST, response, repeats);
        // } else {
        //   digitalWrite(relayGPIOs[inputMessage.toInt()-1], !inputMessage2.toInt());
        // }


      }
      else{
        Serial.print("NC ");
        digitalWrite(relayGPIOs[inputMessage.toInt()-1], inputMessage2.toInt());
      }
    }
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    Serial.println(inputMessage + inputMessage2);
    request->send(200, "text/plain", "OK"); });

  // start OTA
  AsyncElegantOTA.begin(&server); // Start ElegantOTA

  // Start server
  server.begin();

  // sht 31
  Serial.println("SHT31 test");
  if (!sht31.begin(0x44))
  { // Set to 0x45 for alternate i2c addr
    Serial.println("Couldn't find SHT31");
    while (1)
      delay(1);
  }

  Serial.print("Heater Enabled State: ");
  if (sht31.isHeaterEnabled())
    Serial.println("ENABLED");
  else
    Serial.println("DISABLED");

  Serial.println("Setup completed.");
}

void loop()
{
}
