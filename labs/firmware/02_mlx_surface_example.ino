//This is code to run the surface and ambient temperaure sensors (round metal cylinder)

// This #include statement was automatically added by the Particle IDE.
#include "Particle.h"
#include <Adafruit_MLX90614.h>


/* USER INPUT */
int sample_rate = 2 * 1000; // how often you want device to publish where the number in front of a thousand is the seconds you want
/**/

double objectT = 0;
double ambientT = 0;


Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup() {
    //start up the MLX sensor on the SDA and SLA connection running I2c from the library 
    mlx.begin();  
    // Experimental notes
    Particle.variable("object", objectT);
    Particle.variable("ambient", ambientT);

}

void loop() {
    
ambientT = mlx.readAmbientTempC();
objectT = mlx.readObjectTempC();

delay(sample_rate);

}