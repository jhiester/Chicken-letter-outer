#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <functional>
#include "switch.h"
#include "UpnpBroadcastResponder.h"
#include "CallbackFunction.h"
#include "creds.h"

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
bool officeLightsOn();
bool officeLightsOff();
bool kitchenLightsOn();
bool kitchenLightsOff();
bool openCoopDoor();
bool closeCoopDoor();

// Change this before you flash
//const char* ssid = WIFI_SSID;
//const char* password = WIFI_PASS;

boolean wifiConnected = false;

UpnpBroadcastResponder upnpBroadcastResponder;

Switch *door = NULL;
Switch *office = NULL;
Switch *kitchen = NULL;

bool isOfficeLightsOn = false;
bool isKitchenLightstsOn = false;
bool isDoorOpen = false;

void setup()
{

  pinMode(SWITCH_ONE_PIN, INPUT);
  pinMode(SWITCH_TWO_PIN, INPUT);
  pinMode(MOTOR_CW_PIN, OUTPUT);
  pinMode(MOTOR_CCW_PIN, OUTPUT);

  digitalWrite(MOTOR_CW_PIN, LOW);
  digitalWrite(MOTOR_CCW_PIN, LOW);

  Serial.begin(9600);

  // Initialise wifi connection
  wifiConnected = connectWifi();

  if (wifiConnected) {
    upnpBroadcastResponder.beginUdpMulticast();

    // Define your switches here. Max 10
    // Format: Alexa invocation name, local port no, on callback, off callback
    office = new Switch("office lights", 80, officeLightsOn, officeLightsOff);
    kitchen = new Switch("kitchen lights", 81, kitchenLightsOn, kitchenLightsOff);
    door = new Switch("coop door", 82, openCoopDoor, closeCoopDoor);

    Serial.println("Adding switches upnp broadcast responder");
    upnpBroadcastResponder.addDevice(*office);
    upnpBroadcastResponder.addDevice(*kitchen);
    upnpBroadcastResponder.addDevice(*door);
  }
}

void loop()
{
  if (wifiConnected) {
    upnpBroadcastResponder.serverLoop();

    kitchen->serverLoop();
    office->serverLoop();
    door->serverLoop();
  }
}


void open() {
  while (digitalRead(SWITCH_TWO_PIN) == HIGH) {
    digitalWrite(MOTOR_CW_PIN, HIGH);
    digitalWrite(MOTOR_CCW_PIN, LOW);
    yield();
  }

  stop();
  delay(2000);
}


void close() {
  while (digitalRead(SWITCH_ONE_PIN) == HIGH) {
    digitalWrite(MOTOR_CW_PIN, LOW);
    digitalWrite(MOTOR_CCW_PIN, HIGH);
    yield();
  }

  stop();
  delay(2000);
}


void stop() {
  digitalWrite(MOTOR_CW_PIN, LOW);
  digitalWrite(MOTOR_CCW_PIN, LOW);
}

bool openCoopDoor () {
  Serial.println("Open coop door.");
  open();
  isDoorOpen = true;
  return isDoorOpen;
}


bool closeCoopDoor() {
  Serial.println("Open coop door.");
  close();
  isDoorOpen = false;
  return isDoorOpen;
}

bool officeLightsOn() {
  Serial.println("Switch 1 turn on ...");

  isOfficeLightsOn = true;
  return isOfficeLightsOn;
}

bool officeLightsOff() {
  Serial.println("Switch 1 turn off ...");

  isOfficeLightsOn = false;
  return isOfficeLightsOn;
}

bool kitchenLightsOn() {
  Serial.println("Switch 2 turn on ...");

  isKitchenLightstsOn = true;
  return isKitchenLightstsOn;
}

bool kitchenLightsOff() {
  Serial.println("Switch 2 turn off ...");

  isKitchenLightstsOn = false;
  return isKitchenLightstsOn;
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
