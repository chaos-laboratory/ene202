// 1 LIBRARIES, INITIAL SETTINGS AND DEFINITIONS
//libraries that must be included

#include "math.h"
#include <Particle.h>

//--------------------------------------------------
// 1.1 DEVICE VARIABLES AND FUNCTIONS DECLARATIONS
 
int sample_rate = .1 * 1000; //seconds
 
int thermistorPin = A0; 
int LEDpin = A2;

double thermistorTemp = 0.0;
double tempLow = 40;
double tempHigh = 45;

int brightness = 0;

double therm(int);
void setLED(double);


//--------------------------------------------------
// 2 SETUP - RUNS ONLY ONCE
void setup() {
    
    // 2.1 WHICH PINS ARE INPUT, WHICH ARE OUTPUT
    pinMode(A0, INPUT);
    pinMode(A2, OUTPUT);
    
    // 2.2 THE STATUS FOR THE VARIABLES THAT YOU WANT REPORTED IN THE PARTICLE CONSOLE / PHONE APP
    Particle.variable("ThermistorTemp",thermistorTemp);
    Particle.variable("Brightness", brightness);
    // 2.3 FUNCTIONS THAT YOU CAN ACTUATE OR EDIT DIRECTLY IN PARTICLE CONSOLE / PHONE APP 
    // Eventually - set high threshold
    // Eventually - set low threshold
}


//--------------------------------------------------
// 3 LOOP 
void loop() {
    
    //THERMISTOR: Read the thermistor and update it to Cloud    
    thermistorTemp = therm(thermistorPin); //read the thermistor

    // THERMISTOR + LED: Turn on LED light when thermistor exceeds threshold
    setLED(thermistorTemp);                 // thermistorTemp here gets passed on to the setLED in the function in section 4 -> where it is converted into 'temp' for convenience.
    delay(sample_rate);
}


//--------------------------------------------------
// 4 FUNCTIONS

// 4.1 CONTROL LED BRIGHTNESS WITH TEMPERATURE
void setLED(double temp){                                       // we are now writing a new function called setLED: 'void' means that this function does not send back any baked value; 'temp' = renaming of 'thermistorTemp' within the loop, for convenience; '{' opens the function body
    
    temp = constrain(temp, tempLow, tempHigh);                  // the constrain function converts any low or high values exceeding the tempLow & tempHigh bounds into respectively the low or high limits
    brightness = map(temp, tempLow, tempHigh, 0.0, 255.0);      // map low-high -> 0-255 PWM value; 'brightness' is our new variable; 'int' means that our new variable will be whole numbers; 'map' converts the temp range (tempLow to tempHigh) into the PWM range (0-255)
    analogWrite(LEDpin, brightness);                            // remember, only analogWrite can be used with PWM (pulse width modulation)
    // analogWrite(LEDpin, 200);
}
//--------------------------------------------------
// 4.2 THERMISTOR VOLTAGE TO TEMPERATURE
/*  A function that measures the voltage across the thermistor and calcualtes the resistance. 
    A number of samples are collected and averaged,
    - NUMSAMPLES specifies this number above, to smooth noise 
*/
    
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
