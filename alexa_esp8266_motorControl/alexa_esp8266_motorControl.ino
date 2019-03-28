//
// A simple server implementation showing how to:
//  * serve static messages
//  * read GET and POST parameters
//  * handle missing pages / 404s
//

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "webPage.h"
#include "creds.h"

#define MOTOR_CW_PIN 14
#define MOTOR_CCW_PIN 12
#define SWITCH_ONE_PIN 13
#define SWITCH_TWO_PIN 5

AsyncWebServer server(80);
bool isDoorOpen = false;
const char* PARAM_MESSAGE = "message";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void setup() {

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
      digitalWrite(MOTOR_CW_PIN, HIGH);
      Serial.println("let 'em out done");
    } else {
      Serial.println("keep 'em locked up");
    }
    request->redirect("/");
  });

  server.onNotFound(notFound);

  server.begin();
}

void loop() {  
  yield();
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
  isDoorOpen = true;
  return isDoorOpen;
}

bool closeCoopDoor() {
  Serial.println("Close coop door.");  
  close();
  isDoorOpen = false;
  return isDoorOpen;
}
