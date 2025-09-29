//libraries that must be included

#include <math.h>

/*
  The analogWrite() function uses PWM, so if you want to change the pin you're
  using, be sure to use another PWM capable pin. On most Arduino, the PWM pins
  are identified with a "~" sign, like ~3, ~5, ~6, ~9, ~10 and ~11.
  This example code is in the public domain: https://docs.arduino.cc/built-in-examples/basics/Fade
*/

// What pin to connect the sensor to
int thermPin = A0; 

//global variable definition for the temperature
double temperature = 0;

// how often data is collected (ms)
int sample_rate = 1*1000;

// the PWM pin the LED is attached to. You'll have to add this.
int led = D1;

// variable to set how bright the led is. You'll have to add this. 
int brightness = 0;

//defined constants
#define TEMPERATURENOMINAL 25   
// how many samples to take and average, more takes longer
// but is more 'smooth'
#define NUMSAMPLES 5
// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT 3950
// the value of the 'other' resistor
#define SERIESRESISTOR 10000  
uint16_t samples[NUMSAMPLES];
  uint8_t i;
  float average;



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



//setup
void setup() {
  // declare particle variables: temperature and brightness. You'll need to add brightness yourself. 
  Particle.variable("temp", temperature);
  Particle.variable("brightness", brightness);

  // declare pin 7 to be an output:
  pinMode(led, OUTPUT);
}



//void loop. this runs continuously, so we use a delay to prevent your Argon
//from freezing and to prevent a TON of data being sent to the cloud. 
void loop() {
  //customizable delay
  delay(sample_rate);
  // temperature from therm function. Youll need to use the temperature variable you made above.
  temperature = therm(thermPin);
  
  // brightness mapped from temperature.You'll need to use the map() function to correlate the brightness variable you made with temperature. 
  brightness = map((int)(temperature*100), 60*100, 91*100, 0, 255); //map((int)(temperature*100), 2000, 3500, 0, 255);
  
  // set the brightness of pin 7: you’ll need to do this yourself.  
  analogWrite(led, brightness);
}


















