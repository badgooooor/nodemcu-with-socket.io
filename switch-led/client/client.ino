#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <SocketIoClient.h>
#include <string.h>

#define ledPin D3

// Put all of your port here.
const char *ssid = "WIFI_NAME_HERE";
const char *password = "WIFI_PW_HERE";

const char *socketServer = "SERVER_IP_ADDRESS";
const int socketPort = 4200;

SocketIoClient socket;

void event(const char *payload, size_t length)
{
  Serial.printf("got message: %s\n", payload);
}

void toggleLED(const char *payload, size_t length)
{
  char subs[5];
  memcpy(subs, &payload[9], 4);
  subs[4] = '\0';
  
  Serial.printf("LED status: %s\n", subs);
  if(strcmp(subs,"true") == 0) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
  delay(500);
}

void setup()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--)
  {
    Serial.printf("[SETUP] BOOT WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print(".");
  }

  socket.on("event", event);
  socket.begin(socketServer, socketPort);     
  socket.on("led", toggleLED);
}

void loop()
{
  socket.loop();
}
