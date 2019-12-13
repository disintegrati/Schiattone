// Se lo sketch non funziona prova ad aggiungere #include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

ESP8266WiFiMulti WiFiMulti;

int inPin = 12;   
int state = HIGH;      // the current state of the output pin
int reading;           // the current reading from the input pin
int previous = LOW;    // the previous reading from the input pin

void setup() {

  Serial.begin(115200);
  // Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }
  
  // disabilito l'access point dell'ESP8266
  WiFi.mode(WIFI_STA);

  // inserisci qui l'SSID e la password del WiFi
  WiFiMulti.addAP("Telecom-56475423", "Red_Home1234"); 
  Serial.println("mi sono connesso");
  pinMode(inPin, INPUT);
}

void loop()
{
  reading = digitalRead(inPin);
  
  // if the input just went from LOW and HIGH and we've waited long enough
  // to ignore any noise on the circuit, toggle the output pin and remember
  // the time
  if (reading == HIGH && previous == LOW) {
      if ((WiFiMulti.run() == WL_CONNECTED)) {
        WiFiClient client;
        HTTPClient http;
        Serial.print("[HTTP] begin...\n");
        if (http.begin(client, "http://battito.cuoredinapoli.net/setStatus")) {  // HTTP
          Serial.print("[HTTP] GET...\n");
          // start connection and send HTTP header
          int httpCode = http.GET();
          // httpCode will be negative on error
          if (httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            Serial.printf("[HTTP] GET... code: %d\n", httpCode);
            // file found at server
            if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) { 
            }
          } else {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
          }
          http.end();
        } else {
          Serial.printf("[HTTP} Unable to connect\n");
        }
      }
      Serial.println(state);
  }
  previous = reading;
}
