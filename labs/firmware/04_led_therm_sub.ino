// Include Particle Device OS APIs
#include "Particle.h"

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(AUTOMATIC);

// Show system, cloud connectivity, and application logs over USB
// View logs with CLI using 'particle serial monitor --follow'
SerialLogHandler logHandler(LOG_LEVEL_INFO);

//---------------------------------------------------------------------------

int led = A2;

void setup () {
    pinMode(led, OUTPUT);
    digitalWrite(led, LOW);
    Particle.subscribe("KAT SAID", ledON);
}

void ledON(const char *event, const char *data) {
    digitalWrite(led, HIGH);
    delay(300);
    digitalWrite (led, LOW);
    delay(300);
    digitalWrite(led, HIGH);
    delay(300);
    digitalWrite (led, LOW);
}
