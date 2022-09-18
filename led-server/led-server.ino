#include <NeoPixelConnect.h>
#include <WiFiWebServer.h>
#include "secrets.h"

const int PORT = 80;
WiFiWebServer server(PORT);

const int LED_GPIO_PIN = 15; // Data pin 3
const int LED_COUNT = 50;
NeoPixelConnect leds(LED_GPIO_PIN, LED_COUNT);

String body;
void handlePostLeds() {
  body = server.arg("plain");
  if (body.length() != LED_COUNT * 3) {
    server.send(400);
    return;
  }

  for (int ledI = 0; ledI < LED_COUNT; ledI++) {
    leds.neoPixelSetValue(
      ledI,
      body[ledI * 3],
      body[ledI * 3 + 1],
      body[ledI * 3 + 2],
      false
    );
  }

  leds.neoPixelShow();
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);

  int status = WiFi.status();
  while (status == WL_NO_MODULE);
  while (status != WL_CONNECTED) {
    status = WiFi.begin(NETWORK_SSID, NETWORK_PASSWORD);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
  }

  server.on(F("/leds"), handlePostLeds);
  server.begin();
  
  Serial.println(WiFi.localIP());
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  server.handleClient();
}
