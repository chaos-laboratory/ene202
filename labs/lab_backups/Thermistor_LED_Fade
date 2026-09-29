
//-------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------
// 1 LIBRARIES, INITIAL SETTINGS AND DEFINITIONS
//libraries that must be included

#include "math.h"
#include <application.h>

//--------------------------------------------------
// 1.1 THERMISTOR SETUP

int sample_rate = 1*1000;

#define TEMPERATURENOMINAL 25   
#define NUMSAMPLES 5
#define BCOEFFICIENT 3950
#define SERIESRESISTOR 10000  

uint16_t samples[NUMSAMPLES];
uint8_t i;
float average;
 
int thermistorPin = A0;
int LEDpin = A2;
double thermistorTemp = 0;
double tempLow = 25;
double tempHigh = 36;

void setLED(double temp){                                           // we are now writing a new function called setLED: 'int' hands back an integer; 'temp' = renaming of 'thermistorTemp' within the loop, for convenience; '{' opens the function body
                                                                    // we will be using the map function: new variable = map (value, fromLow, fromHigh, toLow, toHigh). In short - where does 'value' sit on the scale of 'fromLow to fromHigh'?
    int brightness = map(temp, tempLow, tempHigh, 0.0, 255.0);      // map low-high -> 0-255 PWM value; 'brightness' is our new variable; 'int' means that our new variable will be whole numbers; 'map' converts the temp range (tempLow to tempHigh) into the PWM range (0-255)
    brightness = constrain(brightness, 0, 255);                     // the constrain function converts any low or high values exceeding the tempLow & tempHigh bounds into respectively the low or high limits; effectively, it overwrites the old brightness with the new brightness. This prevents miscalculation that ay inadvertently turn the LED on
    analogWrite(LEDpin, brightness);
}

0.
//-------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------
// 2 SETUP - HAPPENS ONLY ONCE IN CODE

void setup() {
    
    // 2.1 INITIATE COMMUNICATION VIA I2C PINS (D0(SDA) AND D1(SCL))
    // SHT?
    // MLX?
    
    // 2.1 WHICH PINS ARE INPUT, WHICH ARE OUTPUT
    pinMode(thermistorPin, INPUT);
    pinMode(LEDpin, OUTPUT);
    
    // 2.2 THE STATUS FOR THE VARIABLES THAT YOU WANT REPORTED IN THE PARTICLE CONSOLE / PHONE APP
    Particle.variable("ThermistorTemp",thermistorTemp);
    
    // 2.3 FUNCTIONS THAT YOU CAN ACTUATE OR EDIT DIRECTLY IN PARTICLE CONSOLE / PHONE APP 
    // Eventually - set high threshold
    // Eventually - set low threshold
}



//-------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------
// 3 LOOP - HAPPENS ONLY ONCE, BUT CAN CONTAIN MULTIPLE SUBROUTINES WITHIN THIS ONE LOOP
void loop() {
    
    //THERMISTOR: Read the thermistor and update it to Cloud    
    thermistorTemp = therm(thermistorPin); //read the thermistor

    // THERMISTOR + LED: Turn on LED light when thermistor exceeds threshold
    setLED(thermistorTemp);                 // thermistorTemp here gets sent back to the setLED function declared at the beginning -> where it is converted into 'temp' for convenience.
    delay(sample_rate);
}


//-------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------
// 4 EXTRA DEFINITIONS - CAN BE MULTIPLE

//--------------------------------------------------
// 4.1 THERMISTOR EXTRA CODE
//this is the function that measures the voltage across the thermistor and 
//calcualtes the resistance. a number of samples are collected and averaged,
//NUMSAMPLES specifies this number above, to smooth noise
double therm(int pin) {
  
  // take N samples in a row, with a slight delay
  for (i=0; i< NUMSAMPLES; i++) {
   samples[i] = analogRead(pin);
   delay(10);
  }
  // average all the samples out
  average = 0;
  for (i=0; i< NUMSAMPLES; i++) {
     average += samples[i];
  }
  average /= NUMSAMPLES;
  double reading = average;

  // convert the value to resistance
  reading = (4095 / reading)  - 1;     // (4095/ADC - 1) 
  reading = SERIESRESISTOR / reading;  // 10K / (1023/ADC - 1)
  
  //the steinart method is a standard method of mapping the resistance reading
  //to a temperature
  float steinhart;
  steinhart = reading / SERIESRESISTOR;     // (R/Ro)
  steinhart = log(steinhart);                  // ln(R/Ro)
  steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                 // Invert
  steinhart -= 273.15;                         // convert to C
  
  
//return the calculated value, which is a temperature!  
  return steinhart;
}
