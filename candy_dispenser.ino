// Include libraries
#include <Stepper.h>
#include <Ultrasonic.h>
#include <Bounce2.h> 

// Define constants
#define BUTTON_PIN 9
#define LED_PIN 10

#define SENSOR_TRIGGER_PIN 12
#define SENSOR_ECHO_PIN 13

#define MOTOR_A1 2
#define MOTOR_A2 3
#define MOTOR_B1 4
#define MOTOR_B2 5
#define MOTOR_STEPS_PER_REV 200

// Enable debugging prints. Comment out this line to disable
// #define DEBUG

// Create variables
boolean enabled = false;

// Set up motor, sensor, and button objects.
// Arguments: steps per rotation and pins for each phase
Stepper motor(MOTOR_STEPS_PER_REV, MOTOR_A1, MOTOR_A2, MOTOR_B1, MOTOR_B2);
// Arguments: trigger pin, recieve pin
Ultrasonic sensor(SENSOR_TRIGGER_PIN, SENSOR_ECHO_PIN);
// No arguments, we set it up later
Bounce2::Button button = Bounce2::Button();

// This function runs only one when the microcontroller boots up.
void setup() {
  // Set up motor: set speed (rpm)
  motor.setSpeed(60); 

  // Set up button
  button.attach(BUTTON_PIN, INPUT_PULLUP); // Connect the button to a pin
  button.interval(5); // How much time to allow the button to stabilize, in ms
  button.setPressedState(LOW); // The button is "pressed" when we read a logic LOW

  // Set up logging
  Serial.begin(9600); // 9600 is the baud rate, in bits/sec 
}

// The microcontroller runs this code over and over as fast as it can.
void loop() {
  // Read the button
  button.update();
  if (button.pressed()) {
    // Set the enabled variable to its opposite
    enabled = !enabled;
    #ifdef DEBUG
      if (enabled) {
        Serial.println("enabled");
      } else {
        Serial.println("disabled");
      }
    #endif // DEBUG
    // Set the LED on or off
    digitalWrite(LED_PIN, enabled ? HIGH : LOW);
  }
  
  // If the machine is enabled, run the main logic
  if (enabled) {
    // Read from the sensor, get the distance in inches
    float distance = sensor.read(CM);
    #ifdef DEBUG
      Serial.print("Ultrasonic distance: ");
      Serial.print(distance);
      Serial.println(" cm");
    #endif // DEBUG
    if (distance <= 6) {
      // Move the motor 1/10 rev
      motor.step(MOTOR_STEPS_PER_REV / 10);
    }
  }
}
