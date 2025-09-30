//---------------------------------------------------------------------------
// No need to read anything in this section:

// Include Particle Device OS APIs
#include "Particle.h"
#include <math.h>

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(AUTOMATIC);

// Show system, cloud connectivity, and application logs over USB
// View logs with CLI using 'particle serial monitor --follow'
SerialLogHandler logHandler(LOG_LEVEL_INFO);

//---------------------------------------------------------------------------

// Specify which pin your thermistor is connected to. Most likely, it will be A0, from previous labs.
// As well, specify your sample rate. 3*1000 = 3 seconds, and is the quickest we recommend. 5 to 10 seconds is totally fine.
// Finally, specify the temperature 'threshold' above which you would like the led on the 'subscribing' microcontroller to turn on.
int thermPin = A0;
int sample_rate = 3*1000;
double temperature = 0;
int threshold = 29;

// These terms and numbers are used by the firmware in the next section to convert the incoming analog signal from the thermistor into a comprehensible temperature value.
// No need to do anything here.
#define TEMPERATURENOMINAL 25   
#define NUMSAMPLES 5
#define BCOEFFICIENT 3950
#define SERIESRESISTOR 10000  
uint16_t samples[NUMSAMPLES];
uint8_t i;
float average;

//---------------------------------------------------------------------------

// Here we tell Particle to make the thermistor temperature into a variable that we can read in the Particle Console.
void setup() {
    Particle.variable("thermistor", temperature);
}

// Here we make a loop, that performs the following operation, at the sample rate we have specified above.
// The operation is, first, to read the temperature from the thermistor pin. This temperature value is the result of the analog voltage conversion by the code snippet at the end of this file.
// If the temperature value is above the threshold we specified above, we tell Particle to publish this value to the "KAT SAID" channel
// Importanly, we tell it to publish this value as a string, which will make it readable by the 'subscribe' command on the other microcontroller.
void loop() {
    temperature = therm(thermPin);
    if(temperature >= threshold) {
        Particle.publish("KAT SAID", String(temperature));
    }
    delay (sample_rate);
}

//---------------------------------------------------------------------------
// And down here is where the analog thermistor coltage signal gets converted into a temperature value.
// No need to do anything here. Just marvel at the complexity and potential of some good codin'.

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
