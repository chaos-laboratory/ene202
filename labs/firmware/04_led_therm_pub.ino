// Include Particle Device OS APIs
#include "Particle.h"
#include <math.h>

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(AUTOMATIC);

// Show system, cloud connectivity, and application logs over USB
// View logs with CLI using 'particle serial monitor --follow'
SerialLogHandler logHandler(LOG_LEVEL_INFO);

//---------------------------------------------------------------------------

int thermPin = A0;
int sample_rate = 3*1000;
double temperature = 0;
int threshold = 29;

#define TEMPERATURENOMINAL 25   
#define NUMSAMPLES 5
#define BCOEFFICIENT 3950
#define SERIESRESISTOR 10000  

uint16_t samples[NUMSAMPLES];
uint8_t i;
float average;

//---------------------------------------------------------------------------

void setup() {
    Particle.variable("thermistor", temperature);
}

void loop() {
    temperature = therm(thermPin);
    if(temperature >= threshold) {
        Particle.publish("KAT SAID", String(temperature));
    }
    delay (sample_rate);
}


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
