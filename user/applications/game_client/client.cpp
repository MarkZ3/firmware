#include <application.h>
#include <MQTT.h>
//#include <spark_wiring_print.h>

// ------------
// Blink an LED
// ------------

/*-------------

 We've heavily commented this code for you. If you're a pro, feel free to ignore it.

 Comments start with two slashes or are blocked off by a slash and a star.
 You can read them, but your device can't.
 It's like a secret message just for you.

 Every program based on Wiring (programming language used by Arduino, and Particle devices) has two essential parts:
 setup - runs once at the beginning of your program
 loop - runs continuously over and over

 You'll see how we use these in a second. 

 This program will blink an led on and off every second.
 It blinks the D7 LED on your Particle device. If you have an LED wired to D0, it will blink that LED as well.

 -------------*/

// First, we're going to make some variables.
// This is our "shorthand" that we'll use throughout the program:
int led1 = D0;
int led2 = D1;
int led3 = D2;

int button1 = D3;
int button2 = D4;
int button3 = D5;
int buttonSend = D6;
int ledDebug = D7;

// Having declared these variables, let's move on to the setup function.
// The setup function is a standard part of any microcontroller program.
// It runs only once when the device boots up or is reset.

byte server[] = { 192, 168, 0, 101 };
MQTT client(server, 1883, 0);

void setup() {

	// We are going to tell our device that D0 and D7 (which we named led1 and led2 respectively) are going to be output
	// (That means that we will be sending voltage to them, rather than monitoring voltage that comes from them)

	// It's important you do this here, inside the setup() function rather than outside it or in the loop function.

	Serial.begin(9600);

	pinMode(ledDebug, OUTPUT);

	pinMode(led1, OUTPUT);
	pinMode(led2, OUTPUT);
	pinMode(led3, OUTPUT);

	pinMode(button1, INPUT_PULLUP);
	pinMode(button2, INPUT_PULLUP);
	pinMode(button3, INPUT_PULLUP);
	pinMode(buttonSend, INPUT_PULLUP);
	// publish/subscribe
	if (!client.isConnected()) {
		Serial.printlnf("connected: %d", client.connect("sparkclient"));
	}

}

// Next we have the loop function, the other essential part of a microcontroller program.
// This routine gets repeated over and over, as quickly as possible and as many times as possible, after the setup function is called.
// Note: Code that blocks for too long (like more than 5 seconds), can make weird things happen (like dropping the network connection).  The built-in delay function shown below safely interleaves required background activity, so arbitrarily long delays can safely be done if you need them.

int lastSendButtonState = -1;

void loop() {

	int32_t button1Val = !digitalRead(button1);
	digitalWrite(led1, button1Val);
	int32_t button2Val = !digitalRead(button2);
	digitalWrite(led2, button2Val);
	int32_t button3Val = !digitalRead(button3);
	digitalWrite(led3, button3Val);

	int32_t buttonSendVal = !digitalRead(buttonSend);
	if (buttonSendVal && lastSendButtonState != buttonSendVal) {
		int answer = button1Val | button2Val << 1 | button3Val << 2;
		Serial.println(answer);
		int playerNum = 0;
		char buff[15];
		// player_num[space]timestamp_ms[space]reponse0-7
		sprintf(buff, "%d %d %d", playerNum, Time.now(), answer);
		Serial.printlnf("publish: %d", client.publish("gameplayers", buff));
		digitalWrite(ledDebug, HIGH);
	}
	lastSendButtonState = buttonSendVal;

	// Wait 1 second...
	delay(5);

	// And repeat!
}
