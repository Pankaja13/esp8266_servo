#include "ESP8266_ISR_Servo.h"


#define MIN_MICROS      700
#define MAX_MICROS      2300

int servoIndex = -1;
String thisString;

void setup()
{
	Serial.begin(115200);
	Serial.println("\nStarting");
	servoIndex = ISR_Servo.setupServo(D5, MIN_MICROS, MAX_MICROS);

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
	delay(5);
}