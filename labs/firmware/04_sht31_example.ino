#include "Particle.h"
#include <adafruit-sht31.h>

/* USER INPUT, do not worry about these things...or change them */

int sample_rate = 2*1000; //the rate at which the sensor will collect and publish data to the internet
//pro tip: keep the 1000. particle time is in milliseconds, so a delay rate of
//5 seconds needs to be coded as 5000, this the *1000

Adafruit_SHT31 sht31 = Adafruit_SHT31();

double humidity;
double temp;
double dewPoint;

//setup loop - this always runs once when the device first starts up
void setup() {
    sht31.begin(0x44);
    Particle.variable("temp", temp);
    Particle.variable("humid",humidity);
}

//void loop. this runs continuously, so we use a delay to prevent your Argon
//from freezing and to prevent a TON of data being sent to the cloud. You can
//customize this rate by changing the value of sample_rate above.
void loop() {
    temp = double(sht31.readTemperature());
    humidity = double(sht31.readHumidity());
    
//calculate dewpoint
    double a = 17.62;
    double b = 243.12; // °C
    double alpha = ((a * temp) / (b + temp)) + log(humidity / 100.0);
    dewPoint = (b * alpha) / (a - alpha);
    
    
    delay(sample_rate);//customizable delay
}