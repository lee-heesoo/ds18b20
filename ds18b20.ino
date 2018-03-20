/*
const char* host = "api.thingspeak.com";
String url = "/update?api_key=DWZ923X318S978LL";  
const int httpPort = 80;
int interval = 60000;
float r = 0;
*/

const char* host = "maker.ifttt.com";
String url = "/trigger/hot_temp/with/key/d7m-FGlXNELQ0TYKdBl70S/?value1:";
const int httpPort = 80;
int interval = 5000;
float r = 0;

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS D3

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
const char* ssid = "iPhone";
const char* password = "3wa580g33fyu6";

String working() { 
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures();  // Send the command to get temperatures
  Serial.println("DONE");
  Serial.print("Temperature for Device 1 is: ");
  r = sensors.getTempCByIndex(0);
  Serial.print(sensors.getTempCByIndex(0));
  //add
  url = "/trigger/hot_temp/with/key/d7m-FGlXNELQ0TYKdBl70S/?value1="+String(r);
  return(String("field1=")+String(r));
}
 
void delivering(String payload) { 
  WiFiClient client;
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpPort)) {
    Serial.print("connection failed: ");
    Serial.println(payload);
    return;
  }
  
  String getheader = "GET "+ String(url) +"&"+ String(payload) +" HTTP/1.1";
  client.println(getheader);
  client.println("User-Agent: ESP8266 HEESOO LEE");  
  client.println("Host: " + String(host));  
  client.println("Connection: close");  
  client.println();
  Serial.println(getheader);
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    Serial.println(line);
  }
  Serial.println("Done cycle.");
}

void connect_ap() {
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\n Got WiFi, IP address: ");
  Serial.println(WiFi.localIP());  
}

void setup() {
  Serial.begin(115200);
  Serial.println("Dallas Temperature IC Control Library Demo");
  sensors.begin();
  connect_ap();
  // Serial.println("ESPArduinoThingSpeak.cpp");
}

unsigned long mark = 0;

void loop() {
  if (millis() > mark ) {
     mark = millis() + interval;
     String payload = working();
     delivering(payload);
  }
}

