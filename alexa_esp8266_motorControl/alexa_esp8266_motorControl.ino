#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <functional>
#include "switch.h"
#include "UpnpBroadcastResponder.h"
#include "CallbackFunction.h"
#include "creds.h"  // wifi credentials
#include "webPage.h"

#define MOTOR_CW_PIN 14
#define MOTOR_CCW_PIN 12
#define SWITCH_ONE_PIN 13
#define SWITCH_TWO_PIN 5

ESP8266WebServer webServer(3000);

// prototypes
boolean connectWifi();

void open();
void close();
void stop();

//on/off callbacks
bool openCoopDoor();
bool closeCoopDoor();
bool isDoorOpen();

boolean wifiConnected = false;

UpnpBroadcastResponder upnpBroadcastResponder;

Switch *door = NULL;



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
    door = new Switch("coop door", 82, openCoopDoor, closeCoopDoor);

    Serial.println("Adding switches upnp broadcast responder");
    upnpBroadcastResponder.addDevice(*door);
  } else {
    Serial.println("Could not connect to wifi");
  }

  bool doorStatus = isDoorOpen();

  webServer.on("/", HTTP_GET, [& doorStatus]() {  
    doorStatus = isDoorOpen();  
    webServer.send(200, "text/html", getPage(doorStatus));      
  });

  webServer.on("/", HTTP_POST, [& doorStatus]() {
    if (webServer.hasArg("freerange") == true) {
      openCoopDoor();
      doorStatus = isDoorOpen();
      webServer.send(200, "text/html", getPage(doorStatus));
    } else if (webServer.hasArg("lockdown") == true) {
      closeCoopDoor();
      doorStatus = isDoorOpen();
      webServer.send(200, "text/html", getPage(doorStatus));
    } else
      webServer.send(404, "text/plain", "error");
  });


  webServer.begin();
}

void loop()
{
  if (wifiConnected) {
    upnpBroadcastResponder.serverLoop();

    door->serverLoop();
    webServer.handleClient();
  } else {
    connectWifi();
  }
}

bool isDoorOpen() {
  if (digitalRead(SWITCH_ONE_PIN) == HIGH)
    return true;
  else
    return false;
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

bool openCoopDoor () {
  Serial.println("Open coop door.");
  open();
}

bool closeCoopDoor() {
  Serial.println("Open coop door.");
  close();
}

void notFound() {
  webServer.send(404, "text/plain", "Not found");
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
