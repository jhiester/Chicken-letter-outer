#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <functional>
#include "switch.h"
#include "UpnpBroadcastResponder.h"
#include "CallbackFunction.h"
#include "creds.h"  // wifi credentials
#include "webPage.h" // the web interface for the door control

#define MOTOR_CW_PIN 14
#define MOTOR_CCW_PIN 12
#define SWITCH_ONE_PIN 13
#define SWITCH_TWO_PIN 5

// prototypes
boolean connectWifi();

void open();
void close();
void stop();

//on/off callbacks
bool openCoopDoor();
bool closeCoopDoor();

boolean wifiConnected = false;

UpnpBroadcastResponder upnpBroadcastResponder;

Switch *door = NULL;

bool isDoorOpen = false;

ESP8266WebServer server(81);

void setup()
{

  pinMode(SWITCH_ONE_PIN, INPUT);
  pinMode(SWITCH_TWO_PIN, INPUT);
  pinMode(MOTOR_CW_PIN, OUTPUT);
  pinMode(MOTOR_CCW_PIN, OUTPUT);

  digitalWrite(MOTOR_CW_PIN, LOW);
  digitalWrite(MOTOR_CCW_PIN, LOW);

  Serial.begin(115200);

  // Initialise wifi connection
  wifiConnected = connectWifi();

  if (wifiConnected) {
    upnpBroadcastResponder.beginUdpMulticast();

    // Define your switches here. Max 10
    // Format: Alexa invocation name, local port no, on callback, off callback
    door = new Switch("coop door", 80, openCoopDoor, closeCoopDoor);

    Serial.println("Adding switches upnp broadcast responder");
    upnpBroadcastResponder.addDevice(*door);

    server.on("/", HTTP_GET, handleRoot);
    server.on("/freerange", HTTP_POST, openCoopDoor);
    server.on("/lockdown", HTTP_POST, closeCoopDoor);

    //server.onNotFound(handleNotFound);
    server.begin();
  
    Serial.println("WebServer started on port: ");
    Serial.println("81");
    
  } else {
    Serial.println("Could not connect to wifi");
  }
}

void handleRoot() {
  server.send(200, "text/html", getPage());
}
//https://github.com/esp8266/ESPWebServer/blob/master/src/ESP8266WebServer.h
//https://diyprojects.io/bootstrap-create-beautiful-web-interface-projects-esp8266/
void loop()
{
  if (wifiConnected) {
    upnpBroadcastResponder.serverLoop();

    door->serverLoop();
    server.handleClient();
    delay(1000);
  } else {
    connectWifi();
  }
}


void open() {
  while (digitalRead(SWITCH_TWO_PIN) == HIGH) {
    digitalWrite(MOTOR_CW_PIN, HIGH);
    digitalWrite(MOTOR_CCW_PIN, LOW);
    yield();
  }

  stop();
}


void close() {
  while (digitalRead(SWITCH_ONE_PIN) == HIGH) {
    digitalWrite(MOTOR_CW_PIN, LOW);
    digitalWrite(MOTOR_CCW_PIN, HIGH);
    yield();
  }

  stop();
}


void stop() {
  digitalWrite(MOTOR_CW_PIN, LOW);
  digitalWrite(MOTOR_CCW_PIN, LOW);
  delay(2000);
}

bool openCoopDoor(ESP8266WebServer& server) {
  Serial.println("Open coop door.");
  open();
  isDoorOpen = true;
  server.send(200, "text/html", getPage());
  return isDoorOpen;
}

bool closeCoopDoor() {
  Serial.println("Close coop door.");
  close();
  isDoorOpen = false;
  return isDoorOpen;
}

// connect to wifi – returns true if successful or false if not
boolean connectWifi() {
  boolean state = true;
  int i = 0;

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting ...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 10) {
      state = false;
      break;
    }
    i++;
  }

  if (state) {
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(WIFI_SSID);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("");
    Serial.println("Connection failed.");
  }

  return state;
}
