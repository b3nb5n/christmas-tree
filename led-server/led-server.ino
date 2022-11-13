#include <NeoPixelConnect.h>
#include <WiFiNINA.h>
#include "secrets.h"

const int LED_GPIO_PIN = 15; // Data pin 3
const int LED_COUNT = 50;
NeoPixelConnect leds(LED_GPIO_PIN, LED_COUNT);

const int PORT = 80;
WiFiServer server(PORT);

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

  server.begin();
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println(WiFi.localIP());
}

WiFiClient client;

void handleClient() {
  leds.neoPixelClear();

  int byteCount = min(client.available(), LED_COUNT);
  Serial.print("reading ");
  Serial.print(byteCount);
  Serial.println(" bytes from client");
  
  for (int i = 0; i < byteCount; i++) {
    byte color = client.read();
    float chan;

    chan = (color >> 5 & 0b111) / 7.0 * 255;
    byte r = floor(chan);
    chan = (color >> 2 & 0b111) / 7.0 * 255;
    byte g = floor(chan);
    chan = (color & 0b11)  / 3.0 * 255;
    byte b = floor(chan);

    leds.neoPixelSetValue(i, r, g, b, false);
  }

  leds.neoPixelShow();
  client.stop();
}

void loop() {
  client = server.available();
  if (client) handleClient();
}
