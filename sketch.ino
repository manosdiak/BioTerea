#include <Arduino.h>
#include "simulated_load.h"  //[cite: 1]

// Detection & Thermal Target Constants
#define INSERTION_DELTA_I 1.2f   // Ampere drop threshold signaling stick insertion
#define TARGET_TEMP_C     350.0f // Target operating temperature for susceptor

// PID Controller Parameters
struct PID {
    float Kp = 2.5f;
    float Ki = 0.4f;
    float Kd = 0.1f;
    float integral = 0.0f;
    float prev_error = 0.0f;
};

PID pid;
uint8_t sim_step = 0;
bool stick_inserted = false;

// Empirical transfer function mapping RMS current to temperature
float infer_temperature(float current_rms) {
    return TARGET_TEMP_C * (1.0f - (current_rms / 6.0f)); 
}

// Compute PWM Output via PID
float compute_pid(float target, float current_temp) {
    float error = target - current_temp;
    pid.integral += error;
    float derivative = error - pid.prev_error;
    pid.prev_error = error;
    
    float output = (pid.Kp * error) + (pid.Ki * pid.integral) + (pid.Kd * derivative);
    return constrain(output, 0.0f, 255.0f); // Map to 8-bit PWM
}

void setup() {
    Serial.begin(115200);
    Serial.println("==================================================");
    Serial.println("  INDUCTION DRIVER FIRMWARE INITIALIZED           ");
    Serial.println("  Monitoring Tank Current & Susceptor Impedance   ");
    Serial.println("==================================================");
}

void loop() {
    // 1. Fetch current simulated load profile values
    float steel_current   = RMS_STEEL_BASELINE[sim_step];    //[cite: 1]
    float biochar_current = RMS_BIOCHAR_COMPOSITE[sim_step]; //[cite: 1]
    
    // 2. Auto-Start Insertion Detection
    if (!stick_inserted && (steel_current - biochar_current) >= INSERTION_DELTA_I) {
        stick_inserted = true;
        Serial.println("\n[AUTO-START INTERRUPT] Biochar/Magnetite susceptor insertion detected!");
    }

    // 3. Execution & Thermal Management
    if (stick_inserted) {
        float inferred_temp = infer_temperature(biochar_current);
        float pwm_duty = compute_pid(TARGET_TEMP_C, inferred_temp);

        Serial.printf("Step: %03d | RMS Current: %.2fA | Inferred Temp: %.1f °C | PWM Output: %.0f/255\n", 
                      sim_step, biochar_current, inferred_temp, pwm_duty);
    } else {
        Serial.printf("Step: %03d | Standby Load: %.2fA | Awaiting Stick Insertion...\n", sim_step, steel_current);
    }

    // Step through the dataset
    sim_step = (sim_step + 1) % LOAD_DATA_POINTS; //[cite: 1]
    delay(100);
}