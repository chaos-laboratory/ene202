// This #include statement was automatically added by the Particle IDE.
#include "Particle.h"
#include <adafruit-sht31.h>


int sample_rate = 10*1000; //publish rate


Adafruit_SHT31 sht31 = Adafruit_SHT31();


double humidity;
double temp;
double dewPoint;


void setup() {
    pinMode(D3, OUTPUT);
    pinMode(D2, OUTPUT);
    digitalWrite(D3, HIGH);
    digitalWrite(D2, LOW);
    sht31.begin(0x44);
    Particle.variable("temp", temp);
    Particle.variable("humid",humidity);
}

void loop() {
    // Read values from the sensor
    temp = double(sht31.readTemperature());
    humidity = double(sht31.readHumidity());
    
    delay(sample_rate);
}