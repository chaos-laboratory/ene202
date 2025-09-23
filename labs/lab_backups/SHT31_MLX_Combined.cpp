// This #include statement was automatically added by the Particle IDE.
#include "Particle.h"
#include <adafruit-sht31.h>
#include <Adafruit_MLX90614.h>


int sample_rate = 10*1000; //publish rate


Adafruit_SHT31 sht31 = Adafruit_SHT31();
Adafruit_MLX90614 mlx = Adafruit_MLX90614();


double humidity;
double temp;
double dewPoint;
double objectT = 0;
double ambientT = 0;


void setup() {
    sht31.begin(0x44);
    mlx.begin();  
    Particle.variable("temp", temp);
    Particle.variable("humid",humidity);
    Particle.variable("object", objectT);
    Particle.variable("ambient", ambientT);
}

void loop() {
    // Read values from the sensor
    temp = double(sht31.readTemperature());
    humidity = double(sht31.readHumidity());
    ambientT = mlx.readAmbientTempC();
    objectT = mlx.readObjectTempC();
    
    delay(sample_rate);
}