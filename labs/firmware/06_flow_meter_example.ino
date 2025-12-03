// Include Particle Device OS APIs
#include "Particle.h"

int FLOW_PIN_1 = D5;
int pulse_count_1 = 0;

int sample_rate = 1000;

// setup() runs once, when the device is first turned on
void setup() {
    pinMode(FLOW_PIN_1, INPUT); 
    attachInterrupt(FLOW_PIN_1, IncrementPulseCounter_1, RISING);
}

void loop() {
    flow_rate_1 = calculate_flow_1();
    delay(sample_rate);
}

void IncrementPulseCounter_1() {
    pulse_count_1++; // increment the global counter
    running_pulse_count_1++;
}

double calculate_flow_1() {
    float pulses_per_second = float(pulse_count_1 * 1000) / float(sample_rate);
    float liters_per_minute = pulses_per_second / 4.8;
    float liters_per_second_1 = liters_per_minute / 60.0;
    return double(liters_per_second_1);
}