// This #include statement was automatically added by the Particle IDE.
#include "Particle.h"
#include <adafruit-sht31.h>
#include <Adafruit_MLX90614.h>
#include "math.h"
#include <application.h>


int sample_rate = 10*1000; //publish rate

// Sensor variable definition
Adafruit_SHT31 sht31 = Adafruit_SHT31();
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
int globePin = A0;


double humidity;                            // SHT
double temp;                                // SHT
double dewPoint;                            // SHT
double objectT = 0;                         // MLX
double ambientT = 0;                        // MLX
#define TEMPERATURENOMINAL 25               // Thermistor
#define NUMSAMPLES 5                        // Thermistor
#define BCOEFFICIENT 3950                   // Thermistor
#define SERIESRESISTOR 10000                // Thermistor
double temperature = 0;                     // Thermistor

uint16_t samples[NUMSAMPLES];
uint8_t i;
float average;

void setup() {
    sht31.begin(0x44);                                          
    mlx.begin();  
    Particle.variable("temp", temp);
    Particle.variable("humid",humidity);
    Particle.variable("object", objectT);
    Particle.variable("ambient", ambientT);
    Particle.variable("thermistor", temperature);
}

void loop() {
    // Read values from the sensor
    temp = double(sht31.readTemperature());
    humidity = double(sht31.readHumidity());
    ambientT = mlx.readAmbientTempC();
    objectT = mlx.readObjectTempC();
    temperature = therm(globePin);

    
    delay(sample_rate);
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