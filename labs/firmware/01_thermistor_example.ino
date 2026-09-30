//libraries that must be included
#include "math.h" // lets math happen like log() etc.
#include <Particle.h> // has all the Partilce.io device specfici functions


int sample_rate = 2*1000; //the rate at which the sensor will collect and publish data to the internet
//pro tip: keep the 1000. particle time is in milliseconds, so a delay rate of

// What pin to connect the thermistor to
int thermPin = A0;

//global variable definition for the temperature
double temperature;

//declare functions should happen above loop() which uses the function
double therm (int pin);

//MAIN DEVICE setup - this always runs once when the device first starts up
void setup() {
    //variable command creates a cloud variable that the temperature variable will be stored to called "temp"
    Particle.variable("temp",temperature);
}

//MAIN DEVICE REPEATING LOOP. this runs continuously, so we use a delay to prevent your Argon
//from freezing and to prevent a TON of data being sent to the cloud. You can
//customize this rate by changing the value of sample_rate above.
void loop() {
    //read the temperature of the thermistor using the function "therm" below    
    temperature = therm(thermPin); //read the thermistor using the function defined below
}


//this is the function that measures the voltage across the thermistor and 
//calcualtes the resistance. a number of samples are collected and averaged,
//NUMSAMPLES specifies this number above, to smooth noise
double therm(int pin) {
  
    // Sizes the array below, so this has to be a compile-time integer constant.
    constexpr uint8_t NUMSAMPLES = 5;            // number of ADC samples to average per reading
    constexpr double TEMPERATURENOMINAL = 25;    // nominal temp (deg C) for BCOEFFICIENT
    constexpr uint16_t BCOEFFICIENT = 3950;      // thermistor's Beta coefficient (unitless whole number)
    constexpr uint32_t SERIESRESISTOR = 10000;   // resistance (ohms) of the other resistor in the divider
    constexpr uint16_t ADC_MAX = 4095;           // 12-bit ADC on the Argon/Boron/etc. (0-4095)

    uint16_t samples[NUMSAMPLES];
    for (uint8_t i = 0; i < NUMSAMPLES; i++) {
        samples[i] = analogRead(pin);
        delay(10);
    }

    float average = 0;
    for (uint8_t i = 0; i < NUMSAMPLES; i++) {
        average += samples[i];
    }
    average /= NUMSAMPLES;

    // Convert the averaged ADC reading to resistance
    double resistance = (ADC_MAX / average) - 1;   // (ADC_MAX/ADC - 1)
    resistance = SERIESRESISTOR / resistance;      // SERIESRESISTOR / (ADC_MAX/ADC - 1)

    // Steinhart-Hart approximation: resistance -> temperature (Celsius)
    double steinhart = resistance / SERIESRESISTOR;    // (R/Ro)
    steinhart = log(steinhart);                         // ln(R/Ro)
    steinhart /= BCOEFFICIENT;                          // 1/B * ln(R/Ro)
    steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15);   // + (1/To)
    steinhart = 1.0 / steinhart;                        // invert
    steinhart -= 273.15;                                // Kelvin -> Celsius

    return steinhart;
}
