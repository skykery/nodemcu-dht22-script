#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "FS.h"
#include "DHT.h"


#define LED D1
#define DHTTYPE DHT22
#define DHTPIN D2

const char* ssid = "YOUR_WIFI_NETWORK_NAME";
const char* password = "YOURR_WIFI_NETWORK_PASSWORD";

// DHT22
DHT dht(DHTPIN, DHTTYPE);
float t;
float h;

void setup() {
  // setup code to run once
  pinMode(LED, OUTPUT);

  Serial.begin(115200);
  dht.begin();
  Serial.println();


  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print("Connecting..");
  }
}

void sendData(String t, String h) {
  WiFiClientSecure client;
  HTTPClient http;
  client.setInsecure();

  String APIURL = "https://your-base-url/endpoint";
  String httpRequestData = APIURL + "?t=" + t + "&h=" + h;
  http.begin(client, httpRequestData);
  // Basic Auth if needed
  // http.setAuthorization("username", "password");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpResponseCode = http.GET();
  String payload = http.getString();
  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);
  Serial.println(payload);
  http.end();
}

void loop() {
  digitalWrite(LED, HIGH);
  if (WiFi.status() == WL_CONNECTED) {
    h = dht.readHumidity();
    t = dht.readTemperature();
    sendData(String(t), String(h));
    // https://www.electronicshub.org/esp8266-deep-sleep-mode/
    ESP.deepSleep(10e6 * 6 * 15); // 15 mins
    // delay(1000 * 60);
  }
  else {
    Serial.println("Nothing to do.");
    delay(5000);
  }
}
