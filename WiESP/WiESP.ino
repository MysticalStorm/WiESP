#include "Dispatcher.h"
#include <dummy.h>
#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "SoftwareSerial.h"

ESP8266WebServer server(80);

const char *ssid = "ESPap";
const char *password = "thereisnospoon";

DispatcherClass dispatcher;

void setup() {
	delay(1000);
	Serial.begin(115200);

	dispatcher.init();

	Serial.println();
	Serial.print("Configuring access point...");
	WiFi.softAP(ssid, password);
	
	server.begin();

	pinMode(2, LOW);

	IPAddress myIP = WiFi.softAPIP();
	Serial.print("AP IP address: ");
	Serial.println(myIP);

	Serial.println("HTTP server started");
	/*
	server.on("/", []() {
		server.send(200, "text/plain", "this works as well");

		char a[5] = "1234";
		serialTest.write(a);
	});
	*/

	dispatcher.didNewData(didNewData);
}

void didNewData(Orientation orientation) {
	Serial.println("I had new data");
}

// Main loop, read and display data

void loop() {
	server.handleClient();
	dispatcher.update();
	//Serial.print("New cycle "); Serial.println(millis());
}