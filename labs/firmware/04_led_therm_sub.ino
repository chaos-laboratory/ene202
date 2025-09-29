//libraries that must be included

#include <math.h>

/*
  The analogWrite() function uses PWM, so if you want to change the pin you're
  using, be sure to use another PWM capable pin. On most Arduino, the PWM pins
  are identified with a "~" sign, like ~3, ~5, ~6, ~9, ~10 and ~11.
  This example code is in the public domain: https://docs.arduino.cc/built-in-examples/basics/Fade
*/

//global variable definition for the temperature
double temperature = 0;
// a variable to help see the received temperature data on particle console
double data = 0.00;  
// the PWM pin the LED is attached to. You'll have to add this.
int led = D1;


//setup
void setup() {
  
  // declare pin to be an output:
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  
  // PL we are here adding an input signal, which we receive from a publisher, thanks to our subscribe function:
  Particle.subscribe("temperature", toggleLED);
  Particle.variable("temperature", data);
}


void loop() {
    
}


//Event handler function:
void toggleLED(const char *event, const char *dataIn) { 
    
    data = double(*dataIn); // data = int(*dataIn);
    // if (*dataIn >= 29) {
    //     digitalWrite(led, HIGH);
    // } 
    // else digitalWrite(led, LOW);
    
    if (*dataIn) {
        // Convert string to double
        temperature = data;
        // Control LED based on temperature
        if (temperature >= 29.0) {
            digitalWrite(led, HIGH);
        } 
        else digitalWrite(led, LOW);
    }
}

















