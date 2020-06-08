#include "main.hpp"

#include "ESP8266_ISR_Servo.h"
#include <ESP8266WebServer.h>


ESP8266WebServer server(80);

#define MIN_MICROS      700
#define MAX_MICROS      2300

const char* ssid = WIFI_NAME;
const char* password = PASSWORD;

IPAddress staticIP(STATIC_IP); //ESP static ip
IPAddress gateway(192, 168, 0, 1);   //IP Address of your WiFi Router (Gateway)
IPAddress subnet(255, 255, 0, 0);  //Subnet mask
IPAddress dns(8, 8, 8, 8);  //DNS

int servoIndex = -1;
String thisString;

void handleNotFound();
void handleSpecificArg();
void setServo();

void setup()
{
	Serial.begin(115200);
	Serial.println("\nStarting");
	servoIndex = ISR_Servo.setupServo(D5, MIN_MICROS, MAX_MICROS);

	WiFi.mode(WIFI_STA);
	WiFi.config(staticIP, gateway, subnet, dns);
	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}

	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());

	server.onNotFound( []() {
		server.send(404, "text/plain","You seem to be lost!");
	});

	server.on("/setServo", setServo);

	server.on("/", []() {
		server.send(200, "text/plain", String("hello!"));
	});

	server.begin();
}

void loop()
{
	if (Serial.available() > 0) {
		// read the incoming byte:
		String incomingString = Serial.readString();
		thisString += incomingString;

		if ( incomingString.indexOf('\r') != -1) {
			int number = thisString.toInt();
			thisString = "";
			if (number != 0) {
				Serial.print("I received: ");
				Serial.println(number);
				ISR_Servo.setPosition(servoIndex, number);
			} else {
				Serial.println("Not a number!");
			}
		}
	}
	yield();
	server.handleClient();
	delay(5);
}


void setServo() {
	String message = "";
	String positionArg = server.arg("position");

	if (positionArg == ""){     //Parameter not found
		message = "position Argument not found";
	} else {

		int number = positionArg.toInt();
		if (number != 0) {
			ISR_Servo.setPosition(servoIndex, number);
			message = "position = " + positionArg;
		} else {
			message = "Not a number!";
		}
	}

	server.send(200, "text/plain", message);
}