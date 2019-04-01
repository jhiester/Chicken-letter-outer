//
// A simple server implementation showing how to:
//  * serve static messages
//  * read GET and POST parameters
//  * handle missing pages / 404s
//

#include <Arduino.h>
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
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.printf("WiFi Failed!\n");
    return;
  }

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Hostname: ");
  Serial.println(WiFi.hostname());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/html", getPage());
  });

  // Send a GET request to <IP>/get?message=<message>
  server.on("/freerange", HTTP_GET, [] (AsyncWebServerRequest * request) {
    String message;
    if (request->hasParam(PARAM_MESSAGE)) {
      message = request->getParam(PARAM_MESSAGE)->value();
    } else {
      message = "No message sent";
    }
    request->send(200, "text/plain", "Hello, GET: " + message);
  });

  // Send a POST request to <IP>/post with a form field message set to <message>
  server.on("/post", HTTP_POST, [](AsyncWebServerRequest * request) {
    String message;
    if (request->hasParam(PARAM_MESSAGE, true)) {
      message = request->getParam(PARAM_MESSAGE, true)->value();
    } else {
      message = "No message sent";
    }
    request->send(200, "text/plain", "Hello, POST: " + message);
  });

  // Send a POST request to <IP>/post with a form field message set to <message>
  server.on("/", HTTP_POST, [](AsyncWebServerRequest * request) {
    String state;
    if (request->hasParam("freerange", true)) {
      state = request->getParam("freerange", true)->value();
      Serial.println("let 'em out");
      open();
      request->send(200, "text/plain", "");
      request->redirect("/");
    }

    if (request->hasParam("lockdown", true)) {
      state = request->getParam("freerange", true)->value();
      Serial.println("let 'em out");
      close();
      request->send(200, "text/plain", "");
      request->redirect("/");
    }
  });

  server.onNotFound(notFound);

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
  int value1 = digitalRead(SWITCH_ONE_PIN);
  int value2 = digitalRead(SWITCH_TWO_PIN);
  
  Serial.println(value1, DEC);
  Serial.println(value2, DEC);
  
  while (digitalRead(SWITCH_TWO_PIN) == HIGH) {    
    digitalWrite(MOTOR_CW_PIN, HIGH);
    digitalWrite(MOTOR_CCW_PIN, LOW);
    Serial.println(digitalRead(MOTOR_CW_PIN), DEC);    
    wdt_reset();
  }

  stop();
}


void close() {
  while (digitalRead(SWITCH_ONE_PIN) == HIGH) {
    digitalWrite(MOTOR_CW_PIN, LOW);
    digitalWrite(MOTOR_CCW_PIN, HIGH);
    Serial.println(digitalRead(MOTOR_CCW_PIN), DEC);    
    wdt_reset();
    //yield();
  }

  stop();
}


void stop() {
  digitalWrite(MOTOR_CW_PIN, LOW);
  digitalWrite(MOTOR_CCW_PIN, LOW);
  //delay(2000);
}

bool openCoopDoor() {
  Serial.println("Open coop door.");
  open();
}

bool closeCoopDoor() {
  Serial.println("Close coop door.");  
  close();
}

void notFound() {
  webServer.send(404, "text/plain", "Not found");
}
