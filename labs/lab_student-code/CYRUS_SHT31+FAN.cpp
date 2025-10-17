//-------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------
// 1 LIBRARIES, INITIAL SETTINGS AND DEFINITIONS

// Include Particle Device OS APIs
#include "Particle.h"
#include <math.h>
#include <adafruit-sht31.h>

//--------------------------------------------------
// 1.1 GLOBAL SETTINGS
int Sample_Rate = 1*1000; // How frequently data will be collected and updated in functions that allow for a variable Sample_Rate

// 1.2 ASSIGNING PINS FOR ON/OFF FUNCTIONS
bool set1 = 0;  // boolean (on/off) functionality assigned to 'set1'
int OnOffPin1 = D7;  // choose which digital pin (D#) to use for the on/off function

int OnOff1(String) { // On-Off function for your first chosen pin
    set1 = !set1;
    digitalWrite(OnOffPin1, set1);
    return set1;
}

//--------------------------------------------------
// 1.3 SHT31 TEMPERATURE AND HUMIDITY SENSOR
// SHT31 does not need a specified input pin, if it is connected to D0(SDA) and D1(SCL)
Adafruit_SHT31 sht31 = Adafruit_SHT31();
double SHT31_Temperature;
double SHT31_Humidity;

int SHT31_Humidity_Threshold = 60;

int Set_SHT31_Humidity_Threshold(String command) { // On-Off function for your first chosen pin
    SHT31_Humidity_Threshold = command.toFloat();
    return 1; // if '1' shows up in your Console / Phone App when you change the threshold, this means the change has been successfully registered
}

//-------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------
// 2 SETUP - HAPPENS ONLY ONCE IN CODE

void setup() {
    
    // 2.1 INITIATE COMMUNICATION VIA I2C PINS (D0(SDA) AND D1(SCL))
    sht31.begin(0x44);
    
    // 2.1 WHICH PINS ARE INPUT, WHICH ARE OUTPUT
    pinMode(OnOffPin1, OUTPUT);
    
    // 2.2 THE STATUS FOR THE VARIABLES THAT YOU WANT REPORTED IN THE PARTICLE CONSOLE / PHONE APP
    Particle.variable("SHT31_Temperature", SHT31_Temperature);
    Particle.variable("SHT31_Humidity", SHT31_Humidity);
    
    // 2.3 FUNCTIONS THAT YOU CAN ACTUATE OR EDIT DIRECTLY IN PARTICLE CONSOLE / PHONE APP 
    Particle.function("SHT31_Humidity_Threshold", Set_SHT31_Humidity_Threshold);
    
 }

//-------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------
// 3 LOOP - HAPPENS ONLY ONCE, BUT CAN CONTAIN MULTIPLE SUBROUTINES WITHIN THIS ONE LOOP
// loop() runs over and over again, as quickly as it can execute.
void loop() {
    
    // SHT31: send temperatures to particle.variable
    SHT31_Temperature = double(sht31.readTemperature());
    SHT31_Humidity = double(sht31.readHumidity());
    
    // SHT31 + FAN: 
    if(SHT31_Humidity >= SHT31_Humidity_Threshold) {
        digitalWrite(OnOffPin1, set1 = 1);
    } else if (SHT31_Humidity < SHT31_Humidity_Threshold) {
        digitalWrite(OnOffPin1, set1 = 0);
    }
    
    delay (Sample_Rate);
}