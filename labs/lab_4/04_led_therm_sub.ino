//---------------------------------------------------------------------------
// No need to read anything in this section:

// Include Particle Device OS APIs
#include "Particle.h"

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(AUTOMATIC);

// Show system, cloud connectivity, and application logs over USB
// View logs with CLI using 'particle serial monitor --follow'
SerialLogHandler logHandler(LOG_LEVEL_INFO);

//---------------------------------------------------------------------------

// Choose a pin compatible with PWM on your microcontroller; this will now be called 'led'. Refer to Photon2 Pinout diagram:
int led = A2;

// Here, you're telling your 'led' pin, to output signals only, and to begin in the off position.
// Next, you are subscribing to the 'channel' "KAT SAID" - or whatever arbitrary name that you sepcified in your 'publish' code
// Please change 'KAT SAID' to whatever word / phrase you have chosen in your 'publish' code
void setup () {
    pinMode(led, OUTPUT);
    digitalWrite(led, LOW);
    Particle.subscribe("KAT SAID", ledON);
}

// The first line translates incoming signals from your publishing device, into readable data. 
// Then, the subsequent lines switch the led on for 300ms, and then off for 3ms, several times. Feel free to edit this as desired.
// Once the 'subscribed' channel stops transmitting a signal, the led will automatically stop blinking.
void ledON(const char *event, const char *data) {
    digitalWrite(led, HIGH);
    delay(300);
    digitalWrite (led, LOW);
    delay(300);
    digitalWrite(led, HIGH);
    delay(300);
    digitalWrite (led, LOW);
}
