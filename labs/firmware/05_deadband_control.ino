//libraries that must be included

#include "math.h"
#include <application.h>


/* USER INPUT, do not worry about these things...or change them */


int poll_rate = 500; //the rate at which the sensor will collect and publish data to the internet
//pro tip: keep the 1000. particle time is in milliseconds, so a delay rate of
//5 seconds needs to be coded as 5000, this the *1000


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
 
// What pin to connect the sensor to
int thermPin = A0;
int heaterPin = D19;

//global variable definition for the temperature
double temperature = 0;
double deadband = 2; // ± °C
double setpoint = 24;

double lowerLim = setpoint - deadband;
double upperLim = setpoint + deadband;

bool heating = false;

int ChangeSetpoint(String command) {
    setpoint = command.toFloat();
    lowerLim = setpoint - deadband;
    upperLim = setpoint + deadband;
    return 1;
}

int ChangeDeadband(String command) {
    deadband = command.toFloat();
    lowerLim = setpoint - deadband;
    upperLim = setpoint + deadband;
    return 1;
}

//setup loop - this always runs once when the device first starts up
void setup() {
    pinMode(heaterPin, OUTPUT); 
    pinMode(D7, OUTPUT); // used to mirror the status of heaterPin
    
    //variable command creates a cloud variable that the temperature variable will be stored to called "temp"
    Particle.variable("temp", temperature);
    Particle.variable("setpoint", setpoint);
    Particle.variable("deadband", deadband);
    Particle.variable("lowerLim", lowerLim);
    Particle.variable("upperLim", upperLim);
    Particle.variable("isHeating", heating);
     
    Particle.function("setpoint", ChangeSetpoint);
    Particle.function("deadband", ChangeDeadband);
}

void startHeating() {
    digitalWrite(D7, HIGH);
    digitalWrite(heaterPin, HIGH);
    heating = true;
}

void stopHeating() {
    digitalWrite(D7, LOW);
    digitalWrite(heaterPin, LOW);
    heating = false;
}

//void loop. this runs continuously, so we use a delay to prevent your Argon
//from freezing and to prevent a TON of data being sent to the cloud. You can
//customize this rate by changing the value of sample_rate above.
void loop() {
    //read the temperature of the thermistor using the function "therm" below    
    temperature = therm(thermPin); //read the thermistor
    if (temperature <= lowerLim) {
        if (!heating) {
            startHeating();
        } // else already heating, nothing to do
    }
    else {
        // we're above the setpoint
        if ((temperature > setpoint) && heating) {
            stopHeating();
        }
    }
    delay(poll_rate);
}


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
